#include "Controller/RController.h"

DEFINE_LOG_CATEGORY_STATIC(LogRController, Log, All);

void URController::Init()
{
  UE_LOG(LogRController, Log, TEXT("Initialize %s"), *GetName())
  if (Cast<ARModel>(GetOuter()))
  {
    Owner = Cast<ARModel>(GetOuter());
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