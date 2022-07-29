#include "Controller/ControllerType/BaseController/RBaseController.h"
#include "Math/Quat.h"

URBaseController::URBaseController()
{
  BaseName = TEXT("base_footprint");

  OdomPositionStates.Init(0.0, 3);
  OdomVelocityStates.Init(0.0, 3);
}

void URBaseController::SetControllerParameters(URControllerParameter *&ControllerParameters)
{
  URBaseControllerParameter *BaseControllerParameters = Cast<URBaseControllerParameter>(ControllerParameters);
  if (BaseControllerParameters)
  {
    BaseName = BaseControllerParameters->BaseName;
  }
}

void URBaseController::Init()
{
  Super::Init();

  if (!GetOwner())
  {
    UE_LOG(LogTemp, Error, TEXT("URBaseController not attached to ARModel"));
    }
  else
    {
      URLink* Base = GetOwner()->Links[BaseName];
      Base->SetSimulatePhysics(true);
      Base->SetConstraintMode(EDOFMode::XYPlane);
      TargetPose = Base->GetComponentTransform();
      MaxLinearVelocity = 0.1;
      MaxAngularVelocity = 0.1;
    }
}

void URBaseController::MoveLinear(FVector InVelocity)
{
  LinearVelocity = InVelocity;
}

void URBaseController::Turn(float InVelocity)
{
  AngularVelocity = -InVelocity;
}

void URBaseController::Tick(const float &InDeltaTime)
{
  MoveLinearTick(InDeltaTime);
  TurnTick(InDeltaTime);
  CalculateOdomStates(InDeltaTime);
}

void URBaseController::TurnTick(float InDeltaTime)
{
  URLink* Base = GetOwner()->Links[BaseName];
  FQuat BaseRotation = Base->GetComponentQuat();
  FQuat AngularMotion = FQuat(FVector(0.0f, 0.0f, 1.0f), AngularVelocity * InDeltaTime);
  TargetPose.ConcatenateRotation(AngularMotion);

  float TargetAngle = TargetPose.GetRotation().GetAngle();
  //Normalize the TargetAngle in Interval [-pi, pi]
  while(TargetAngle > PI)
    {
      TargetAngle -= 2* PI;
    }
  while(TargetAngle < -1 * PI)
    {
      TargetAngle += 2* PI;
    }

  TargetAngle *= TargetPose.GetRotation().GetRotationAxis().Z;
  float CurrentAngle = BaseRotation.GetRotationAxis().Z * BaseRotation.GetAngle();

  if (!bIsKinematic)
  {
    float AngularDistance = TargetAngle - CurrentAngle;
    //Normalize the AngularDistance in Interval [-pi, pi]
    while(AngularDistance > PI)
      {
        AngularDistance -= 2* PI;
      }
    while(AngularDistance < -1 * PI)
      {
        AngularDistance += 2* PI;
      }

    FVector NextVel = FVector(0.0f, 0.0f, AngularDistance / InDeltaTime);
    Base->SetPhysicsAngularVelocityInRadians(NextVel * HackRotationFactor);
  }
  else
  {
    float AngularDistance = (TargetAngle - CurrentAngle) / PI * 180;
    Base->AddWorldRotation(FRotator(0.f, FRotator::NormalizeAxis(AngularDistance), 0.f));
  }
}

