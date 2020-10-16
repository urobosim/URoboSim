#include "Controller/RBaseController.h"
#include "Math/Quat.h"

URBaseController::URBaseController()
{
  BaseName = TEXT("base_footprint");

  OdomPositionStates.Add(0);
  OdomPositionStates.Add(0);
  OdomPositionStates.Add(0);

  WheelSetting.WheelVelocities.Add(0);
  WheelSetting.WheelVelocities.Add(0);
  WheelSetting.WheelVelocities.Add(0);
  WheelSetting.WheelVelocities.Add(0);

  OdomVelocityStates.Add(0);
  OdomVelocityStates.Add(0);
  OdomVelocityStates.Add(0);
}

void URBaseController::Init(ARModel* InModel)
{
  if(!InModel)
    {
      UE_LOG(LogTemp, Error, TEXT("RobotComandsComponent not attached to ARModel"));
    }
  else
    {
      Model = InModel;
      URLink* Base = Model->Links[BaseName];
      // Base->GetCollision()->SetSimulatePhysics(false);
      Base->GetCollision()->SetConstraintMode(EDOFMode::XYPlane);
      TargetPose = Base->GetCollision()->GetComponentTransform();
      MaxLinearVelocity = 0.5;
      MaxAngularVelocity = 0.5;
    }
}

void URBaseController::MoveLinear(FVector InVelocity)
{
  LinearVelocity = InVelocity;
  if(MaxLinearVelocity < InVelocity.Size())
    {
      MaxLinearVelocity = InVelocity.Size();
    }
}

void URBaseController::Turn(float InVelocity)
{
  AngularVelocity = -InVelocity;

  if(MaxAngularVelocity < FMath::Abs(AngularVelocity))
    {
      MaxAngularVelocity = FMath::Abs(AngularVelocity);
    }
}

void URBaseController::Tick(float InDeltaTime)
{
  MoveLinearTick(InDeltaTime);
  TurnTick(InDeltaTime);
  CalculateOdomStates(InDeltaTime);
  MoveWheelTick(InDeltaTime);
}

void URBaseController::TurnTick(float InDeltaTime)
{
  URLink* Base = Model->Links[BaseName];
  FQuat BaseRotation = Base->GetCollision()->GetComponentQuat();
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
  if(NextVel.Size() > MaxAngularVelocity)
    {
      NextVel = NextVel.GetClampedToMaxSize(MaxAngularVelocity);
    }
  Base->GetCollision()->SetPhysicsAngularVelocityInRadians(NextVel);
}

void URBaseController::MoveLinearTick(float InDeltaTime)
{
  URLink* Base = Model->Links[BaseName];
  FRotator BaseOrientation = Base->GetCollision()->GetComponentRotation();

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

  //Calculate velocity in order to move from current position to the target position
  FVector NextVel = TargetPose.GetLocation() - Base->GetCollision()->GetComponentLocation();
  NextVel /= InDeltaTime;

  Base->GetCollision()->SetPhysicsLinearVelocity(NextVel);
}

void URBaseController::CalculateOdomStates(float InDeltaTime)
{
  FVector BasePose =FConversions::UToROS(Model->GetActorLocation());
  FQuat BaseQuaternion =FConversions::UToROS(Model->GetActorRotation().Quaternion());
  FRotator BaseRotation = BaseQuaternion.Rotator();

  OdomVelocityStates[0] = (BasePose.X - OdomPositionStates[0])/InDeltaTime;
  OdomVelocityStates[1] = (BasePose.Y - OdomPositionStates[1])/InDeltaTime;
  OdomVelocityStates[2] = (FMath::DegreesToRadians(BaseRotation.Yaw) - OdomPositionStates[2])/InDeltaTime;

  OdomPositionStates[0] = BasePose.X;
  OdomPositionStates[1] = BasePose.Y;
  OdomPositionStates[2] = FMath::DegreesToRadians(BaseRotation.Yaw);
}

