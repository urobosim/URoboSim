#include "Factory/RSensorFactory.h"
#include "Factory/RSensorBuilder.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogRSensorFactory, Log, All);

URSensorFactory::URSensorFactory(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
  // AActor subclass this ActorFactory creates.
  NewActorClassName = FString("AActor");
  NewActorClass = AActor::StaticClass();
}

bool URSensorFactory::CanCreateActorFrom(const FAssetData &AssetData, FText &OutErrorMsg)
{
  // Only designed for SensorAsset Asset.
  return AssetData.GetClass()->IsChildOf(URSensorDataAsset::StaticClass());
}

AActor *URSensorFactory::GetDefaultActor(const FAssetData &AssetData)
{
  return NewActorClass->GetDefaultObject<AActor>();
}

AActor *URSensorFactory::SpawnActor(UObject *Asset, ULevel *InLevel, const FTransform &Transform, EObjectFlags InObjectFlags, const FName Name)
{
  static bool bDrag = false;
  if (bDrag)
  {
    URSensorDataAsset *SensorDataAsset = CastChecked<URSensorDataAsset>(Asset);
    if (SensorDataAsset)
    {
      TArray<AActor *> WorldActors;
      UGameplayStatics::GetAllActorsOfClass(InLevel->OwningWorld, ARModel::StaticClass(), WorldActors);

      TArray<ARModel *> Robots;
      for (AActor *&Actor : WorldActors)
      {
        if (SensorDataAsset->RobotNames.ContainsByPredicate([&](FString RobotName){ return Actor->GetName().Contains(RobotName); }))
        {
          UE_LOG(LogRSensorFactory, Log, TEXT("Found ARModel %s in SensorDataAsset %s"), *Actor->GetName(), *SensorDataAsset->GetName())
          Robots.Add(Cast<ARModel>(Actor));
        }
      }
      if (Robots.Num() != 0)
      {
        URSensorBuilder *SensorBuilder = NewObject<URSensorBuilder>();
        SensorBuilder->Init(Robots, SensorDataAsset->SensorConfiguration);
        SensorBuilder->Build();
      }
    }
    else
    {
      UE_LOG(LogRSensorFactory, Error, TEXT("Asset cast to USDFDataAsset failed"))
    }
  }
  bDrag = !bDrag;
  return nullptr;
}
