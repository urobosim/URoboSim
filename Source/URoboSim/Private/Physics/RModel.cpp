// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)

#include "Physics/RModel.h"
#include "Physics/RJoint.h"
#include "Conversions.h"
#include "SDF/SDFDataAsset.h"
// #include "SDF/SDFJoint.h"
// #include "SDF/SDFLink.h"


// Sets default values
ARModel::ARModel()
{
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
  PrimaryActorTick.TickGroup = TG_PrePhysics;

  // UE_LOG(LogTemp, Error, TEXT("[%s] Ptr: %p"), TEXT(__FUNCTION__), this);

}

// Dtor
ARModel::~ARModel()
{
  // UE_LOG(LogTemp, Error, TEXT("[%s] Ptr: %p"), TEXT(__FUNCTION__), this);
}

// Called when the game starts or when spawned
void ARModel::BeginPlay()
{
  Super::BeginPlay();
  // UE_LOG(LogTemp, Error, TEXT("[%s] Ptr: %p"), TEXT(__FUNCTION__), this);

  // GetComponents<URGraspComponent>(Grippers);
}

// Called every frame
void ARModel::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
  // for(auto& Gripper : Grippers)
  // {
  // 	Gripper->TryToFixate();
  // }
}

// Load model
void ARModel::Load(USDFModel* InModel)
{
  LoadLinks(InModel->Links);
  // UE_LOG(LogTemp, Error, TEXT("2"));
  LoadJoints(InModel->Joints);
  // UE_LOG(LogTemp, Error, TEXT("3"));
  // CalculateCumulatedJointMass();
}


// Load links
void ARModel::LoadLinks(TArray<USDFLink*> InLinks)
{
  for(USDFLink* Link : InLinks)
    {
      URLink::Load(this, Link);
    }
}

// Load joints
void ARModel::LoadJoints(TArray<USDFJoint*> InJoints)
{
  for(USDFJoint* Joint : InJoints)
    {
      URJoint::Load(this, Joint);
    }
}

void ARModel::AddLink(URLink* Link)
{
  Links.Add(Link->GetName(), Link);
}

void ARModel::AddJoint(URJoint* Joint)
{
  Joints.Add(Joint->GetName(), Joint);
}

FJointState ARModel::GetJointState()
{
  FJointState JointState;

  for(auto& Joint : Joints)
    {
      JointState.JointNames.Add(Joint.Key);
      JointState.JointPositions.Add(Joint.Value->GetJointPosition());
      JointState.JointVelocities.Add(Joint.Value->GetJointVelocity());
    }

  return JointState;
}
