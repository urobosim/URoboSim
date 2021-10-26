#include "Controller/ControllerType/SpecialController/RTFController.h"
#include "Physics/RModel.h"

void UTFController::SetWorld(UWorld* InWorld)
{
  World = InWorld;
  for(auto& Handler: TFHanderList)
    {
      Handler->SetWorld(World);
    }
}

void UTFController::Tick(const float &InDeltaTime)
{
  Time += InDeltaTime;
}

void UTFController::AddTF(FString InFrameName, FTFInfo InTFInfo)
{
  FTFInfo &Info = TFList.FindOrAdd(InFrameName);
  Info = InTFInfo;
}

void UTFController::Init()
{
  Super::Init();

  for(auto& Handler: TFHanderList)
    {
      Handler->FixedFrame = FixedFrame;
      Handler->Frames.Append(Frames);
    }

  TArray<AActor *> FoundActors;
  UGameplayStatics::GetAllActorsOfClass(World, AStaticMeshActor::StaticClass(), FoundActors);
  for (auto &ActorItr : FoundActors)
    {
      ActorItr->DisableComponentsSimulatePhysics();
      // UE_LOG(LogTemp, Error, TEXT("%s"), *ActorItr->GetName());
    }

  const TArray<ULevelStreaming*>& streamedLevels = GetWorld()->GetStreamingLevels();
  for (ULevelStreaming* streamingLevel : streamedLevels)
    {
      if(streamingLevel->IsLevelLoaded())
        {
          UE_LOG(LogTemp, Error, TEXT("%s loaded"), *streamingLevel->GetName());
        }
      else
        {
          UE_LOG(LogTemp, Error, TEXT("%s not loaded"), *streamingLevel->GetName());
        }
    }
  // for (TActorIterator<AStaticMeshActor> ActorItr = TActorIterator<AStaticMeshActor>(Level->GetWorld()); ActorItr; ++ActorItr)
  //   {
  //     ActorItr->DisableComponentsSimulatePhysics();
  //     UE_LOG(LogTemp, Error, TEXT("%s"), *ActorItr->GetName());
  //   }
}

void UTFHander::UpdateTF(const FString& TFFrame, const FTFInfo& InTFInfo)
{
  ChildFrame = TFFrame;
  ParentFrame = InTFInfo.ParentFrame;

  if(TFActors.Contains(ChildFrame))
    {
      SetPose(InTFInfo.Pose);
    }
  else
    {
      SearchFrames(InTFInfo.Pose);
    }
}

void UTFAActorHander::SetPose(const FTransform& InPose)
{
  USceneComponent* Child;
  USceneComponent* Parent;
  FTransform ParentTransform;
  bool bParentFound = false;

  Child = TFActors.FindOrAdd(ChildFrame).Child;
  Parent = TFActors.FindOrAdd(ChildFrame).Parent;
  ParentTransform = TFActors.FindOrAdd(ChildFrame).ParentTransform;
  bParentFound = TFActors.FindOrAdd(ChildFrame).bParentFound;

  UE_LOG(LogTemp, Error, TEXT("SetPoseActor: ChildFrame %s ParentFrame %s Pose %s"), *ChildFrame, *ParentFrame, *InPose.ToString() );
  if(Child)
    {
      if(bParentFound)
        {
          if(Parent)
            {
              ParentTransform = Parent->GetComponentTransform();
            }
          FHitResult Result;
          // Child->SetRelativeTransform(InPose, false, &Result, ETeleportType::ResetPhysics);
          Child->SetRelativeLocationAndRotation(InPose.GetLocation(), InPose.GetRotation(), false, &Result, ETeleportType::ResetPhysics);
        }
    }
}

