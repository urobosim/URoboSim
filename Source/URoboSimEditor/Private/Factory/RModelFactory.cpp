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
  static bool bDrag = false;
  if (bDrag)
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

      FTransform ModelPose = Transform;
      ModelPose.AddToTranslation(FVector(0.f, 0.f, 4.f)); // To avoid collision with ground
      ARModel *Model = CastChecked<ARModel>(InLevel->OwningWorld->SpawnActor(NewActorClass, &ModelPose, SpawnInfo));
      ModelBuilder->Model = Model;
      ModelBuilder->LoadSDF(SDFAsset->Model, ModelPose);
      if (Model)
      {
        Model->SetActorTransform(ModelPose);
        FActorLabelUtilities::SetActorLabelUnique(Model, SDFAsset->Model->GetName());
        return Model;
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
  bDrag = !bDrag;
  return nullptr;
}