#pragma once

#include "Controller/ControllerType/SpecialController/PR2GripperController.h"
#include "ROSCommunication/Action/Server/RActionServer.h"
// clang-format off
#include "GCAFeedbackPublisher.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URGCAFeedbackPublisher : public URActionPublisher
{
	GENERATED_BODY()

public:
	URGCAFeedbackPublisher();

public:
  void Publish() override;

public:
	// TODO: Add ROS parameters

protected:
	void Init() override;

private:
	UPR2GripperController *GripperController;
};
