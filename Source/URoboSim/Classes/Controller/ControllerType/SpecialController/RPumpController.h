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
    FVector TraySlot1Frame;

  UPROPERTY(EditAnywhere)
    FVector TraySlot2Frame;
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

public:

    UPROPERTY(EditAnywhere)
    FString TrayReferenceLink;

    UPROPERTY(EditAnywhere)
    FString TrayReferenceFrame;

  UPROPERTY(EditAnywhere)
    FVector TraySlot1Frame;

  UPROPERTY(EditAnywhere)
    FVector TraySlot2Frame;

  UFUNCTION()
    void SetConstraintTray1(AActor* Object);

  UFUNCTION()
    void SetConstraintTray2(AActor* Object);

  UFUNCTION()
    void ReleaseConstraintTray1(AActor* Object);

  UFUNCTION()
    void ReleaseConstraintTray2(AActor* Object);
protected:

  UFUNCTION()
  virtual void OnTray1AreaBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	                                             class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                                             bool bFromSweep, const FHitResult& SweepResult);

  // Function called when an item leaves the fixation overlap area
  UFUNCTION()
    void OnTray1AreaEndOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	                                           class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

  UFUNCTION()
    void OnTray2AreaBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	                                             class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                                             bool bFromSweep, const FHitResult& SweepResult);

  // Function called when an item leaves the fixation overlap area
  UFUNCTION()
    void OnTray2AreaEndOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	                                           class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


  UPROPERTY(VisibleAnywhere)
    USphereComponent* Tray1Overlap;

  UPROPERTY(VisibleAnywhere)
    USphereComponent* Tray2Overlap;

  UPROPERTY(VisibleAnywhere)
    UPhysicsConstraintComponent* Tray1Constaint;

  UPROPERTY(VisibleAnywhere)
    UPhysicsConstraintComponent* Tray2Constaint;

  UPROPERTY(VisibleAnywhere)
    TArray<URGraspComponent*> GraspComps;

  UPROPERTY(VisibleAnywhere)
    UPrimitiveComponent* Ref;

  UPROPERTY(VisibleAnywhere)
    URPumpControllerHandler* PumpHandler = nullptr;
};
