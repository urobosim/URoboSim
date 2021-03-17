#include "Factory/RControllerBuilderFactory.h"

void URControllerBuilder::Init(TArray<URControllerComponent*> InControllerComponents, const URControllerConfiguration* InControllerConfiguration)
{
  if(InControllerComponents.Num() == 0)
    {
      return;
    }

  if(!InControllerConfiguration)
    {
      return;
    }

  ControllerComponents = InControllerComponents;
  ControllerConfiguration = InControllerConfiguration;
}

void URControllerBuilder::Build()
{
  for(auto& ControllerComp : ControllerComponents)
    {
      URController* Controller = CreateController(ControllerComp);
      ControllerComp->Controller.ControllerList.Add(ControllerConfiguration->ControllerName, Controller);
    }
}

URController* URGripperControllerBuilder::CreateController(UObject* InOwner)
{
  return NewObject<URGripperController>(InOwner);
}

URControllerBuilder* URControllerFactory::CreateBuilder(TArray<URControllerComponent*> InControllerComponents, const URControllerConfiguration* InControllerConfiguration)
{
  if(InControllerComponents.Num() == 0)
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

  ControllerBuilder->Init(InControllerComponents, ControllerConfiguration);
  return ControllerBuilder;
}

void URControllerFactory::CreateBuilder()
{
  if(ControllerConfiguration->ControllerType == URGripperController::StaticClass())
    {
      ControllerBuilder = NewObject<URGripperControllerBuilder>(this);
      return;
    }
}
