#include "Controller/ControllerType/SpecialController/RWSGGripperController.h"

URWSGGripperController::URWSGGripperController()
{
  GripperAction = EGripperAction::None;
}

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
  for(auto& GripperJoint : GripperJoints)
    {
      if (!GripperJoint)
        {
          UE_LOG(LogTemp, Error, TEXT("GripperJoint of %s not found"), *GetName());
          return;
        }

      if(bTimoutActive)
        {
          Timer += InDeltaTime;

          if(Timer >= Timeout)
            {
              if (JointController->DesiredJointStates.Contains(GripperJointNames[0]))
                {
                  JointController->DesiredJointStates[GripperJointNames[0]].JointPosition = GripperJoint->GetJointPosition();
                }
              if (JointController->DesiredJointStates.Contains(GripperFingerJointName))
                {
                  JointController->DesiredJointStates[GripperFingerJointName].JointPosition = -GripperJoint->GetJointPosition()/2;
                }

              switch(GripperAction)
                {
                case EGripperAction::Grasp:
                  Grasp();
                  break;
                case EGripperAction::Release:
                  Release();
                  break;
                }

              bTimoutActive = false;
              Timer = 0;
              GripperAction = EGripperAction::None;
            }
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
    Timer = 0;
    bTimoutActive = true;

    float PoseInM = InPose/1000.;
    if (JointController->DesiredJointStates.Contains(GripperJointNames[0]))
    {
      OldPosition = JointController->DesiredJointStates[GripperJointNames[0]].JointPosition;

      if(PoseInM < OldPosition)
        {
          if(bInvertGraspCondition)
            {
              GripperAction = EGripperAction::Release;
            }
          else
            {
              GripperAction = EGripperAction::Grasp;
            }
        }
      else if(PoseInM > OldPosition)
        {
          if(bInvertGraspCondition)
            {
              GripperAction = EGripperAction::Grasp;
            }
          else
            {
              GripperAction = EGripperAction::Release;
            }
        }

      JointController->DesiredJointStates[GripperJointNames[0]].JointPosition = PoseInM;
      // JointController->DesiredJointStates[GripperJointName].JointPosition = InPose;

      if (JointController->DesiredJointStates.Contains(GripperFingerJointName))
        {
          JointController->DesiredJointStates[GripperFingerJointName].JointPosition = -PoseInM/2.;
          // JointController->DesiredJointStates[GripperFingerJointName].JointPosition = -InPose/2.;
        }
    }
  }
}
