#include "RPhysicsConstraintComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Physics/RJoint.h"
// #include "Physics/RJoint.h"

//TODO Get angle returns in left handed system

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

void URPrismaticConstraintComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
  // UpdateJointVelocity(DeltaTime);
}

void URPrismaticConstraintComponent::UpdateJointVelocity()
{
  if(Child && Parent)
    {
      float ChildMass = Child->GetMass();

      FQuat JointQuat = GetComponentTransform().GetRotation();
      FVector TargetJointVelocity = TargetVelocity * RefAxis * 5;
      TargetJointVelocity = JointQuat.RotateVector(TargetJointVelocity); // Rotation Axis in Global Frame

      FVector COM = Child->GetBodyInstance()->GetCOMPosition();

      FVector ParentLinearVelocity = Parent->GetPhysicsLinearVelocityAtPoint(COM);
      FVector ParentAngularVelocity = Parent->GetPhysicsAngularVelocityInRadians();

      FVector TargetChildLinearVelocity = ParentLinearVelocity + TargetJointVelocity;

      Child->NextTickLinearVelocity = TargetChildLinearVelocity;
      Child->NextTickAngularVelocity = ParentAngularVelocity;

      Child->SetPhysicsLinearVelocity(TargetChildLinearVelocity);
      Child->SetPhysicsAngularVelocityInRadians(ParentAngularVelocity);

      // if(GetName().Equals("torso_lift_joint_constraint"))
      // {
      //     UE_LOG(LogTemp, Warning, TEXT("TorsoJointPos: %f"), GetJointPosition());
      // }


      if(Child->IsGravityEnabled())
        {
          Child->AddForce(Child->GetMass() * FVector(0.f,0.f,980.f));
        }
      // Child->UpdateVelocity();
    }
}

void URContinuousConstraintComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// UpdateJointVelocity(DeltaTime);
}

FVector UAngularVelocityToROS(FVector InAngularVelocity)
{
	return FVector(-InAngularVelocity[0], InAngularVelocity[1], -InAngularVelocity[2]);
}

FVector ROSAngularVelocityToU(FVector InAngularVelocity)
{
	return FVector(-InAngularVelocity[0], InAngularVelocity[1], -InAngularVelocity[2]);
}

void URContinuousConstraintComponent::UpdateJointVelocity()
{
  if(Child && Parent)
    {
      float ChildMass = Child->GetMass();

      // FRotator ParentRotation = Parent->GetComponentRotation();
      // FVector TargetJointAngularVelocityWorld = ParentRotation.Quaternion().RotateVector(FMath::DegreesToRadians(TargetVelocity)*RefAxis);

      FRotator JointRotation = GetComponentRotation();
      FVector TargetJointAngularVelocity = TargetVelocity * RefAxis;
      TargetJointAngularVelocity = ROSAngularVelocityToU(TargetJointAngularVelocity);

      FVector TargetJointAngularVelocityWorld = JointRotation.Quaternion().RotateVector(TargetJointAngularVelocity);

      FVector COM = Child->GetBodyInstance()->GetCOMPosition();
      FVector DistanceJointCOM = COM - GetComponentLocation();

      FVector COMTargetVelocity = FVector::CrossProduct(TargetJointAngularVelocityWorld, DistanceJointCOM);
      COMTargetVelocity += Parent->GetPhysicsLinearVelocityAtPoint(COM);

      FVector CurrentParentAngularVelocity = Parent->GetPhysicsAngularVelocityInRadians();

      Child->NextTickLinearVelocity = COMTargetVelocity;
      Child->NextTickAngularVelocity = CurrentParentAngularVelocity + TargetJointAngularVelocityWorld;

      Child->SetPhysicsLinearVelocity(COMTargetVelocity);
      Child->SetPhysicsAngularVelocityInRadians(CurrentParentAngularVelocity + TargetJointAngularVelocityWorld);

      if(Child->IsGravityEnabled())
        {
          Child->AddForce(ChildMass * FVector(0.f,0.f,980.f));
        }
      // Child->UpdateVelocity();
    }
}


void URConstraintComponent::SetParentChild(URStaticMeshComponent* InParent, URStaticMeshComponent* InChild)
{
  Parent = InParent;
  Child = InChild;
}

