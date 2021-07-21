// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "Physics/RModel.h"
#include "Components/SceneCaptureComponent2D.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/TextureRenderTarget2D.h"
#include "GameFramework/Actor.h"
#include "CoreMinimal.h"
#include "RSensor.generated.h"

UCLASS(ClassGroup=(RobotSensor), meta=(BlueprintSpawnableComponent))
class UROBOSIM_API URSensor : public USceneComponent
{
    GENERATED_BODY()

public:

      virtual void Init(){};
    // virtual void Tick(float InDeltaTime){};


};
