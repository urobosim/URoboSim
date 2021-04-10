#include "Factory/RControllerFactory.h"
#include "Factory/RControllerBuilder.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogRControllerFactory, Log, All);

URControllerFactory::URControllerFactory(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
  // AActor subclass this ActorFactory creates.
  NewActorClassName = FString("AActor");
  NewActorClass = AActor::StaticClass();
}

bool URControllerFactory::CanCreateActorFrom(const FAssetData &AssetData, FText &OutErrorMsg)
{
  // Only designed for ControllerAsset Asset.
  return AssetData.GetClass()->IsChildOf(URControllerDataAsset::StaticClass());
}

AActor *URControllerFactory::GetDefaultActor(const FAssetData &AssetData)
{
  return NewActorClass->GetDefaultObject<AActor>();
}

AActor *URControllerFactory::SpawnActor(UObject *Asset, ULevel *InLevel, const FTransform &Transform, EObjectFlags InObjectFlags, const FName Name)
{
  static bool bDrag = false;
  if (bDrag)
  {
    URControllerDataAsset *ControllerDataAsset = CastChecked<URControllerDataAsset>(Asset);
    if (ControllerDataAsset)
    {
      TArray<AActor *> WorldActors;
      UGameplayStatics::GetAllActorsOfClass(InLevel->OwningWorld, ARModel::StaticClass(), WorldActors);

      TArray<ARModel *> Robots;
      for (AActor *&Actor : WorldActors)
      {
        if (ControllerDataAsset->RobotNames.ContainsByPredicate([&](FString RobotName){ return Actor->GetName().Contains(RobotName); }))
        {
          UE_LOG(LogRControllerFactory, Log, TEXT("Found ARModel %s in ControllerDataAsset %s"), *Actor->GetName(), *ControllerDataAsset->GetName())
          Robots.Add(Cast<ARModel>(Actor));
        }
      }
      if (Robots.Num() != 0)
      {
        URControllerBuilder *ControllerBuilder = NewObject<URControllerBuilder>();
        ControllerBuilder->Init(Robots, ControllerDataAsset->ControllerConfiguration);
        ControllerBuilder->Build();
      }
    }
    else
    {
      UE_LOG(LogRControllerFactory, Error, TEXT("Asset cast to USDFDataAsset failed"))
    }
  }
  bDrag = !bDrag;
  return nullptr;
}
