#include "Controller/RController.h"
#include "Physics/RLink.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

void URController::CancelAction()
{
  if(bCancel)
    {
      if(bActive)
        {
          GoalStatusList.Last().Status = 6;
          bActive = false;
        }
      else
        {
          GoalStatusList.Last().Status = 2;
          bActive = false;
        }
      bPublishResult = true;
    }
}

URBaseController::URBaseController()
{
	BaseName = TEXT("base_footprint");
}



URControllerComponent::URControllerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;
	Model = Cast<ARModel>(GetOwner());
	if(Model)
	{
          Model->Plugins.Add(TEXT("ControllerComponent"), this);
	}
}

URControllerComponent::~URControllerComponent()
{
	UE_LOG(LogTemp, Error, TEXT("destroy component"));
	// if(Model)
	// {
	// 	Model->Plugins.FindAndRemoveChecked(TEXT("ControllerComponent"));
	// }
}

/* URROSControllerComponent::URROSControllerComponent() */
/* { */
/* 	// RosComunication.ControllerComponent = this; */
/* } */

void URBaseController::MoveLinear(FVector InVelocity)
{
  LinearVelocity = InVelocity;
}

void URBaseController::Turn(float InVelocity)
{
  AngularVelocity = -InVelocity;
}

void URGripperController::SetupCollisionEvent()
{
  UE_LOG(LogTemp, Error, TEXT("HitEvent Registerd"));
  RightFingerTip->Child->GetCollision()->SetNotifyRigidBodyCollision(true);
  LeftFingerTip->Child->GetCollision()->SetNotifyRigidBodyCollision(true);
  // RightFingerTip->Child->GetCollision()->OnComponentHit.AddDynamic(this, &URGripperController::OnCollision);
  // LeftFingerTip->Child->GetCollision()->OnComponentHit.AddDynamic(this, &URGripperController::OnCollision);
}

void URGripperController::OnCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
  UE_LOG(LogTemp, Error, TEXT("%s in contact with %s in Direction %s"), *HitComponent->GetName(), *OtherActor->GetName(), *NormalImpulse.ToString());
}

void URGripperController::UpdateGripper()
{
    // if(OldPosition > Position)
    // {
    //   if(GraspComponent->FixatedObject)
    //     {
    //       // Position = 2.0;
    //       bMoved = false;
    //       bSuccessGrasp = true;
    //     }
    //   else
    //     {
    //       bMoved = true;
    //       bSuccessGrasp = Grasp();
    //       if(bSuccessGrasp)
    //         {
    //           // Position = 2.0;
    //         }
    //     }
    // }
    // else if(OldPosition < Position)
    // {
    //     bMoved = true;
    //     Release();
    // }
    // else
    // {
    //     bMoved = false;
    // }
    bActive = true;
}

void URGripperController::CheckGripperActionResult(float InError, float InThreshold = 0.5)
{
  if(FMath::Abs(InError) < InThreshold)
    {
      Result.FillValues(Position, MaxEffort, false, true);
      GoalStatusList.Last().Status = 3;
      bActive = false;
      bPublishResult = true;
    }

  if(ActionDuration > 2)
    {
      ActionDuration = 0.0;
      GoalStatusList.Last().Status = 3;
      bActive = false;
      bPublishResult = true;
      bStalled = true;
      Result.FillValues(Position, MaxEffort, bStalled, false);
    }
}

