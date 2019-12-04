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
          // Model->Joints["torso_lift_joint"]->DesiredJointPose = 0.10;
          // Model->Joints["torso_lift_joint"]->MaxJointVel = 2.0;
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
	// if(Controller.ControllerList.Contains(InGripperIndex))
	// {
	// 	Cast<URGripperController>(Controller.ControllerList[InGripperIndex])->Grasp();
	// }

}

void URControllerComponent::Release(FString InGripperIndex)
{
	// if(Controller.ControllerList.Contains(InGripperIndex) )
	// {
	// 	Cast<URGripperController>(Controller.ControllerList[InGripperIndex])->Release();
	// }
}