void URContinuousConstraintComponent::BeginPlay()
{
  JointAccuracy = 0.10;
  FQuat ParentOrientation = Parent->GetComponentQuat();
  FQuat ChildOrientation = Child->GetComponentQuat();
  QInitial = ParentOrientation.Inverse() * ChildOrientation;
  Super::BeginPlay();
}

void URPrismaticConstraintComponent::BeginPlay()
{
  JointAccuracy = 0.01;
  Super::BeginPlay();

  if(GetOuter()->GetName().Equals("torso_lift_joint"))
    {
      SetConstraintReferencePosition(EConstraintFrame::Type::Frame1, FVector(0.0, 0.0, 0.0));
    }

  SetConstraintReferencePosition(EConstraintFrame::Type::Frame2, -ParentChildDistance - Offset);
}


void URContinuousConstraintComponent::SetTargetPosition(float InTargetPos)
{
  SetAngularOrientationTarget(UKismetMathLibrary::RotatorFromAxisAndAngle(RefAxis, InTargetPos));
}

void URRevoluteConstraintComponent::SetTargetPosition(float InTargetPos)
{
  // ConstraintInstance.SetAngularOrientationTarget(FQuat(RefAxis, InTargetPos-RotationOffset));
  FRotator Temp =UKismetMathLibrary::RotatorFromAxisAndAngle(RefAxis, InTargetPos-RotationOffset);
  SetAngularOrientationTarget(Temp);
  // UE_LOG(LogTemp, Warning, TEXT("SetJointTarget for %s:%s %f %f"), *GetName(), *Temp.ToString(), InTargetPos, RotationOffset);
  // ConstraintInstance.SetAngularOrientationTarget(FQuat(RefAxis, InTargetPos-FMath::DegreesToRadians(15)));
  // ConstraintInstance.SetAngularOrientationTarget(FQuat(RefAxis, InTargetPos-RotationOffset));
}

void URContinuousConstraintComponent::EnableMotor(bool InEnable)
{
  SetOrientationDriveTwistAndSwing(InEnable, InEnable);
}

void URPrismaticConstraintComponent::EnableMotor(bool InEnable)
{
  bool bEnableX = false;
  bool bEnableY = false;
  bool bEnableZ = false;
  if(RefAxis[0] == 1)
    {
      bEnableX = InEnable;
    }
  if(RefAxis[1] == 1)
    {
      bEnableY = InEnable;
    }
  if(RefAxis[2] == 1)
    {
      bEnableZ = InEnable;
    }
  SetLinearPositionDrive(bEnableX, bEnableY, bEnableZ);
}

void URFixedConstraintComponent::ConnectToComponents()
{
    // AttachToComponent(Cast<USceneComponent>(Parent), FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
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
	// SetWorldLocation(Cast<UPrimitiveComponent>(Child)->GetComponentLocation());
	// SetWorldRotation(Cast<UPrimitiveComponent>(Parent)->GetComponentRotation());
}


void URPrismaticConstraintComponent::SetPosition(USDFJoint* InJoint)
{
	Super::SetPosition(InJoint);
}


float URPrismaticConstraintComponent::GetJointPositionInUUnits()
{
    FVector ParentPosition = Parent->GetComponentLocation();
    FVector ChildPosition = Child->GetComponentLocation();

    FRotator ParentRotation = Parent->GetComponentRotation();
	FVector JointAxis = GetComponentQuat().RotateVector(RefAxis);
	/* FVector JointAxis = GetComponentQuat().RotateVector(FVector(1.0f, -1.0f, 1.0f) * RefAxis); */
    // float JointPosition = (ParentPosition-ChildPosition+ParentChildDistance)*JointAxis;
	// UE_LOG(LogTemp, Warning, TEXT("ParentLoc: %s, ChildLoc: %s, JointAxis %s"), *ParentPosition.ToString(),*ChildPosition.ToString(), *JointAxis.ToString());
    float JointPosition = ((ParentPosition-ChildPosition)*JointAxis).Size()-(ParentChildDistance*RefAxis).Size();
    return JointPosition;
}


float URPrismaticConstraintComponent::GetJointPosition()
{
	//TODO make  this a function in Conversions
	return GetJointPositionInUUnits()/100.f;
}

