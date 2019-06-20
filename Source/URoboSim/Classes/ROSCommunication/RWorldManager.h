// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "RWCManager.h"
#include "RWorldManager.generated.h"


UCLASS()
class UROSCONTROL_API URoboSimWCM : public URWCManager
{
	GENERATED_BODY()

public:
	URoboSimWCM(){
          UE_LOG(LogTemp, Error, TEXT("ResetLevelserver created"));
        };
	~URoboSimWCM(){};
	virtual void SetupServiceServers() override;

private:
	FString Namespace;
};
