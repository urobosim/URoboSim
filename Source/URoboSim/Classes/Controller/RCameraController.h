#pragma once

#include "RController.h"
#include "ConstructorHelpers.h"
#include "Physics/RJoint.h"
#include "Physics/RModel.h"
#include "RGraspComponent.h"
#include "Containers/Queue.h"
#include "Conversions.h"
#include "RGBDCamera.h"
// clang-format off
#include "RCameraController.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URCameraController : public URController
{
  GENERATED_BODY()

public:
  URCameraController();

public:
  virtual void Init() override;

  virtual void PerceiveObject();

  virtual void Tick(const float &InDeltaTime) override;

  UPROPERTY(BlueprintReadWrite, Instanced, EditAnywhere, export, noclear)
  TArray<UPerceivedObject*> PerceivedObjects;

  UPROPERTY(EditAnywhere)
  TArray<AActor*> PerceivedActors;

  UPROPERTY()
  FString TypeToPerceive;

  UPROPERTY()
  ARGBDCamera* Camera;

  UPROPERTY()
  UPerceivedObject* PerceivedObject;

protected:
  UPROPERTY(EditAnywhere)
  FString CameraRef;

  UPROPERTY(EditAnywhere)
  FString CameraName;

  UPROPERTY(EditAnywhere)
  FTransform PoseOffset;
};