void UTFAActorHander::SearchFrames(const FTransform& InPose)
{
  USceneComponent* Child = nullptr;
  USceneComponent* Parent = nullptr;
  FTransform ParentTransform;
  bool bParentFound = false;
  TArray<FString> ChildFrameParts;
  ChildFrame.ParseIntoArray(ChildFrameParts, TEXT("/"));

  TArray<FString> ParentFrameParts;
  ParentFrame.ParseIntoArray(ParentFrameParts, TEXT("/"));

  FTFActors TF = TFActors.FindOrAdd(ChildFrame);
  // Child = TFActors.FindOrAdd(ChildFrame).Child;
  // Parent = TFActors.FindOrAdd(ChildFrame).Parent;
  // ParentTransform = TFActors.FindOrAdd(ChildFrame).ParentTransform;
  // bParentFound = TFActors.FindOrAdd(ChildFrame).bParentFound;

  if(ParentFrame.Equals(FixedFrame))
    {
      ParentTransform = FTransform();
      bParentFound = true;
    }

  // for (TActorIterator<AStaticMeshActor> ActorItr = TActorIterator<AStaticMeshActor>(World); ActorItr; ++ActorItr)
  for (TActorIterator<AActor> ActorItr = TActorIterator<AActor>(World); ActorItr; ++ActorItr)
    {
      if(ActorItr->GetName().Equals(ChildFrame) || ActorItr->GetName().Equals(ChildFrameParts.Last()))
        {
          Child = ActorItr->GetRootComponent();
        }

      if(ActorItr->GetName().Equals(ParentFrame) || ActorItr->GetName().Equals(ParentFrameParts.Last()))
        {
          bParentFound = true;
          Parent = ActorItr->GetRootComponent();
          ParentTransform = ActorItr->GetActorTransform();
        }
      else if(Frames.Contains(ParentFrame) && !bParentFound)
        {
          bParentFound = true;
          ParentTransform = Frames[ParentFrame];
        }

      if(Child)
        {
          if(bParentFound)
            {

              TF.Child = Child;
              TF.Parent = Parent;
              TF.ParentTransform = ParentTransform;
              TF.bParentFound = bParentFound;

              FHitResult Result;
              UE_LOG(LogTemp, Error, TEXT("SearchActor: ChildFrame %s ParentFrame %s Pose"), *ChildFrame, *ParentFrame, *InPose.ToString());

              Child->SetRelativeLocationAndRotation(InPose.GetLocation(), InPose.GetRotation(), false, &Result, ETeleportType::ResetPhysics);
              break;
            }
        }
    }
}

void UTFRobotHander::SetPose(const FTransform& InPose)
{
  USceneComponent* Child = nullptr;
  USceneComponent* Parent = nullptr;
  FTransform ParentTransform;
  bool bParentFound = false;

  FTFActors TF;
  if(TFActors.Contains(ChildFrame))
    {
      TF = TFActors.FindOrAdd(ChildFrame);
      Child = TF.Child;
      Parent = TF.Parent;
      ParentTransform = TF.ParentTransform;
      bParentFound = TF.bParentFound;
    }

  if(Child)
    {
      if(bParentFound)
        {
          if(Parent)
            {
              ParentTransform = Parent->GetComponentTransform();
            }


          FTransform NewChildTransform = InPose;
          NewChildTransform.Accumulate(OffsetList[ChildFrame]);
          // FTransform RootTransform = ChildLink->GetCollision()->GetComponentTransform();
          // FTransform Offset = ChildTransform.GetRelativeTransform(RootTransform);
          // ChildTransform.Accumulate(Offset);

          UE_LOG(LogTemp, Error, TEXT("SetPoseRobot: ChildFrame %s ParentFrame %s Pose %s Offset %s"), *ChildFrame, *ParentFrame, *NewChildTransform.ToString(), *OffsetList[ChildFrame].ToString());
          FHitResult Result;
          Child->SetRelativeTransform(NewChildTransform, false, &Result, ETeleportType::ResetPhysics);
          // Child->SetWorldTransform(NewChildTransform, false, &Result, ETeleportType::ResetPhysics);
        }
      else
        {
          UE_LOG(LogTemp, Error, TEXT("Parent not found for %s"), *ChildFrame);
        }
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("Child not found for %s"), *ChildFrame);
    }
}

