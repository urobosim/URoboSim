#include "Controller/ROmniwheelController.h"

UROmniwheelController::UROmniwheelController()
{
  BaseName = TEXT("base_footprint");

  OdomPositionStates.Init(0.0, 3);
  WheelSetting.WheelVelocities.Init(0.0, 4);
  OdomVelocityStates.Init(0.0, 3);
}

void UROmniwheelController::Tick(float InDeltaTime)
{
  MoveLinearTick(InDeltaTime);
  TurnTick(InDeltaTime);
  CalculateOdomStates(InDeltaTime);
  MoveWheelTick(InDeltaTime);
}

void UROmniwheelController::MoveWheelTick(const float &InDeltaTime)
{
  if (GetOwner()->Links.Contains(WheelSetting.WheelFrontLeft) &&
      GetOwner()->Links.Contains(WheelSetting.WheelBackLeft) &&
      GetOwner()->Links.Contains(WheelSetting.WheelFrontRight) &&
      GetOwner()->Links.Contains(WheelSetting.WheelBackRight))
      {
        WheelSetting.WheelVelocities[0] = (LinearVelocity.X + LinearVelocity.Y + WheelSetting.WheelToCenterSum * AngularVelocity) / WheelSetting.WheelRadius;
        WheelSetting.WheelVelocities[1] = (LinearVelocity.X - LinearVelocity.Y - WheelSetting.WheelToCenterSum * AngularVelocity) / WheelSetting.WheelRadius;
        WheelSetting.WheelVelocities[2] = (LinearVelocity.X - LinearVelocity.Y + WheelSetting.WheelToCenterSum * AngularVelocity) / WheelSetting.WheelRadius;
        WheelSetting.WheelVelocities[3] = (LinearVelocity.X + LinearVelocity.Y - WheelSetting.WheelToCenterSum * AngularVelocity) / WheelSetting.WheelRadius;

        FVector RotationAxis = GetOwner()->Links[BaseName]->GetCollision()->GetComponentQuat().GetAxisY();
        GetOwner()->Links[WheelSetting.WheelFrontLeft]->GetCollision()->SetPhysicsAngularVelocityInRadians(RotationAxis * WheelSetting.WheelVelocities[0]);
        GetOwner()->Links[WheelSetting.WheelFrontRight]->GetCollision()->SetPhysicsAngularVelocityInRadians(RotationAxis * WheelSetting.WheelVelocities[1]);
        GetOwner()->Links[WheelSetting.WheelBackLeft]->GetCollision()->SetPhysicsAngularVelocityInRadians(RotationAxis * WheelSetting.WheelVelocities[2]);
        GetOwner()->Links[WheelSetting.WheelBackRight]->GetCollision()->SetPhysicsAngularVelocityInRadians(RotationAxis * WheelSetting.WheelVelocities[3]);
      }
  else
  {
    UE_LOG(LogTemp, Error, TEXT("OmniwheelController can not find omni wheels in the GetOwner()"))
  }
  
}
