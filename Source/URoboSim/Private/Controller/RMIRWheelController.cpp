#include "Controller/RMIRWheelController.h"

URMIRWheelController::URMIRWheelController()
{
  BaseName = TEXT("base_footprint");

  OdomPositionStates.Init(0.0, 3);
  WheelSetting.WheelVelocities.Init(0.0, 8);
  OdomVelocityStates.Init(0.0, 3);
}

void URMIRWheelController::Tick(float InDeltaTime)
{
  MoveLinearTick(InDeltaTime);
  TurnTick(InDeltaTime);
  CalculateOdomStates(InDeltaTime);
  MoveWheelTick(InDeltaTime);
}

void SetAngularVelocityY(UStaticMeshComponent* Link, const double &AngularVelocity)
{
  FVector RotationAxis = Link->GetComponentQuat().GetAxisY();
  Link->SetPhysicsAngularVelocityInRadians(RotationAxis * AngularVelocity);
}

void SetAngularVelocityZ(UStaticMeshComponent* Link, const double &AngularVelocity)
{
  FVector RotationAxis = Link->GetComponentQuat().GetAxisZ();
  Link->SetPhysicsAngularVelocityInRadians(RotationAxis * AngularVelocity);
}

void URMIRWheelController::MoveWheelTick(const float &InDeltaTime)
{
  if (GetOwner()->Links.Contains(WheelSetting.WheelFrontLeftRotation) &&
      GetOwner()->Links.Contains(WheelSetting.WheelFrontLeftTranslation) &&
      GetOwner()->Links.Contains(WheelSetting.WheelFrontRightRotation) &&
      GetOwner()->Links.Contains(WheelSetting.WheelFrontRightTranslation) &&
      GetOwner()->Links.Contains(WheelSetting.WheelBackLeftRotation) &&
      GetOwner()->Links.Contains(WheelSetting.WheelBackLeftTranslation) &&
      GetOwner()->Links.Contains(WheelSetting.WheelBackRightRotation) &&
      GetOwner()->Links.Contains(WheelSetting.WheelBackRightTranslation))
      {
        // WheelSetting.WheelVelocities[0] = 0.0;
        // WheelSetting.WheelVelocities[1] = 0.0;
        // WheelSetting.WheelVelocities[2] = 0.0;
        // WheelSetting.WheelVelocities[3] = 0.0;
        // WheelSetting.WheelVelocities[4] = 0.0;
        // WheelSetting.WheelVelocities[5] = 0.0;
        // WheelSetting.WheelVelocities[6] = 0.0;
        // WheelSetting.WheelVelocities[7] = 0.0;

        SetAngularVelocityZ(GetOwner()->Links[WheelSetting.WheelFrontLeftRotation]->GetCollision(), WheelSetting.WheelVelocities[0]);
        SetAngularVelocityY(GetOwner()->Links[WheelSetting.WheelFrontLeftTranslation]->GetCollision(), WheelSetting.WheelVelocities[1]);
        SetAngularVelocityZ(GetOwner()->Links[WheelSetting.WheelFrontRightRotation]->GetCollision(), WheelSetting.WheelVelocities[2]);
        SetAngularVelocityY(GetOwner()->Links[WheelSetting.WheelFrontRightTranslation]->GetCollision(), WheelSetting.WheelVelocities[3]);
        SetAngularVelocityZ(GetOwner()->Links[WheelSetting.WheelBackLeftRotation]->GetCollision(), WheelSetting.WheelVelocities[4]);
        SetAngularVelocityY(GetOwner()->Links[WheelSetting.WheelBackLeftTranslation]->GetCollision(), WheelSetting.WheelVelocities[5]);
        SetAngularVelocityZ(GetOwner()->Links[WheelSetting.WheelBackRightRotation]->GetCollision(), WheelSetting.WheelVelocities[6]);
        SetAngularVelocityY(GetOwner()->Links[WheelSetting.WheelBackRightTranslation]->GetCollision(), WheelSetting.WheelVelocities[7]);
      }
  else
  {
    UE_LOG(LogTemp, Error, TEXT("RMIRWheelController can not find wheels in the GetOwner()"))
  }
}
