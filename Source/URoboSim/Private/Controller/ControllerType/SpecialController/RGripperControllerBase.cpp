#include "Controller/ControllerType/SpecialController/RGripperControllerBase.h"
#include "URoboSimSettings.h"

bool FGripperJointAssociation::InitRefLinks(ARModel* Model)
{
  bool bSuccess = true;
  if(Model)
    {
      for(auto& LinkName : ReferenceFrameNames)
        {
          URLink* Reference = Model->Links.FindRef(LinkName);
          if(Reference)
            {
              ReferenceFrames.Add(Reference);
            }
          else
            {
              bSuccess = bSuccess && false;
              UE_LOG(LogTemp, Error, TEXT("%s not found in ARModel"), *LinkName);
            }
        }
    }
  return bSuccess;
}

URGripperControllerBase::URGripperControllerBase()
{
  GripperJointNames.Empty();
	GripperJointNames.Add(TEXT("?_gripper_joint"));
        InitPriority = 2;
	// GraspComponent = CreateDefaultSubobject<URGraspComponent>(FName(GetName() + TEXT("_GraspComp")));
	// GraspComponent->RegisterComponent();
	// if (GetOwner())
	// {
	// 	GraspComponent->AttachToComponent(GetOwner()->GetRootComponent(),
	// 	                                  FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("%s not attached to ARModel"), *GetName());
	// }
}

void URGripperControllerBase::SetControllerParameters(URControllerParameter*& ControllerParameters)
{
	URGripperControllerBaseParameter* GripperControllerParameters = Cast<URGripperControllerBaseParameter>(
		ControllerParameters);
	if (GripperControllerParameters)
	{
		GripperJointNames = GripperControllerParameters->GripperJointNames;
		GripperJointReferenceFrames = GripperControllerParameters->GripperJointReferenceFrames;
		GraspCompSetting = GripperControllerParameters->GraspCompSetting;
		GraspComponentName = GripperControllerParameters->GraspComponentName;
		GraspPartNames = GripperControllerParameters->GraspPartNames;
		EnableDrive = GripperControllerParameters->EnableDrive;
		Mode = GripperControllerParameters->Mode;
		bOverwriteConfig = GripperControllerParameters->bOverwriteConfig;
		bInvertGraspCondition = GripperControllerParameters->bInvertGraspCondition;
	}
}

