#include "ROSCommunication/Action/Server/PerceiveObjectAction/POAFeedbackPublisher.h"
#include "urobosim_msgs/PerceiveObjectActionFeedback.h"

URPOAFeedbackPublisher::URPOAFeedbackPublisher()
{
  MessageType = TEXT("urobosim_msgs/PerceiveObjectActionFeedback");
}

void URPOAFeedbackPublisher::Init()
{
  Super::Init();
  CameraController = Cast<URCameraController>(Controller);
}

void URPOAFeedbackPublisher::Publish()
{
  if (CameraController)
  {
    /* code */
  }
}