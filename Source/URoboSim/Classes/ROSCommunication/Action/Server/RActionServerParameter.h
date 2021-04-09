#pragma once

#include "ROSCommunication/Publisher/RPublisherParameter.h"
#include "ROSCommunication/Subscriber/RSubscriberParameter.h"
// clang-format off
#include "RActionServerParameter.generated.h"
// clang-format on

UCLASS(BlueprintType, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URActionServerParameter : public UObject
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere)
  FString ActionName;
};

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