void URGripperController::Tick(float InDeltaTime)
{
    float GripperPosition = 0.;
    float Error = 0;
    if(!RightFinger)
      {
        return;
      }

    if(!LeftFinger)
      {
        return;
      }

    RightFinger->bActuate = false;
    LeftFinger->bActuate = false;


    GripperPosition = FMath::Abs((RightFingerTip->Constraint->GetComponentLocation() - LeftFingerTip->Constraint->GetComponentLocation()).Size() -3 );
    Error = Position-GripperPosition ;
    if(bActive)
    {
      bStalled = false;
      CheckGripperActionResult(Error, 0.5);
      if(bActive)
        {
          GoalStatusList.Last().Status = 1;
          ActionDuration += InDeltaTime;
          RightFinger->EnableMotor(false);
          LeftFinger->EnableMotor(false);


          float Force = 1.0;
          // if(bStalled)
          //   {
          //     Force = MaxEffort * 100;
          //     UE_LOG(LogTemp, Error, TEXT("MaxEffort %f"), MaxEffort);
          //   }

          FVector DirectionTipToTip = (RightFingerTip->Constraint->GetComponentLocation() - LeftFingerTip->Constraint->GetComponentLocation());
          if(Error < 0)
            {
              RightFingerTip->Child->GetCollision()->AddForce(-Force*DirectionTipToTip.GetSafeNormal());
              LeftFingerTip->Child->GetCollision()->AddForce(Force*DirectionTipToTip.GetSafeNormal());
            }
          else
            {
              RightFingerTip->Child->GetCollision()->AddForce(Force*DirectionTipToTip.GetSafeNormal());
              LeftFingerTip->Child->GetCollision()->AddForce(-Force*DirectionTipToTip.GetSafeNormal());
            }
          // RightFinger->MaxJointVel = 0.2;
          // RightFinger->DesiredJointPose = RightFinger->GetEncoderValue();
          // LeftFinger->MaxJointVel = 0.2;
          // LeftFinger->DesiredJointPose = LeftFinger->GetEncoderValue();
          // RightFinger->Constraint->SetTargetPosition(RightFinger->Constraint->GetConstraintPosition());
          // LeftFinger->Constraint->SetTargetPosition(LeftFinger->Constraint->GetConstraintPosition());
          RightFinger->Constraint->SetTargetPosition(RightFinger->GetJointPosition());
          LeftFinger->Constraint->SetTargetPosition(LeftFinger->GetJointPosition());
        }
      else if (bStalled)
        {
          RightFinger->Constraint->SetTargetPosition(RightFinger->GetJointPosition());
          LeftFinger->Constraint->SetTargetPosition(LeftFinger->GetJointPosition());
          RightFinger->EnableMotor(true);
          LeftFinger->EnableMotor(true);
          Grasp();
        }
      else
        {
          RightFinger->EnableMotor(true);
          LeftFinger->EnableMotor(true);
          Release();
        }
    }


    // if(bActive)
    // {
    //   GoalStatusList.Last().Status = 1;
    //   UE_LOG(LogTemp, Error, TEXT("Error %f Position %f GripperPosition %f"), Error, Position, GripperPosition);
    //   CheckGripperActionResult(Error, 0.5);
    // }

}

void URBaseController::Tick(float InDeltaTime)
{
        TurnTick(InDeltaTime);
	MoveLinearTick(InDeltaTime);
}




void URBaseController::TurnTick(float InDeltaTime)
{
	if(AngularVelocity != 0.f)
	{
		FRotator TestRotation = FRotator(0.0f, AngularVelocity *InDeltaTime, 0.0f);
		// for(auto& Link : Model->Links)
		// {
		// 	Link.Value->GetCollision()->SetSimulatePhysics(false);
		// }
        //
		URLink* Base = Model->Links[BaseName];
		FRotator BaseOrientation = Base->GetCollision()->GetComponentRotation();
		FVector BasePosition = Base->GetCollision()->GetComponentLocation();


		for(auto& Link : Model->Links)
		{
			FRotator Orientation = Link.Value->GetCollision()->GetComponentRotation();
			FVector Position = Link.Value->GetCollision()->GetComponentLocation();

			FQuat NewRot = TestRotation.Quaternion() * Orientation.Quaternion() ;
			// Link.Value->GetCollision()->SetWorldRotation(NewRot, false, nullptr, ETeleportType::None);

			FVector LinkBaseOffset = Position - BasePosition;
			FVector NewPosition = TestRotation.RotateVector(LinkBaseOffset) + BasePosition;

			// Link.Value->GetCollision()->SetWorldLocation(NewPosition, false, nullptr, ETeleportType::None);
			Link.Value->GetCollision()->SetWorldLocationAndRotation(NewPosition, NewRot, false, nullptr, ETeleportType::None);
			// UE_LOG(LogTemp, Warning, TEXT("%s: %f"), *Link.Value->GetName(), realtimeSeconds);
		}
		// AngularVelocity = 0.0f;
	}
}

