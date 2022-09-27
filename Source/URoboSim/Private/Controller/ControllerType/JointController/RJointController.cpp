#include "Controller/ControllerType/JointController/RJointController.h"
#include "ROSTime.h"

DEFINE_LOG_CATEGORY_STATIC(LogRJointController, Log, All);

FConfigOverwrite::FConfigOverwrite()
{
  Mode = UJointControllerMode::Dynamic;
}

FConfigOverwrite::FConfigOverwrite(const UJointControllerMode& InMode, const FEnableDrive& InEnableDrive) : Mode(InMode), EnableDrive(InEnableDrive)
{
}

URJointController::URJointController()
{
  Mode = UJointControllerMode::Dynamic;
  bDisableCollision = false;
  bControllAllJoints = false;
  EnableDrive.PositionStrength = 1E5;
  EnableDrive.VelocityStrength = 1E4;
  EnableDrive.MaxForce = 1E10;
  InitPriority = 0;
  SetOwner();
}

void URJointController::AddConfigOverwrite(const FString& InJointName, const FConfigOverwrite& InConfigOverwrite)
{
  ConfigOverwrite.Add(InJointName,InConfigOverwrite);
}

void URJointController::SetControllerParameters(URControllerParameter *&ControllerParameters)
{
  URJointControllerParameter *JointControllerParameters = Cast<URJointControllerParameter>(ControllerParameters);
  if (JointControllerParameters)
  {
    Mode = JointControllerParameters->Mode;
    bDisableCollision = JointControllerParameters->bDisableCollision;
    bControllAllJoints = JointControllerParameters->bControllAllJoints;
    EnableDrive = JointControllerParameters->EnableDrive;
  }
}

void URJointController::Init()
{
  Super::Init();

  bPublishResult = false;
  if (GetOwner())
  {
    SetMode();
    SetPhysics();

    if (bControllAllJoints)
      {
        TArray<FString> JointNames;
        for (URJoint *&Joint : GetOwner()->GetJoints())
          {
            JointNames.Add(Joint->GetName());
          }
        SetJointNames(JointNames);
      }
    else
      {
        for (const TPair<FString, FJointState> &DesiredJointState : DesiredJointStates)
          {
            const FString JointName = DesiredJointState.Key;
            if (URJoint *Joint = GetOwner()->GetJoint(JointName))
              {
                if(FConfigOverwrite* CO = ConfigOverwrite.Find(JointName))
                  {
                    Joint->SetDrive((*CO).EnableDrive);
                  }
                else
                  {
                    Joint->SetDrive(EnableDriveInternal);
                    // Joint->SetMotorJointState(DesiredJointStates.FindRef(JointName));
                  }
              }
            else
              {
                UE_LOG(LogRJointController, Error, TEXT("%s of %s not found"), *JointName, *GetOwner()->GetName());
              }

          }
      }
  }
  else
  {
    UE_LOG(LogRJointController, Error, TEXT("%s is not attached to ARModel"), *GetName())
  }
}

void URJointController::SetMode()
{
  if (GetOwner())
  {
      bEnablePhysics = true;
      switch (Mode)
        {
        case UJointControllerMode::Kinematic:
          EnableDriveInternal.bPositionDrive = false;
          EnableDriveInternal.bVelocityDrive = false;
          bEnablePhysics = false;
          break;

        case UJointControllerMode::Dynamic:
          EnableDriveInternal.bPositionDrive = EnableDrive.bPositionDrive;
          EnableDriveInternal.bVelocityDrive = EnableDrive.bVelocityDrive;
          EnableDriveInternal.PositionStrength = EnableDrive.PositionStrength;
          EnableDriveInternal.VelocityStrength = EnableDrive.VelocityStrength;
          break;
        }
  }
  else
  {
    UE_LOG(LogRJointController, Error, TEXT("Could not set Mode because of Owner"));
  }
}

