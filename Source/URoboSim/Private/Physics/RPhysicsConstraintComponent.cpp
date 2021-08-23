#include "Physics/RPhysicsConstraintComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Physics/RJoint.h"
#include "Conversions.h"

float URConstraintComponent::GetUpperLimit()
{
  if(SoftUpper != 0.0f)
    {
      return SoftUpper;
    }
  else
    {
      return Upper;
    }
}

float URConstraintComponent::GetLowerLimit()
{
  if(SoftLower != 0.0f)
    {
      return SoftLower;
    }
  else
    {
      return Lower;
    }
}

const FTransform URConstraintComponent::GetChildPoseInJointFrame() const
{
	FTransform ChildPose = Child->GetComponentTransform();
	FTransform JointPose = GetComponentTransform();
	return ChildPose * JointPose.Inverse();
}

float URConstraintComponent::GetJointPositionInUUnits()
{
  DeltaPoseInJointFrame = InitChildPoseInJointFrame.Inverse() * GetChildPoseInJointFrame();
  return 0.f;
}

float URPrismaticConstraintComponent::ClampJointStateToConstraintLimit(float InJointState)
{
  float JointValue;
  float UsedUpper = GetUpperLimit();
  float UsedLower = GetLowerLimit();

  if(InJointState > UsedUpper)
    {
      JointValue =  UsedUpper;
    }
  else if(InJointState < UsedLower)
    {
      JointValue =  UsedLower;
    }
  else
    {
      JointValue = InJointState;
    }
  return JointValue;
}

float URRevoluteConstraintComponent::ClampJointStateToConstraintLimit(float InJointState)
{
  float JointValue;
  float UsedUpper = GetUpperLimit();
  float UsedLower = GetLowerLimit();
  if(InJointState > UsedUpper)
    {
      JointValue =  UsedUpper;
    }
  else if(InJointState < UsedLower)
    {
      JointValue =  UsedLower;
    }
  else
    {
      JointValue = InJointState;
    }

  return JointValue;
}

float URContinuousConstraintComponent::CheckPositionRange(float InTargetJointPos)
{
  bool bNormalized = false;
  while(!bNormalized)
    {
      if(InTargetJointPos > PI)
        {
          InTargetJointPos -= 2 * PI;
        }
      else if(InTargetJointPos < -PI)
        {
          InTargetJointPos += 2 * PI;
        }
      else
        {
          bNormalized = true;
        }
    }
  return InTargetJointPos;
}

void URConstraintComponent::UpdateEncoderValue(float InValue)
{
  Encoder->UpdateValue(InValue);
}

void URPrismaticConstraintComponent::UpdateJointVelocity(float InDeltaT)
{
  if(Child && Parent)
    {
      float ChildMass = Child->GetMass();

      FQuat JointQuat = GetComponentTransform().GetRotation();
      FVector TargetJointVelocity = TargetVelocity * RefAxis;
      TargetJointVelocity = JointQuat.RotateVector(TargetJointVelocity); // Rotation Axis in Global Frame

      FVector COM = Child->GetBodyInstance()->GetCOMPosition();

      FVector ParentLinearVelocity = Parent->GetPhysicsLinearVelocityAtPoint(COM);
      FVector ParentAngularVelocity = Parent->GetPhysicsAngularVelocityInRadians();

      FVector TargetChildLinearVelocity = ParentLinearVelocity + TargetJointVelocity;

      Child->SetPhysicsLinearVelocity(TargetChildLinearVelocity);
      Child->SetPhysicsAngularVelocityInRadians(ParentAngularVelocity);

    }
}

FVector UAngularVelocityToROS(FVector InAngularVelocity)
{
  return FVector(-InAngularVelocity[0], InAngularVelocity[1], -InAngularVelocity[2]);
}

FVector ROSAngularVelocityToU(FVector InAngularVelocity)
{
  return FVector(-InAngularVelocity[0], InAngularVelocity[1], -InAngularVelocity[2]);
}

