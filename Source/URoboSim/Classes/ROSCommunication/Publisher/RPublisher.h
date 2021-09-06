#pragma once

#include "ROSCommunication/RROSCommunication.h"
#include "ROSBridgePublisher.h"
#include "RobotInterface.h"
// clang-format off
#include "RPublisher.generated.h"
// clang-format on

UCLASS(BlueprintType, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URPublisherParameter : public UObject
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere)
  FString Topic;

  UPROPERTY(VisibleAnywhere)
  FString MessageType;
};

UCLASS()
class UROBOSIM_API UPublisher : public UROSCommunication
{
  GENERATED_BODY()

};

UCLASS()
class UROBOSIM_API URPublisher : public UPublisher, public IRobotInterface
{
  GENERATED_BODY()

public:
  void Tick() override;

public:
  virtual void Publish() {}

  virtual void SetPublishParameters(URPublisherParameter *&PublisherParameters);

protected:
  virtual void Init() override;

  virtual void CreatePublisher();

public:
  UPROPERTY(EditAnywhere)
  FString Topic;

protected:
  UPROPERTY(VisibleAnywhere)
  FString MessageType;

  TSharedPtr<FROSBridgePublisher> Publisher;
};
