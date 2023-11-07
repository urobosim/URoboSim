#pragma once

#include "Physics/RModel.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
// clang-format off
#include "RPumpController.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URPumpControllerHandler : public UObject
{
  GENERATED_BODY()

};

UCLASS()
class UROBOSIM_API URPumpControllerParameter : public URControllerParameter
{
  GENERATED_BODY()
public:

    UPROPERTY(EditAnywhere)
    FString TrayReferenceLink = TEXT("tracebot_base_link");

    UPROPERTY(EditAnywhere)
      FString TrayReferenceFrame = TEXT("tracebot_base_link_fixed_joint_lump__tracebot_pump_collision_7");

    UPROPERTY(EditAnywhere)
      FString HolderReferenceFrame = TEXT("tracebot_base_link_fixed_joint_lump__tracebot_pump_collision_7");

  UPROPERTY(EditAnywhere)
    FVector TraySlot1Frame;

  UPROPERTY(EditAnywhere)
    FVector TraySlot2Frame;

  UPROPERTY(EditAnywhere)
    FVector HolderFrame;
};

UCLASS()
class UROBOSIM_API URPumpController: public URController
{
  GENERATED_BODY()

public:
  URPumpController();

public:
  virtual void Init() override;

  virtual void SetControllerParameters(URControllerParameter *&ControllerParameters) override;

  virtual UPrimitiveComponent* ParseChildFramesForRef(TArray<USceneComponent*> InChildFrames, FString RefName);
public:

    UPROPERTY(EditAnywhere)
    FString TrayReferenceLink;

    UPROPERTY(EditAnywhere)
    FString TrayReferenceFrame;

  UPROPERTY(EditAnywhere)
    FVector TraySlot1Frame;

  UPROPERTY(EditAnywhere)
    FVector TraySlot2Frame;

  UPROPERTY(EditAnywhere)
    FString HolderReferenceFrame;

  UPROPERTY(EditAnywhere)
    FVector HolderFrame;

  UFUNCTION()
    void ReleaseObject(AActor* Object);

  UFUNCTION()
    void SetObjectTray1(AActor* Object);

  UFUNCTION()
    void SetObjectTray2(AActor* Object);

  UFUNCTION()
    void SetObjectHolder(AActor* Object);
protected:

  UFUNCTION()
  virtual void OnTrayAreaBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	                                             class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                                             bool bFromSweep, const FHitResult& SweepResult);

  // Function called when an item leaves the fixation overlap area
  UFUNCTION()
    void OnTrayAreaEndOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	                                           class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

  virtual USphereComponent* SetupOverlap(const FName& InName, const FVector& InOffset, UPrimitiveComponent* InReference, float InRadius = 2.2);

  UPROPERTY(VisibleAnywhere)
    USphereComponent* Tray1Overlap;

  UPROPERTY(VisibleAnywhere)
    USphereComponent* Tray2Overlap;

  UPROPERTY(VisibleAnywhere)
    USphereComponent* HolderOverlap;

  UPROPERTY(VisibleAnywhere)
    TArray<URGraspComponent*> GraspComps;

  //Reference of the tray
  UPROPERTY(VisibleAnywhere)
    UPrimitiveComponent* Ref = nullptr;

  UPROPERTY(VisibleAnywhere)
    UPrimitiveComponent* HolderRef = nullptr;

  UPROPERTY(VisibleAnywhere)
    URPumpControllerHandler* PumpHandler = nullptr;
};
