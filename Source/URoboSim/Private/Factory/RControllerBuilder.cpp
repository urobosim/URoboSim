#include "Factory/RControllerBuilder.h"
#include "Controller/ControllerType/RBaseController.h"
#include "Controller/ControllerType/RJointController.h"

DEFINE_LOG_CATEGORY_STATIC(LogRControllerBuilder, Log, All);

void URControllerBuilder::Init(const TArray<ARModel *> &InModels, const FRControllerConfiguration &InControllerConfiguration)
{
  if (InModels.Num() == 0)
  {
    UE_LOG(LogRControllerBuilder, Error, TEXT("No models found"))
    return;
  }

  if (InControllerConfiguration.ControllerParameters.Num() == 0)
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
    URPluginComponent *ControllerComponent = NewObject<URControllerComponent>(Model, TEXT("ControllerComponent"));
    ControllerComponent->RegisterComponent();

    for (TPair<FString, URControllerParameter *> ControllerParameters : ControllerConfiguration.ControllerParameters)
    {
      UE_LOG(LogRControllerBuilder, Log, TEXT("Create %s of %s"), *ControllerParameters.Key, *Model->GetName());
      URController *Controller = CreateController(Model, ControllerParameters);
      Controller->SetControllerParameters(ControllerParameters.Value);
      Cast<URControllerComponent>(ControllerComponent)->AddController(Controller);
    }
  }
}

URController *URControllerBuilder::CreateController(ARModel *&InOwner, const TPair<FString, URControllerParameter *> ControllerParameters)
{
  if (Cast<URJointControllerParameter>(ControllerParameters.Value))
  {
    return NewObject<URJointController>(InOwner, *ControllerParameters.Key);
  }
  else if (Cast<URBaseControllerParameter>(ControllerParameters.Value))
  {
    return NewObject<URBaseController>(InOwner, *ControllerParameters.Key);
  }
  else
  {
    return NewObject<URController>(InOwner, *ControllerParameters.Key);
  }
}