void URGripperControllerBase::Init()
{
	Super::Init();

	if (!GetOwner())
	{
		UE_LOG(LogTemp, Error, TEXT("%s not attached to ARModel"), *GetName());
	}
	else
	{

          const UURoboSimSettings* Settings = GetDefault<UURoboSimSettings>();
          if(Settings)
            {
              bDebugMode = Settings->bDebugMode;
            }

          for(auto & GripperJointName: GripperJointNames)
            {
              URJoint* GripperJoint = GetOwner()->Joints.FindRef(GripperJointName);

              if (!GripperJoint)
		{
                  UE_LOG(LogTemp, Error, TEXT("GripperJoint %s of %s not found"), *GetName(), *GripperJointName);
                  return;
		}
              else
		{
                  UE_LOG(LogTemp, Log, TEXT("GripperJoint %s of %s found"), *GetName(), *GripperJointName);
		}
              GripperJoints.Add(GripperJoint);
              // OldPositions.Add(GripperJointName,0);
              if(GripperJointReferenceFrames.Contains(GripperJointName))
                {
                  GripperJointReferenceFrames[GripperJointName].InitRefLinks(GetOwner());
                }
            }

          JointController = Cast<URJointController>(GetOwner()->GetController(TEXT("JointController")));

		if (!JointController)
		{
			UE_LOG(LogTemp, Error, TEXT("%s: JointController not found"), *GetName());
			return;
		}

		TArray<FString> JointNames;
		JointNames.Append(GripperJointNames);
		JointController->SetJointNames(JointNames, EnableDrive);
		if (bOverwriteConfig)
		{
                  for(auto & GripperJoint: GripperJoints)
                    {
			JointController->AddConfigOverwrite(GripperJoint->GetName(), FConfigOverwrite(Mode, EnableDrive));
                    }
		}

                for(auto & GPN: GraspPartNames)
                  {
                    for(auto& GP: GPN.Value.GripperPartNames)
                      {
                        URLink* Reference = GetOwner()->Links.FindRef(GP);
                        if(Reference)
                          {
                            // Reference->BodyInstance.SetInstanceNotifyRBCollision(true);
                            // Reference->OnComponentHit.AddDynamic(this, &URGripperControllerBase::GraspHit);
                            Reference->GetCollision()->BodyInstance.SetInstanceNotifyRBCollision(true);
                            Reference->GetCollision()->OnComponentHit.AddDynamic(this, &URGripperControllerBase::GraspHit);
                            GraspParts.Add(Reference->GetCollision(),GPN.Key);
                          }
                      }
                  }

                // OldPosition = JointController->DesiredJointStates.FindRef(GripperJointName).JointPosition;

		GraspComponent = NewObject<URGraspComponent>(GetOwner(), FName(GetName() + TEXT("_GraspComp")));
		GraspComponent->CreationMethod = EComponentCreationMethod::Instance;
		GraspComponent->RegisterComponent();

		ReferenceLink = GetOwner()->Links[GraspCompSetting.GripperName];
		GraspComponent->AttachToComponent(ReferenceLink, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		GraspComponent->Constraint->AttachToComponent(GraspComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		GraspComponent->AddRelativeLocation(GraspCompSetting.ToolCenterPoint);
                GraspComponent->GraspRadius = GraspCompSetting.Radius;
		GraspComponent->Init(ReferenceLink);
 	}
}

void URGripperControllerBase::GraspHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
                            UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
  UE_LOG(LogTemp, Error, TEXT("%s: Hit %s %s"), *HitComp->GetName(), *OtherComp->GetName(), *HitComp->GetAttachParent()->GetName());
  //check if the other component
  if(HitObjects.Contains(OtherComp))
    {
      // GraspParts[HitComp] returns Part of the Gripper that the HitComp belongs to. E.g.
      // G1 for multi-finger gripper. check if a part of the gripper e.g. part1 of G1
      // is already in contact with the HitObject. If not its the first gripper part in
      // contact with this object else its the second grasp part in contact with the object
      // TODO: check more than HitComps[0]
      if(GraspParts[HitObjects[OtherComp].HitComps[0]] == GraspParts[HitComp])
        {
          // UE_LOG(LogTemp, Error, TEXT("GripperPart %s already in contact"), *GraspParts[HitComp]);
        }
      else
        {
          HitObjects[OtherComp].HitComps.AddUnique(HitComp);
        }

      if((GraspingParts.Part1 == HitComp) || (GraspingParts.Part2 == HitComp))
        {
          OldPosition = GraspingParts.GetDistance();
        }
    }
  else
    {
      FGraspObjects Temp;
      Temp.HitComps.Add(HitComp);
      HitObjects.Add(OtherComp,Temp);
    }

}

void URGripperControllerBase::Tick(const float& InDeltaTime)
{
  float Diff = 0;
  float JointPos = 0;
  // float OldPosition = 0;
  TArray<URLink*> ActiveLinks;
  if(!GraspingParts.Part1 && !GraspingParts.Part2)
    {
      for(auto& HitObject : HitObjects)
        {
          if(HitObject.Value.HitComps.Num() > 1)
            {
              Grasp();
              GraspingParts.Part1 = HitObject.Value.HitComps[0];
              GraspingParts.Part2 = HitObject.Value.HitComps[1];
              // UE_LOG(LogTemp, Error, TEXT("GraspingParts Distance: %f"), GraspingParts.GetDistance());
              OldPosition = GraspingParts.GetDistance();
            }
        }
    }
  else
    {
      Diff = GraspingParts.GetDistance() - OldPosition;
      // UE_LOG(LogTemp, Error, TEXT("GraspingParts Diff: %f"), Diff);
      if(Diff > 1.0)
        {
          Release();
          HitObjects.Empty();
          GraspingParts.Part1 = nullptr;
          GraspingParts.Part2 = nullptr;
        }
    }

  // for(auto& GripperJoint : GripperJoints)
  //   {
  //     // if (GripperJoint)
  //     //   {
  //     //     UE_LOG(LogTemp, Error, TEXT("GripperJoint: %s of %s not set"), *GetName(), *GripperJointNames[0]);
  //     //     return;
  //     //   }
  //     float TempPos = JointController->DesiredJointStates.FindRef(GripperJoint->GetName()).JointPosition;

  //     Diff = TempPos - OldPositions[GripperJoint->GetName()];
  //     // if(JointDiff > Diff)
  //     //   {
  //     //     Diff = JointDiff;
  //     //     JointPos = TempPos;
  //     //     OldPosition = OldPositions[GripperJoint->GetName()];
  //     //   }

  //     // if (FMath::Abs(Diff) > 0.01)
  //     //   {
  //     //     if(GripperJointReferenceFrames.Contains(GripperJoint))
  //     //       {
  //     //         ActiveLinks.Append(GripperJointReferenceFrames[GripperJoint]);
  //     //       }
  //     //   }
  //     OldPositions[GripperJoint->GetName()] = TempPos;
  //   }

  // if(bDebugMode)
  //   {
  //     UE_LOG(LogTemp, Log, TEXT("%s: Diff %f"), *GetName(), Diff);
  //   }
  // if (FMath::Abs(Diff) > 0.01)
  //       {
  //       	if (Diff < 0)
  //       	{
  //       		if (bInvertGraspCondition)
  //       		{
  //       			Release();
  //       		}
  //       		else
  //       		{
  //       			Grasp();
  //       		}
  //       	}
  //       	else if (Diff > 0)
  //       	{
  //       		if (bInvertGraspCondition)
  //       		{
  //       			Grasp();
  //       		}
  //       		else
  //       		{
  //       			Release();
  //       		}
  //       	}
  //       }
	// OldPosition = JointPos;
}

bool URGripperControllerBase::Grasp()
{
  if(bDebugMode)
    {
      UE_LOG(LogTemp, Log, TEXT("%s: Grasp"), *GetName());
    }
	if (GraspComponent)
	{
		return GraspComponent->TryToFixate();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("%s: No Grasp Component"), *GetName());
	}
	return false;
}

void URGripperControllerBase::Release()
{
  if(bDebugMode)
    {
      UE_LOG(LogTemp, Log, TEXT("%s: Release"), *GetName());
    }

	if (GraspComponent)
	{
		GraspComponent->TryToDetach();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("%s: No Grasp Component"), *GetName());
	}
}
