#include "Factory/RControllerBuilderFactory.h"
#include "Components/ActorComponent.h"

void URControllerBuilder::Init(TArray<ARModel*> InModels, const URControllerConfiguration* InControllerConfiguration)
{
  if(InModels.Num() == 0)
    {
      return;
    }

  if(!InControllerConfiguration)
    {
      return;
    }

  Models = InModels;
  ControllerComponents.Empty();
  ControllerConfiguration = InControllerConfiguration;
}

void URControllerBuilder::Build()
{
  for(auto& Model : Models)
    {
      URControllerComponent* ControllerComponent = Cast<URControllerComponent>(Model->Plugins["ControllerComponent"]);
      if(ControllerComponent)
        {
          ControllerComponents.Add(ControllerComponent);
          UE_LOG(LogTemp, Error, TEXT("Controller Comp found"));
        }
      else
        {
          // ControllerComponent = NewObject<URControllerComponent*>(Model);
          UE_LOG(LogTemp, Error, TEXT("Create Controller Comp"));
          ControllerComponent = Cast<URControllerComponent>(Model->AddComponent(FName(*URControllerComponent::StaticClass()->GetName()), true, FTransform(), nullptr));
          if(ControllerComponent)
            {
              ControllerComponents.Add(ControllerComponent);
            }
          else
            {
              UE_LOG(LogTemp, Error, TEXT("ControllerComponent could not be created"));
            }

        }
    }

  for(auto& ControllerComp : ControllerComponents)
    {
      UE_LOG(LogTemp, Error, TEXT("Create Controller"));
      URController* Controller = CreateController(ControllerComp);
      ControllerComp->Controller.ControllerList.Add(ControllerConfiguration->ControllerName, Controller);
    }
}

URController* URGripperControllerBuilder::CreateController(UObject* InOwner)
{
  return NewObject<URGripperController>(InOwner);
}

URControllerBuilder* URControllerBuilderFactory::CreateBuilder(TArray<ARModel*> InModels, const URControllerConfiguration* InControllerConfiguration)
{
  if(InModels.Num() == 0)
    {
      UE_LOG(LogTemp, Error, TEXT("No Robot Specified"));
      return nullptr;
    }

  if(!InControllerConfiguration)
    {
      UE_LOG(LogTemp, Error, TEXT("No controller configuration provided"));
      return nullptr;
    }
  else
    {
      ControllerConfiguration = InControllerConfiguration;
    }

  if(!ControllerConfiguration->ControllerType)
    {
      UE_LOG(LogTemp, Error, TEXT("No ControllerType set"));
      return nullptr;
    }

  CreateBuilder();

  ControllerBuilder->Init(InModels, ControllerConfiguration);
  return ControllerBuilder;
}

void URControllerBuilderFactory::CreateBuilder()
{
  if(ControllerConfiguration->ControllerType == URGripperController::StaticClass())
    {
      ControllerBuilder = NewObject<URGripperControllerBuilder>(this);
      return;
    }
}
