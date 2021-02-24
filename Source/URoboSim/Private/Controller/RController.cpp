#include "Controller/RController.h"
#include "Physics/RLink.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"

void URController::SetOwner(ARModel* Model)
{
  Owner = Model;
}

const ARModel* URController::GetOwner()
{
  return Owner;
}

void URController::CancelAction()
{
  if(bCancel)
    {
      if(bActive)
        {
          GoalStatusList.Last().Status = 6;
          bActive = false;
        }
      else
        {
          GoalStatusList.Last().Status = 2;
          bActive = false;
        }
      bPublishResult = true;
    }
}

void URCameraController::PerceiveObject()
{
  if(bActive)
    {
      GoalStatusList.Last().Status = 1;
      bool bObjectFound = false;
      // PerceivedActors.Empty();
      UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(*TypeToPerceive), PerceivedActors);

      if(PerceivedActors.Num()>0)
        {
          bObjectFound = true;
          PerceivedObject = NewObject<UPerceivedObject>(this);
          PerceivedObject->PoseWorld.SetLocation(PerceivedActors[0]->GetActorLocation());
          PerceivedObject->PoseWorld.SetRotation(PerceivedActors[0]->GetActorQuat());
          PerceivedObject->Name = PerceivedActors[0]->GetName();
        }


      // for(auto & Object : PerceivedObjects)
      //   {
      //     if(Object->Type.Equals(TypeToPerceive, ESearchCase::IgnoreCase))
      //       {
      //         PerceivedObject = Object;
      //         bObjectFound = true;
      //       }
      //   }

      if(bObjectFound)
        {

          URLink* ReferenceLink = GetOwner()->Links.FindRef(TEXT("base_footprint"));
          FTransform ReferenceLinkTransform = ReferenceLink->GetCollision()->GetComponentTransform();
          FVector Location = ReferenceLinkTransform.GetLocation();
          Location.Z = 0.0f;
          ReferenceLinkTransform.SetLocation(Location);

          FVector Temp = PerceivedObject->PoseWorld.GetLocation() - ReferenceLinkTransform.GetLocation();
          FVector Pose = ReferenceLinkTransform.GetRotation().Inverse().RotateVector(Temp) ;

          PerceivedObject->Pose.SetLocation(Pose);
          FQuat TempRotator = PerceivedObject->PoseWorld.GetRotation() * ReferenceLinkTransform.GetRotation().Inverse() ;
          PerceivedObject->Pose.SetRotation(TempRotator);

          GoalStatusList.Last().Status = 3;
          bActive = false;
          bPublishResult = true;
        }
      else
        {
          UE_LOG(LogTemp, Error, TEXT("No Object of Type %s found"), *TypeToPerceive);
        }
    }

}

void URCameraController::Tick(float InDeltaTime)
{
}

void URCameraController::Init()
{
  if(!GetOwner())
    {
      UE_LOG(LogTemp, Error, TEXT("RobotComandsComponent not attached to ARModel"));
      return;
    }
  TArray<AActor*> FoundActors;
  TArray<URStaticMeshComponent*> ActorComponents;
  URStaticMeshComponent* ReferenceLink = nullptr;
  GetOwner()->GetComponents(ActorComponents);

  for(auto & Component : ActorComponents)
    {
      if(Component->GetName().Equals(CameraRef))
        {
          ReferenceLink = Component;
        }
    }
  if(!ReferenceLink)
    {
      UE_LOG(LogTemp, Error, TEXT("CameraRef not found"));
      return;
    }

  UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), FoundActors);

  for(auto& MyCamera : FoundActors)
    {
      if(MyCamera->GetName().Equals(CameraName))
        {
		  MyCamera->AttachToComponent(ReferenceLink, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
          MyCamera->AddActorLocalOffset(PoseOffset.GetLocation());
          MyCamera->AddActorLocalRotation(PoseOffset.GetRotation());
          return;
        }
    }
  UE_LOG(LogTemp, Error, TEXT("Camera %s not found"), *CameraName);

}


void URTFController::Init()
{
  if(!GetOwner())
    {
      UE_LOG(LogTemp, Error, TEXT("RobotComandsComponent not attached to ARModel"));
    }
  else
    {
      Time = 0.0f;
      for(auto& Link : GetOwner()->Links)
        {
          Link.Value->GetCollision()->SetSimulatePhysics(false);
          // Link.Value->GetCollision()->SetCollisionProfileName(false);
        }
    }
}

void URTFController::AddTF(FString InFrameName, FTFInfo InTFInfo)
{
  FTFInfo& Info = TFList.FindOrAdd(InFrameName);
  Info = InTFInfo;
}

TMap<FString, FTFInfo> URTFController::GetTFList()
{
  return TFList;
}

bool URTFController::UpdateFramePoses()
{
  if(Time > 1./UpdateRate)
    {
      Time = 0;
      for(auto & TF : TFList)
        {
          FString ChildName = TF.Key;
          FString ParentName = TF.Value.ParentFrame;
          URStaticMeshComponent* Child = nullptr;
          URStaticMeshComponent* Parent = nullptr;
          for(auto& Link : GetOwner()->Links)
            {
              if(!Child)
                {
                  Child = Link.Value->GetCollision(ChildName, false);
                }

              if(!Parent)
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

void URTFController::SetLinkPose(URStaticMeshComponent* InChildLink, URStaticMeshComponent* InParentLink, FTransform InPose)
{
  if (!InChildLink)
    { return; }
  if (!InParentLink)
    { return; }

  FTransform ParentTransform = InParentLink->GetComponentTransform();
  FVector NewLocation = ParentTransform.GetLocation() + ParentTransform.GetRotation().RotateVector(InPose.GetLocation());
  // FQuat NewRotation = ParentTransform.GetRotation() * InPose.GetRotation();
  FQuat NewRotation = ParentTransform.GetRotation() * InPose.GetRotation();

  FTransform NewTransform = FTransform(NewRotation, NewLocation, FVector(1.0f, 1.0f, 1.0f));
  UE_LOG(LogTemp, Error, TEXT("ChildName %s NewTransform %s"), *InChildLink->GetName(), *NewTransform.ToString());
  InChildLink->SetWorldTransform(NewTransform, false, nullptr, ETeleportType::TeleportPhysics);
}

void URTFController::Tick(float InDeltaTime)
{
  Time += InDeltaTime;
}
