#include "Controller/RController.h"
#include "Controller/RControllerComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogRController, Log, All);

void URController::Init()
{
  // SetOwner();
}

const uint32 URController::GetInitPriority()
{
  return InitPriority;
}

void URController::SetOwner()
{
  if(GetWorld())
    {
      UE_LOG(LogRController, Log, TEXT("SetOwner of %s"), *GetName())
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
          UE_LOG(LogRController, Error, TEXT("Owner of %s not found, Outer is %s"), *GetName(), *GetOuter()->GetName());
        }
    }
}

void URController::CancelAction()
{
  if (bCancel)
  {
    if (bActive)
    {
      GoalStatusList.Last().Status = 6;
      bActive = false;
    }
    else
    {
      GoalStatusList.Last().Status = 2;
    }
    bPublishResult = true;
  }
}
