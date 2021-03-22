#include "Factory/RModelFactory.h"
#include "Editor/EditorEngine.h"
#include "Factory/RModelBuilder.h"
#include "Controller/ControllerType/RJointController.h"
#include "Controller/RControllerComponent.h"

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
    UE_LOG(LogRModelFactory, Log, TEXT("Model Factory created %s"), *SDFAsset->Model->GetName());

    URModelBuilder *ModelBuilder = NewObject<URModelBuilder>(this);
    ARModel *NewRobot = Cast<ARModel>(Super::SpawnActor(Asset, InLevel, Transform, InObjectFlags, Name));
    ModelBuilder->Model = NewRobot;
    ModelBuilder->LoadSDF(SDFAsset->Model, Transform.GetLocation());
    if (NewRobot)
    {
      URControllerComponent *Controllers = NewObject<URControllerComponent>(NewRobot);
      Controllers->AddController(NewObject<URJointController>(Controllers));
      NewRobot->AddPlugin(Controllers);
      NewRobot->Init();
      NewRobot->SetActorTransform(Transform);
      FActorLabelUtilities::SetActorLabelUnique(NewRobot, SDFAsset->Model->GetName());
    }
    return NewRobot;
  }
  else
  {
    UE_LOG(LogTemp, Error, TEXT("Asset cast to USDFDataAsset failed"));
    return nullptr;
  }
}