void URBaseController::MoveLinearTick(float InDeltaTime)
{
	if(LinearVelocity.Size() != 0.f)
	{
		URLink* Base = Model->Links[BaseName];
		FRotator BaseOrientation = Base->GetCollision()->GetComponentRotation();
		FVector DistanceTraveld = BaseOrientation.Quaternion().RotateVector(LinearVelocity*InDeltaTime);

		for(auto& Link : Model->Links)
		{
			FVector Position = Link.Value->GetCollision()->GetComponentLocation();
			Link.Value->GetCollision()->SetWorldLocation(DistanceTraveld + Position, false, nullptr, ETeleportType::TeleportPhysics);
		}
	}
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
			FVector Position = Link.Value->GetCollision()->GetComponentLocation();
			Link.Value->GetCollision()->SetWorldLocation(DistanceTraveld + Position, false, nullptr, ETeleportType::TeleportPhysics);
		}
	}
}

void URBaseController::MoveLinearToWorld(FVector InVelocity, float InDeltaTime)
{
	FVector DistanceTraveld = InVelocity*InDeltaTime;

	for(auto& Link : Model->Links)
	{
		FVector Position = Link.Value->GetCollision()->GetComponentLocation();
		Link.Value->GetCollision()->SetWorldLocation(DistanceTraveld + Position, false, nullptr, ETeleportType::TeleportPhysics);
	}
}



void URBaseController::Turn(float InVelocity, float InDeltaTime)
{
	if(InVelocity != 0.f)
	{
		FRotator TestRotation = FRotator(0.0f, InVelocity *InDeltaTime, 0.0f);

		URLink* Base = Model->Links[BaseName];
		FRotator BaseOrientation = Base->GetCollision()->GetComponentRotation();
		FVector BasePosition = Base->GetCollision()->GetComponentLocation();

		for(auto& Link : Model->Links)
		{
			FRotator Orientation = Link.Value->GetCollision()->GetComponentRotation();
			FVector Position = Link.Value->GetCollision()->GetComponentLocation();

			FQuat NewRot = TestRotation.Quaternion() * Orientation.Quaternion() ;
			Link.Value->GetCollision()->SetWorldRotation(NewRot, false, nullptr, ETeleportType::TeleportPhysics);

			FVector LinkBaseOffset = Position - BasePosition;
			FVector NewPosition = TestRotation.RotateVector(LinkBaseOffset) + BasePosition;

			Link.Value->GetCollision()->SetWorldLocation(NewPosition, false, nullptr, ETeleportType::TeleportPhysics);
		}
	}
}

URGripperController::URGripperController()
{
    OldPosition = 0.0;
    // GraspComponent = CreateDefaultSubobject<URGraspComponent>(FName(*(GetName() + TEXT("_GraspComp"))));


}

bool URGripperController::Grasp()
{
	if(GraspComponent)
	{
          return GraspComponent->TryToFixate();
	}
    return false;
}

void URGripperController::Release()
{
	if(GraspComponent)
	{
		GraspComponent->TryToDetach();
	}
}

