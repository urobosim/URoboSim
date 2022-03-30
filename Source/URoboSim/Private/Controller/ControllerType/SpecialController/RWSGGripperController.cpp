#include "Controller/ControllerType/SpecialController/RWSGGripperController.h"

void URWSGGripperController::SetControllerParameters(URControllerParameter *&ControllerParameters)
{
  Super::SetControllerParameters(ControllerParameters);
  URWSGGripperControllerParameter *GripperControllerParameters = Cast<URWSGGripperControllerParameter>(ControllerParameters);
  if (GripperControllerParameters)
  {
    GripperFingerJointName = GripperControllerParameters->GripperFingerJointName;
  }
}

void URWSGGripperController::Tick(const float &InDeltaTime)
{
  Super::Tick(InDeltaTime);

  if(bTimoutActive)
    {
      Timer += InDeltaTime;

      if(Timer >= Timeout)
        {
          if (JointController->DesiredJointStates.Contains(GripperJointName))
            {
              JointController->DesiredJointStates[GripperJointName].JointPosition = GripperJoint->GetJointPosition();
            }
          if (JointController->DesiredJointStates.Contains(GripperFingerJointName))
            {
              JointController->DesiredJointStates[GripperFingerJointName].JointPosition = -GripperJoint->GetJointPosition()/2;
            }
          bTimoutActive = false;
          Timer = 0;
        }
    }
}

void URWSGGripperController::Init()
{
  Super::Init();


  if (!GetOwner())
  {
    UE_LOG(LogTemp, Error, TEXT("%s not attached to ARModel"), *GetName());
  }
  else
  {
    GripperFingerJoint = GetOwner()->Joints.FindRef(GripperFingerJointName);

    if (!GripperFingerJoint)
    {
      UE_LOG(LogTemp, Error, TEXT("GripperFingerJoint of %s not found"), *GetName());
      return;
    }

    if (!JointController)
    {
      UE_LOG(LogTemp, Error, TEXT("%s: JointController not found"), *GetName());
      return;
    }

    if(bOverwriteConfig)
      {
        JointController->AddConfigOverwrite(GripperFingerJoint->GetName(), FConfigOverwrite(Mode, EnableDrive));
        TArray<FString> JointNames;
        JointNames.Add(GripperFingerJointName);
        JointController->SetJointNames(JointNames, EnableDrive);
      }
  }
}

void URWSGGripperController::SetPose(const float& InPose)
{
  if (JointController)
  {
    bTimoutActive = true;
    if (JointController->DesiredJointStates.Contains(GripperJointName))
    {
      JointController->DesiredJointStates[GripperJointName].JointPosition = InPose/1000.;
    }
    if (JointController->DesiredJointStates.Contains(GripperFingerJointName))
    {
      JointController->DesiredJointStates[GripperFingerJointName].JointPosition = -InPose/2000.;
    }
  }
}
