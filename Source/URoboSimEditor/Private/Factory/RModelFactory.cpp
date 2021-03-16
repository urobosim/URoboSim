#include "Factory/RModelFactory.h"
#include "Factory/RModelBuilder.h"
// #include "Physics/RModel.h"
#include "Editor/EditorEngine.h"

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
  if (SDFAsset)
  {
    ARModel *NewRobot = nullptr;
    for (USDFModel *Model : SDFAsset->Models)
    {
      AActor *DefaultActor = GetDefaultActor(FAssetData(Asset));
      if (DefaultActor)
      {
        FActorSpawnParameters SpawnInfo;
        SpawnInfo.OverrideLevel = InLevel;
        SpawnInfo.ObjectFlags = InObjectFlags;
        // SpawnInfo.Name = Name;

        //TODO fix name of spawned model
        SpawnInfo.Name = FName(*Model->GetName());
        UE_LOG(LogTemp, Error, TEXT("Create Model %s"), *SpawnInfo.Name.ToString());

        // Creates RRobot Actor.

        URModelBuilder *ModelBuilder = NewObject<URModelBuilder>(this);
        NewRobot = (ARModel *)InLevel->OwningWorld->SpawnActor(DefaultActor->GetClass(), &Transform, SpawnInfo);
        ModelBuilder->Model = NewRobot;
        ModelBuilder->LoadSDF(Model);
        if (NewRobot)
        {

          // Only do this if the actor wasn't already given a name
          // if (Name == NAME_None && Asset)
          //   {
          //change postion according to transform afer drag and drop
          NewRobot->SetActorTransform(Transform);
          FActorLabelUtilities::SetActorLabelUnique(NewRobot, Model->GetName());
          PostSpawnActor(Asset, NewRobot);
          //   }
        }
      }
    }
    return NewRobot;
  }
  else
  {
    UE_LOG(LogTemp, Error, TEXT("Asset cast to USDFDataAsset failed"));
    return nullptr;
  }
}

// void URModelFactory::CreateModels(ARModel *OutModel, USDFDataAsset *SDFData)
// {
//   // OutModel->Load(Model);
// }
