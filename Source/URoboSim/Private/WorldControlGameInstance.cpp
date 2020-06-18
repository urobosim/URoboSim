// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldControlGameInstance.h"
#include "ROSCommunication/ResetLevelServer.h"


void UWorldControlGameInstance::OnStart()
{
  Super::OnStart();

  if(ROSHandler.IsValid())
    {
      UWorld* World = GetWorld();
      if(World)
      {
	ROSHandler->AddServiceServer(MakeShareable<FROSResetLevelServer>(new FROSResetLevelServer(FString("UnrealSim"), TEXT("reset_level"),  this)));
      }
      else
        {
          UE_LOG(LogTemp, Error, TEXT("World not ready"));
        }
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("GameInstance: Handler not valid"));
    }
}

// void UWorldControlGameInstance::Tick(float DeltaTime)
// {
//   Super::Tick(DeltaTime);
//   UE_LOG(LogTemp, Error, TEXT("Tick"));
// }
