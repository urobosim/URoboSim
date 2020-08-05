// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckpointSystem/RobotCheckpointHandler.h"
#include "Kismet/GameplayStatics.h"
#include "Controller/RJointController.h"
#include "Controller/RController.h"
#include "Physics/RModel.h"

USaveGame* URobotCheckpointHandler::CreateSaveGameFile()
{
  return UGameplayStatics::CreateSaveGameObject(URobotCheckpointSaveGame::StaticClass());
}

void URobotCheckpointHandler::GetContent()
{
  Super::GetContent();
  ARModel* Robot = Cast<ARModel>(SavedObject);
  if(Robot)
    {
      if(Robot->Plugins.Contains(TEXT("ControllerComponent")))
        {
          URControllerComponent* ControllerComp = Cast<URControllerComponent>(Robot->Plugins[TEXT("ControllerComponent")]);
          if(ControllerComp)
            {
              URJointController* Controller = Cast<URJointController>(ControllerComp->ControllerList(TEXT("JointController")));
              if(Controller)
                {
                  URobotCheckpointSaveGame* RobotState = Cast<URobotCheckpointSaveGame>(SaveGame);
                  if(RobotState)
                    {
                      RobotState->SetJointState(Controller->DesiredJointState);
                    }
                }
              else
                {
                  UE_LOG(LogTemp, Error, TEXT("%s: JointController not found"), *GetName());
                }
            }
          else
            {
              UE_LOG(LogTemp, Error, TEXT("%s: Controller component not found"), *GetName());
            }
        }
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("%s: SavedObject is not castable into ARModel"), *GetName());
    }
}

void URobotCheckpointHandler::SetActorState(UActorCheckpointSaveGame* InActorState)
{
  ARModel* Robot = Cast<ARModel>(SavedObject);
  if(Robot)
    {
      if(Robot->Plugins.Contains(TEXT("ControllerComponent")))
        {
          URControllerComponent* ControllerComp = Cast<URControllerComponent>(Robot->Plugins[TEXT("ControllerComponent")]);
          if(ControllerComp)
            {
              URBaseController* Controller = Cast<URBaseController>(ControllerComp->ControllerList(TEXT("BaseController")));
              if(Controller)
                {
                  if(InActorState)
                    {
                      Controller->SetTransform(InActorState->ActorState.Pose);
                    }
                }
              else
                {
                  UE_LOG(LogTemp, Error, TEXT("%s: BaseController not found"), *GetName());
                }
            }
          else
            {
              UE_LOG(LogTemp, Error, TEXT("%s: Controller component not found"), *GetName());
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
  URobotCheckpointSaveGame* RobotState = Cast<URobotCheckpointSaveGame>(SaveGame);
  SetJointState(RobotState);
}

void URobotCheckpointHandler::SetJointState(URobotCheckpointSaveGame* InRobotState)
{
  ARModel* Robot = Cast<ARModel>(SavedObject);
  if(Robot)
    {
      if(Robot->Plugins.Contains(TEXT("ControllerComponent")))
        {
          URControllerComponent* ControllerComp = Cast<URControllerComponent>(Robot->Plugins[TEXT("ControllerComponent")]);
          if(ControllerComp)
            {
              URJointController* Controller = Cast<URJointController>(ControllerComp->ControllerList(TEXT("JointController")));
              if(Controller)
                {
                  if(InRobotState)
                    {
                      for(auto& JointState : InRobotState->GetJointStates())
                        {
                          Controller->SetDesiredJointState(JointState.Key, JointState.Value);
                        }
                    }
                }
              else
                {
                  UE_LOG(LogTemp, Error, TEXT("%s: JointController not found"), *GetName());
                }
            }
          else
            {
              UE_LOG(LogTemp, Error, TEXT("%s: Controller component not found"), *GetName());
            }
        }
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("ResetCheckpoint: SavedObject can not be cast ARModel"));
    }

}
