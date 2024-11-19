#include "ROSCommunication/Service/Client/CollisionDetectionClient.h"

UCollisionDetectionClient::UCollisionDetectionClient()
{
    ServiceName = TEXT("collision_detection");
    ServiceType = TEXT("urobosim_msgs/CollisionDetection");
}

void UCollisionDetectionClient::SetServiceClientParameters(URServiceClientParameter *&ServiceClientParameters)
{
  if (UCollisionDetectionClientParameter *ClientParameter = Cast<UCollisionDetectionClientParameter>(ServiceClientParameters))
  {
    Super::SetServiceClientParameters(ServiceClientParameters);
  }
}

void UCollisionDetectionClient::CreateServiceClient()
{

  ServiceClient = MakeShareable<FCollisionDetectionClient>(new FCollisionDetectionClient(ServiceName, ServiceType));
  // Request = MakeShareable(new urobosim_msgs::CollisionDetection::Request(ObjectA, ComponentA, ObjectB, ComponentB, ImpactPoint, ImpactNormal));
  Request = MakeShareable(new urobosim_msgs::CollisionDetection::Request());
  //   // Create an empty response instance
  Response = MakeShareable(new urobosim_msgs::CollisionDetection::Response());
}

void UCollisionDetectionClient::CallService()
{
  if(Request.IsValid())
    {
     //  UE_LOG(LogTemp, Error, TEXT("Call Service with Request %s"), *Request->ToString());
    }
  Handler->CallService(ServiceClient, Request, Response);
}

FCollisionDetectionClient::FCollisionDetectionClient(const FString &InName, const FString &InType) : FROSBridgeSrvClient(InName, InType)
{
}

void FCollisionDetectionClient::Callback(TSharedPtr<FROSBridgeSrv::SrvResponse> InResponse)
{
  TSharedPtr<urobosim_msgs::CollisionDetection::Response> Response =
      StaticCastSharedPtr<urobosim_msgs::CollisionDetection::Response>(InResponse);

  if(Response.IsValid())
    {
      // UE_LOG(LogTemp, Error, TEXT("Response: %s"), *Response->ToString());
    }
}

void UCollisionDetectionClient::SetRequest(TSharedPtr<FROSBridgeSrv::SrvRequest> InRequest)
{
  Request = StaticCastSharedPtr<urobosim_msgs::CollisionDetection::Request>(InRequest);
}
