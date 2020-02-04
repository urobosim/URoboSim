#include "Controller/RController.h"
#include "Physics/RLink.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

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

URBaseController::URBaseController()
{
	BaseName = TEXT("base_footprint");
}

URControllerComponent::URControllerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;
	Model = Cast<ARModel>(GetOwner());
	if(Model)
	{
          Model->Plugins.Add(TEXT("ControllerComponent"), this);
	}
}

URControllerComponent::~URControllerComponent()
{
}

void URBaseController::MoveLinear(FVector InVelocity)
{
  LinearVelocity = InVelocity;
}

void URBaseController::Turn(float InVelocity)
{
  AngularVelocity = -InVelocity;
}

void URBaseController::Tick(float InDeltaTime)
{
        TurnTick(InDeltaTime);
	MoveLinearTick(InDeltaTime);
}

void URBaseController::TurnTick(float InDeltaTime)
{
  if(AngularVelocity != 0.f)
    {
      FRotator TestRotation = FRotator(0.0f, AngularVelocity *InDeltaTime, 0.0f);
      URLink* Base = Model->Links[BaseName];
      FRotator BaseOrientation = Base->GetCollision()->GetComponentRotation();
      FVector BasePosition = Base->GetCollision()->GetComponentLocation();


      for(auto& Link : Model->Links)
        {
          FRotator Orientation = Link.Value->GetCollision()->GetComponentRotation();
          FVector Position = Link.Value->GetCollision()->GetComponentLocation();

          FQuat NewRot = TestRotation.Quaternion() * Orientation.Quaternion() ;

          FVector LinkBaseOffset = Position - BasePosition;
          FVector NewPosition = TestRotation.RotateVector(LinkBaseOffset) + BasePosition;

          Link.Value->GetCollision()->SetWorldLocationAndRotation(NewPosition, NewRot, false, nullptr, ETeleportType::None);
        }
    }
}

void URBaseController::MoveLinearTick(float InDeltaTime)
{
  if(LinearVelocity.Size() != 0.f)
    {
      URLink* Base = Model->Links[BaseName];
      FRotator BaseOrientation = Base->GetCollision()->GetComponentRotation();
      FVector DistanceTraveld = BaseOrientation.Quaternion().RotateVector(LinearVelocity*InDeltaTime);

      for(auto& Link : Model->Links)
        {
          FVector Position = Link.Value->GetCollision()->GetComponentLocation();
          Link.Value->GetCollision()->SetWorldLocation(DistanceTraveld + Position, false, nullptr, ETeleportType::TeleportPhysics);
        }
    }
}

void URBaseController::MoveLinear(FVector InVelocity, float InDeltaTime)
{
  if(InVelocity.Size() != 0.f)
    {
      URLink* Base = Model->Links[BaseName];
      FRotator BaseOrientation = Base->GetCollision()->GetComponentRotation();
      FVector DistanceTraveld = BaseOrientation.Quaternion().RotateVector(InVelocity*InDeltaTime);

      for(auto& Link : Model->Links)
        {
          // FVector Position = Link.Value->GetCollision()->GetComponentLocation();
          // Link.Value->GetCollision()->SetWorldLocation(DistanceTraveld + Position, false, nullptr, ETeleportType::TeleportPhysics);
          AddRelativeLocation(Link.Value, DistanceTraveld);
        }
    }
}

void URBaseController::MoveLinearToWorld(FVector InVelocity, float InDeltaTime)
{
  FVector DistanceTraveld = InVelocity*InDeltaTime;

  for(auto& Link : Model->Links)
    {
      // FVector Position = Link.Value->GetCollision()->GetComponentLocation();
      // Link.Value->GetCollision()->SetWorldLocation(DistanceTraveld + Position, false, nullptr, ETeleportType::TeleportPhysics);
      AddRelativeLocation(Link.Value, DistanceTraveld);
    }
}



void URBaseController::Turn(float InVelocity, float InDeltaTime)
{
  if(InVelocity != 0.f)
    {
      FRotator TestRotation = FRotator(0.0f, InVelocity *InDeltaTime, 0.0f);
      for(auto& Link : Model->Links)
        {
          AddRelativeRotation(Link.Value, TestRotation);
        }
    }
}

