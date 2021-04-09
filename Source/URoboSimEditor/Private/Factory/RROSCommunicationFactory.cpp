#include "Factory/RROSCommunicationFactory.h"
#include "Factory/RROSCommunicationBuilder.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogRROSCommunicationFactory, Log, All);

URROSCommunicationFactory::URROSCommunicationFactory(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
  // AActor subclass this ActorFactory creates.
  NewActorClassName = FString("AActor");
  NewActorClass = AActor::StaticClass();
}

bool URROSCommunicationFactory::CanCreateActorFrom(const FAssetData &AssetData, FText &OutErrorMsg)
{
  // Only designed for ControllerAsset Asset.
  return AssetData.GetClass()->IsChildOf(URROSCommunicationDataAsset::StaticClass());
}

AActor *URROSCommunicationFactory::GetDefaultActor(const FAssetData &AssetData)
{
  return NewActorClass->GetDefaultObject<AActor>();
}

AActor *URROSCommunicationFactory::SpawnActor(UObject *Asset, ULevel *InLevel, const FTransform &Transform, EObjectFlags InObjectFlags, const FName Name)
{
  static bool bDrag = false;
  if (bDrag)
  {
    URROSCommunicationDataAsset *ROSCommunicationDataAsset = CastChecked<URROSCommunicationDataAsset>(Asset);
    if (ROSCommunicationDataAsset)
    {
      TArray<AActor *> WorldActors;
      UGameplayStatics::GetAllActorsOfClass(InLevel->OwningWorld, ARModel::StaticClass(), WorldActors);

      TArray<ARModel *> Robots;
      for (AActor *&Actor : WorldActors)
      {
        if (ROSCommunicationDataAsset->RobotNames.ContainsByPredicate([&](FString RobotName) { return Actor->GetName().Contains(RobotName); }))
        {
          UE_LOG(LogRROSCommunicationFactory, Log, TEXT("Found ARModel %s in ROSCommunicationDataAsset %s"), *Actor->GetName(), *ROSCommunicationDataAsset->GetName())
          Robots.Add(Cast<ARModel>(Actor));
        }
      }
      if (Robots.Num() != 0)
      {
        URROSCommunicationBuilder *ROSCommunicationBuiler = NewObject<URROSCommunicationBuilder>();
        ROSCommunicationBuiler->Init(Robots, ROSCommunicationDataAsset->ROSCommunicationConfiguration);
        ROSCommunicationBuiler->Build(ROSCommunicationDataAsset->PublisherConfiguration,
                                      ROSCommunicationDataAsset->SubscriberConfiguration,
                                      ROSCommunicationDataAsset->ServiceClientConfiguration,
                                      ROSCommunicationDataAsset->ActionServerConfiguration);
      }
    }
    else
    {
      UE_LOG(LogTemp, Error, TEXT("Asset cast to USDFDataAsset failed"))
    }
  }
  bDrag = !bDrag;
  return nullptr;
}
