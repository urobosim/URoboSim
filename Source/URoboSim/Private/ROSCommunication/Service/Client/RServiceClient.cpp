#include "ROSCommunication/Service/Client/RServiceClient.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogRServiceClient, Log, All)

void UServiceClient::SetServiceClientParameters(URServiceClientParameter *&ServiceClientParameters)
{
  if (ServiceClientParameters)
  {
    ServiceName = ServiceClientParameters->ServiceName;
    ServiceType = ServiceClientParameters->ServiceType;
  }
}

void UServiceClient::Init()
{
  CreateServiceClient();
}

void UServiceClient::Tick()
{
  if (Handler.IsValid())
  {
    Handler->Process();
  }
}

void URServiceClient::Init()
{
  SetOwner(GetOuter());
  Super::Init();
}