void URBaseController::AddRelativeLocation(URLink* InLink, FVector InPosition)
{
  FVector Position = InLink->GetCollision()->GetComponentLocation();
  InLink->GetCollision()->SetWorldLocation(InPosition + Position, false, nullptr, ETeleportType::TeleportPhysics);
}

void URBaseController::AddRelativeRotation(URLink* InLink, FRotator InRotator)
{
  URLink* Base = Model->Links[BaseName];
  FRotator BaseOrientation = Base->GetCollision()->GetComponentRotation();
  FVector BasePosition = Base->GetCollision()->GetComponentLocation();
  FRotator Orientation = InLink->GetCollision()->GetComponentRotation();
  FVector Position = InLink->GetCollision()->GetComponentLocation();

  FQuat NewRot = InRotator.Quaternion() * Orientation.Quaternion() ;
  InLink->GetCollision()->SetWorldRotation(NewRot, false, nullptr, ETeleportType::TeleportPhysics);

  FVector LinkBaseOffset = Position - BasePosition;
  FVector NewPosition = InRotator.RotateVector(LinkBaseOffset) + BasePosition;

  InLink->GetCollision()->SetWorldLocation(NewPosition, false, nullptr, ETeleportType::TeleportPhysics);
}

void URBaseController::SetLocation(FVector InPosition)
{
  URLink* Base = Model->Links[BaseName];
  FVector BasePosition = Base->GetCollision()->GetComponentLocation();
  FVector DistanceTraveld = InPosition - BasePosition;

  for(auto& Link : Model->Links)
    {
      AddRelativeLocation(Link.Value, DistanceTraveld);
    }
}
void URBaseController::SetRotation(FRotator InRotation)
{
  URLink* Base = Model->Links[BaseName];
  FRotator BaseOrientation = Base->GetCollision()->GetComponentRotation();
  FRotator NewRotation = InRotation - BaseOrientation;

  for(auto& Link : Model->Links)
    {
      AddRelativeRotation(Link.Value, NewRotation);
    }
}

void URBaseController::SetLocationAndRotation(FVector InPosition, FRotator InRotation)
{
  SetLocation(InPosition);
  SetRotation(InRotation);
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
  URStaticMeshComponent* ReferenceLink;
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

  for(auto& Camera : FoundActors)
    {
      if(Camera->GetName().Equals(CameraName))
        {
          Camera->AttachToComponent(ReferenceLink, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
          Camera->AddActorLocalOffset(PoseOffset.GetLocation());
        }
      else
        {
          UE_LOG(LogTemp, Error, TEXT("Camera not found"));
        }
    }

}

void URBaseController::Init(ARModel* InModel)
{
  if(!InModel)
    {
      UE_LOG(LogTemp, Error, TEXT("RobotComandsComponent not attached to ARModel"));
    }
  else
    {
      Model = InModel;
      URLink* Base = Model->Links[BaseName];
      Base->GetCollision()->SetSimulatePhysics(false);
    }
}

void URControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
  float realtimeSeconds = FPlatformTime::Seconds();
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
  for(auto& C : Controller.ControllerList)
    {
      C.Value->Tick(DeltaTime);
    }
}

void URControllerComponent::BeginPlay()
{
  Super::BeginPlay();

  if(!Model)
    {
      UE_LOG(LogTemp, Error, TEXT("Owner is no RModel."));
    }
  else
    {
      for(auto& C : Controller.ControllerList)
        {
          C.Value->Init(Model);
        }
    }
}

void URControllerComponent::SetJointVelocities(TArray<FString> InJointNames, TArray<float> InJointVelocities)
{
  for(int i = 0; i < InJointNames.Num();i++)
    {
      Model->Joints[InJointNames[i]]->SetJointVelocity(InJointVelocities[i]);
    }
}

URController* URControllerComponent::ControllerList(FString ControllerName)
{
  if(Controller.ControllerList.Contains(ControllerName))
    {
      return Controller.ControllerList[ControllerName];
    }
  return nullptr;
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
