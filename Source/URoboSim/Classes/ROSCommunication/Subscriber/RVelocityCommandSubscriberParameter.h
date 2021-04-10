#pragma once

#include "RSubscriberParameter.h"
// clang-format off
#include "RVelocityCommandSubscriberParameter.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URVelocityCommandSubscriberParameter : public URSubscriberParameter
{
  GENERATED_BODY()

public:
  URVelocityCommandSubscriberParameter()
  {
    Topic = TEXT("/base_controller/command");
    MessageType = TEXT("geometry_msgs/Twist");
    BaseControllerName = TEXT("BaseController");
  }

public:
  UPROPERTY(EditAnywhere)
  FString BaseControllerName;
};