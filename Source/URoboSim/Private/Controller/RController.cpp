#include "Controller/RController.h"

DEFINE_LOG_CATEGORY_STATIC(LogRController, Log, All);

ARModel *URController::GetOwner()
{
  if (!Owner)
  {
    UE_LOG(LogRController, Error, TEXT("%s is not initialized"), *GetName())
  }
  return Owner;
}

// void URController::CancelAction()
// {
//   if (bCancel)
//   {
//     if (bActive)
//     {
//       GoalStatusList.Last().Status = 6;
//       bActive = false;
//     }
//     else
//     {
//       GoalStatusList.Last().Status = 2;
//       bActive = false;
//     }
//     bPublishResult = true;
//   }
// }