void URContinuousConstraintComponent::UpdateJointVelocity(float InDeltaT)
{
  if(Child && Parent)
    {
      // float ChildMass = Child->GetMass();

      FVector COM = Child->GetBodyInstance()->GetCOMPosition();
      FVector DistanceJointCOM = COM - GetComponentLocation();
      FVector DistanceJointParent = GetComponentLocation() - Parent->GetBodyInstance()->GetCOMPosition();
      FVector COMParentChild = COM - Parent->GetBodyInstance()->GetCOMPosition();

      FRotator JointRotation = GetComponentRotation();
      FVector TargetJointAngularVelocity = TargetVelocity * RefAxis;
      TargetJointAngularVelocity = ROSAngularVelocityToU(TargetJointAngularVelocity);

      FVector ParentAngularVelocityDegrees = Parent->GetPhysicsAngularVelocityInDegrees();
      FRotator ParentAngularVelocityRotator(-ParentAngularVelocityDegrees.Y, ParentAngularVelocityDegrees.Z, -ParentAngularVelocityDegrees.X);
      ParentAngularVelocityRotator *= InDeltaT;

      FVector TargetJointAngularVelocityWorld = JointRotation.Quaternion().RotateVector(TargetJointAngularVelocity);
      FRotator TargetJointAngularVelocityRotator(-TargetJointAngularVelocityWorld.Y, TargetJointAngularVelocityWorld.Z, -TargetJointAngularVelocityWorld.X);
      TargetJointAngularVelocityRotator *= InDeltaT;

      FVector NewDistanceJointCOM = ParentAngularVelocityRotator.Quaternion().RotateVector(DistanceJointCOM);
      FVector COMTargetVelocity = TargetJointAngularVelocityRotator.Quaternion().RotateVector(NewDistanceJointCOM) - DistanceJointCOM;
      COMTargetVelocity /= InDeltaT;

      FVector NewCOMParentChild = ParentAngularVelocityRotator.Quaternion().RotateVector(COMParentChild) - COMParentChild;
      NewCOMParentChild /= InDeltaT;

      // COMTargetVelocity += NewCOMParentChild;

      // FVector COMTargetVelocityOld = FVector::CrossProduct(TargetJointAngularVelocityWorld, DistanceJointCOM);

      FVector ParentLinearVelocity = Parent->GetPhysicsLinearVelocity();
      if(GetName().Contains("l_elbow_flex"))
        {
          // UE_LOG(LogTemp, Error, TEXT("COMParentChild %s NewCOMParentChild %s"), *COMParentChild.ToString(), *NewCOM.ToString());
          // UE_LOG(LogTemp, Error, TEXT("COM %s ComponenLocation %s"), *COM.ToString(), *Child->GetComponentLocation().ToString());
          // UE_LOG(LogTemp, Error, TEXT("ParentLinearvelocity %s NewCOMParentChild %s"), *COMTargetVelocityOld.ToString(), *COMTargetVelocity.ToString());
        }


      COMTargetVelocity += ParentLinearVelocity;


      FVector ParentAngularVelocity = Parent->GetPhysicsAngularVelocityInRadians();
      Child->SetPhysicsLinearVelocity(COMTargetVelocity);
      Child->SetPhysicsAngularVelocityInRadians(ParentAngularVelocity + TargetJointAngularVelocityWorld);
    }
}


void URConstraintComponent::SetParentChild(UStaticMeshComponent* InParent, UStaticMeshComponent* InChild)
{
  Parent = InParent;
  Child = InChild;
}

void URConstraintComponent::BeginPlay()
{
  Super::BeginPlay();
  if(!Parent)
    {
      Parent = Cast<UStaticMeshComponent>(GetComponentInternal(EConstraintFrame::Frame1));
    }

  if(!Child)
    {
      Child = Cast<UStaticMeshComponent>(GetComponentInternal(EConstraintFrame::Frame2));
    }

  InitChildPoseInJointFrame = GetChildPoseInJointFrame();
  InitChildMeshPoseInJointFrame = Child->GetComponentTransform().GetRelativeTransform(this->GetComponentTransform());
}

void URContinuousConstraintComponent::BeginPlay()
{
  FQuat ParentOrientation = Parent->GetComponentQuat();
  FQuat ChildOrientation = Child->GetComponentQuat();
  QInitial = ParentOrientation.Inverse() * ChildOrientation;
  Super::BeginPlay();
}

void URPrismaticConstraintComponent::BeginPlay()
{
  Super::BeginPlay();
  if(!GetName().Contains("gripper"))
    {
      FVector FramePos = ParentChildDistance + Offset;
      SetConstraintReferencePosition(EConstraintFrame::Type::Frame2, FVector(0.0, 0.0, 0.0));
      SetConstraintReferencePosition(EConstraintFrame::Type::Frame1, FramePos);
    }
}


void URContinuousConstraintComponent::SetTargetPosition(float InTargetPos)
{
  SetAngularOrientationTarget(UKismetMathLibrary::RotatorFromAxisAndAngle(RefAxis, InTargetPos));
}

