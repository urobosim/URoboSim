#include "Controller/ControllerType/SpecialController/RTFController.h"

void URTFController::Init()
{
  Super::Init();

  if (!GetOwner())
  {
    UE_LOG(LogTemp, Error, TEXT("RobotComandsComponent not attached to ARModel"));
  }
  else
  {
    Time = 0.0f;
    for (URLink *&Link : GetOwner()->GetLinks())
    {
      Link->GetCollision()->SetSimulatePhysics(false);
      // Link->GetCollision()->SetCollisionProfileName(false);
    }
  }
}

void URTFController::AddTF(FString InFrameName, FTFInfo InTFInfo)
{
  FTFInfo &Info = TFList.FindOrAdd(InFrameName);
  Info = InTFInfo;
}

TMap<FString, FTFInfo> URTFController::GetTFList()
{
  return TFList;
}

bool URTFController::UpdateFramePoses()
{
  if (Time > 1. / UpdateRate)
  {
    Time = 0;
    for (auto &TF : TFList)
    {
      FString ChildName = TF.Key;
      FString ParentName = TF.Value.ParentFrame;
      UStaticMeshComponent *Child = nullptr;
      UStaticMeshComponent *Parent = nullptr;
      for (auto &Link : GetOwner()->Links)
      {
        if (!Child)
        {
          Child = Link.Value->GetCollision(ChildName, false);
        }

        if (!Parent)
        {
          Parent = Link.Value->GetCollision(ParentName, false);
        }

        // if(Child)
        //   {
        //     UE_LOG(LogTemp, Error, TEXT("Model does not contain Frame %s"), *TF.Key);
        //     break;
        //   }
      }

      // if(!Child)
      //   {
      //     UE_LOG(LogTemp, Error, TEXT("Model does not contain Frame %s"), *TF.Key);
      //     continue;
      //   }

      // if(!Parent)
      //   {
      //     UE_LOG(LogTemp, Error, TEXT("Model does not contain ParentFrame %s"), *TF.Value.ParentFrame);
      //     continue;
      //   }

      SetLinkPose(Child, Parent, TF.Value.Pose);
    }
  }

  return false;
}

void URTFController::SetLinkPose(UStaticMeshComponent *InChildLink, UStaticMeshComponent *InParentLink, FTransform InPose)
{
  if (!InChildLink)
  {
    return;
  }
  if (!InParentLink)
  {
    return;
  }

  FTransform ParentTransform = InParentLink->GetComponentTransform();
  FVector NewLocation = ParentTransform.GetLocation() + ParentTransform.GetRotation().RotateVector(InPose.GetLocation());
  // FQuat NewRotation = ParentTransform.GetRotation() * InPose.GetRotation();
  FQuat NewRotation = ParentTransform.GetRotation() * InPose.GetRotation();

  FTransform NewTransform = FTransform(NewRotation, NewLocation, FVector(1.0f, 1.0f, 1.0f));
  UE_LOG(LogTemp, Error, TEXT("ChildName %s NewTransform %s"), *InChildLink->GetName(), *NewTransform.ToString());
  InChildLink->SetWorldTransform(NewTransform, false, nullptr, ETeleportType::TeleportPhysics);
}

void URTFController::Tick(const float &InDeltaTime)
{
  Time += InDeltaTime;
}
