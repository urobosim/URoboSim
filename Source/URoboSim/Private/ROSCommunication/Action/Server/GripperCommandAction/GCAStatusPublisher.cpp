#include "ROSCommunication/Action/Server/GripperCommandAction/GCAStatusPublisher.h"

DEFINE_LOG_CATEGORY_STATIC(LogGCAStatusPublisher, Log, All)

URGCAStatusPublisher::URGCAStatusPublisher()
{
  MessageType = TEXT("");
}

void URGCAStatusPublisher::Init()
{
  Super::Init();
  GripperController = Cast<URGripperController>(Controller);
}

void URGCAStatusPublisher::Publish()
{
  // TODO: Implement
}