void URBaseController::MoveWheelTick(float InDeltaTime)
{
  if (Model->Links.Contains(WheelSetting.WheelLeftUp) &&
      Model->Links.Contains(WheelSetting.WheelLeftDown) &&
      Model->Links.Contains(WheelSetting.WheelRightUp) &&
      Model->Links.Contains(WheelSetting.WheelRightDown))
      {
        WheelSetting.WheelVelocities[0] = (LinearVelocity.X + LinearVelocity.Y + WheelSetting.WheelToCenterSum * AngularVelocity) / WheelSetting.WheelRadius;
        WheelSetting.WheelVelocities[1] = (LinearVelocity.X - LinearVelocity.Y - WheelSetting.WheelToCenterSum * AngularVelocity) / WheelSetting.WheelRadius;
        WheelSetting.WheelVelocities[2] = (LinearVelocity.X - LinearVelocity.Y + WheelSetting.WheelToCenterSum * AngularVelocity) / WheelSetting.WheelRadius;
        WheelSetting.WheelVelocities[3] = (LinearVelocity.X + LinearVelocity.Y - WheelSetting.WheelToCenterSum * AngularVelocity) / WheelSetting.WheelRadius;

        FVector RotationAxis = Model->Links[BaseName]->GetCollision()->GetComponentQuat().GetAxisY();
        Model->Links[WheelSetting.WheelLeftUp]->GetCollision()->SetPhysicsAngularVelocityInRadians(RotationAxis * WheelSetting.WheelVelocities[0]);
        Model->Links[WheelSetting.WheelRightUp]->GetCollision()->SetPhysicsAngularVelocityInRadians(RotationAxis * WheelSetting.WheelVelocities[1]);
        Model->Links[WheelSetting.WheelLeftDown]->GetCollision()->SetPhysicsAngularVelocityInRadians(RotationAxis * WheelSetting.WheelVelocities[2]);
        Model->Links[WheelSetting.WheelRightDown]->GetCollision()->SetPhysicsAngularVelocityInRadians(RotationAxis * WheelSetting.WheelVelocities[3]);
      }
}

TArray<double> URBaseController::GetOdomPositionStates()
{
  return OdomPositionStates;
}

TArray<double> URBaseController::GetOdomVelocityStates()
{
  return OdomVelocityStates;
}

void URBaseController::MoveLinear(FVector InVelocity, float InDeltaTime)
{
  if(InVelocity.Size() != 0.f)
    {
      URLink* Base = Model->Links[BaseName];
      FRotator BaseOrientation = Base->GetCollision()->GetComponentRotation();
      FVector DistanceTraveld = BaseOrientation.Quaternion().RotateVector(InVelocity*InDeltaTime);

      for(auto& Link : Model->Links)
        {
          AddRelativeLocation(Link.Value, DistanceTraveld);
        }
    }
}

void URBaseController::MoveLinearToWorld(FVector InVelocity, float InDeltaTime)
{
  FVector DistanceTraveld = InVelocity*InDeltaTime;

  for(auto& Link : Model->Links)
    {
      AddRelativeLocation(Link.Value, DistanceTraveld);
    }
}

void URBaseController::Turn(float InVelocity, float InDeltaTime)
{
  if(InVelocity != 0.f)
    {
      FRotator TestRotation = FRotator(0.0f, InVelocity *InDeltaTime, 0.0f);
      for(auto& Link : Model->Links)
        {
          AddRelativeRotation(Link.Value, TestRotation);
        }
    }
}

void URBaseController::AddRelativeLocation(URLink* InLink, FVector InPosition)
{
  FVector Position = InLink->GetCollision()->GetComponentLocation();
  InLink->GetCollision()->SetWorldLocation(InPosition + Position, false, nullptr, ETeleportType::TeleportPhysics);
}

void URBaseController::AddRelativeRotation(URLink* InLink, FRotator InRotator)
{
  URLink* Base = Model->Links[BaseName];
  FRotator BaseOrientation = Base->GetCollision()->GetComponentRotation();
  FVector BasePosition = Base->GetCollision()->GetComponentLocation();
  FRotator Orientation = InLink->GetCollision()->GetComponentRotation();
  FVector Position = InLink->GetCollision()->GetComponentLocation();

  FQuat NewRot = InRotator.Quaternion() * Orientation.Quaternion() ;
  InLink->GetCollision()->SetWorldRotation(NewRot, false, nullptr, ETeleportType::TeleportPhysics);

  FVector LinkBaseOffset = Position - BasePosition;
  FVector NewPosition = InRotator.RotateVector(LinkBaseOffset) + BasePosition;

  InLink->GetCollision()->SetWorldLocation(NewPosition, false, nullptr, ETeleportType::TeleportPhysics);
}

void URBaseController::SetLocation(FVector InPosition)
{
  URLink* Base = Model->Links[BaseName];
  FVector BasePosition = Base->GetCollision()->GetComponentLocation();
  FVector DistanceTraveld = InPosition - BasePosition;

  for(auto& Link : Model->Links)
    {
      AddRelativeLocation(Link.Value, DistanceTraveld);
    }
}
void URBaseController::SetRotation(FRotator InRotation)
{
  URLink* Base = Model->Links[BaseName];
  FRotator BaseOrientation = Base->GetCollision()->GetComponentRotation();
  FRotator NewRotation = InRotation - BaseOrientation;
  NewRotation.Pitch = 0;
  NewRotation.Roll = 0;

  UE_LOG(LogTemp, Log, TEXT("BaseOrientation %s, DesRotation %s, Delta %s"), *BaseOrientation.ToString(), *InRotation.ToString(), *NewRotation.ToString())
  for(auto& Link : Model->Links)
    {
      AddRelativeRotation(Link.Value, NewRotation);
    }
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