void URCameraController::PerceiveObject()
{
    if(bActive)
    {
        UE_LOG(LogTemp, Warning, TEXT("PerceiveObject is a dummy function"));
        GoalStatusList.Last().Status = 1;


        bool bObjectFound = false;
        for(auto & Object : PerceivedObjects)
          {
            if(Object->Type.Equals(TypeToPerceive, ESearchCase::IgnoreCase))
              {
                PerceivedObject = Object;
                bObjectFound = true;
              }
          }

        if(bObjectFound)
          {
            URLink* ReferenceLink = Model->Links.FindRef(TEXT("base_footprint"));
            FTransform ReferenceLinkTransform = ReferenceLink->GetCollision()->GetComponentTransform();

            FVector Temp = PerceivedObject->PoseWorld.GetLocation() - ReferenceLinkTransform.GetLocation();
            FVector Pose = ReferenceLinkTransform.GetRotation().Inverse().RotateVector(Temp) ;

            PerceivedObject->Pose.SetLocation(Pose);
            FQuat TempRotator = PerceivedObject->PoseWorld.GetRotation() * ReferenceLinkTransform.GetRotation().Inverse() ;
            PerceivedObject->Pose.SetRotation(TempRotator);

            GoalStatusList.Last().Status = 3;
            bActive = false;
            bPublishResult = true;
            // UE_LOG(LogTemp, Warning, TEXT("Base: %s, :World %s"), *Pose.ToString(), *TempRotator.ToString());
          }
        else
          {
            UE_LOG(LogTemp, Error, TEXT("No Object of Type %s found"), *TypeToPerceive);
          }

        // PerceivedObject.Name = NamePerceivedObject;
        // PerceivedObject.Type = TypePerceivedObject;

        // FVector Temp = PosePerceivedObject.GetLocation() - ReferenceLinkTransform.GetLocation();

    }

}

void URCameraController::Tick(float InDeltaTime)
{
    // PerceiveObject();
}

void URCameraController::Init(ARModel* InModel)
{
  if(!InModel)
    {
      UE_LOG(LogTemp, Error, TEXT("RobotComandsComponent not attached to ARModel"));
      return;
    }
  Model = InModel;
  TArray<AActor*> FoundActors;
  TArray<URStaticMeshComponent*> ActorComponents;
  // URLink* ReferenceLink;
  URStaticMeshComponent* ReferenceLink;
  Model->GetComponents(ActorComponents);

  for(auto & Component : ActorComponents)
    {
      if(Component->GetName().Equals(CameraRef))
        {
          ReferenceLink = Component;
        }
    }
  if(!ReferenceLink)
    {
      UE_LOG(LogTemp, Error, TEXT("CameraRef not found"));
      return;
    }

  UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), FoundActors);

  for(auto& Camera : FoundActors)
    {
      if(Camera->GetName().Equals(CameraName))
        {

          // URLink* ReferenceLink = Model->Links[CameraRef];
          // Camera->AttachToComponent(ReferenceLink->GetCollision(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
          Camera->AttachToComponent(ReferenceLink, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
          Camera->AddActorLocalOffset(PoseOffset.GetLocation());
        }
      else
        {
          UE_LOG(LogTemp, Error, TEXT("Camera not found"));
        }
    }

}


