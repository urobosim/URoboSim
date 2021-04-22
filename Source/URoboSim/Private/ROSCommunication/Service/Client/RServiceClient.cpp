#include "ROSCommunication/Service/Client/RServiceClient.h"
#include "ROSCommunication/RROSCommunicationComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogRServiceClient, Log, All)

void URServiceClient::Tick()
{
  if (Handler.IsValid())
  {
    Handler->Process();
  }
}

void URServiceClient::SetServiceClientParameters(URServiceClientParameter *&ServiceClientParameters)
{
  if (ServiceClientParameters)
  {
    ServiceName = ServiceClientParameters->ServiceName;
    ServiceType = ServiceClientParameters->ServiceType;
  }
}