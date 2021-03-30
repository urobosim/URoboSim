#include "ROSCommunication/CheckVisibilityService.h"
#include "ROSCommunication/CheckVisibilityServiceServer.h"

void UCheckVisibilityService::SetOwner(UObject* InOwner)
{
  Owner = Cast<ARModel>(InOwner);
  if(!Owner)
    {
      UE_LOG(LogTemp, Error, TEXT("CheckVisibilityService: Owner is not of type ARModel"));
    }
}

void UCheckVisibilityService::SetType()
{
  Type = TEXT("urobosim_msgs/CheckVisibility");
}

void UCheckVisibilityService::CreateServiceServer()
{
  ServiceServer = MakeShareable<FROSCheckVisebilityServer>(new FROSCheckVisebilityServer(Name, Type, GetWorld(), Owner));
}