void URGripperController::Init(ARModel* InModel)
{
	if(!InModel)
	{
		UE_LOG(LogTemp, Error, TEXT("RobotComandsComponent not attached to ARModel"));
	}
	else
	{

          Model = InModel;
          TArray<URGraspComponent* > TempGraspComponents;
          Model->GetComponents<URGraspComponent>(TempGraspComponents);

          RightFinger = Model->Joints.FindRef(RightJointName);
          LeftFinger = Model->Joints.FindRef(LeftJointName);

          RightFingerTip = Model->Joints.FindRef(RightFingerTipName);
          LeftFingerTip = Model->Joints.FindRef(LeftFingerTipName);

          for(auto& GraspComp : TempGraspComponents)
            {
              if(GraspComp->GetName().Equals(GraspComponentName))
                {
                  // UE_LOG(LogTemp, Error, TEXT("Grasp Comp found %s"), *GraspComp->GetName());
                  GraspComponent = GraspComp;
                  URLink* ReferenceLink = Model->Links[GraspCompSetting.GripperName];
                  GraspComponent->AttachToComponent(ReferenceLink->GetCollision(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
                  GraspComponent->AddRelativeLocation(GraspCompSetting.ToolCenterPoint);
                  TArray<URStaticMeshComponent*> TempFixationPoints;
                  TempFixationPoints.Add(RightFinger->Child->GetCollision());
                  TempFixationPoints.Add(LeftFinger->Child->GetCollision());
                  GraspComponent->Init(ReferenceLink->GetCollision(), TempFixationPoints);
                }
            }

          // FString GraspCompName = GetName() + TEXT("_Grasp_Comp");


          //USE CREATEDEFAULTSUBOBJECT IN CONSTRUCTOR OF THE CONTROLLER


          // GraspComponent = NewObject<URGraspComponent>(Model);
          // GraspComponent->RegisterComponent();
          // GraspComponent->SetSimulatePhysics(false);
          // URLink* ReferenceLink = Model->Links[GraspCompSetting.GripperName];
          // GraspComponent->AttachToComponent(ReferenceLink->GetCollision(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
          // GraspComponent->AddRelativeLocation(GraspCompSetting.ToolCenterPoint);
          // GraspComponent->Init(ReferenceLink->GetCollision());
          // GraspComponent->AttachToComponent(Model->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
          // UE_LOG(LogTemp, Error, TEXT("RefLokation %s"), *ReferenceLink->GetCollision()->GetComponentLocation().ToString());
          // UE_LOG(LogTemp, Error, TEXT("GraspPosition %s"), *GraspComponent->GetComponentLocation().ToString());
          // GraspComponent->SetSimulatePhysics(true);



          // RightJointName = GraspCompSetting.GripperName + TEXT("_r_finger_joint");
          // LeftJointName = GraspCompSetting.GripperName + TEXT("_l_finger_joint");

          // RightFingerTipName = GraspCompSetting.GripperName + TEXT("_r_finger_tip_joint");
          // LeftFingerTipName = GraspCompSetting.GripperName + TEXT("_l_finger_tip_joint");
          // SetupCollisionEvent();
          RightFinger->bActuate = false;
          LeftFinger->bActuate = false;
          bStalled = false;
          RightFinger->DesiredJointPose = RightFinger->GetEncoderValue();
          LeftFinger->DesiredJointPose = LeftFinger->GetEncoderValue();

          //next four lines can be deleted four new robots
          RightFinger->Constraint->SetAngularDriveMode(EAngularDriveMode::TwistAndSwing);
          LeftFinger->Constraint->SetAngularDriveMode(EAngularDriveMode::TwistAndSwing);
          // RightFinger->Constraint->SetTargetPosition(RightFinger->Constraint->GetConstraintPosition()*0.9);
          // LeftFinger->Constraint->SetTargetPosition(LeftFinger->Constraint->GetConstraintPosition()*0.9);
          RightFinger->Constraint->SetTargetPosition(0);
          LeftFinger->Constraint->SetTargetPosition(0);
          RightFinger->Constraint->SetAngularDriveParams(Stiffness, Dampening, MaxForce);
          LeftFinger->Constraint->SetAngularDriveParams(Stiffness, Dampening, MaxForce);

          RightFinger->EnableMotor(true);
          LeftFinger->EnableMotor(true);
	}
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
      Base->GetCollision()->SetSimulatePhysics(false);
    }
}

void URControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	float realtimeSeconds = FPlatformTime::Seconds();
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	for(auto& C : Controller.ControllerList)
	{
		C.Value->Tick(DeltaTime);
	}

	ExcecuteCommands();
}

void URControllerComponent::BeginPlay()
{
	Super::BeginPlay();

	// Model = Cast<ARModel>(GetOwner());
	if(!Model)
	{
		UE_LOG(LogTemp, Error, TEXT("Owner is no RModel."));
	}
	else
	{
          Model->Joints["torso_lift_joint"]->DesiredJointPose = 0.10;
          Model->Joints["torso_lift_joint"]->MaxJointVel = 2.0;
          // Model->Joints["torso_lift_joint"]->Encoder->Test();
		for(auto& C : Controller.ControllerList)
		{
			C.Value->Init(Model);
		}
	}
}

