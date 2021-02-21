// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckpointSystem/RobotCheckpointSaveGame.h"


void URobotCheckpointSaveGame::SetJointState(TMap<FString, float> InJointState)
{
  JointStates = InJointState;
}

TMap<FString, float> URobotCheckpointSaveGame::GetJointStates()
{
  return JointStates;
}
