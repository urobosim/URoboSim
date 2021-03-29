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
  }
  MoveLinearTick(DeltaTime);
  MoveAngularTick(DeltaTime);
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

// void URBaseController::MoveLinear(FVector InVelocity, float InDeltaTime)
// {
//   if (InVelocity.Size() != 0.f)
//   {
//     URLink *Base = GetOwner()->Links[BaseName];
//     FRotator BaseOrientation = Base->GetCollision()->GetComponentRotation();
//     FVector DistanceTraveld = BaseOrientation.Quaternion().RotateVector(InVelocity * InDeltaTime);

//     for (auto &Link : GetOwner()->Links)
//     {
//       AddRelativeLocation(Link.Value, DistanceTraveld);
//     }
//   }
// }

// void URBaseController::MoveLinearToWorld(FVector InVelocity, float InDeltaTime)
// {
//   FVector DistanceTraveld = InVelocity * InDeltaTime;

//   for (auto &Link : GetOwner()->Links)
//   {
//     AddRelativeLocation(Link.Value, DistanceTraveld);
//   }
// }

// void URBaseController::Turn(float InVelocity, float InDeltaTime)
// {
//   if (InVelocity != 0.f)
//   {
//     FRotator TestRotation = FRotator(0.0f, InVelocity * InDeltaTime, 0.0f);
//     for (auto &Link : GetOwner()->Links)
//     {
//       AddRelativeRotation(Link.Value, TestRotation);
//     }
//   }
// }

// void URBaseController::AddRelativeLocation(URLink *InLink, FVector InPosition)
// {
//   FVector Position = InLink->GetCollision()->GetComponentLocation();
//   InLink->GetCollision()->SetWorldLocation(InPosition + Position, false, nullptr, ETeleportType::TeleportPhysics);
// }

// void URBaseController::AddRelativeRotation(URLink *InLink, FRotator InRotator)
// {
//   URLink *Base = GetOwner()->Links[BaseName];
//   FRotator BaseOrientation = Base->GetCollision()->GetComponentRotation();
//   FVector BasePosition = Base->GetCollision()->GetComponentLocation();
//   FRotator Orientation = InLink->GetCollision()->GetComponentRotation();
//   FVector Position = InLink->GetCollision()->GetComponentLocation();

//   FQuat NewRot = InRotator.Quaternion() * Orientation.Quaternion();
//   InLink->GetCollision()->SetWorldRotation(NewRot, false, nullptr, ETeleportType::TeleportPhysics);

//   FVector LinkBaseOffset = Position - BasePosition;
//   FVector NewPosition = InRotator.RotateVector(LinkBaseOffset) + BasePosition;

//   InLink->GetCollision()->SetWorldLocation(NewPosition, false, nullptr, ETeleportType::TeleportPhysics);
// }

// void URBaseController::SetLocation(FVector InPosition)
// {
//   URLink *Base = GetOwner()->Links[BaseName];
//   // FVector BasePosition = Base->GetCollision()->GetComponentLocation();
//   // FVector DistanceTraveld = InPosition - BasePosition;

//   TargetPose.SetLocation(InPosition);
//   // for(auto& Link : GetOwner()->Links)
//   //   {
//   //     AddRelativeLocation(Link.Value, DistanceTraveld);
//   //   }
// }
// void URBaseController::SetRotation(FRotator InRotation)
// {
//   URLink *Base = GetOwner()->Links[BaseName];
//   TargetPose.SetRotation(InRotation.Quaternion());
//   // FRotator BaseOrientation = Base->GetCollision()->GetComponentRotation();
//   // FRotator NewRotation = InRotation - BaseOrientation;
//   // NewRotation.Pitch = 0;
//   // NewRotation.Roll = 0;

//   // UE_LOG(LogTemp, Log, TEXT("BaseOrientation %s, DesRotation %s, Delta %s"), *BaseOrientation.ToString(), *InRotation.ToString(), *NewRotation.ToString())
//   // for(auto& Link : GetOwner()->Links)
//   //   {
//   //     AddRelativeRotation(Link.Value, NewRotation);
//   //   }
// }

