#pragma once

#include "Controller/ControllerType/SpecialController/RGripperController.h"
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
	URGripperController *GripperController;
};