void URRevoluteConstraintComponent::SetTargetPosition(float InTargetPos)
{
  FRotator Temp;
  if (RefAxis[0]== 1)
    {
      Temp=UKismetMathLibrary::RotatorFromAxisAndAngle(RefAxis, InTargetPos+FMath::RadiansToDegrees(RotationOffset));
    }
  else if (RefAxis[1]== 1)
    {
      Temp=UKismetMathLibrary::RotatorFromAxisAndAngle(RefAxis, InTargetPos-FMath::RadiansToDegrees(RotationOffset));
    }
  else if (RefAxis[2]==1)
    {
      Temp=UKismetMathLibrary::RotatorFromAxisAndAngle(RefAxis, InTargetPos-FMath::RadiansToDegrees(RotationOffset));
    }
  // Temp=UKismetMathLibrary::RotatorFromAxisAndAngle(RefAxis, InTargetPos-FMath::RadiansToDegrees(RotationOffset));
  SetAngularOrientationTarget(Temp);
}

void URContinuousConstraintComponent::SetDrive(const FEnableDrive &EnableDrive)
{
  SetAngularDriveMode(EAngularDriveMode::TwistAndSwing);
  SetAngularDriveParams(EnableDrive.PositionStrength, EnableDrive.VelocityStrength, EnableDrive.MaxForce);
  if (RefAxis.GetAbs().Equals(FVector::ForwardVector))
  {
    SetAngularOrientationDrive(false, EnableDrive.bPositionDrive);
    SetAngularVelocityDrive(false, EnableDrive.bVelocityDrive);
  }
  else
  {
    SetAngularOrientationDrive(EnableDrive.bPositionDrive, false);
    SetAngularVelocityDrive(EnableDrive.bVelocityDrive, false);
  }
}

void URPrismaticConstraintComponent::SetDrive(const FEnableDrive &EnableDrive)
{
  SetLinearDriveParams(EnableDrive.PositionStrength, EnableDrive.VelocityStrength, EnableDrive.MaxForce);
  if (RefAxis.GetAbs().Equals(FVector::ForwardVector))
  {
    SetLinearPositionDrive(EnableDrive.bPositionDrive, false, false);
    SetLinearVelocityDrive(EnableDrive.bVelocityDrive, false, false);
  }
  else if (RefAxis.GetAbs().Equals(FVector::RightVector))
  {
    SetLinearPositionDrive(false, EnableDrive.bPositionDrive, false);
    SetLinearVelocityDrive(false, EnableDrive.bVelocityDrive, false);
  }
  else
  {
    SetLinearPositionDrive(false, false, EnableDrive.bPositionDrive);
    SetLinearVelocityDrive(false, false, EnableDrive.bVelocityDrive);
  }
}

void URFixedConstraintComponent::ConnectToComponents()
{
  RegisterComponent();
  ConstraintActor1 = Parent->GetOwner();
  ConstraintActor2 = Child->GetOwner();
  SetConstrainedComponents(Cast<UPrimitiveComponent>(Parent), NAME_None, Cast<UPrimitiveComponent>(Child), NAME_None);

}

void URPrismaticConstraintComponent::ConnectToComponents()
{
  Super::ConnectToComponents();
  if(Parent && Child)
    {
      ParentChildDistance = Child->GetComponentLocation() - Parent->GetComponentLocation();
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("Parent not found in prismatic"));
    }

}
void URFixedConstraintComponent::SetPosition(USDFJoint* InJoint)
{
}


void URPrismaticConstraintComponent::SetPosition(USDFJoint* InJoint)
{
  Super::SetPosition(InJoint);
}


float URPrismaticConstraintComponent::GetJointPositionInUUnits()
{
  // FVector ParentPosition = Parent->GetComponentLocation();
  // FVector ChildPosition = Child->GetComponentLocation();

  // FVector JointAxis = GetComponentQuat().RotateVector(RefAxis);
  // float JointPosition = FVector::DotProduct(ChildPosition - ParentPosition, JointAxis) / JointAxis.Size() - FVector::DotProduct(ParentChildDistance, RefAxis) / RefAxis.Size();

  // return JointPosition
  Super::GetJointPositionInUUnits();
  return FVector::DotProduct(DeltaPoseInJointFrame.GetLocation(), RefAxis);
}


float URPrismaticConstraintComponent::GetJointPosition()
{
  return FConversions::CmToM(GetJointPositionInUUnits());
}