void URJointController::SetPhysics()
{
  for (URJoint *&Joint : GetOwner()->GetJoints())
    {
      if (bDisableCollision)
        {
          Joint->Child->DisableCollision();
        }
      FConfigOverwrite* CO = ConfigOverwrite.Find(Joint->GetName());
      if(CO)
        {
          switch ((*CO).Mode)
            {
            case UJointControllerMode::Kinematic:
              Joint->SetSimulatePhysics(false);
              UE_LOG(LogRJointController, Error, TEXT("%s false"), *Joint->GetName())
              break;

            case UJointControllerMode::Dynamic:

              UE_LOG(LogRJointController, Error, TEXT("%s true"), *Joint->GetName())
              Joint->SetSimulatePhysics(true);
              break;
            }
        }
      else if(!Joint->bIsMimic)
        {
          Joint->SetSimulatePhysics(bEnablePhysics);
        }
    }
  if (GetOwner())
  {
  for (URLink *&Link : GetOwner()->GetLinks())
    {
      Link->SetEnableGravity(false);
    }
  }
  else
  {
    UE_LOG(LogRJointController, Error, TEXT("Could not set physics for links because of Owner"));
  }

}

const UJointControllerMode URJointController::GetMode()
{
  return Mode;
}

void URJointController::SetJointNames(const TArray<FString> &JointNames, const FEnableDrive &InEnableDrive)
{
  for (const FString &JointName : JointNames)
  {
    if (!DesiredJointStates.Contains(JointName) && GetOwner()->GetJoint(JointName))
    {
      DesiredJointStates.Add(JointName, FJointState());
      GetOwner()->GetJoint(JointName)->SetDrive(InEnableDrive);
    }
  }
}

void URJointController::SetJointNames(const TArray<FString> &JointNames)
{
  for (const FString &JointName : JointNames)
  {

    // UE_LOG(LogRJointController, Error, TEXT("%s Pos %s Vel %s"), *JointName, EnableDrive ? "True" : "False", bBlockingHit ? "True" : "False")


    if (!DesiredJointStates.Contains(JointName) && GetOwner()->GetJoint(JointName))
    {
      DesiredJointStates.Add(JointName, FJointState());
      GetOwner()->GetJoint(JointName)->SetDrive(EnableDriveInternal);
    }
  }
}

void URJointController::Tick(const float &InDeltaTime)
{
  if (GetOwner())
  {
    // for (auto &Joint : GetOwner()->Joints)
    // {
    //   Joint.Value->UpdateEncoder();
    // }

    MoveJoints(InDeltaTime);
  }
  else
  {
    UE_LOG(LogRJointController, Error, TEXT("Owner of %s not found"), *GetName())
  }
}

void URJointController::MoveJoints(const float &InDeltaTime)
{

  UJointControllerMode TempMode;
  FString JointName;

  for (const TPair<FString, FJointState> &DesiredJointState : DesiredJointStates)
  {
    JointName = DesiredJointState.Key;
    if(ConfigOverwrite.Contains(JointName))
      {
        TempMode = ConfigOverwrite[JointName].Mode;
      }
    else
      {
        TempMode = Mode;
      }

    switch (TempMode)
      {
      case UJointControllerMode::Kinematic:
        MoveJointKinematic(JointName);
        break;

      case UJointControllerMode::Dynamic:
        MoveJointDynamic(InDeltaTime, JointName);
        break;
      }
  }
}

void URJointController::MoveJointDynamic(const float &InDeltaTime, const FString &InJointName)
{
  if (URJoint *Joint = GetOwner()->GetJoint(InJointName))
    {
      FJointState TempJointState = DesiredJointStates.FindRef(InJointName);
      TempJointState.JointVelocity = CalculateJointVelocity(InDeltaTime, InJointName);
      Joint->SetMotorJointState(TempJointState);
    }
  else
    {
      UE_LOG(LogRJointController, Error, TEXT("%s of %s not found"), *InJointName, *GetOwner()->GetName());
    }
}

void URJointController::MoveJointKinematic(const FString &InJointName)
{
  if (URJoint *Joint = GetOwner()->GetJoint(InJointName))
    {
      Joint->SetJointPosition(DesiredJointStates.FindRef(InJointName).JointPosition, nullptr);
    }
  else
    {
      UE_LOG(LogRJointController, Error, TEXT("%s of %s not found"), *InJointName, *GetOwner()->GetName());
    }
}

float URJointController::CalculateJointVelocity(float InDeltaTime, FString InJointName)
{
  float DesiredPos = DesiredJointStates[InJointName].JointPosition;
  URJoint *Joint = GetOwner()->Joints[InJointName];
  float CurrentJointPos = Joint->GetEncoderValue();
  float Diff = DesiredPos - CurrentJointPos;

  // float Vel = Diff / InDeltaTime;
  float Vel = Diff ;

  // UE_LOG(LogRJointController, Error, TEXT("%s: Diff %f Vel %f"), *InJointName, Diff, Vel);
  return Vel;

}
