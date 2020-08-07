#include "Controller/RBaseController.h"

URBaseController::URBaseController()
{
  BaseName = TEXT("base_footprint");

  this->OdomPositionStates.Add(0);
  this->OdomPositionStates.Add(0);
  this->OdomPositionStates.Add(0);

  this->OdomVelocityStates.Add(0);
  this->OdomVelocityStates.Add(0);
  this->OdomVelocityStates.Add(0);
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

void URBaseController::Tick(float InDeltaTime)
{
  TurnTick(InDeltaTime);
  MoveLinearTick(InDeltaTime);
  CalculateOdomStates(InDeltaTime);
}

void URBaseController::TurnTick(float InDeltaTime)
{
  FQuat AngularMotion = FQuat(FVector(0.0f, 0.0f, 1.0f), AngularVelocity * InDeltaTime);
  URLink* Base = Model->Links[BaseName];
  TargetPose.ConcatenateRotation(AngularMotion);
  float AngularDistance = TargetPose.GetRotation().AngularDistance(Base->GetCollision()->GetComponentQuat());
  FVector NextVel = FVector(0.0f, 0.0f, AngularDistance / InDeltaTime);
  float AngularVelocityAbs = FMath::Abs(AngularVelocity);
  if(NextVel.Size() > 1.2 * AngularVelocityAbs)
    {
      NextVel = NextVel.GetClampedToMaxSize(1.2 * AngularVelocityAbs);
    }
  Base->GetCollision()->SetPhysicsAngularVelocityInRadians(FMath::Sign(AngularVelocity) * NextVel);
}

void URBaseController::MoveLinearTick(float InDeltaTime)
{
  URLink* Base = Model->Links[BaseName];
  FRotator BaseOrientation = Base->GetCollision()->GetComponentRotation();
  FVector VelocityInBaseCoordinates = BaseOrientation.Quaternion().RotateVector(LinearVelocity);

  TargetPose.AddToTranslation(VelocityInBaseCoordinates * InDeltaTime);

  FVector NextVel = TargetPose.GetLocation() - Base->GetCollision()->GetComponentLocation();
  NextVel /= InDeltaTime;
  if(NextVel.Size() > 1.2 * LinearVelocity.Size())
    {
      NextVel = NextVel.GetClampedToMaxSize(1.2 * LinearVelocity.Size());
    }


  Base->GetCollision()->SetPhysicsLinearVelocity(NextVel);
}

void URBaseController::CalculateOdomStates(float InDeltaTime)
{
  TArray<double> OdomPositionStatesOld = OdomPositionStates;

  FVector BasePose =FConversions::UToROS(Model->GetActorLocation());
  FQuat BaseQuaternion =FConversions::UToROS(Model->GetActorRotation().Quaternion());
  FRotator BaseRotation = BaseQuaternion.Rotator();

  OdomPositionStates[0]=BasePose.X;
  OdomPositionStates[1]=BasePose.Y;
  OdomPositionStates[2]=FMath::DegreesToRadians(BaseRotation.Yaw);

  double c_phi = cos(OdomPositionStates[2]);
  double s_phi = sin(OdomPositionStates[2]);
  double v_x = (OdomPositionStates[0] - OdomPositionStatesOld[0])/InDeltaTime;
  double v_y = (OdomPositionStates[1] - OdomPositionStatesOld[1])/InDeltaTime;

  OdomVelocityStates[0] = v_x * c_phi + v_y * s_phi;
  OdomVelocityStates[1] = -v_x * s_phi + v_y * c_phi;
  OdomVelocityStates[2] = (OdomPositionStates[2] - OdomPositionStatesOld[2])/InDeltaTime;
}

TArray<double> URBaseController::GetOdomPositionStates()
{
  return this->OdomPositionStates;
}

TArray<double> URBaseController::GetOdomVelocityStates()
{
  return this->OdomVelocityStates;
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
          // FVector Position = Link.Value->GetCollision()->GetComponentLocation();
          // Link.Value->GetCollision()->SetWorldLocation(DistanceTraveld + Position, false, nullptr, ETeleportType::TeleportPhysics);
          AddRelativeLocation(Link.Value, DistanceTraveld);
        }
    }
}

void URBaseController::MoveLinearToWorld(FVector InVelocity, float InDeltaTime)
{
  FVector DistanceTraveld = InVelocity*InDeltaTime;

  for(auto& Link : Model->Links)
    {
      // FVector Position = Link.Value->GetCollision()->GetComponentLocation();
      // Link.Value->GetCollision()->SetWorldLocation(DistanceTraveld + Position, false, nullptr, ETeleportType::TeleportPhysics);
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
