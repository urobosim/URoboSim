#pragma once

#include "RActionServer.h"
// clang-format off
#include "RActionCancelSubscriber.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URActionCancelSubscriberParameter : public URSubscriberParameter
{
  GENERATED_BODY()

public:
  URActionCancelSubscriberParameter()
  {
    MessageType = TEXT("actionlib_msgs/GoalID");
  }
};

UCLASS()
class UROBOSIM_API URActionCancelSubscriber : public URActionSubscriber
{
	GENERATED_BODY()

public:
	URActionCancelSubscriber();
};