// Fill out your copyright notice in the Description page of Project Settings.

#include "CheckpointSystem/RobotCheckpointHandler.h"
#include "Controller/ControllerType/BaseController/RBaseController.h"
#include "Controller/ControllerType/JointController/RJointController.h"
#include "Controller/RController.h"
#include "Controller/RControllerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Physics/RModel.h"

USaveGame *URobotCheckpointHandler::CreateSaveGameFile()
{
  return UGameplayStatics::CreateSaveGameObject(URobotCheckpointSaveGame::StaticClass());
}

void URobotCheckpointHandler::GetContent()
{
  Super::GetContent();
  ARModel *Robot = Cast<ARModel>(SavedObject);
  if (Robot)
  {
    URJointController *Controller = Cast<URJointController>(Robot->GetController(TEXT("JointController")));
    if (Controller)
    {
      URobotCheckpointSaveGame *RobotState = Cast<URobotCheckpointSaveGame>(SaveGame);
      if (RobotState)
      {
        RobotState->SetJointState(Controller->DesiredJointStates);
      }
    }
  }
  else
  {
    UE_LOG(LogTemp, Error, TEXT("%s: SavedObject is not castable into ARModel"), *GetName());
  }
}

void URobotCheckpointHandler::SetActorState(UActorCheckpointSaveGame *InActorState)
{
  ARModel *Robot = Cast<ARModel>(SavedObject);
  if (Robot)
  {
    URBaseController *Controller = Cast<URBaseController>(Robot->GetController(TEXT("BaseController")));
    if (Controller)
    {
      if (InActorState)
      {
        Controller->SetTransform(InActorState->ActorState.Pose);
      }
    }
  }
  else
  {
    UE_LOG(LogTemp, Error, TEXT("ResetCheckpoint: SavedObject can not be cast ARModel"));
  }
}

void URobotCheckpointHandler::ResetCheckpoint()
{
  Super::ResetCheckpoint();
  URobotCheckpointSaveGame *RobotState = Cast<URobotCheckpointSaveGame>(SaveGame);
  SetJointState(RobotState);
}

void URobotCheckpointHandler::SetJointState(URobotCheckpointSaveGame *InRobotState)
{
  ARModel *Robot = Cast<ARModel>(SavedObject);
  if (Robot)
  {
    URJointController *Controller = Cast<URJointController>(Robot->GetController(TEXT("JointController")));
    if (Controller)
    {
      if (InRobotState)
      {
        for (auto &JointState : InRobotState->GetJointStates())
        {
          Controller->DesiredJointStates[JointState.Key] = JointState.Value;
        }
      }
    }
  }
  else
  {
    UE_LOG(LogTemp, Error, TEXT("ResetCheckpoint: SavedObject can not be cast ARModel"));
  }
}
