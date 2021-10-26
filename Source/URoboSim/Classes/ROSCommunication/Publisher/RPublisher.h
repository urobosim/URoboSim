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

public:
  UPROPERTY(EditAnywhere)
  FString Topic;

  void Tick() override;

  virtual void Publish() {}

  virtual void SetPublishParameters(URPublisherParameter *&PublisherParameters);

protected:

  UPROPERTY(VisibleAnywhere)
  FString MessageType;

  TSharedPtr<FROSBridgePublisher> Publisher;

  virtual void Init() override;

  virtual void CreatePublisher();

};

UCLASS()
class UROBOSIM_API URPublisher : public UPublisher, public IRobotInterface
{
  GENERATED_BODY()

protected:

  virtual void Init() override;

};
