#include "ROSCommunication/Publisher/RPublisher.h"
#include "ROSCommunication/RROSCommunicationComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogRPublisher, Log, All)

void URPublisher::Init()
{
  Super::Init();
  CreatePublisher();
}

void URPublisher::Tick()
{
  if (Handler.IsValid())
  {
    Publish();
  }
}

void URPublisher::CreatePublisher()
{
  Publisher = MakeShareable<FROSBridgePublisher>(new FROSBridgePublisher(Topic, MessageType));
  if (Publisher.IsValid())
  {
    Handler->AddPublisher(Publisher);
  }
}

void URPublisher::SetPublishParameters(URPublisherParameter *&PublisherParameters)
{
  if (PublisherParameters)
  {
    Topic = PublisherParameters->Topic;
    MessageType = PublisherParameters->MessageType;
  }
}