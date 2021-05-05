#pragma once

#include "Controller/ControllerType/SpecialController/RCameraController.h"
#include "ROSCommunication/Action/Server/RActionServer.h"
// clang-format off
#include "POAFeedbackPublisher.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URPOAFeedbackPublisher : public URActionPublisher
{
	GENERATED_BODY()

public:
	URPOAFeedbackPublisher();
	
public:
	void Publish() override;

protected:
	void Init() override;

private:
	URCameraController *CameraController;
};
