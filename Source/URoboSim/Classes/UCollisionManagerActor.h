
#pragma once

#include "CoreMinimal.h"
#include "ROSCommunication/Service/Client/CollisionDetectionClient.h"
#include "RosSettings.h"
#include "URoboSimSettings.h"
#include "WorldControlSubsystem.h"
#include "SrvCallbacks/SpawnModelsServer.h"
#include "Conversions.h"
#include "RPluginComponent.h"
#include "Physics/RModel.h"
#include "Controller/RControllerComponent.h"
#include "UCollisionManagerActor.generated.h"


USTRUCT()
struct FCollisionInfo
{
  GENERATED_BODY()
public:

  UPROPERTY()
  AActor* SelfActor = nullptr;

  UPROPERTY()
  AActor* OtherActor = nullptr;

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

  UPROPERTY()
  bool bEventPerception = false;

  ACollisionManagerActor(){

    Client = CreateDefaultSubobject<UCollisionDetectionClient>(FName(*(GetOuter()->GetName() + TEXT("_CollisionDetectionClient"))));
  };

  virtual void BeginPlay() override
  {
    const UURoboSimSettings* URoboSimSettings = GetDefault<UURoboSimSettings>();
	bEventPerception = URoboSimSettings->bEnableEventPerception;
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
	if(bEventPerception)
    {
        AActor* Actor = Cast<AActor>(InObject);
        if(Actor)
          {

            TArray<UStaticMeshComponent*> ActorComponents;
            Actor->GetComponents(ActorComponents);
            for(auto& Comp : ActorComponents)
              {
                Comp->BodyInstance.SetInstanceNotifyRBCollision(true);
                Comp->OnComponentHit.AddDynamic(this, &ACollisionManagerActor::CallCollisionService);
              }
             //UE_LOG(LogTemp, Display, TEXT("Actor %s assertet and startet to track"), *Actor->GetName());
          }
        else
          {
            UE_LOG(LogTemp, Error, TEXT("Object %s not suported"), *InObject->GetName());
          }
    }
  };

  UFUNCTION()
  void CallCollisionService(UPrimitiveComponent* HitComp, AActor* OtherActor,
                            UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
  {
    bool bIsAttached = false;


    if(Client)
      {
        if(ARModel* Model = Cast<ARModel>(OtherActor))
          {
            URControllerComponent* ControllerComp = Cast<URControllerComponent>(Model->GetPlugin(TEXT("ControllerComponent")));
            if(ControllerComp)
              {
                for(auto& Controller: ControllerComp->Controllers)
                  {
                    URGripperControllerBase* GripperCont = Cast<URGripperControllerBase>(Controller);
                    if(GripperCont)
                      {
                        if(GripperCont->GraspComponent->FixatedComponent == HitComp)
                          {
                            bIsAttached = true;
                          }
                      }
                  }
              }
          }

        if(!bIsAttached)
          {
            FString SemlogId = FTags::GetValue(HitComp->GetOwner()->Tags, TEXT("SemLog"), TEXT("Id"));
            if(SemlogId == "")
              {
                UE_LOG(LogTemp, Error, TEXT("SemlogId for %s empty, use actor Name"), *HitComp->GetOwner()->GetName());
                SemlogId = HitComp->GetOwner()->GetName();
              }
            Client->SetRequest(MakeShareable(new urobosim_msgs::CollisionDetection::Request(SemlogId,
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
      }
  };

  UPROPERTY()
  UCollisionDetectionClient* Client;

  UPROPERTY()
  TArray<UObject*> ListOfObjects;

  UPROPERTY()
  TArray<FCollisionInfo> ListOfCollisions;
};
