#include "Controller/RController.h"
#include "Physics/RLink.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"

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
      TArray<AActor*> PerceivedActors;
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

          URLink* ReferenceLink = Model->Links.FindRef(TEXT("base_footprint"));
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

void URCameraController::Init(ARModel* InModel)
{
  if(!InModel)
    {
      UE_LOG(LogTemp, Error, TEXT("RobotComandsComponent not attached to ARModel"));
      return;
    }
  Model = InModel;
  TArray<AActor*> FoundActors;
  TArray<URStaticMeshComponent*> ActorComponents;
  URStaticMeshComponent* ReferenceLink = nullptr;
  Model->GetComponents(ActorComponents);

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


void URTFController::Init(ARModel* InModel)
{
  if(!InModel)
    {
      UE_LOG(LogTemp, Error, TEXT("RobotComandsComponent not attached to ARModel"));
    }
  else
    {
      Model = InModel;
      Time = 0.0f;
      for(auto& Link : Model->Links)
        {
          Link.Value->GetCollision()->SetSimulatePhysics(false);
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
          if(Model->Links.Contains(ChildName) && Model->Links.Contains(ParentName))
            {
              SetLinkPose(Model->Links[ChildName], Model->Links[ParentName], TF.Value.Pose);
            }
          else
            {
              UE_LOG(LogTemp, Error, TEXT("Model does not contain Frame %s or ParentFrame %s"), *TF.Key, *TF.Value.ParentFrame);
            }
        }
    }

  return false;
}

void URTFController::SetLinkPose(URLink* InChildLink, URLink* InParentLink, FTransform InPose)
{
  FTransform ParentTransform = InParentLink->GetCollision()->GetComponentTransform();
  FVector NewLocation = ParentTransform.GetLocation() + ParentTransform.GetRotation().RotateVector(InPose.GetLocation());
  FQuat NewRotation = ParentTransform.GetRotation() * InPose.GetRotation();

  FTransform NewTransform = FTransform(NewRotation, NewLocation, FVector(1.0f, 1.0f, 1.0f));
  InChildLink->GetCollision()->SetWorldTransform(NewTransform, false, nullptr, ETeleportType::TeleportPhysics);
}

void URTFController::Tick(float InDeltaTime)
{
  Time += InDeltaTime;
}
