#include "ROSCommunication/Action/Server/GripperCommandAction/GCAResultPublisher.h"

DEFINE_LOG_CATEGORY_STATIC(LogGCAResultPublisher, Log, All)

URGCAResultPublisher::URGCAResultPublisher()
{
  MessageType = TEXT("");
}

void URGCAResultPublisher::Init()
{
  Super::Init();
  GripperController = Cast<URGripperController>(Controller);
}

void URGCAResultPublisher::Publish()
{
  // TODO: Implement
}