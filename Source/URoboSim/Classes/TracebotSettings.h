// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "TracebotSettings.generated.h"

/**
 *
 */
UCLASS(Config=Tracebot, defaultconfig, meta = (DisplayName="Tracebot"))
class UROBOSIM_API UTracebotSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

        UPROPERTY(EditAnywhere, Config)
        FString JointStateTopic;

        UPROPERTY(EditAnywhere, Config)
        bool bDynamicGripper = true;

        UPROPERTY(EditAnywhere, Config)
        FString TrayReferenceLink = "tracebot_base_link";

        UPROPERTY(EditAnywhere, Config)
        FString TrayReferenceFrame = "tracebot_pump_two_bottle_holder_body_link_collision";

        UPROPERTY(EditAnywhere, Config)
          FVector TraySlot1Frame = FVector(-3.0, -1.65, 1.7);

        UPROPERTY(EditAnywhere, Config)
        FVector TraySlot2Frame = FVector(1.15, 1.9, 1.7);
};
