#include "RPhysicsConstraintComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Physics/RJoint.h"

float URContinuousConstraintComponent::CheckPositionRange(float InTargetJointPos)
{
  bool bNormalized = false;
  while (!bNormalized)
  {
    if (InTargetJointPos > PI)
    {
      InTargetJointPos -= 2 * PI;
    }
    else if (InTargetJointPos < -PI)
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
  if (Child && Parent)
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
  if (Child && Parent)
  {
    float ChildMass = Child->GetMass();

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
  if (!GetName().Contains("gripper"))
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
  FRotator Temp = UKismetMathLibrary::RotatorFromAxisAndAngle(RefAxis, InTargetPos - RotationOffset);
  SetAngularOrientationTarget(Temp);
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
  if (RefAxis[0] == 1)
  {
    bEnableX = InEnable;
  }
  if (RefAxis[1] == 1)
  {
    bEnableY = InEnable;
  }
  if (RefAxis[2] == 1)
  {
    bEnableZ = InEnable;
  }
  SetLinearPositionDrive(bEnableX, bEnableY, bEnableZ);
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
  if (Parent && Child)
  {
    ParentChildDistance = Child->GetComponentLocation() - Parent->GetComponentLocation();
    UE_LOG(LogTemp, Error, TEXT("JointName %s ParentChildDistance %s"), *GetName(), *ParentChildDistance.ToString());
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
  FVector ParentPosition = Parent->GetComponentLocation();
  FVector ChildPosition = Child->GetComponentLocation();

  FRotator ParentRotation = Parent->GetComponentRotation();
  FVector JointAxis = GetComponentQuat().RotateVector(RefAxis);
  float JointPosition = ((ParentPosition - ChildPosition) * JointAxis).Size() - (ParentChildDistance * RefAxis).Size();
  return JointPosition;
}

float URPrismaticConstraintComponent::GetJointPosition()
{
  //TODO make  this a function in Conversions
  float Pos = GetJointPositionInUUnits() / 100.f;
  return Pos;
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
  if (RefAxis[0] == 1)
  {
    return ConstraintInstance.GetCurrentTwist();
  }
  else if (RefAxis[1] == 1)
  {
    return ConstraintInstance.GetCurrentSwing2();
  }
  else if (RefAxis[2] == 1)
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
  FQuat QRel = ParentTransform.GetRotation().Inverse() * ChildTransform.GetRotation();
  FQuat QRelZerod = QRel * QInitial.Inverse();

  FVector QRelImag(QRelZerod.X, QRelZerod.Y, QRelZerod.Z);
  float cost2 = QRelZerod.W;
  float sint2 = FMath::Sqrt(QRelZerod.X * QRelZerod.X + QRelZerod.Y * QRelZerod.Y + QRelZerod.Z * QRelZerod.Z);
  float theta = (FVector::DotProduct(QRelImag, AxisV) >= 0) ? (2 * FGenericPlatformMath::Atan2(sint2, cost2)) : (2 * FGenericPlatformMath::Atan2(sint2, -cost2));

  if (theta > PI)
    theta -= 2 * PI;
  theta = -theta;
  return theta;
}

float URContinuousConstraintComponent::GetJointPositionInUUnits()
{
  float OutVelocity = FMath::RadiansToDegrees(GetJointPosition());
  return OutVelocity;
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
  float OutVelocity = FMath::RadiansToDegrees(GetJointVelocity());

  return OutVelocity;
}

void URPrismaticConstraintComponent::SetJointPosition(float Angle, FHitResult* OutSweepHitResult)
{
  float OldAngle = Encoder->GetValue();
  FVector RotAxis = GetComponentRotation().Quaternion().RotateVector(RefAxis);
  FVector DeltaPos = RotAxis * (Angle - OldAngle);
  Child->AddWorldOffset(DeltaPos * 100, true, OutSweepHitResult, ETeleportType::None);
}

void URPrismaticConstraintComponent::SetJointVelocity(float Velocity)
{
  TargetVelocity = Velocity;
}

void URPrismaticConstraintComponent::SetJointEffortFromROS(float InEffort)
{
  //TODO make  this a function in Conversions
  float Effort = InEffort * 100;
  SetJointEffort(Effort);
}

void URPrismaticConstraintComponent::SetJointEffort(float InEffort)
{
  FRotator ParentRotation = Parent->GetComponentRotation();
  FVector RotAxis = ParentRotation.Quaternion().RotateVector(RefAxis);

  Child->AddForce(InEffort * RotAxis);
  Parent->AddForce(-InEffort * RotAxis);
}

void URContinuousConstraintComponent::SetJointPosition(float Angle, FHitResult* OutSweepHitResult)
{
  float OldAngle = Encoder->GetValue();
  FVector RotAxis = GetComponentRotation().Quaternion().RotateVector(RefAxis);
  FQuat Rot = FQuat(RotAxis, OldAngle - Angle);
  Child->AddWorldRotation(Rot, true, OutSweepHitResult, ETeleportType::None);
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
  Child->AddTorqueInDegrees(Effort * RotAxis);
}

void URContinuousConstraintComponent::SetJointEffortFromROS(float InEffort)
{
  //TODO make  this a function in Conversions
  float Effort = InEffort * 1000;
  SetJointEffort(Effort);
}
