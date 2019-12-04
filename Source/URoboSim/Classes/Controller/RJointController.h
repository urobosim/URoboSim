#pragma once

#include "Controller/RController.h"
#include "RJointController.generated.h"


UENUM()
enum class UJointControllerState : uint8
{
  Normal,
  FollowJointTrajectory,
  Off
};

UENUM()
enum class UJointControllerMode : uint8
{
  Dynamic,
  Kinematic
};

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URJointController : public URController
{
  GENERATED_BODY()
    public:
    URJointController();

  virtual void Init(ARModel* InModel) override;
  virtual void Tick(float InDeltaTime);

  UPROPERTY(VisibleAnywhere)
    UJointControllerState State;

  UPROPERTY(EditAnywhere)
    FString BaseLink;

  UPROPERTY()
    TArray<FTrajectoryPoints> Trajectory;

  UPROPERTY(EditAnywhere)
    TMap<FString,float> DesiredJointState;

  UPROPERTY()
    TArray<bool> bTrajectoryPointsReached;

  // UPROPERTY()
  //   bool bFollowTrajectory = false;

  UPROPERTY(EditAnywhere)
    TArray<FString> IgnoreList;

  UPROPERTY(EditAnywhere)
    float MaxJointAngularVel;

  // UPROPERTY()
  //   TMap<FString, FJointInfo> JointStateList;

  UPROPERTY()
    FTrajectoryStatus TrajectoryStatus;

  virtual void FollowTrajectory();
  virtual void SetJointNames(TArray<FString> InNames);
  virtual void SwitchMode(UJointControllerMode InMode, bool IsInit = false);

  UJointControllerState GetState();
 protected:
  UPROPERTY(EditAnywhere)
    UJointControllerMode Mode;

  virtual void MoveJoints(float InDeltaTime);
  virtual void MoveJointsDynamic(float InDeltaTime);
  virtual void MoveJointsKinematic();

// #if WITH_EDITOR
//   void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
//   {
//     FName PropertyName = (PropertyChangedEvent.Property != NULL) ? PropertyChangedEvent.Property->GetFName() : NAME_None;

//     if (PropertyName == GET_MEMBER_NAME_CHECKED(URJointController, Mode))
//       {
//         if(Model)
//           {
//             for(auto& Link : Model->Links)
//               {
//                     bool bEnablePhysics;
//                     if(Mode == UJointControllerMode::Kinematic)
//                       {
//                         bEnablePhysics = false;
//                       }
//                     else
//                       {
//                         bEnablePhysics = true;
//                       }
//                     Link.Value->GetCollision()->SetSimulatePhysics(bEnablePhysics);
//               }
//           }
//       }
//     Super::PostEditChangeProperty(PropertyChangedEvent);
//   };
// #endif

  UPROPERTY()
    ARModel* Model;

  UPROPERTY()
    uint32 TrajectoryPointIndex;

  void UpdateDesiredJointAngle(float InDeltaTime);
  bool CheckTrajectoryStatus();
  void CallculateJointVelocities(float InDeltaTime);
};
