#include "Factory/RModelFactory.h"
#include "Factory/RModelBuilder.h"
#include "Physics/RModel.h"
#include "Editor/EditorEngine.h"

URModelFactory::URModelFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    // AActor subclass this ActorFactory creates.
    NewActorClassName = FString("ARModel");
    NewActorClass = ARModel::StaticClass();
}

bool URModelFactory::CanCreateActorFrom(const FAssetData & AssetData, FText & OutErrorMsg)
{
    // Only designed for SDFData Asset.
    return AssetData.GetClass()->IsChildOf( USDFDataAsset::StaticClass());
    // return AssetData.AssetClass.IsEqual(FName("SDFDataAsset"));
}

AActor* URModelFactory::GetDefaultActor(const FAssetData & AssetData)
{
    return NewActorClass->GetDefaultObject<ARModel>();
}


AActor* URModelFactory::SpawnActor(UObject* Asset, ULevel* InLevel, const FTransform & Transform, EObjectFlags InObjectFlags, const FName Name)
{
    USDFDataAsset* SDFAsset = CastChecked<USDFDataAsset>(Asset);
  if(SDFAsset)
    {
      ARModel* NewRobot = nullptr;
      for(USDFModel* Model : SDFAsset->Models)
        {
          AActor* DefaultActor = GetDefaultActor(FAssetData(Asset));
          if (DefaultActor)
            {
              FActorSpawnParameters SpawnInfo;
              SpawnInfo.OverrideLevel = InLevel;
              SpawnInfo.ObjectFlags = InObjectFlags;
              // SpawnInfo.Name = Name;

              //TODO fix name of spawned model
              SpawnInfo.Name = FName(*Model->Name);
              UE_LOG(LogTemp, Error, TEXT("Create Model %s"), *SpawnInfo.Name.ToString());

              // Creates RRobot Actor.

              URModelBuilder* ModelBuilder = NewObject<URModelBuilder>(this);
              NewRobot = (ARModel*)InLevel->OwningWorld->SpawnActor(DefaultActor->GetClass(), &Transform, SpawnInfo);
              ModelBuilder->Load(Model, NewRobot,FVector(0,0,0));
              // NewRobot->Load(Model);
              // URModelFactory::CreateModels(NewRobot, SDFAsset);
              if ( NewRobot )
                {

                  // Only do this if the actor wasn't already given a name
                  // if (Name == NAME_None && Asset)
                  //   {
                   //change postion according to transform afer drag and drop
                  NewRobot->SetActorTransform(Transform);
                  FActorLabelUtilities::SetActorLabelUnique(NewRobot, Model->Name);
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

    }

  // Creates RRobot Actor.
  UE_LOG(LogTemp, Warning, TEXT("No default Robot Actor available\n"));
  return nullptr;

}


void URModelFactory::CreateModels(ARModel* OutModel, USDFDataAsset* SDFData)
{
  // OutModel->Load(Model);
}