// void URBaseController::SetTransform(FTransform InTransform)
// {
//   SetLocation(InTransform.GetLocation());
//   SetRotation(InTransform.GetRotation().Rotator());
// }

// void URBaseController::SetLocationAndRotation(FVector InPosition, FRotator InRotation)
// {
//   SetLocation(InPosition);
//   SetRotation(InRotation);
// }

// void URBaseControllerKinematic::TurnTick(float InDeltaTime)
// {
//   if (AngularVelocity != 0.f)
//   {
//     FRotator TestRotation = FRotator(0.0f, AngularVelocity * InDeltaTime, 0.0f);
//     URLink *Base = GetOwner()->Links[BaseName];
//     FRotator BaseOrientation = Base->GetCollision()->GetComponentRotation();
//     FVector BasePosition = Base->GetCollision()->GetComponentLocation();

//     for (auto &Link : GetOwner()->Links)
//     {
//       FRotator Orientation = Link.Value->GetCollision()->GetComponentRotation();
//       FVector Position = Link.Value->GetCollision()->GetComponentLocation();

//       FQuat NewRot = TestRotation.Quaternion() * Orientation.Quaternion();

//       FVector LinkBaseOffset = Position - BasePosition;
//       FVector NewPosition = TestRotation.RotateVector(LinkBaseOffset) + BasePosition;

//       Link.Value->GetCollision()->SetWorldLocationAndRotation(NewPosition, NewRot, false, nullptr, ETeleportType::None);
//     }
//   }
// }

// void URBaseControllerKinematic::MoveLinearTick(float InDeltaTime)
// {
//   if (LinearVelocity.Size() != 0.f)
//   {
//     URLink *Base = GetOwner()->Links[BaseName];
//     FRotator BaseOrientation = Base->GetCollision()->GetComponentRotation();
//     FVector DistanceTraveld = BaseOrientation.Quaternion().RotateVector(LinearVelocity * InDeltaTime);

//     for (auto &Link : GetOwner()->Links)
//     {
//       FVector Position = Link.Value->GetCollision()->GetComponentLocation();
//       Link.Value->GetCollision()->SetWorldLocation(DistanceTraveld + Position, false, nullptr, ETeleportType::TeleportPhysics);
//     }
//   }
// }

// void URBaseControllerKinematic::SetLocation(FVector InPosition)
// {
//   URLink *Base = GetOwner()->Links[BaseName];
//   FVector BasePosition = Base->GetCollision()->GetComponentLocation();
//   FVector DistanceTraveled = InPosition - BasePosition;
//   DistanceTraveled.Z = 0;

//   AddRelativeLocation(Base, DistanceTraveled);
//   // TargetPose.SetLocation(InPosition);
//   // for(auto& Link : GetOwner()->Links)
//   //   {
//   //     AddRelativeLocation(Link.Value, DistanceTraveld);
//   //   }
// }

// void URBaseControllerKinematic::SetRotation(FRotator InRotation)
// {
//   URLink *Base = GetOwner()->Links[BaseName];
//   // TargetPose.SetRotation(InRotation.Quaternion());
//   FRotator BaseOrientation = Base->GetCollision()->GetComponentRotation();
//   FRotator NewRotation = InRotation - BaseOrientation;
//   NewRotation.Pitch = 0;
//   NewRotation.Roll = 0;

//   // UE_LOG(LogTemp, Log, TEXT("BaseOrientation %s, DesRotation %s, Delta %s"), *BaseOrientation.ToString(), *InRotation.ToString(), *NewRotation.ToString());
//   AddRelativeRotation(Base, NewRotation);
//   // for(auto& Link : GetOwner()->Links)
//   //   {
//   //     AddRelativeRotation(Link.Value, NewRotation);
//   //   }
// }
