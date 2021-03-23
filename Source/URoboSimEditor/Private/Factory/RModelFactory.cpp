#include "Factory/RModelFactory.h"
#include "Controller/ControllerType/RJointController.h"
#include "Controller/RControllerComponent.h"
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
    UE_LOG(LogRModelFactory, Log, TEXT("Model Factory created %s"), *SDFAsset->Model->GetName());

    URModelBuilder *ModelBuilder = NewObject<URModelBuilder>(this);
    AActor *DefaultActor = GetDefaultActor(FAssetData(Asset));
    if (DefaultActor)
    {
      FActorSpawnParameters SpawnInfo;
      SpawnInfo.OverrideLevel = InLevel;
      SpawnInfo.ObjectFlags = InObjectFlags;
      SpawnInfo.Name = Name;

      ARModel *NewRobot = CastChecked<ARModel>(InLevel->OwningWorld->SpawnActor(DefaultActor->GetClass(), &Transform, SpawnInfo));
      ModelBuilder->Model = NewRobot;
      ModelBuilder->LoadSDF(SDFAsset->Model, Transform.GetLocation());
      if (NewRobot)
      {
        NewRobot->SetActorTransform(Transform);
        FActorLabelUtilities::SetActorLabelUnique(NewRobot, SDFAsset->Model->GetName());
        PostSpawnActor(Asset, NewRobot);
      }
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
    URControllerComponent *Controllers = NewObject<URControllerComponent>(NewRobot, TEXT("Controllers"));
    Controllers->AddController(NewObject<URJointController>(Controllers, TEXT("JointController")));
    NewRobot->AddPlugin(Controllers);
    NewRobot->Init();
  }
}

void URModelFactory::PostCreateBlueprint(UObject *Asset, AActor *CDO)
{
}