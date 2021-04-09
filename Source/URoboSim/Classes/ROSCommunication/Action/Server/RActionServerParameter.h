#pragma once

#include "ROSCommunication/Publisher/RPublisherParameter.h"
// clang-format off
#include "RActionServerParameter.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URActionStatusPublisherParameter : public URPublisherParameter
{
  GENERATED_BODY()

public:
  URActionStatusPublisherParameter()
  {
    MessageType = TEXT("actionlib_msgs/GoalStatusArray");
    FrameId = TEXT("odom");
  }

public:
  UPROPERTY(EditAnywhere)
  FString FrameId; 
};