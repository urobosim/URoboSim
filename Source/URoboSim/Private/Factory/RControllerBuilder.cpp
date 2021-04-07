#include "Factory/RControllerBuilder.h"
#include "Controller/ControllerType/RBaseController.h"
#include "Controller/ControllerType/RJointController.h"

DEFINE_LOG_CATEGORY_STATIC(LogRControllerBuilder, Log, All);

void URControllerBuilder::Init(const TArray<ARModel *> &InModels, const URControllerConfiguration *&InControllerConfiguration)
{
  if (InModels.Num() == 0)
  {
    UE_LOG(LogRControllerBuilder, Error, TEXT("No models found"))
    return;
  }

  if (!InControllerConfiguration)
  {
    UE_LOG(LogRControllerBuilder, Error, TEXT("No ControllerConfiguration found"))
    return;
  }

  Models = InModels;
  ControllerConfiguration = InControllerConfiguration;
}

void URControllerBuilder::Build()
{
  for (ARModel *&Model : Models)
  {
    URPluginComponent *ControllerComponent = Model->GetPlugin(TEXT("ControllerComponent"));
    if (ControllerComponent)
    {
      UE_LOG(LogRControllerBuilder, Log, TEXT("ControllerComponent of %s found"), *Model->GetName())
    }
    else
    {
      UE_LOG(LogRControllerBuilder, Log, TEXT("ControllerComponent of %s not found, create one"), *Model->GetName())
      ControllerComponent = NewObject<URControllerComponent>(Model, TEXT("ControllerComponent"));
      if (!Cast<URControllerComponent>(ControllerComponent))
      {
        UE_LOG(LogRControllerBuilder, Log, TEXT("ControllerComponent of %s could not be created"), *Model->GetName())
        return;
      }
      else
      {
        ControllerComponent->RegisterComponent();
      }
    }
    UE_LOG(LogRControllerBuilder, Log, TEXT("Create %s of %s"), *ControllerConfiguration->ControllerName, *Model->GetName());
    URController *Controller = CreateController(Model);
    Configure();
    Cast<URControllerComponent>(ControllerComponent)->AddController(Controller);
  }
}

URController *URBaseControllerBuilder::CreateController(ARModel *&InOwner)
{
  return NewObject<URBaseController>(InOwner, *ControllerConfiguration->ControllerName);
}

void URBaseControllerBuilder::Configure()
{
}

URController *URJointControllerBuilder::CreateController(ARModel *&InOwner)
{
  return NewObject<URJointController>(InOwner, *ControllerConfiguration->ControllerName);
}

void URJointControllerBuilder::Configure()
{
}

URControllerBuilder *CreateBuilder(const TArray<ARModel *> InModels, const URControllerConfiguration *&InControllerConfiguration)
{
  URControllerBuilder *ControllerBuilder = nullptr;
  if (!InControllerConfiguration->ControllerType)
  {
    UE_LOG(LogRControllerBuilder, Error, TEXT("ControllerType is not set"));
  }
  else if (InControllerConfiguration->ControllerType == URJointController::StaticClass())
  {
    ControllerBuilder = NewObject<URJointControllerBuilder>();
    ControllerBuilder->Init(InModels, InControllerConfiguration);
  }
  else if (InControllerConfiguration->ControllerType == URBaseController::StaticClass())
  {
    ControllerBuilder = NewObject<URBaseControllerBuilder>();
    ControllerBuilder->Init(InModels, InControllerConfiguration);
  }
  return ControllerBuilder;
}