#include "Controller/RDonbotGripperController.h"

void URDonbotGripperController::UpdateGripper()
{
    JointController->SetDesiredJointState(FingerRightName, 2*GripperPosition);
    JointController->SetDesiredJointState(FingerLeftName, -GripperPosition);
}

void URDonbotGripperController::Tick(float InDeltaTime)
{
    if (!FingerRight)
    {
        UE_LOG(LogTemp, Error, TEXT("FingerRight of %s not found"), *GetName());
        return;
    }
    if (!FingerLeft)
    {
        UE_LOG(LogTemp, Error, TEXT("FingerLeft of %s not found"), *GetName());
        return;
    }

    if (bAutoGrasp)
        Grasp();
    else
        Release();

    UpdateGripper();
}

URDonbotGripperController::URDonbotGripperController()
{
    FingerRightName = TEXT("gripper_joint");
    FingerLeftName = TEXT("gripper_base_gripper_left_joint");
}

void URDonbotGripperController::Grasp()
{
    if (GraspComponent && GraspComponent->TryToFixate())
    {
        bIsGrasping = true;
        GripperPosition = 0.01f; //TODO: Change hardcode
    }
}

void URDonbotGripperController::Release()
{
    if (GraspComponent && bIsGrasping)
    {
        UE_LOG(LogTemp, Warning, TEXT("Start releasing"))
        bIsGrasping = false;
        GripperPosition = 0.051f; //TODO: Change hardcode
        GraspComponent->TryToDetach();
    }
}

void URDonbotGripperController::Init(ARModel* InModel)
{
    if (!InModel)
    {
        UE_LOG(LogTemp, Error, TEXT("GripperController not attached to ARModel"))
    }
    else
    {
        Model = InModel;

        FingerRight = Model->Joints.FindRef(FingerRightName);
        FingerLeft = Model->Joints.FindRef(FingerLeftName);
        if (!FingerRight)
        {
            UE_LOG(LogTemp, Error, TEXT("FingerRight of %s not found"), *GetName())
            return;
        }
        if (!FingerLeft)
        {
            UE_LOG(LogTemp, Error, TEXT("FingerLeft of %s not found"), *GetName())
            return;
        }

        URControllerComponent* ControllerComp = Cast<URControllerComponent>(GetOuter());
        JointController = Cast<URJointController>(ControllerComp->ControllerList("JointController"));
        if (!JointController)
        {
            UE_LOG(LogTemp, Error, TEXT("JointController not found"))
            return;
        }
        JointController->DesiredJointState.FindOrAdd(FingerRightName);
        JointController->DesiredJointState.FindOrAdd(FingerLeftName);

        TArray<URGraspComponent*> TempGraspComponents;
        Model->GetComponents<URGraspComponent>(TempGraspComponents);
        for (auto& GraspComp : TempGraspComponents)
        {
            if (GraspComp->GetName().Equals(GraspComponentName))
            {
                GraspComponent = GraspComp;
                URLink* ReferenceLink = Model->Links[GraspCompSetting.GripperName];
                GraspComponent->AttachToComponent(ReferenceLink->GetCollision(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
                GraspComponent->AddRelativeLocation(GraspCompSetting.ToolCenterPoint);
                GraspComponent->Init(ReferenceLink->GetCollision());
            }
        }

        GripperPosition = 0.051f; //TODO: Change hardcode
    }
}