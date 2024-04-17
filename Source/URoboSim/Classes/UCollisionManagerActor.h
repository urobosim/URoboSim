
#pragma once

#include "CoreMinimal.h"
#include "ROSCommunication/Service/Client/CollisionDetectionClient.h"
#include "RosSettings.h"
#include "WorldControlSubsystem.h"
#include "SrvCallbacks/SpawnModelsServer.h"
#include "Conversions.h"
#include "UCollisionManagerActor.generated.h"



USTRUCT()
struct FCollisionInfo
{
  GENERATED_BODY()
public:

  UPROPERTY()
  AActor* SelfActor;

  UPROPERTY()
  AActor* OtherActor;

  UPROPERTY()
  FVector NormalImpulse;

  UPROPERTY()
  FHitResult HitResult;
};

UCLASS( ClassGroup=Physics, meta=(BlueprintSpawnableComponent), DefaultToInstanced, ConversionRoot, MinimalAPI, ComponentWrapperClass)
class ACollisionManagerActor : public AActor
{
  GENERATED_BODY()
  public:

  ACollisionManagerActor(){

    Client = CreateDefaultSubobject<UCollisionDetectionClient>(FName(*(GetOuter()->GetName() + TEXT("_CollisionDetectionClient"))));
  };

  virtual void BeginPlay() override
  {
    const URosSettings* Settings = GetDefault<URosSettings>();
    FString WebsocketIPAddr = Settings->ROSBridgeServerHost;
    uint32 WebsocketPort = Settings->ROSBridgeServerPort;

    Client->Connect(WebsocketIPAddr, WebsocketPort);


    UWorldControlSubsystem* Subsystem = nullptr;
    UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(this);
    if(GameInstance)
      {
        Subsystem = GameInstance->GetSubsystem<UWorldControlSubsystem>();
      }

    if(Subsystem)
      {

        for (auto& Service : Subsystem->Manager->ServicesToPublish)
          {
            TSharedPtr<FROSSpawnModelServer> Ptr =  StaticCastSharedPtr<FROSSpawnModelServer>(Service);
            if(Ptr.IsValid())
              {
                if(Ptr->GetName().Contains(TEXT("spawn_model")))
                  {
                    Ptr->OnObjectAsserted.AddUObject(this, &ACollisionManagerActor::AddObjectToCollisionCheck);
                  }
              }
          }
      }



    // Client->Request->ObjectA = TEXT("TEST");

    // FTimerDelegate D = FTimerDelegate::CreateUObject( Client,  &UCollisionDetectionClient::CallService, ActionStart.GetTimeAsDouble(), );


    // FTimerHandle MyTimerHandle;
    // float t = 1.0;
    // GetWorldTimerManager().SetTimer(MyTimerHandle, Client, &UCollisionDetectionClient::CallService, t, false);
  };


  UFUNCTION()
  void AddObjectToCollisionCheck(UObject * InObject){
    AActor* Actor = Cast<AActor>(InObject);

    if(Actor)
      {

        TArray<UStaticMeshComponent*> ActorComponents;
        Actor->GetComponents(ActorComponents);
        for(auto& Comp : ActorComponents)
          {
            Comp->OnComponentHit.AddDynamic(this, &ACollisionManagerActor::CallCollisionService);
          }
        // UE_LOG(LogTemp, Display, TEXT("Actor %s assertet and startet to track"), *Actor->GetName());
      }
    else
      {
        UE_LOG(LogTemp, Error, TEXT("Object %s not suported"), *InObject->GetName());
      }
    // OnComponentBeginOverlap.AddUniqueDynamic(this, &URGraspComponent::OnFixationGraspAreaBeginOverlap);
    //OnComponentEndOverlap.AddUniqueDynamic(this, &URGraspComponent::OnFixationGraspAreaEndOverlap);
    // void UCollisionEventManager::Callback(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
    // void URGraspComponent::OnFixationGraspAreaBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
    //                                                        class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    //                                                        bool bFromSweep, const FHitResult & SweepResult)
  };

  UFUNCTION()
  void CallCollisionService(UPrimitiveComponent* HitComp, AActor* OtherActor,
                            UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
  {


    if(Client)
      {
        Client->SetRequest(MakeShareable(new urobosim_msgs::CollisionDetection::Request(HitComp->GetOwner()->GetName(),
                                                                                        HitComp->GetName(),
                                                                                        OtherActor->GetName(),
                                                                                        OtherComp->GetName(),
                                                                                        geometry_msgs::Vector3(HitComp->GetPhysicsLinearVelocity()),
                                                                                        geometry_msgs::Vector3(OtherComp->GetPhysicsLinearVelocity()),
                                                                                        geometry_msgs::Pose(geometry_msgs::Point(FConversions::UToROS(HitComp->GetComponentLocation())),
                                                                                                            geometry_msgs::Quaternion(FConversions::UToROS(HitComp->GetComponentQuat()))),
                                                                                        geometry_msgs::Pose(geometry_msgs::Point(FConversions::UToROS(OtherComp->GetComponentLocation())),
                                                                                                            geometry_msgs::Quaternion(FConversions::UToROS(OtherComp->GetComponentQuat()))),
                                                                                        geometry_msgs::Vector3(FConversions::UToROS(Hit.ImpactPoint)),
                                                                                        geometry_msgs::Vector3(FConversions::UToROS(Hit.ImpactNormal)))));
        Client->CallService();
      }
  };

  UPROPERTY()
  UCollisionDetectionClient* Client;

  UPROPERTY()
  TArray<UObject*> ListOfObjects;

  UPROPERTY()
  TArray<FCollisionInfo> ListOfCollisions;
};
