#include "ROSCommunication/Publisher/RPublisher.h"
#include "ROSCommunication/RROSCommunicationComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogRPublisher, Log, All)

void UPublisher::SetPublishParameters(URPublisherParameter *&PublisherParameters)
{
  if (PublisherParameters)
  {
    Topic = PublisherParameters->Topic;
    MessageType = PublisherParameters->MessageType;
  }
}

void UPublisher::Init()
{
  CreatePublisher();
  if (Publisher.IsValid())
  {
    Handler->AddPublisher(Publisher);
  }
}

void UPublisher::Tick()
{
  if (Handler.IsValid())
  {
    Publish();
  }
}

void UPublisher::CreatePublisher()
{
  Publisher = MakeShareable<FROSBridgePublisher>(new FROSBridgePublisher(Topic, MessageType));
}

void URPublisher::Init()
{
  SetOwner(GetOuter());
  Super::Init();
}