float URPrismaticConstraintComponent::GetJointVelocity()
{
  FQuat JointQuat = GetComponentTransform().GetRotation();
  FVector GlobalAxis = JointQuat.RotateVector(RefAxis); // Rotation Axis in Global Frame

  FVector ParentAvel = Parent->GetPhysicsLinearVelocity();
  FVector ChildAvel = Child->GetPhysicsLinearVelocity();
  float JointVelocity = FVector::DotProduct(ChildAvel - ParentAvel, GlobalAxis);
  return JointVelocity;
}

float URPrismaticConstraintComponent::GetJointVelocityInUUnits()
{
  //TODO make  this a function in Conversions
  return GetJointVelocity();
}

float URContinuousConstraintComponent::GetConstraintPosition()
{
  if (RefAxis[0]== 1)
    {
      return ConstraintInstance.GetCurrentTwist();
    }
  else if (RefAxis[1]== 1)
    {
      return ConstraintInstance.GetCurrentSwing2();
    }
  else if (RefAxis[2]==1)
    {
      return ConstraintInstance.GetCurrentSwing1();
    }
  return 0.;
}

float URContinuousConstraintComponent::GetJointPosition()
{
  // float ResultAngle = 0.;

  // FTransform ParentTransform = Parent->GetComponentTransform();
  // FTransform ChildTransform = Child->GetComponentTransform();
  // FTransform JointTransform = GetComponentTransform();

  // FTransform RelativeTransform = JointTransform.GetRelativeTransform(ParentTransform);

  // FVector AxisV = RelativeTransform.GetRotation().RotateVector(RefAxis);
  // FQuat QRel = ParentTransform.GetRotation().Inverse() * ChildTransform.GetRotation();
  // FQuat QRelZerod = QRel * QInitial.Inverse();

  // FVector QRelImag(QRelZerod.X, QRelZerod.Y, QRelZerod.Z);
  // float cost2 = QRelZerod.W;
  // float sint2 = FMath::Sqrt(QRelZerod.X * QRelZerod.X + QRelZerod.Y * QRelZerod.Y + QRelZerod.Z * QRelZerod.Z);
  // float theta = (FVector::DotProduct( QRelImag, AxisV) >= 0) ? (2 * FGenericPlatformMath::Atan2(sint2, cost2)) : (2 * FGenericPlatformMath::Atan2(sint2, -cost2));

  // if (theta > PI) theta -= 2*PI;
  // theta = -theta;
  // return theta;
  if (RefAxis.GetAbs().Equals(FVector::UpVector))
  {
    return -FMath::DegreesToRadians(GetJointPositionInUUnits());
  }
  else
  {
    return FMath::DegreesToRadians(GetJointPositionInUUnits());
  }
}

void URContinuousConstraintComponent::SetMotorJointState(float TargetPosition, float TargetJointVelocity)
{
  SetMotorJointStateInUUnits(FMath::RadiansToDegrees(ClampJointStateToConstraintLimit(TargetPosition)), -FMath::RadiansToDegrees(TargetJointVelocity));
}

void URContinuousConstraintComponent::SetMotorJointStateInUUnits(float TargetPosition, float TargetJointVelocity)
{
  SetTargetPosition(TargetPosition);
  // SetAngularOrientationTarget(UKismetMathLibrary::RotatorFromAxisAndAngle(RefAxis, TargetPosition));
  SetAngularVelocityTarget(RefAxis * TargetJointVelocity / 360.f);
  Child->WakeRigidBody();
}

float URContinuousConstraintComponent::GetJointPositionInUUnits()
{
  // float OutVelocity =FMath::RadiansToDegrees(GetJointPosition());
  // return OutVelocity;
  Super::GetJointPositionInUUnits();
  return FVector::DotProduct(DeltaPoseInJointFrame.GetRotation().Euler(), RefAxis);
}

float URContinuousConstraintComponent::GetJointVelocity()
{

  FQuat JointQuat = GetComponentTransform().GetRotation();
  FVector GlobalAxis = JointQuat.RotateVector(RefAxis); // Rotation Axis in Global Frame

  FVector ParentAvel = Parent->GetPhysicsAngularVelocityInRadians();
  FVector ChildAvel = Child->GetPhysicsAngularVelocityInRadians();
  float HingeVel = FVector::DotProduct(ChildAvel - ParentAvel, GlobalAxis);

  return HingeVel;
}

