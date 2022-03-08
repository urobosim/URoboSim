#pragma once

#include "RServiceServer.h"
#include "AJointActor.h"

class FSetEnvironmentJointStatesServerCallback : public FROSBridgeSrvServer
{
public:
  FSetEnvironmentJointStatesServerCallback(FString Name, FString Type, TMap<FString,AJointActor*>* InJointList );

  TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

  TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;

  FEnableDrive EnableDrive;
  float ErrorTollerance = 0.01;
private:
  TMap<FString,AJointActor*>* JointList;

  FThreadSafeBool bAllSuccessfull;
};