void UTFRobotHander::SearchFrames(const FTransform& InPose)
{
  USceneComponent* Child = nullptr;
  USceneComponent* Parent = nullptr;
  FTransform ParentTransform;
  bool bParentFound = false;

  // UE_LOG(LogTemp, Error, TEXT("ChildFrame %s ParentFrame %s"), *ChildFrame, *ParentFrame);
  TArray<FString> ChildFrameParts;
  ChildFrame.ParseIntoArray(ChildFrameParts, TEXT("/"));

  TArray<FString> ParentFrameParts;
  ParentFrame.ParseIntoArray(ParentFrameParts, TEXT("/"));

  FTransform ChildTransform;
  URLink* ChildLink;

  FTFActors TF = TFActors.FindOrAdd(ChildFrame);

  for (TActorIterator<ARModel> ActorItr = TActorIterator<ARModel>(World); ActorItr; ++ActorItr)
    {
      if(ActorItr->Links.Contains(ChildFrame))
        {
          ChildLink = ActorItr->Links[ChildFrame];
          Child = ChildLink->GetCollision();
        }
      else if (ActorItr->Links.Contains(ChildFrameParts.Last()))
        {
          ChildLink = ActorItr->Links[ChildFrameParts.Last()];
          Child = ActorItr->Links[ChildFrameParts.Last()]->GetCollision();
        }

      if(Child)
        {
          ChildTransform = ChildLink->PoseComponent->GetComponentTransform();
          FTransform RootTransform = Child->GetComponentTransform();
          if(!OffsetList.Contains(ChildFrame))
            {
              // OffsetList.Add(ChildFrame, ChildTransform.GetRelativeTransformReverse(RootTransform));
              OffsetList.Add(ChildFrame, ChildTransform.GetRelativeTransform(RootTransform));
            }
        }

      if(ActorItr->Links.Contains(ParentFrame))
        {
          Parent = ActorItr->Links[ParentFrame]->PoseComponent;
        }
      else if(ActorItr->Links.Contains(ParentFrameParts.Last()))
        {
          Parent = ActorItr->Links[ParentFrameParts.Last()]->PoseComponent;
        }

      if(Parent)
        {
          bParentFound = true;
          ParentTransform = ActorItr->GetActorTransform();
        }
      else if(ParentFrame.Equals(FixedFrame))
        {
          ParentTransform = FTransform();
          bParentFound = true;
        }
      else if(Frames.Contains(ParentFrame))
        {
          bParentFound = true;
          ParentTransform = Frames[ParentFrame];
        }
      else if(VirtualFrames.Contains(ParentFrame))
        {
          bParentFound = true;
          ParentTransform = VirtualFrames[ParentFrame].Pose;
        }

      TF.Child = Child;
      TF.Parent = Parent;
      TF.ParentTransform = ParentTransform;
      TF.bParentFound = bParentFound;

      if(Child)
        {
          if(bParentFound)
            {
              FHitResult Result;
              FTransform NewChildTransform = InPose;
              NewChildTransform.Accumulate(OffsetList[ChildFrame]);
              Child->SetRelativeTransform(NewChildTransform, false, &Result, ETeleportType::ResetPhysics);
              UE_LOG(LogTemp, Error, TEXT("SearchRobot: ChildFrame %s ParentFrame %s Pose %s Offset %s"), *ChildFrame, *ParentFrame, *NewChildTransform.ToString(), *OffsetList[ChildFrame].ToString());
              break;
            }
        }
      else
        {
          // if(bParentFound)
          //   {
          //     if(!VirtualFrames.Contains(ChildFrame) && !Frames.Contains(ChildFrame))
          //       {
          //         FTFInfo TFInfo;
          //         TFInfo.ParentFrame = ParentFrame;
          //         TFInfo.Pose = InPose;
          //         VirtualFrames.Add(ChildFrame, TFInfo);
          //       }
          //   }
        }
    }

}

