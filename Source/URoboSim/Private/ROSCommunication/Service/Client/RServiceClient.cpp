#include "ROSCommunication/Service/Client/RServiceClient.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogRServiceClient, Log, All)

void URServiceClient::SetServiceClientParameters(URServiceClientParameter *&ServiceClientParameters)
{
  if (ServiceClientParameters)
  {
    ServiceName = ServiceClientParameters->ServiceName;
    ServiceType = ServiceClientParameters->ServiceType;
  }
}

void URServiceClient::Init()
{
  CreateServiceClient();
}

void URServiceClient::Tick()
{
  if (Handler.IsValid())
  {
    Handler->Process();
  }
}