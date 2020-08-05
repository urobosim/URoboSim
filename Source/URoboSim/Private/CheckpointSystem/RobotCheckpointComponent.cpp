// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckpointSystem/RobotCheckpointComponent.h"
#include "CheckpointSystem/RobotCheckpointHandler.h"
#include "Physics/RModel.h"


URobotCheckpointComponent::URobotCheckpointComponent()
{
}

void URobotCheckpointComponent::BeginPlay()
{
  UE_LOG(LogTemp, Error, TEXT("%s BeginPlay"), *GetName());
  FString Name = GetName();
  CheckpointHandler = NewObject<URobotCheckpointHandler>(this, FName(*(Name + TEXT("_CheckpointHandler"))));

  ARModel* Robot = Cast<ARModel>(GetOwner());
  if(Robot)
    {
      CheckpointObject = Robot;
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("%s is not attached to a robot"), *GetName());
    }
  Super::BeginPlay();
}
