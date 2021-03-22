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
  URControllerDataAsset* ControllerAsset = CastChecked<URControllerDataAsset>(Asset);
  if(ControllerAsset)
    {
      ARModel* NewRobot = nullptr;
      TArray<AActor*> WorldActors;
      UGameplayStatics::GetAllActorsOfClass(InLevel->OwningWorld, ARModel::StaticClass(), WorldActors);


      TArray<ARModel*> TargetModel;
      for (auto & Robot : WorldActors)
        {
          if (ControllerAsset->RobotNames.Contains(Robot->GetName()))
            {
              UE_LOG(LogTemp, Error, TEXT("Robot found"));
              TargetModel.Add(Cast<ARModel>(Robot));




              /* FActorSpawnParameters SpawnInfo; */
              /* SpawnInfo.OverrideLevel = InLevel; */
              /* SpawnInfo.ObjectFlags = InObjectFlags; */
              /* // SpawnInfo.Name = Name; */

              /* //TODO fix name of spawned model */
              /* SpawnInfo.Name = FName(*Model->Name); */
              /* UE_LOG(LogTemp, Error, TEXT("Create Model %s"), *SpawnInfo.Name.ToString()); */

              /* // Creates RRobot Actor. */

              /* URControllerDataAsset* ModelBuilder = NewObject<URModelBuilder>(this); */
              /* ModelBuilder->Load(Model, NewRobot,FVector(0,0,0)); */
              /* // NewRobot->Load(Model); */
              /* // URControllerFactory::CreateModels(NewRobot, SDFAsset); */
              /* if ( NewRobot ) */
              /*   { */

              /*     // Only do this if the actor wasn't already given a name */
              /*     // if (Name == NAME_None && Asset) */
              /*     //   { */
              /*     //change postion according to transform afer drag and drop */
              /*     NewRobot->SetActorTransform(Transform); */
              /*     FActorLabelUtilities::SetActorLabelUnique(NewRobot, Model->Name); */
                  /* PostSpawnActor(Asset, NewRobot); */
                    // }
            }
        }

      URControllerBuilderFactory* ControllerFacotry = NewObject<URControllerBuilderFactory>(this);
      for(auto & Configuration : ControllerAsset->ControllerConfigurations)
        {
          UE_LOG(LogTemp, Error, TEXT("Create Builder"));
          URControllerBuilder* ControllerBuilder = ControllerFacotry->CreateBuilder(TargetModel, Configuration);
          ControllerBuilder->Build();
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


void URControllerFactory::CreateModels(ARModel* OutModel, USDFDataAsset* SDFData)
{
  // OutModel->Load(Model);
}
