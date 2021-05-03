
#pragma once

#include "Controller/ControllerType/BaseController/RBaseController.h"
#include "Conversions.h"
#include "ROdomPublisher.h"
// clang-format off
#include "RDonbotOdomPublisher.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URDonbotOdomPublisherParameter final : public UROdomPublisherParameter
{
  GENERATED_BODY()

public:
  URDonbotOdomPublisherParameter()
  {
    Topic = TEXT("/joint_states");
		MessageType = TEXT("sensor_msgs/JointState");
		OdomFrameXId = TEXT("odom_x_joint");
		OdomFrameYId = TEXT("odom_y_joint");
		OdomFrameZId = TEXT("odom_z_joint");
  }

public:
  UPROPERTY(EditAnywhere)
	FString OdomFrameXId;

	UPROPERTY(EditAnywhere)
	FString OdomFrameYId;

	UPROPERTY(EditAnywhere)
	FString OdomFrameZId;
};

UCLASS()
class UROBOSIM_API URDonbotOdomPublisher final : public UROdomPublisher
{
	GENERATED_BODY()

public:
	URDonbotOdomPublisher();

public:
	void Publish() override;

  void SetPublishParameters(URPublisherParameter *&PublisherParameters) override;

protected:
  void Init() override;

public:
	UPROPERTY(EditAnywhere)
	FString OdomFrameXId;

	UPROPERTY(EditAnywhere)
	FString OdomFrameYId;

	UPROPERTY(EditAnywhere)
	FString OdomFrameZId;

private:
	URBaseController *BaseController;
};