float URPrismaticConstraintComponent::GetJointVelocity()
{
    FQuat JointQuat = GetComponentTransform().GetRotation();
    FVector GlobalAxis = JointQuat.RotateVector(RefAxis); // Rotation Axis in Global Frame

    FVector ParentAvel = Parent->GetPhysicsLinearVelocity();
    FVector ChildAvel = Child->GetPhysicsLinearVelocity();
	// UE_LOG(LogTemp, Log, TEXT("ParentVel %s, ChildVel %s"), *RefAxis.ToString(), *GlobalAxis.ToString());
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
	float ResultAngle = 0.;

	FTransform ParentTransform = Parent->GetComponentTransform();
	FTransform ChildTransform = Child->GetComponentTransform();
	FTransform JointTransform = GetComponentTransform();

	FTransform RelativeTransform = JointTransform.GetRelativeTransform(ParentTransform);

	FVector AxisV = RelativeTransform.GetRotation().RotateVector(RefAxis);
	/* FVector AxisV = RelativeTransform.GetRotation().RotateVector(FVector(1.0f, -1.0f, 1.0f) * RefAxis); */
	FQuat QRel = ParentTransform.GetRotation().Inverse() * ChildTransform.GetRotation();
	FQuat QRelZerod = QRel * QInitial.Inverse();

	FVector QRelImag(QRelZerod.X, QRelZerod.Y, QRelZerod.Z);
	float cost2 = QRelZerod.W;
	float sint2 = FMath::Sqrt(QRelZerod.X * QRelZerod.X + QRelZerod.Y * QRelZerod.Y + QRelZerod.Z * QRelZerod.Z);
	float theta = (FVector::DotProduct( QRelImag, AxisV) >= 0) ? (2 * FGenericPlatformMath::Atan2(sint2, cost2)) : (2 * FGenericPlatformMath::Atan2(sint2, -cost2));
	// float theta = (FVector::DotProduct( QRelImag,  FVector(1.0f, -1.0f, 1.0f) *AxisV) >= 0) ? (2 * FGenericPlatformMath::Atan2(sint2, cost2)) : (2 * FGenericPlatformMath::Atan2(sint2, -cost2));

	if (theta > PI) theta -= 2*PI;
	theta = -theta;


	// UE_LOG(LogTemp, Warning, TEXT("2 %s %f || %s"), *QRelZerod.GetRotationAxis().GetSafeNormal().ToString(), QRelZerod.GetAngle(), *QRelZerod.Euler().ToString());
    //
	// // UE_LOG(LogTemp, Warning, TEXT("QImag    %s"), *QRelImag.ToString());
	/* UE_LOG(LogTemp, Warning, TEXT("RefAxis %s"), *RefAxis.ToString()); */
	// UE_LOG(LogTemp, Warning, TEXT("AxisV %s"), *AxisV.ToString());
	/* UE_LOG(LogTemp, Warning, TEXT("%f"), theta); */
    return theta;
}

float URContinuousConstraintComponent::GetJointPositionInUUnits()
{
	float OutVelocity =FMath::RadiansToDegrees(GetJointPosition());
    // while (OutVelocity > 180)
    //     OutVelocity -= 360;
    // while (OutVelocity < -180 )
    //     OutVelocity += 360;
	return OutVelocity;
}

float URContinuousConstraintComponent::GetJointVelocity()
{

    FQuat JointQuat = GetComponentTransform().GetRotation();
    FVector GlobalAxis = JointQuat.RotateVector(RefAxis); // Rotation Axis in Global Frame

    FVector ParentAvel = Parent->GetPhysicsAngularVelocity();
    FVector ChildAvel = Child->GetPhysicsAngularVelocity();
    float HingeVel = FVector::DotProduct(ChildAvel - ParentAvel, GlobalAxis);

    return HingeVel;
}

float URContinuousConstraintComponent::GetJointVelocityInUUnits()
{
	float OutVelocity =FMath::RadiansToDegrees(GetJointVelocity());

	return OutVelocity;
}

void URPrismaticConstraintComponent::SetJointPosition(float Angle)
{}

