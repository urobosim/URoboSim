// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen

#include "ROSCommunication/RWorldManager.h"
#include "ROSCommunication/ResetLevelServer.h"


void URoboSimWCM::SetupServiceServers()
{
  UE_LOG(LogTemp, Error, TEXT("Setup Service"));
  // Add Service Servers
  UWorld* World = GetWorld();
  ServicesToPublish.Add(MakeShareable<FROSResetLevelServer>(new FROSResetLevelServer(Namespace, TEXT("reset_level"), World, this)));
}
