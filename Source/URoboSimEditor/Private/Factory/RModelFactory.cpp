#include "Factory/RModelFactory.h"
#include "Controller/ControllerType/RJointController.h"
#include "Controller/ControllerType/RBaseController.h"
#include "Controller/RControllerComponent.h"
#include "ROSCommunication/RROSCommunicationComponent.h"
#include "ROSCommunication/Publisher/RJointStatePublisher.h"
#include "ROSCommunication/Publisher/ROdomPublisher.h"
#include "ROSCommunication/Publisher/RJointTrajectoryControllerStatePublisher.h"
#include "ROSCommunication/Subscriber/RVelocityCommandSubscriber.h"
#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTAServer.h"
#include "Editor/EditorEngine.h"
#include "Factory/RModelBuilder.h"

DEFINE_LOG_CATEGORY_STATIC(LogRModelFactory, Log, All);

URModelFactory::URModelFactory(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
  // AActor subclass this ActorFactory creates.
  NewActorClassName = FString("ARModel");
  NewActorClass = ARModel::StaticClass();
}

bool URModelFactory::CanCreateActorFrom(const FAssetData &AssetData, FText &OutErrorMsg)
{
  // Only designed for SDFData Asset.
  return AssetData.GetClass()->IsChildOf(USDFDataAsset::StaticClass());
}

AActor *URModelFactory::GetDefaultActor(const FAssetData &AssetData)
{
  return NewActorClass->GetDefaultObject<ARModel>();
}

AActor *URModelFactory::SpawnActor(UObject *Asset, ULevel *InLevel, const FTransform &Transform, EObjectFlags InObjectFlags, const FName Name)
{
  USDFDataAsset *SDFAsset = CastChecked<USDFDataAsset>(Asset);
  if (SDFAsset && SDFAsset->Model)
  {
    UE_LOG(LogRModelFactory, Log, TEXT("Create %s"), *SDFAsset->Model->GetName());

    URModelBuilder *ModelBuilder = NewObject<URModelBuilder>(this);
    FActorSpawnParameters SpawnInfo;
    SpawnInfo.OverrideLevel = InLevel;
    SpawnInfo.ObjectFlags = InObjectFlags;
    SpawnInfo.Name = *SDFAsset->Model->GetName();

    ARModel *NewRobot = CastChecked<ARModel>(InLevel->OwningWorld->SpawnActor(NewActorClass, &Transform, SpawnInfo));
    ModelBuilder->Model = NewRobot;
    ModelBuilder->LoadSDF(SDFAsset->Model, Transform);
    if (NewRobot)
    {
      NewRobot->SetActorTransform(Transform);
      FActorLabelUtilities::SetActorLabelUnique(NewRobot, SDFAsset->Model->GetName());
      return NewRobot;
    }
    else
    {
      return nullptr;
    }
  }
  else
  {
    UE_LOG(LogTemp, Error, TEXT("Asset cast to USDFDataAsset failed"));
    return nullptr;
  }
}

void URModelFactory::PostSpawnActor(UObject *Asset, AActor *NewActor)
{
  ARModel *NewRobot = CastChecked<ARModel>(NewActor);
  if (NewRobot)
  {
    // URROSCommunicationComponent *ROSCommunicationComponent = NewObject<URROSCommunicationComponent>(NewRobot, TEXT("ROSCommunicationComponent"));

    // URPublisher *JointStatePublisher = NewObject<URJointStatePublisher>(NewRobot, TEXT("JointStatePublisher"));
    // JointStatePublisher->Topic = TEXT("iai_donbot/body/joint_states");
    // Cast<URJointStatePublisher>(JointStatePublisher)->FrameId = TEXT("odom");
    // ROSCommunicationComponent->AddPublisher(JointStatePublisher);

    // URPublisher *OdomPublisher = NewObject<UROdomPublisher>(NewRobot, TEXT("OdomPublisher"));
    // OdomPublisher->Topic = TEXT("iai_donbot/base/joint_states");
    // Cast<UROdomPublisher>(OdomPublisher)->FrameId = TEXT("odom");
    // ROSCommunicationComponent->AddPublisher(OdomPublisher);

    // URPublisher *JointTrajectoryControllerStatePublisher = NewObject<URJointTrajectoryControllerStatePublisher>(NewRobot, TEXT("JointTrajectoryControllerStatePublisher"));
    // JointTrajectoryControllerStatePublisher->Topic = TEXT("iai_donbot/whole_body_controller/body/state");
    // Cast<URJointTrajectoryControllerStatePublisher>(JointTrajectoryControllerStatePublisher)->FrameId = TEXT("odom");
    // ROSCommunicationComponent->AddPublisher(JointTrajectoryControllerStatePublisher);

    // URSubscriber *VelocityCommandSubscriber = NewObject<URVelocityCommandSubscriber>(NewRobot, TEXT("VelocityCommandSubscriber"));
    // ROSCommunicationComponent->AddSubscriber(VelocityCommandSubscriber);

    // URActionServer *FJTAServer = NewObject<URFJTAServer>(NewRobot, TEXT("FJTAServer"));
    // FJTAServer->ActionName = TEXT("iai_donbot/whole_body_controller/body/follow_joint_trajectory");
    // Cast<URFJTAServer>(FJTAServer)->SetFrameId(TEXT("odom"));
    // Cast<URFJTAServer>(FJTAServer)->SetJointParamPath(TEXT("iai_donbot/whole_body_controller/body/joints"));
    // ROSCommunicationComponent->AddActionServer(FJTAServer);

    // ROSCommunicationComponent->OnComponentCreated();
    
    NewRobot->Init();
  }
}

void URModelFactory::PostCreateBlueprint(UObject *Asset, AActor *CDO)
{
}