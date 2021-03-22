#include "Factory/RControllerFactory.h"
#include "Factory/RControllerBuilderFactory.h"
#include "Physics/RModel.h"
#include "Kismet/GameplayStatics.h"
#include "Editor/EditorEngine.h"

URControllerFactory::URControllerFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    // AActor subclass this ActorFactory creates.
    NewActorClassName = FString("ARModel");
    NewActorClass = ARModel::StaticClass();
    bDrag = false;
}

bool URControllerFactory::CanCreateActorFrom(const FAssetData & AssetData, FText & OutErrorMsg)
{
    // Only designed for ControllerAsset Asset.
    return AssetData.GetClass()->IsChildOf( URControllerDataAsset::StaticClass());
}

AActor* URControllerFactory::GetDefaultActor(const FAssetData & AssetData)
{
    return NewActorClass->GetDefaultObject<ARModel>();
}


AActor* URControllerFactory::SpawnActor(UObject* Asset, ULevel* InLevel, const FTransform & Transform, EObjectFlags InObjectFlags, const FName Name)
{

  if(bDrag)
    {
      URControllerDataAsset* ControllerAsset = CastChecked<URControllerDataAsset>(Asset);
      if(ControllerAsset)
        {
          ARModel* NewRobot = nullptr;
          TArray<AActor*> WorldActors;
          UGameplayStatics::GetAllActorsOfClass(InLevel->OwningWorld, ARModel::StaticClass(), WorldActors);


          TArray<ARModel*> TargetModel;
          for (auto & Robot : WorldActors)
            {
              UE_LOG(LogTemp, Error, TEXT("Found Model %s"), *Robot->GetName());
              if (ControllerAsset->RobotNames.Contains(Robot->GetName()))
                {
                  UE_LOG(LogTemp, Error, TEXT("Robot found"));
                  TargetModel.Add(Cast<ARModel>(Robot));
                }
            }
          if(TargetModel.Num() != 0)
            {
              URControllerBuilderFactory* ControllerFacotry = NewObject<URControllerBuilderFactory>(this);
              for(auto & Configuration : ControllerAsset->ControllerConfigurations)
                {
                  UE_LOG(LogTemp, Error, TEXT("Create Builder"));
                  URControllerBuilder* ControllerBuilder = ControllerFacotry->CreateBuilder(TargetModel, Configuration);
                  ControllerBuilder->Build();
                }

            }


          return NewRobot;
        }
      else
        {
          UE_LOG(LogTemp, Error, TEXT("Asset cast to USDFDataAsset failed"));

        }

      UE_LOG(LogTemp, Warning, TEXT("No default Robot Actor available\n"));
    }
  else
    {
      bDrag = true;
    }

  // Creates RRobot Actor.
  return nullptr;

}


void URControllerFactory::CreateModels(ARModel* OutModel, USDFDataAsset* SDFData)
{
  // OutModel->Load(Model);
}