void URBaseController::MoveLinearTick(float InDeltaTime)
{
  URLink* Base = GetOwner()->Links[BaseName];
  FRotator BaseOrientation = Base->GetComponentRotation();

  //Check if AngularVelocity is 0 to avoid divivision by 0
  if(FMath::Abs(AngularVelocity) > 0.0001f)
    {
      // Calculate the resulting position after one tick by using the Integral of Rx (R = rotation matrix, x = Position https://en.wikipedia.org/wiki/Rotation_matrix)
      // in the intervall of 0 to InDeltaTime
      float Theta0 = FMath::DegreesToRadians(BaseOrientation.Yaw);
      float Theta1 = Theta0 + AngularVelocity * InDeltaTime;
      float dX = (FMath::Sin(Theta1) * LinearVelocity.X  + FMath::Cos(Theta1) * LinearVelocity.Y) / AngularVelocity;
      dX = dX - (FMath::Sin(Theta0) * LinearVelocity.X  + FMath::Cos(Theta0) * LinearVelocity.Y) / AngularVelocity;
      float dY = (-1 * FMath::Cos(Theta1) * LinearVelocity.X  + FMath::Sin(Theta1) * LinearVelocity.Y) / AngularVelocity;
      dY = dY - (-1 * FMath::Cos(Theta0) * LinearVelocity.X  + FMath::Sin(Theta0) * LinearVelocity.Y) / AngularVelocity;
      TargetPose.AddToTranslation(FVector(dX, dY, 0.0f));
    }
  else
    {
      //if the AngularVelocity == 0, rotate the LinearVelocity from Base to World coordinates
      FVector VelocityInBaseCoordinates = BaseOrientation.RotateVector(LinearVelocity);
      TargetPose.AddToTranslation(VelocityInBaseCoordinates * InDeltaTime);
    }

  if (!bIsKinematic)
  {
    //Calculate velocity in order to move from current position to the target position
    FVector NextVel = TargetPose.GetLocation() - Base->GetComponentLocation();

    NextVel /= InDeltaTime;

    Base->SetPhysicsLinearVelocity(NextVel * HackLinearFactor);
  }
  else
  {
    Base->AddWorldOffset(TargetPose.GetLocation() - Base->GetComponentLocation());
  }
}

void URBaseController::CalculateOdomStates(float InDeltaTime)
{
  FVector BasePose =FConversions::UToROS(GetOwner()->GetActorLocation());
  FQuat BaseQuaternion =FConversions::UToROS(GetOwner()->GetActorRotation().Quaternion());
  FRotator BaseRotation = BaseQuaternion.Rotator();

  OdomVelocityStates[0] = (BasePose.X - OdomPositionStates[0])/InDeltaTime;
  OdomVelocityStates[1] = (BasePose.Y - OdomPositionStates[1])/InDeltaTime;
  OdomVelocityStates[2] = (FMath::DegreesToRadians(BaseRotation.Yaw) - OdomPositionStates[2])/InDeltaTime;

  OdomPositionStates[0] = BasePose.X;
  OdomPositionStates[1] = BasePose.Y;
  OdomPositionStates[2] = FMath::DegreesToRadians(BaseRotation.Yaw);
}

TArray<double> URBaseController::GetOdomPositionStates()
{
  return OdomPositionStates;
}

TArray<double> URBaseController::GetOdomVelocityStates()
{
  return OdomVelocityStates;
}

FVector URBaseController::GetLinearVel()
{
  return FVector(OdomVelocityStates[0], OdomVelocityStates[1], 0);  
}
FVector URBaseController::GetAngularVel()
{
  return  FVector(0, 0, OdomVelocityStates[2]);
}

void URBaseController::MoveLinear(FVector InVelocity, float InDeltaTime)
{
  if(InVelocity.Size() != 0.f)
    {
      URLink* Base = GetOwner()->Links[BaseName];
      FRotator BaseOrientation = Base->GetComponentRotation();
      FVector DistanceTraveld = BaseOrientation.Quaternion().RotateVector(InVelocity*InDeltaTime);

      for(auto& Link : GetOwner()->Links)
        {
          AddRelativeLocation(Link.Value, DistanceTraveld);
        }
    }
}

void URBaseController::MoveLinearToWorld(FVector InVelocity, float InDeltaTime)
{
  FVector DistanceTraveld = InVelocity*InDeltaTime;

  for(auto& Link : GetOwner()->Links)
    {
      AddRelativeLocation(Link.Value, DistanceTraveld);
    }
}

void URBaseController::Turn(float InVelocity, float InDeltaTime)
{
  if(InVelocity != 0.f)
    {
      FRotator TestRotation = FRotator(0.0f, InVelocity *InDeltaTime, 0.0f);
      for(auto& Link : GetOwner()->Links)
        {
          AddRelativeRotation(Link.Value, TestRotation);
        }
    }
}

