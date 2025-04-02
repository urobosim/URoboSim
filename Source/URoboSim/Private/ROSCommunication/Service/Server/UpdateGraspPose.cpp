#include "ROSCommunication/Service/Server/UpdateGraspPose.h"
#include "Controller/RController.h"
#include "EngineUtils.h"
#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "world_control_msgs/srv/SetModelPose.h"
#include "Tags.h"
#include "DrawDebugHelpers.h"
#include "AssetUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogRUpdateGraspPoseServer, Log, All)

URUpdateGraspPoseServer::URUpdateGraspPoseServer()
{
  Type = TEXT("world_control_msgs/SetModelPose");
}

void URUpdateGraspPoseServer::CreateServiceServer()
{
  ServiceServer = MakeShareable<FRUpdateGraspPoseServerCallback>(new FRUpdateGraspPoseServerCallback(Name, Type, GetWorld(), GetOwner()));
}

FRUpdateGraspPoseServerCallback::FRUpdateGraspPoseServerCallback(FString Name, FString Type, UWorld *InWorld, UObject *InOwner) : FROSBridgeSrvServer(Name, Type)
{
  World = InWorld;
  Owner = Cast<ARModel>(InOwner);
}

TSharedPtr<FROSBridgeSrv::SrvRequest> FRUpdateGraspPoseServerCallback::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
  TSharedPtr<FROSSetModelPoseSrv::Request> Request =
      MakeShareable(new FROSSetModelPoseSrv::Request());
  Request->FromJson(JsonObject);
  return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request);
}


