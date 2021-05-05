// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckpointSystem/RobotCheckpointSaveGame.h"


void URobotCheckpointSaveGame::SetJointState(TMap<FString, FJointState> InJointStates)
{
  JointStates = InJointStates;
}

TMap<FString, FJointState> URobotCheckpointSaveGame::GetJointStates()
{
  return JointStates;
}