float URContinuousConstraintComponent::GetJointVelocityInUUnits()
{
  float OutVelocity =FMath::RadiansToDegrees(GetJointVelocity());

  return OutVelocity;
}

void URPrismaticConstraintComponent::SetMotorJointState(float TargetPosition, float TargetJointVelocity)
{
  SetMotorJointStateInUUnits(-FConversions::MToCm((float)ClampJointStateToConstraintLimit(TargetPosition)), -FConversions::MToCm((float)TargetJointVelocity));
}

void URPrismaticConstraintComponent::SetMotorJointStateInUUnits(float TargetPosition, float TargetJointVelocity)
{
  SetLinearPositionTarget(RefAxis * TargetPosition + Offset);
  SetLinearVelocityTarget(RefAxis * TargetJointVelocity);
  Child->WakeRigidBody();
}

void URPrismaticConstraintComponent::SetJointPosition(float Angle, FHitResult * OutSweepHitResult)
{
  Child->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);

  FVector DeltaJointLocationInJointFrame = RefAxis * FConversions::MToCm((float)Angle);
  FVector ChildLocationInJointFrame = DeltaJointLocationInJointFrame + InitChildMeshPoseInJointFrame.GetTranslation();
  Child->SetRelativeLocation(ChildLocationInJointFrame);

  Child->AttachToComponent(Parent, FAttachmentTransformRules::KeepWorldTransform);

  // float OldAngle = Encoder->GetValue();
  // FVector RotAxis = GetComponentRotation().Quaternion().RotateVector(RefAxis);
  // FVector DeltaPos = RotAxis * (Angle - OldAngle);
  // Child->AddWorldOffset(DeltaPos * 100, true, OutSweepHitResult, ETeleportType::None);
}

void URPrismaticConstraintComponent::SetJointVelocity(float Velocity)
{
  //Source of JointVelocity is ROS -> m/s convertet into cm/s
  TargetVelocity = Velocity * 100;
}

void URPrismaticConstraintComponent::SetJointEffortFromROS(float InEffort)
{
  //TODO make  this a function in Conversions
  float Effort = InEffort * 100;
  SetJointEffort( Effort);
}

void URPrismaticConstraintComponent::SetJointEffort(float InEffort)
{
  FRotator ParentRotation = Parent->GetComponentRotation();
  FVector RotAxis = ParentRotation.Quaternion().RotateVector(RefAxis);

  Child->AddForce(InEffort*RotAxis);
  Parent->AddForce(-InEffort*RotAxis);
}

void URContinuousConstraintComponent::SetJointPosition(float Angle, FHitResult * OutSweepHitResult)
{
  Child->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);

  FQuat DeltaJointRotationInJointFrame = FQuat(RefAxis, -Angle);
  FQuat ChildRotationInJointFrame = DeltaJointRotationInJointFrame * InitChildMeshPoseInJointFrame.GetRotation();
  Child->SetRelativeRotation(ChildRotationInJointFrame);

  Child->AttachToComponent(Parent, FAttachmentTransformRules::KeepWorldTransform);

  // float OldAngle = Encoder->GetValue();
  // FVector RotAxis = GetComponentRotation().Quaternion().RotateVector(RefAxis);
  // FQuat Rot= FQuat(RotAxis, OldAngle - Angle);
  // Child->AddWorldRotation(Rot,true , OutSweepHitResult, ETeleportType::None);
}


void URContinuousConstraintComponent::SetJointVelocity(float Velocity)
{
  TargetVelocity = Velocity;
}

void URPrismaticConstraintComponent::SetJointVelocityInUUnits(float Velocity)
{
  SetJointVelocity(Velocity);
}

void URPrismaticConstraintComponent::SetTargetPosition(float InTargetPos)
{
}

void URContinuousConstraintComponent::SetJointVelocityInUUnits(float Velocity)
{
  Velocity = FMath::DegreesToRadians(Velocity);
  SetJointVelocity(Velocity);
}

void URContinuousConstraintComponent::SetJointEffort(float Effort)
{
  FRotator ParentRotation = Parent->GetComponentRotation();
  FVector RotAxis = ParentRotation.Quaternion().RotateVector(RefAxis);
  float JointPosition = GetJointPosition();
  Child->AddTorqueInDegrees(Effort*RotAxis);
}

void URContinuousConstraintComponent::SetJointEffortFromROS(float InEffort)
{
  //TODO make  this a function in Conversions
  float Effort = InEffort * 1000;
  SetJointEffort( Effort);
}
