#include "Controller/ControllerType/BaseController/RMIRWheelController.h"

URMIRWheelController::URMIRWheelController()
{
  WheelSetting.WheelVelocities.Init(0.0, 5);
}

void URMIRWheelController::SetControllerParameters(URControllerParameter *&ControllerParameters)
{
  URMIRWheelControllerParameter *MIRWheelControllerParameters = Cast<URMIRWheelControllerParameter>(ControllerParameters);
  if (MIRWheelControllerParameters)
  {
    Super::SetControllerParameters(ControllerParameters);
    WheelSetting = MIRWheelControllerParameters->WheelSetting;
  }
}

void URMIRWheelController::Tick(const float &InDeltaTime)
{
  MoveLinearTick(InDeltaTime);
  TurnTick(InDeltaTime);
  CalculateOdomStates(InDeltaTime);
  MoveWheelTick(InDeltaTime);
}

void SetAngularVelocityY(UStaticMeshComponent* Link, const double &AngularVelocity)
{
  Link->SetPhysicsAngularVelocityInRadians(Link->GetComponentQuat().GetAxisY() * AngularVelocity);
}

void SetAngularVelocityZ(UStaticMeshComponent* Link, const double &AngularVelocity)
{
  Link->SetPhysicsAngularVelocityInRadians(Link->GetComponentQuat().GetAxisZ() * AngularVelocity);
}

void URMIRWheelController::MoveWheelTick(const float &InDeltaTime)
{
  if (GetOwner()->Links.Contains(WheelSetting.WheelLeft) &&
      GetOwner()->Links.Contains(WheelSetting.WheelRight) &&
      GetOwner()->Links.Contains(WheelSetting.WheelFrontLeftRotation) &&
      GetOwner()->Links.Contains(WheelSetting.WheelFrontLeftTranslation) &&
      GetOwner()->Links.Contains(WheelSetting.WheelFrontRightRotation) &&
      GetOwner()->Links.Contains(WheelSetting.WheelFrontRightTranslation) &&
      GetOwner()->Links.Contains(WheelSetting.WheelBackLeftRotation) &&
      GetOwner()->Links.Contains(WheelSetting.WheelBackLeftTranslation) &&
      GetOwner()->Links.Contains(WheelSetting.WheelBackRightRotation) &&
      GetOwner()->Links.Contains(WheelSetting.WheelBackRightTranslation))
      {
        WheelSetting.WheelVelocities[0] = LinearVelocity.X / (2 * PI * WheelSetting.WheelRadius);
        SetAngularVelocityY(GetOwner()->Links[WheelSetting.WheelLeft]->GetCollision(), WheelSetting.WheelVelocities[0]);
        SetAngularVelocityY(GetOwner()->Links[WheelSetting.WheelRight]->GetCollision(), WheelSetting.WheelVelocities[0]);
        SetAngularVelocityY(GetOwner()->Links[WheelSetting.WheelFrontLeftTranslation]->GetCollision(), FMath::Abs(WheelSetting.WheelVelocities[0]));
        SetAngularVelocityY(GetOwner()->Links[WheelSetting.WheelFrontRightTranslation]->GetCollision(), FMath::Abs(WheelSetting.WheelVelocities[0]));
        SetAngularVelocityY(GetOwner()->Links[WheelSetting.WheelBackLeftTranslation]->GetCollision(), FMath::Abs(WheelSetting.WheelVelocities[0]));
        SetAngularVelocityY(GetOwner()->Links[WheelSetting.WheelBackRightTranslation]->GetCollision(), FMath::Abs(WheelSetting.WheelVelocities[0]));
        float WheelSteeringAngle = GetOwner()->Links[BaseName]->GetCollision()->GetComponentRotation().Yaw;
        float DeltaWheelSteeringAngle = 0.0;
        if (LinearVelocity.X > 1E-4)
        {
          DeltaWheelSteeringAngle = FMath::RadiansToDegrees(FMath::Asin((AngularVelocity * WheelSetting.WheelDistanceLength) / (2 * LinearVelocity.X)));
        }
        else if (LinearVelocity.X < -1E-4)
        {
          DeltaWheelSteeringAngle = 180.0 + FMath::RadiansToDegrees(FMath::Asin((AngularVelocity * WheelSetting.WheelDistanceLength) / (2 * LinearVelocity.X)));
        }
        else
        {
          DeltaWheelSteeringAngle = 90.0 * FMath::Sign(AngularVelocity);
        }
        FRotator WheelSteeringAngleFront = GetOwner()->Links[BaseName]->GetCollision()->GetComponentRotation().Add(0.f, DeltaWheelSteeringAngle, 0.f);
        FRotator WheelSteeringAngleBack = GetOwner()->Links[BaseName]->GetCollision()->GetComponentRotation().Add(0.f, -DeltaWheelSteeringAngle, 0.f);
        WheelSetting.WheelVelocities[1] = 0.1 * ((WheelSteeringAngleFront - GetOwner()->Links[WheelSetting.WheelFrontLeftRotation]->GetCollision()->GetComponentRotation()).GetNormalized().Yaw) / InDeltaTime;
        WheelSetting.WheelVelocities[2] = 0.1 * ((WheelSteeringAngleFront - GetOwner()->Links[WheelSetting.WheelFrontRightRotation]->GetCollision()->GetComponentRotation()).GetNormalized().Yaw) / InDeltaTime;
        WheelSetting.WheelVelocities[3] = 0.1 * ((WheelSteeringAngleBack - GetOwner()->Links[WheelSetting.WheelBackLeftRotation]->GetCollision()->GetComponentRotation()).GetNormalized().Yaw) / InDeltaTime;
        WheelSetting.WheelVelocities[4] = 0.1 * ((WheelSteeringAngleBack - GetOwner()->Links[WheelSetting.WheelBackRightRotation]->GetCollision()->GetComponentRotation()).GetNormalized().Yaw) / InDeltaTime;
        SetAngularVelocityZ(GetOwner()->Links[WheelSetting.WheelFrontLeftRotation]->GetCollision(), WheelSetting.WheelVelocities[1]);
        SetAngularVelocityZ(GetOwner()->Links[WheelSetting.WheelFrontRightRotation]->GetCollision(), WheelSetting.WheelVelocities[2]);
        SetAngularVelocityZ(GetOwner()->Links[WheelSetting.WheelBackLeftRotation]->GetCollision(), WheelSetting.WheelVelocities[3]);
        SetAngularVelocityZ(GetOwner()->Links[WheelSetting.WheelBackRightRotation]->GetCollision(), WheelSetting.WheelVelocities[4]);
      }
  else
  {
    UE_LOG(LogTemp, Error, TEXT("RMIRWheelController can not find wheels in the GetOwner()"))
  }
}
