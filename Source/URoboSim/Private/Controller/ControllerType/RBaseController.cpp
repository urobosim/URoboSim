#include "Controller/ControllerType/RBaseController.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogRBaseController, Log, All)

void URBaseController::Init()
{
  Super::Init();

  if (GetOwner())
  {
    BaseLink = GetOwner()->GetBaseLink();
    if (BaseLink)
    {
      BaseLink->GetCollisionMeshes()[0]->SetConstraintMode(EDOFMode::XYPlane);
      TargetPose = BaseLink->GetCollisionMeshes()[0]->GetComponentTransform();
    }
  }
  else
  {
    UE_LOG(LogRBaseController, Error, TEXT("%s is not attached to ARModel"), *GetName())
  }
}

void URBaseController::MoveLinear(const FVector &InVelocity)
{
  LinearVelocity = InVelocity;
}

void URBaseController::MoveAngular(const float &InVelocity)
{
  AngularVelocity = InVelocity;
}

void URBaseController::Tick(const float &DeltaTime)
{
  if (BaseLink)
  {
    BasePose = BaseLink->GetCollisionMeshes()[0]->GetComponentTransform();
    MoveLinearTick(DeltaTime);
    MoveAngularTick(DeltaTime);
  }
}

void URBaseController::MoveLinearTick(const float &DeltaTime)
{
  // Check if AngularVelocity is 0 to avoid division by 0
  if (FMath::Abs(AngularVelocity) > 1E-6f)
  {
    // Calculate the resulting position after one tick by using the Integral of Rx (R = rotation matrix, x = Position https://en.wikipedia.org/wiki/Rotation_matrix)
    // in the interval of 0 to DeltaTime
    float Theta0 = FMath::DegreesToRadians(BasePose.GetRotation().Rotator().Yaw);
    float Theta1 = Theta0 + AngularVelocity * DeltaTime;
    float VelX = (FMath::Sin(Theta1) * LinearVelocity.X + FMath::Cos(Theta1) * LinearVelocity.Y) / AngularVelocity - (FMath::Sin(Theta0) * LinearVelocity.X + FMath::Cos(Theta0) * LinearVelocity.Y) / AngularVelocity;
    float VelY = (-FMath::Cos(Theta1) * LinearVelocity.X + FMath::Sin(Theta1) * LinearVelocity.Y) / AngularVelocity - (-FMath::Cos(Theta0) * LinearVelocity.X + FMath::Sin(Theta0) * LinearVelocity.Y) / AngularVelocity;
    TargetPose.AddToTranslation(FVector(VelX, VelY, 0.f));
  }
  else
  {
    // if the AngularVelocity == 0, rotate the LinearVelocity from Base to World frame
    FVector VelocityInBaseFrame = BasePose.GetRotation().RotateVector(LinearVelocity);
    TargetPose.AddToTranslation(VelocityInBaseFrame * DeltaTime);
  }
  // Calculate velocity in order to move from current position to the target position
  BaseLink->GetCollisionMeshes()[0]->SetPhysicsLinearVelocity((TargetPose.GetLocation() - BasePose.GetLocation()) / DeltaTime);
}

void URBaseController::MoveAngularTick(const float &DeltaTime)
{
  FRotator DeltaRotation = UKismetMathLibrary::RotatorFromAxisAndAngle(BasePose.GetRotation().GetAxisZ(), AngularVelocity * DeltaTime);
  TargetPose.ConcatenateRotation(DeltaRotation.Quaternion());
  float TargetAngle = FRotator::NormalizeAxis(TargetPose.GetRotation().Rotator().Yaw);
  float CurrentAngle = FRotator::NormalizeAxis(BasePose.GetRotation().Rotator().Yaw);

  BaseLink->GetCollisionMeshes()[0]->SetPhysicsAngularVelocityInDegrees(FVector::UpVector * FRotator::NormalizeAxis(TargetAngle - CurrentAngle) / DeltaTime);
}