void URPrismaticConstraintComponent::SetJointVelocity(float Velocity)
{
	// Child = InChild;
	// Parent = InParent;
	TargetVelocity = Velocity;
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

void URContinuousConstraintComponent::SetJointPosition(float Angle)
{
  FRotator ParentRotation = Parent->GetComponentRotation();
  FRotator ChildRotation = Child->GetComponentRotation();

  FVector ParentLocation =Parent->GetComponentLocation();
  FVector ChildLocation =Child->GetComponentLocation();

  FVector RotAxis = ParentRotation.Quaternion().RotateVector(RefAxis);
  FQuat Rot= FQuat(RotAxis, FMath::DegreesToRadians(-Angle));

  FVector ParentChildRelLoc = ChildLocation - ParentLocation;
  FVector NewJointPosition = Rot.RotateVector(ParentChildRelLoc) + ParentLocation;
  FQuat TargetJointAngle =  Rot * ParentRotation.Quaternion();

  // if(GetName().Equals("l_elbow_flex_joint_constraint"))
  // {
  // 	// UE_LOG(LogTemp, Warning, TEXT("Target Joint Angle %s"), *TargetJointAngle.Rotator().ToString());
  // 	// UE_LOG(LogTemp, Warning, TEXT("Current Child Angle %s"), *ChildRotation.ToString());
  // 	UE_LOG(LogTemp, Warning, TEXT("Commanded Angle %f"), Angle);
  // 	UE_LOG(LogTemp, Warning, TEXT("Constraint Component pos %s"), *GetComponentLocation().ToString());

  // }
  // InChild->SetWorldLocation(NewJointPosition);
  Child->SetWorldRotation(TargetJointAngle,true , nullptr, ETeleportType::None);
}


void URContinuousConstraintComponent::SetJointVelocity(float Velocity)
{
	TargetVelocity = Velocity;
}

void URPrismaticConstraintComponent::SetJointVelocityInUUnits(float Velocity)
{
	//TODO wirte code
	SetJointVelocity(Velocity);
}

void URPrismaticConstraintComponent::SetTargetPosition(float InTargetPos)
{
  // SetAngularOrientationTarget(UKismetMathLibrary::RotatorFromAxisAndAngle(RefAxis, InTargetPos));
  // SetLinearPositionTarget(RefAxis * InTargetPos   + ParentChildDistance.ProjectOnTo(RefAxis));
}

void URContinuousConstraintComponent::SetJointVelocityInUUnits(float Velocity)
{
	// float InVelocity = Velocity;
   	// while (InVelocity < 0)
    //     InVelocity += 360;
    // while (InVelocity > 360 )
    //     InVelocity -= 360;
	Velocity = FMath::DegreesToRadians(Velocity);
	SetJointVelocity(Velocity);
}

void URContinuousConstraintComponent::SetJointEffort(float Effort)
{
	//TODO Efforts are not correct or behave different than expected
	FRotator ParentRotation = Parent->GetComponentRotation();
	FVector RotAxis = ParentRotation.Quaternion().RotateVector(RefAxis);
	// InChild->AddAngularImpulseInUUnits(Velocity*RotAxis);
	float JointPosition = GetJointPosition();
	// if(GetName().Equals("l_elbow_flex_joint_constraint"))
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Current Angle %f"), JointPosition);
	// 	UE_LOG(LogTemp, Warning, TEXT("Upper %f"), Upper);
	// 	UE_LOG(LogTemp, Warning, TEXT("Lower %f"), Lower);
	// }

	// UE_LOG(LogTemp, Warning, TEXT("RotAxis %s"), *RotAxis.ToString());
	Child->AddTorqueInDegrees(Effort*RotAxis);
	// InParent->AddTorqueInUUnits(-Effort*RotAxis);
	// if(JointPosition<Upper-5.1 && JointPosition>Lower+5.1)
	// {
	// 	InChild->AddTorqueInUUnits(Effort*RotAxis);
	// 	InChild->AddTorqueInUUnits(-Effort*RotAxis);
	// }
	// else if(JointPosition>Upper-5.1 && Effort < 0 )
	// {
	// 	InChild->AddTorqueInUUnits(Effort*RotAxis);
	// 	InChild->AddTorqueInUUnits(-Effort*RotAxis);
	// }
	// else if(JointPosition<Lower+5.1 && Effort > 0 )
	// {
	// 	InChild->AddTorqueInUUnits(Effort*RotAxis);
	// 	InChild->AddTorqueInUUnits(-Effort*RotAxis);
	// }

}

void URContinuousConstraintComponent::SetJointEffortFromROS(float InEffort)
{
	//TODO make  this a function in Conversions
	float Effort = InEffort * 1000;
	SetJointEffort( Effort);

}