void URControllerComponent::SetJointVelocities(TArray<FString> InJointNames, TArray<float> InJointVelocities)
{
	for(int i = 0; i < InJointNames.Num();i++)
	{
		Model->Joints[InJointNames[i]]->SetJointVelocity(InJointVelocities[i]);
	}
}

void URControllerComponent::ExcecuteCommands()
{
	FString Command;
	while(!CommandQuerry.IsEmpty())
	{
		CommandQuerry.Dequeue(Command);
		ExcecuteCommand(Command);
	}
}

void URControllerComponent::ExcecuteCommands(TArray<FString> InCommands)
{
	for(auto& Command : InCommands)
	{
		ExcecuteCommand(Command);
	}

}

void URControllerComponent::ExcecuteCommand(FString InCommand)
{
	//TODO maybe like Exec function from Editor
	//TODO use GripperIndex
	if(InCommand.Equals("GRASP"))
	{
		Grasp("GripperController");
	}
	else if(InCommand.Equals("RELEASE"))
	{
		Release("GripperController");
	}

}

URController* URControllerComponent::ControllerList(FString ControllerName)
{
	if(Controller.ControllerList.Contains(ControllerName))
	{
		return Controller.ControllerList[ControllerName];
	}
	return nullptr;
}

void URControllerComponent::Grasp(FString InGripperIndex)
{
	if(Controller.ControllerList.Contains(InGripperIndex))
	{
		Cast<URGripperController>(Controller.ControllerList[InGripperIndex])->Grasp();
	}

}

void URControllerComponent::Release(FString InGripperIndex)
{
	if(Controller.ControllerList.Contains(InGripperIndex) )
	{
		Cast<URGripperController>(Controller.ControllerList[InGripperIndex])->Release();
	}
}

URHeadTrajectoryController::URHeadTrajectoryController()
{
}

void URHeadTrajectoryController::Init(ARModel* InModel)
{
	bActive = false;
	if(!InModel)
	{
		UE_LOG(LogTemp, Error, TEXT("RobotComandsComponent not attached to ARModel"));
	}
	else
	{
          Model = InModel;
        }
}

void URHeadTrajectoryController::Tick(float InDeltaTime)
{
  CancelAction();
  if(bActive)
    {
      GoalStatusList.Last().Status = 1;
      CheckPointHeadState();
    }
}

FVector URHeadTrajectoryController::CalculateNewViewDirection()
{
  //TODO make it a request to tf
  FVector Direction;
  if(Model)
    {
      FTransform ReferenceLinkTransform ;
      if(FrameId.Equals(TEXT("map")))
        {

        }
      else
        {
          URLink* ReferenceLink = Model->Links.FindRef(FrameId);
          if(!ReferenceLink)
            {
              UE_LOG(LogTemp, Error, TEXT("ReferenceLink %s not found"), *FrameId);
              return FVector();
            }
          ReferenceLinkTransform = ReferenceLink->GetCollision()->GetComponentTransform();
        }

      TArray<URStaticMeshComponent*> ActorComponents;
      Model->GetComponents(ActorComponents);
      URStaticMeshComponent* PointingLink = nullptr;
      for(auto & Component : ActorComponents)
        {
          if(Component->GetName().Contains(PointingFrame))
            {
              PointingLink = Component;
            }
        }
      // URLink* PointingLink = Model->Links.FindRef(PointingFrame);
      if(!PointingLink)
        {
          UE_LOG(LogTemp, Error, TEXT("PointingLink %s not found"), *PointingFrame);
          return FVector();
        }

      // Add offset to get the right position in pr2 high_def_frame
      FVector PointInWorldCoord = ReferenceLinkTransform.GetRotation().RotateVector(Point) +  ReferenceLinkTransform.GetLocation();

      // Direction = PointInWorldCoord - PointingLink->GetCollision()->GetComponentTransform().GetLocation();
      Direction = PointInWorldCoord - (PointingLink->GetComponentQuat().RotateVector(FVector(7.0, 11.0, 12.0)) + PointingLink->GetComponentTransform().GetLocation());

    }
  return Direction;
}