bool UTFController::UpdateFramePoses()
{

  for(auto& TF : TFList)
    {
      for(auto& Handler: TFHanderList)
        {
          Handler->UpdateTF(TF.Key, TF.Value);
        }

      // FString ChildFrame = TF.Key;
      // FString ParentFrame = TF.Value.ParentFrame;
      // USceneComponent* Child;
      // USceneComponent* Parent;
      // FTransform ParentTransform;
      // bool bParentFound = false;

      // if(TFActors.Contains(ChildFrame))
      //   {
      //     Child = TFActors.FindOrAdd(ChildFrame).Child;
      //     Parent = TFActors.FindOrAdd(ChildFrame).Parent;
      //     ParentTransform = TFActors.FindOrAdd(ChildFrame).ParentTransform;
      //     bParentFound = TFActors.FindOrAdd(ChildFrame).bParentFound;

      //     if(Child)
      //       {
      //         if(bParentFound)
      //           {
      //             if(Parent)
      //               {
      //                 ParentTransform = Parent->GetComponentTransform();
      //               }
      //             // FTransform NewTransform = ParentTransform;
      //             // NewTransform.Accumulate(TF.Value.Pose);
      //             FHitResult Result;
      //             Child->SetRelativeTransform(TF.Value.Pose, false, &Result, ETeleportType::ResetPhysics);
      //             break;
      //           }
      //       }
      //   }
      // else
      //   {
      //     Child = TFActors.FindOrAdd(ChildFrame).Child;
      //     Parent = TFActors.FindOrAdd(ChildFrame).Parent;
      //     ParentTransform = TFActors.FindOrAdd(ChildFrame).ParentTransform;
      //     bParentFound = TFActors.FindOrAdd(ChildFrame).bParentFound;

      //     TArray<FString> ChildFrameParts;
      //     ChildFrame.ParseIntoArray(ChildFrameParts, TEXT("/"));

      //     TArray<FString> ParentFrameParts;
      //     ParentFrame.ParseIntoArray(ParentFrameParts, TEXT("/"));

      //     if(ParentFrame.Equals(FixedFrame))
      //       {
      //         ParentTransform = FTransform();
      //         bParentFound = true;
      //       }

      //     for (TActorIterator<AActor> ActorItr = TActorIterator<AActor>(World); ActorItr; ++ActorItr)
      //       {
      //         if(ActorItr->GetName().Equals(ChildFrame) || ActorItr->GetName().Equals(ChildFrameParts.Last()))
      //           {
      //             Child = ActorItr->GetRootComponent();
      //           }

      //         if(ActorItr->GetName().Equals(ParentFrame) || ActorItr->GetName().Equals(ParentFrameParts.Last()))
      //           {
      //             bParentFound = true;
      //             Parent = ActorItr->GetRootComponent();
      //             ParentTransform = ActorItr->GetActorTransform();
      //           }

      //         if(Child)
      //           {
      //             if(bParentFound)
      //               {

      //                 // FTransform NewTransform = ParentTransform;
      //                 // NewTransform.Accumulate(TF.Value.Pose);
      //                 FHitResult Result;
      //                 Child->SetRelativeTransform(TF.Value.Pose, false, &Result, ETeleportType::ResetPhysics);
      //                 break;
      //               }
      //           }
      //       }

        //   FTransform ChildTransform;
        //   URLink* ChildLink;
        //   for (TActorIterator<ARModel> ActorItr = TActorIterator<ARModel>(World); ActorItr; ++ActorItr)
        //     {
        //       if(ActorItr->Links.Contains(ChildFrame) || ActorItr->Links.Contains(ChildFrameParts.Last()))
        //         {
        //           ChildLink = ActorItr->Links[ChildFrame];
        //           Child = ChildLink->PoseComponent;
        //           if(!Child)
        //             {
        //               ChildLink = ActorItr->Links[ChildFrameParts.Last()];
        //               Child = ActorItr->Links[ChildFrameParts.Last()]->PoseComponent;
        //             }

        //           ChildTransform = Child->GetComponentTransform();
        //           FTransform RootTransform = ChildLink->GetCollision()->GetComponentTransform();
        //           FTransform Offset = ChildTransform.GetRelativeTransform(RootTransform);
        //           ChildTransform.Accumulate(Offset);
        //         }

        //       if(ActorItr->Links.Contains(ParentFrame) || ActorItr->Links.Contains(ParentFrameParts.Last()))
        //         {
        //           Parent = ActorItr->Links[ParentFrame]->PoseComponent;
        //           if(!Parent)
        //             {
        //               Parent = ActorItr->Links[ParentFrameParts.Last()]->PoseComponent;
        //             }
        //           if(Parent)
        //             {
        //               bParentFound = true;
        //               ParentTransform = ActorItr->GetActorTransform();
        //             }

        //           if(Child)
        //             {
        //               if(bParentFound)
        //                 {

        //                   // FTransform NewTransform = ParentTransform;
        //                   // NewTransform.Accumulate(TF.Value.Pose);
        //                   FHitResult Result;
        //                   Child->SetWorldTransform(TF.Value.Pose, false, &Result, ETeleportType::ResetPhysics);
        //                   break;
        //                 }
        //             }
        //         }
        //     }
    }
  return true;
}

