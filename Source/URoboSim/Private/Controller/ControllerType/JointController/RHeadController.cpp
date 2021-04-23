#include "Controller/ControllerType/JointController/RHeadController.h"
#include "Physics/RLink.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

URHeadTrajectoryController::URHeadTrajectoryController()
{
}

void URHeadTrajectoryController::Init()
{
	bActive = false;
	if(!GetOwner())
	{
		UE_LOG(LogTemp, Error, TEXT("RobotComandsComponent not attached to ARModel"));
	}
	else
	{
        }


        ControllerComp = Cast<URControllerComponent>(GetOuter());
        JointController = Cast<URJointController>(ControllerComp->ControllerList("JointController"));

        if(!JointController)
          {
            UE_LOG(LogTemp, Error, TEXT("JointController not found"));
          }
        ActionDuration = 0;
}

void URHeadTrajectoryController::Tick(float InDeltaTime)
{
  // CancelAction();
  if(bActive)
    {
      GoalStatusList.Last().Status = 1;
      UpdateHeadDirection();
      CheckPointHeadState();
      if(bActive)
        {
          ActionDuration += InDeltaTime;
        }
    }
}

FVector URHeadTrajectoryController::CalculateNewViewDirection()
{
  FVector Direction;
  if(GetOwner())
    {
      FTransform ReferenceLinkTransform ;
      if(FrameId.Equals(TEXT("map")))
        {

        }
      else
        {
          URLink* ReferenceLink = GetOwner()->Links.FindRef(FrameId);
          if(!ReferenceLink)
            {
              UE_LOG(LogTemp, Error, TEXT("ReferenceLink %s not found"), *FrameId);
              return FVector();
            }
          ReferenceLinkTransform = ReferenceLink->GetCollision()->GetComponentTransform();
        }

      TArray<URStaticMeshComponent*> ActorComponents;
      GetOwner()->GetComponents(ActorComponents);
      URStaticMeshComponent* PointingLink = nullptr;
      for(auto & Component : ActorComponents)
        {
          if(Component->GetName().Contains(PointingFrame))
            {
              PointingLink = Component;
            }
        }
      if(!PointingLink)
        {
          UE_LOG(LogTemp, Error, TEXT("PointingLink %s not found"), *PointingFrame);
          return FVector();
        }

      // Add offset to get the right position in pr2 high_def_frame
      FVector PointInWorldCoord = ReferenceLinkTransform.GetRotation().RotateVector(Point) +  ReferenceLinkTransform.GetLocation();

      Direction = PointInWorldCoord - (PointingLink->GetComponentQuat().RotateVector(FVector(7.0, 11.0, 12.0)) + PointingLink->GetComponentTransform().GetLocation());

    }
  return Direction;
}

void URPR2HeadTrajectoryController::UpdateHeadDirection()
{
    if(PointingFrame.Equals("high_def_frame"))
    {
        PointingFrame = TEXT("head_tilt_link");
    }

    FVector NewDirection = CalculateNewViewDirection();
    MoveToNewPosition(NewDirection);
}

void URPR2HeadTrajectoryController::CheckPointHeadState()
{
    if(GetOwner())
    {
        URJoint* AzimuthJoint = GetOwner()->Joints.FindRef("head_pan_joint");
        URJoint* ElevationJoint = GetOwner()->Joints.FindRef("head_tilt_joint");

        float Az = AzimuthJoint->GetJointPosition();
        float El = ElevationJoint->GetJointPosition();


        float DesiredAz = JointController->DesiredJointState["head_pan_joint"];
        float DesiredEl = JointController->DesiredJointState["head_tilt_joint"];

        float DiffAz = DesiredAz - Az;
        float DiffEl = DesiredEl - El;

        if((FMath::Abs(DiffAz) < 0.02 && FMath::Abs(DiffEl) < 0.02)|| ActionDuration > 1.0f)
        {
            GoalStatusList.Last().Status = 3;
            bPublishResult = true;
            bActive = false;
        }
    }
}

void URPR2HeadTrajectoryController::MoveToNewPosition(FVector InNewDirection)
{
  if(GetOwner())
    {
      TArray<URStaticMeshComponent*> ActorComponents;
      GetOwner()->GetComponents(ActorComponents);
      URStaticMeshComponent* PointingLink = nullptr;
      for(auto & Component : ActorComponents)
        {
          if(Component->GetName().Contains(PointingFrame))
            {
              PointingLink = Component;
            }
        }
      if(!PointingLink)
        {
          UE_LOG(LogTemp, Error, TEXT("PointingLink %s not found"), *PointingFrame);
          return;
        }
      FQuat ReferenceQuat = PointingLink->GetComponentQuat();
      FVector2D AzEl = FMath::GetAzimuthAndElevation(InNewDirection.GetSafeNormal(), ReferenceQuat.GetAxisX(), ReferenceQuat.GetAxisY(), ReferenceQuat.GetAxisZ());

      URJoint* AzimuthJoint = GetOwner()->Joints.FindRef("head_pan_joint");
      URJoint* ElevationJoint = GetOwner()->Joints.FindRef("head_tilt_joint");

      float Az = AzimuthJoint->GetJointPosition();
      float El = ElevationJoint->GetJointPosition();

      float& DesAz = JointController->DesiredJointState.FindOrAdd("head_pan_joint");
      DesAz = AzimuthJoint->Constraint->ClampJointStateToConstraintLimit(Az - AzEl.X);
      // DesAz = Az - AzEl.X;
      float& DesEl = JointController->DesiredJointState.FindOrAdd("head_tilt_joint");
      DesEl = ElevationJoint->Constraint->ClampJointStateToConstraintLimit(El - AzEl.Y);
      // DesEl = El - AzEl.Y;
    }
}
