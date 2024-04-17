#pragma once

#include "RServiceClient.h"
#include "ROSBridgeSrvClient.h"
#include "urobosim_msgs/CollisionDetection.h"
// clang-format off
#include "CollisionDetectionClient.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API UCollisionDetectionClientParameter final : public URServiceClientParameter
{
  GENERATED_BODY()

public:

};

UCLASS()
class UROBOSIM_API UCollisionDetectionClient final : public UServiceClient
{
  GENERATED_BODY()

public:
    UCollisionDetectionClient();

  void SetServiceClientParameters(URServiceClientParameter *&ServiceClientParameters) override;

  void SetRequest(TSharedPtr<FROSBridgeSrv::SrvRequest> InRequest) override;
  void CallService() override;

protected:
  void CreateServiceClient() override;

private:
  TSharedPtr<class FCollisionDetectionClient> ServiceClient;

  TSharedPtr<urobosim_msgs::CollisionDetection::Request> Request;

  TSharedPtr<urobosim_msgs::CollisionDetection::Response> Response;
};

class FCollisionDetectionClient final : public FROSBridgeSrvClient
{
public:
  FCollisionDetectionClient(const FString &InName, const FString &InType);

  void Callback(TSharedPtr<FROSBridgeSrv::SrvResponse> InResponse) override;
};