TMap<FString, FTFInfo> UTFController::GetTFList()
{
  return TFList;
}

// void URTFController::Init()
// {
//   Super::Init();

//   if (!GetOwner())
//   {
//     UE_LOG(LogTemp, Error, TEXT("RobotComandsComponent not attached to ARModel"));
//   }
//   else
//   {
//     Time = 0.0f;
//     for (URLink *&Link : GetOwner()->GetLinks())
//     {
//       Link->GetCollision()->SetSimulatePhysics(false);
//       // Link->GetCollision()->SetCollisionProfileName(false);
//     }
//   }
// }

// bool URTFController::UpdateFramePoses()
// {
//   if (Time > 1. / UpdateRate)
//   {
//     Time = 0;
//     for (auto &TF : TFList)
//     {
//       FString ChildName = TF.Key;
//       FString ParentName = TF.Value.ParentFrame;
//       UStaticMeshComponent *Child = nullptr;
//       UStaticMeshComponent *Parent = nullptr;
//       for (auto &Link : GetOwner()->Links)
//       {
//         if (!Child)
//         {
//           Child = Link.Value->GetCollision(ChildName, false);
//         }

//         if (!Parent)
//         {
//           Parent = Link.Value->GetCollision(ParentName, false);
//         }

//         // if(Child)
//         //   {
//         //     UE_LOG(LogTemp, Error, TEXT("Model does not contain Frame %s"), *TF.Key);
//         //     break;
//         //   }
//       }

//       // if(!Child)
//       //   {
//       //     UE_LOG(LogTemp, Error, TEXT("Model does not contain Frame %s"), *TF.Key);
//       //     continue;
//       //   }

//       // if(!Parent)
//       //   {
//       //     UE_LOG(LogTemp, Error, TEXT("Model does not contain ParentFrame %s"), *TF.Value.ParentFrame);
//       //     continue;
//       //   }

//       SetLinkPose(Child, Parent, TF.Value.Pose);
//     }
//   }

//   return false;
// }

// void URTFController::SetLinkPose(UStaticMeshComponent *InChildLink, UStaticMeshComponent *InParentLink, FTransform InPose)
// {
//   if (!InChildLink)
//   {
//     return;
//   }
//   if (!InParentLink)
//   {
//     return;
//   }

//   FTransform ParentTransform = InParentLink->GetComponentTransform();
//   FVector NewLocation = ParentTransform.GetLocation() + ParentTransform.GetRotation().RotateVector(InPose.GetLocation());
//   // FQuat NewRotation = ParentTransform.GetRotation() * InPose.GetRotation();
//   FQuat NewRotation = ParentTransform.GetRotation() * InPose.GetRotation();

//   FTransform NewTransform = FTransform(NewRotation, NewLocation, FVector(1.0f, 1.0f, 1.0f));
//   UE_LOG(LogTemp, Error, TEXT("ChildName %s NewTransform %s"), *InChildLink->GetName(), *NewTransform.ToString());
//   InChildLink->SetWorldTransform(NewTransform, false, nullptr, ETeleportType::TeleportPhysics);
// }
