#include "Controller/RController.h"
#include "Controller/RControllerComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogRController, Log, All);

void URController::Init()
{
  UE_LOG(LogRController, Log, TEXT("Initialize %s"), *GetName())
  if (!Owner)
  {
    if (Cast<ARModel>(GetOuter()))
    {
      Owner = Cast<ARModel>(GetOuter());
    }
    else if (Cast<URControllerComponent>(GetOuter()) && Cast<ARModel>(Cast<URControllerComponent>(GetOuter())->GetOwner()))
    {
      Owner = Cast<ARModel>(Cast<URControllerComponent>(GetOuter())->GetOwner());
    }
  }
  if (!Owner)
  {
    UE_LOG(LogRController, Error, TEXT("Owner of %s not found, Outer is %s"), *GetName(), *GetOuter()->GetName())
  }
}

ARModel *URController::GetOwner()
{
  if (!Owner)
  {
    UE_LOG(LogRController, Error, TEXT("%s is not initialized"), *GetName())
  }
  return Owner;
}

void URController::CancelAction()
{
  if (bCancel)
  {
    GoalStatusList.Last().Status = 2;
    bPublishResult = true;
  }
}