void URBaseController::AddRelativeLocation(URLink* InLink, FVector InPosition)
{
  FVector Position = InLink->GetComponentLocation();
  InLink->SetWorldLocation(InPosition + Position, false, nullptr, ETeleportType::TeleportPhysics);
}

void URBaseController::AddRelativeRotation(URLink* InLink, FRotator InRotator)
{
  URLink* Base = GetOwner()->Links[BaseName];
  FVector BasePosition = Base->GetComponentLocation();
  FRotator Orientation = InLink->GetComponentRotation();
  FVector Position = InLink->GetComponentLocation();

  FQuat NewRot = InRotator.Quaternion() * Orientation.Quaternion() ;
  InLink->SetWorldRotation(NewRot, false, nullptr, ETeleportType::TeleportPhysics);

  FVector LinkBaseOffset = Position - BasePosition;
  FVector NewPosition = InRotator.RotateVector(LinkBaseOffset) + BasePosition;

  InLink->SetWorldLocation(NewPosition, false, nullptr, ETeleportType::TeleportPhysics);
}

void URBaseController::SetLocation(FVector InPosition)
{
  URLink* Base = GetOwner()->Links[BaseName];
  TargetPose.SetLocation(InPosition);
}
void URBaseController::SetRotation(FRotator InRotation)
{
  URLink* Base = GetOwner()->Links[BaseName];
  TargetPose.SetRotation(InRotation.Quaternion());
}

void URBaseController::SetTransform(FTransform InTransform)
{
  SetLocation(InTransform.GetLocation());
  SetRotation(InTransform.GetRotation().Rotator());
}

void URBaseController::SetLocationAndRotation(FVector InPosition, FRotator InRotation)
{
  SetLocation(InPosition);
  SetRotation(InRotation);
}

void URBaseControllerKinematic::TurnTick(float InDeltaTime)
{
  if(AngularVelocity != 0.f)
    {
      FRotator TestRotation = FRotator(0.0f, AngularVelocity *InDeltaTime, 0.0f);
      URLink* Base = GetOwner()->Links[BaseName];
      FRotator BaseOrientation = Base->GetComponentRotation();
      FVector BasePosition = Base->GetComponentLocation();

      FQuat NewRot = TestRotation.Quaternion() * BaseOrientation.Quaternion() ;
      Base->SetWorldRotation(NewRot, false, nullptr, ETeleportType::TeleportPhysics);
    }
}

void URBaseControllerKinematic::MoveLinearTick(float InDeltaTime)
{
  if(LinearVelocity.Size() != 0.f)
    {
      URLink* Base = GetOwner()->Links[BaseName];
      FRotator BaseOrientation = Base->GetComponentRotation();
      FVector DistanceTraveld = BaseOrientation.Quaternion().RotateVector(LinearVelocity*InDeltaTime);

      FVector Position = Base->GetComponentLocation();
      Base->SetWorldLocation(DistanceTraveld + Position, false, nullptr, ETeleportType::TeleportPhysics);
    }
}

void URBaseControllerKinematic::SetLocation(FVector InPosition)
{
  URLink* Base = GetOwner()->Links[BaseName];
  FVector BasePosition = Base->GetComponentLocation();
  FVector DistanceTraveled = InPosition - BasePosition;
  DistanceTraveled.Z = 0;

  AddRelativeLocation(Base, DistanceTraveled);
}

void URBaseControllerKinematic::SetRotation(FRotator InRotation)
{
  URLink* Base = GetOwner()->Links[BaseName];
  FRotator BaseOrientation = Base->GetComponentRotation();
  FRotator NewRotation = InRotation - BaseOrientation;
  NewRotation.Pitch = 0;
  NewRotation.Roll = 0;

  AddRelativeRotation(Base, NewRotation);
}

void URBaseControllerKinematic::Init()
{
  if (!GetOwner())
  {
    UE_LOG(LogTemp, Error, TEXT("URBaseController not attached to ARModel"));
    }
  else
    {
      URLink* Base = GetOwner()->Links[BaseName];
      Base->SetSimulatePhysics(false);
      TargetPose = Base->GetComponentTransform();
    }
}