TSharedPtr<FROSBridgeSrv::SrvResponse> FRUpdateGraspPoseServerCallback::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
  TSharedPtr<FROSSetModelPoseSrv::Request> UpdateGraspPoseRequest =
      StaticCastSharedPtr<FROSSetModelPoseSrv::Request>(Request);

  FString UniqueId = UpdateGraspPoseRequest->GetId();
  FVector Location = FVector(-9.393488, -91.5171, 124);
  FQuat Rotator = FQuat::MakeFromEuler(FVector(270, 178, 80));

  TArray<FString> GripperControllerList;
  GripperControllerList.Add(TEXT("L1GripperController"));
  TArray<FString> ActiveControllerList;
  bool bActorAttached = false;
  bool ServiceSuccess = false;

  bool bSuccess = false;
  if (Owner)
  {
      if (!World)
      {
        UE_LOG(LogRUpdateGraspPoseServer, Error, TEXT("World not found"));
      }
      else
      {
        // Execute on game thread
        FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]() {

                World = GEngine->GameViewport->GetWorld();

                for(auto& GC: GripperControllerList)
                  {
                    URGripperControllerBase *GripperController = Cast<URGripperControllerBase>(Owner->GetController(GC));
                    if (GripperController)
                      {
                        AStaticMeshActor* Actor = GripperController->GraspComponent->FixatedObject;

			UAssetUtils* AssetUtil = NewObject<UAssetUtils>(Actor);
                        if(Actor)
                          {

			  TArray<UPhysicsConstraintComponent*> Constraints;
			  Actor->GetComponents<UPhysicsConstraintComponent>(Constraints, false);

			  TArray<UStaticMeshComponent*> Components;
			  Actor->GetComponents<UStaticMeshComponent>(Components, false);

			  UStaticMeshComponent* Oj1 = nullptr;
			  UStaticMeshComponent* Oj2 = nullptr;

			  UStaticMeshComponent* Root1 = nullptr;
			  UStaticMeshComponent* Root2 = nullptr;
			  UStaticMeshComponent* Parent1 = nullptr;
			  UStaticMeshComponent* Parent2 = nullptr;

			  // Prevent objects being checked twice if multiple constraints are connected to an object
			  TArray<UStaticMeshComponent*> HandledObject;

			  for(auto& Constraint : Constraints)
			  {
				  if(!Constraint->IsBroken())
				  {
					  //TODO: Should it be checked if physics is enabled or not?
					  if(AActor* Actor1 = Constraint->ConstraintActor1)
					  {
						  Oj1 = Cast<UStaticMeshComponent>(Actor1->GetDefaultSubobjectByName(Constraint->ComponentName1.ComponentName));
						  if(Oj1)
						  {
							  // Only change physics of Oj1 if it simulates Physics, else find the root component that has physics enable and therfore is movable
							  if(!HandledObject.Contains(Oj1))
							  {
								  Parent1 = Cast<UStaticMeshComponent>(Oj1->GetAttachParent());
								  if(Parent1)
								  {
									  while(Parent1)
									  {
										  if(Parent1->GetAttachParent())
										  {
											  Parent1 = Cast<UStaticMeshComponent>(Parent1->GetAttachParent());
										  }
										  else
										  {
											  Root1 = Parent1;
											  break;
										  }
									  }
								  }
								  else
								  {
									  Root1 = Oj1;
								  }
								  Root1->SetSimulatePhysics(false);
								  HandledObject.Add(Oj1);
								  UE_LOG(LogTemp, Warning, TEXT("[%s]: Root1 Name %s"), *FString(__FUNCTION__), *Root1->GetName());
								  if(Cast<USceneComponent>(Root1) != Actor->GetRootComponent())
								  {
									  Root1->AttachToComponent(Actor->GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::KeepWorld, false), NAME_None);
								  }
							  }
						  }
					  }

					  if(AActor* Actor2 = Constraint->ConstraintActor2)
					  {
						  Oj2 = Cast<UStaticMeshComponent>(Actor2->GetDefaultSubobjectByName(Constraint->ComponentName2.ComponentName));
						  if(Oj2)
						  {
							  if(!HandledObject.Contains(Oj2))
							  {
								  // Only change physics of Oj1 if it simulates Physics, else find the root component that has physics enable and therfore is movable
								  Parent2 = Cast<UStaticMeshComponent>(Oj2->GetAttachParent());
								  if(Parent2)
								  {
									  while(Parent2)
									  {
										  if(Parent2->GetAttachParent())
										  {
											  Parent2 = Cast<UStaticMeshComponent>(Parent2->GetAttachParent());
										  }
										  else
										  {
											  Root2 = Parent2;
											  break;
										  }
									  }
								  }
								  else
								  {
									  Root2 = Oj2;
								  }
								  Root2->SetSimulatePhysics(false);
								  HandledObject.Add(Oj2);
								  UE_LOG(LogTemp, Warning, TEXT("[%s]: Root2 Name %s"), *FString(__FUNCTION__), *Root2->GetName());

								  if(Cast<USceneComponent>(Root2) != Actor->GetRootComponent())
								  {
									  Root2->AttachToComponent(Actor->GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::KeepWorld, false), NAME_None);
								  }
							  }
						  }
					  }
					  if(Root2)
					  {
						  UE_LOG(LogTemp, Warning, TEXT("[%s]: 1 %s"), *FString(__FUNCTION__), *Root2->GetName());
					  }
					  else
					  {
						  UE_LOG(LogTemp, Warning, TEXT("[%s]: 2"), *FString(__FUNCTION__));
					  }

					  //FTimerDelegate ConstraintInit = FTimerDelegate::CreateUObject( AssetUtil,  &UAssetUtils::ReinitConstraintAndSetPhysics, Constraint, Root1, Root2, true);
					  //Actor->GetWorldTimerManager().SetTimerForNextTick(ConstraintInit);
				  }
			  }

			  Actor->SetActorLocationAndRotation(Location, Rotator, false, nullptr, ETeleportType::ResetPhysics);
			  bSuccess = true;

			  for(auto& Constraint : Constraints)
			  {
				  if(!Constraint->IsBroken())
				  {
					  AssetUtil->ReinitConstraintAndSetPhysics(Constraint, Root1, Root2, true);
				  }
			  }
			  UPhysicsConstraintComponent* GripperConstraint = GripperController->GraspComponent->Constraint;
			  bActorAttached = true;
			  GripperConstraint->InitComponentConstraint();
			  }
                      }
                  }
                if(!bActorAttached)
                  {
                    UE_LOG(LogTemp, Warning, TEXT("Actor with id:\"%s\" is not grasped and can therefore not be moved."), *UniqueId);
                  }


        }, TStatId(), nullptr, ENamedThreads::GameThread);

        //wait code above to complete
        FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
      }

  }
	return MakeShareable<FROSBridgeSrv::SrvResponse>
		(new FROSSetModelPoseSrv::Response(ServiceSuccess));
}