void URPR2HeadTrajectoryController::UpdateHeadDirection()
{
    if(PointingFrame.Equals("high_def_frame"))
    {
        PointingFrame = TEXT("head_tilt_link");
    }

    FVector NewDirection = CalculateNewViewDirection();
    MoveToNewPosition(NewDirection);
}

void URPR2HeadTrajectoryController::CheckPointHeadState()
{
    if(Model)
    {
        URJoint* AzimuthJoint = Model->Joints.FindRef("head_pan_joint");
        URJoint* ElevationJoint = Model->Joints.FindRef("head_tilt_joint");

        float Az = AzimuthJoint->GetJointPosition();
        float El = ElevationJoint->GetJointPosition();

        float DiffAz = AzimuthJoint->DesiredJointPose - Az;
        float DiffEl = ElevationJoint->DesiredJointPose - El;

        if(FMath::Abs(DiffAz) < 0.02 && FMath::Abs(DiffEl) < 0.02 )
            // if(AzEl.Y == 0.0 && AzEl.X == 0.0 )
        {
            GoalStatusList.Last().Status = 3;
            bPublishResult = true;
            bActive = false;
        }
    }
}

void URPR2HeadTrajectoryController::MoveToNewPosition(FVector InNewDirection)
{
  //TODO use max vel from msg
  //TODO Currently only work for objects infront
  if(Model)
    {
      TArray<URStaticMeshComponent*> ActorComponents;
      Model->GetComponents(ActorComponents);
      URStaticMeshComponent* PointingLink = nullptr;
      for(auto & Component : ActorComponents)
        {
          if(Component->GetName().Contains(PointingFrame))
            {
              PointingLink = Component;
            }
        }
      // URLink* PointingLink = Model->Links.FindRef(PointingFrame);
      if(!PointingLink)
        {
          UE_LOG(LogTemp, Error, TEXT("PointingLink %s not found"), *PointingFrame);
          return;
        }
      FQuat ReferenceQuat = PointingLink->GetComponentQuat();
      // FQuat ReferenceQuat = PointingLink->GetCollision()->GetComponentQuat();
      FVector2D AzEl = FMath::GetAzimuthAndElevation(InNewDirection.GetSafeNormal(), ReferenceQuat.GetAxisX(), ReferenceQuat.GetAxisY(), ReferenceQuat.GetAxisZ());


      // UE_LOG(LogTemp, Error, TEXT("HeadTrajController: %f %f"), FMath::RadiansToDegrees(AzEl.X), FMath::RadiansToDegrees(AzEl.Y));

      URJoint* AzimuthJoint = Model->Joints.FindRef("head_pan_joint");
      URJoint* ElevationJoint = Model->Joints.FindRef("head_tilt_joint");

      float Az = AzimuthJoint->GetJointPosition();
      float El = ElevationJoint->GetJointPosition();

      // UE_LOG(LogTemp, Error, TEXT("Elevation: %s"), *(NewDirectionElevation).Euler().ToString());
      // UE_LOG(LogTemp, Error, TEXT("azimuth: %s"), *(NewDirectionAzimuth).Euler().ToString());
      // UE_LOG(LogTemp, Error, TEXT("%f %f"), AzimuthVel, ElevationVel);
      AzimuthJoint->DesiredJointPose = Az - AzEl.X;
      AzimuthJoint->MaxJointVel = 0.17;
      // AzimuthJoint->SetJointVelocity(AzimuthVel);
      ElevationJoint->DesiredJointPose = El - AzEl.Y;
      ElevationJoint->MaxJointVel = 0.17;
      // ElevationJoint->SetJointVelocity(ElevationVel);

    }
}
