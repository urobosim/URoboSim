#include "Controller/ControllerType/SpecialController/RHeadController.h"
#include "Kismet/GameplayStatics.h"
#include "Physics/RLink.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogRHeadTrajectoryController, Log, All);

URHeadTrajectoryController::URHeadTrajectoryController()
{
}

void URHeadTrajectoryController::Init()
{
  Super::Init();

  bActive = false;
  if (GetOwner())
  {
    JointController = Cast<URJointController>(GetOwner()->GetController(TEXT("JointController")));
    if (!JointController)
    {
      UE_LOG(LogRHeadTrajectoryController, Error, TEXT("JointController not found"));
    }
    ActionDuration = 0;
  }
  else
  {
    UE_LOG(LogRHeadTrajectoryController, Error, TEXT("%s not attached to ARModel"), *GetName());
  }
}

void URHeadTrajectoryController::Tick(const float &InDeltaTime)
{
  // CancelAction();
  if (GetOwner())
  {
    if (bActive)
    {
      GoalStatusList.Last().Status = 1;
      UpdateHeadDirection();
      CheckPointHeadState();
      if (bActive)
      {
        ActionDuration += InDeltaTime;
      }
    }
  }
}

FVector URHeadTrajectoryController::CalculateNewViewDirection()
{
  FVector Direction;
  if (GetOwner())
  {
  UE_LOG(LogRHeadTrajectoryController, Error, TEXT("CalculateNewViewDirection"));
    FTransform ReferenceLinkTransform;
    if (!FrameId.Equals(TEXT("map")))
    {
      URLink *ReferenceLink = GetOwner()->Links.FindRef(FrameId);
      if (!ReferenceLink)
      {
        UE_LOG(LogRHeadTrajectoryController, Error, TEXT("ReferenceLink %s not found"), *FrameId);
        return FVector();
      }
      ReferenceLinkTransform = ReferenceLink->GetCollision()->GetComponentTransform();
    }

    TArray<UStaticMeshComponent *> ActorComponents;
    GetOwner()->GetComponents(ActorComponents);
    UStaticMeshComponent *PointingLink = nullptr;
    for (auto &Component : ActorComponents)
    {
      if (Component->GetName().Contains(PointingFrame))
      {
        PointingLink = Component;
      }
    }
    if (!PointingLink)
    {
      UE_LOG(LogRHeadTrajectoryController, Error, TEXT("PointingLink %s not found"), *PointingFrame);
      return FVector();
    }

    // Add offset to get the right position in pr2 high_def_frame
    FVector PointInWorldCoord = ReferenceLinkTransform.GetRotation().RotateVector(Point) + ReferenceLinkTransform.GetLocation();

    Direction = PointInWorldCoord - (PointingLink->GetComponentQuat().RotateVector(FVector(7.0, 11.0, 12.0)) + PointingLink->GetComponentTransform().GetLocation());
  }
  return Direction;
}

void URPR2HeadTrajectoryController::UpdateHeadDirection()
{
  UE_LOG(LogRHeadTrajectoryController, Error, TEXT("UpdateHeadDirection"));
  if (PointingFrame.Equals("high_def_frame"))
  {
    PointingFrame = TEXT("head_tilt_link");
  }

  FVector NewDirection = CalculateNewViewDirection();
  MoveToNewPosition(NewDirection);
}

void URPR2HeadTrajectoryController::CheckPointHeadState()
{
  if (GetOwner())
  {
    UE_LOG(LogTemp, Error, TEXT("CheckPointHeadState"));
    URJoint *AzimuthJoint = GetOwner()->Joints.FindRef("head_pan_joint");
    URJoint *ElevationJoint = GetOwner()->Joints.FindRef("head_tilt_joint");

    float Az = AzimuthJoint->GetJointPosition();
    float El = ElevationJoint->GetJointPosition();

    float DesiredAz = JointController->DesiredJointStates["head_pan_joint"].JointPosition;
    float DesiredEl = JointController->DesiredJointStates["head_tilt_joint"].JointPosition;

    float DiffAz = DesiredAz - Az;
    float DiffEl = DesiredEl - El;

    if ((FMath::Abs(DiffAz) < 0.02 && FMath::Abs(DiffEl) < 0.02) || ActionDuration > 1.0f)
    {
      GoalStatusList.Last().Status = 3;
      bPublishResult = true;
      bActive = false;
    }
  }
}

void URPR2HeadTrajectoryController::MoveToNewPosition(FVector InNewDirection)
{
  if (GetOwner())
  {
    UE_LOG(LogTemp, Error, TEXT("MoveToNewPosition"));
    TArray<UStaticMeshComponent *> ActorComponents;
    GetOwner()->GetComponents(ActorComponents);
    UStaticMeshComponent *PointingLink = nullptr;
    for (auto &Component : ActorComponents)
    {
      if (Component->GetName().Contains(PointingFrame))
      {
        PointingLink = Component;
      }
    }
    if (!PointingLink)
    {
      UE_LOG(LogTemp, Error, TEXT("PointingLink %s not found"), *PointingFrame);
      return;
    }
    FQuat ReferenceQuat = PointingLink->GetComponentQuat();
    FVector2D AzEl = FMath::GetAzimuthAndElevation(InNewDirection.GetSafeNormal(), ReferenceQuat.GetAxisX(), ReferenceQuat.GetAxisY(), ReferenceQuat.GetAxisZ());

    URJoint *AzimuthJoint = GetOwner()->Joints.FindRef("head_pan_joint");
    URJoint *ElevationJoint = GetOwner()->Joints.FindRef("head_tilt_joint");

    float Az = AzimuthJoint->GetJointPosition();
    float El = ElevationJoint->GetJointPosition();

    float &DesAz = JointController->DesiredJointStates.FindOrAdd("head_pan_joint").JointPosition;
    DesAz = AzimuthJoint->Constraint->ClampJointStateToConstraintLimit(Az - AzEl.X);
    // DesAz = Az - AzEl.X;
    float &DesEl = JointController->DesiredJointStates.FindOrAdd("head_tilt_joint").JointPosition;
    DesEl = ElevationJoint->Constraint->ClampJointStateToConstraintLimit(El - AzEl.Y);
    // DesEl = El - AzEl.Y;
  }
  else
    {

      UE_LOG(LogTemp, Error, TEXT("MoveToNewPosition failed because GetOwner"));
    }
}
