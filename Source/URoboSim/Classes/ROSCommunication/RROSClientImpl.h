#pragma once

#include "CoreMinimal.h"
#include "ROSBridgeHandler.h"
#include "srv/ConfigureJointStatePublisher.h"
#include "Physics/RModel.h"
#include "ROSBridgeSrvClient.h"

class URJointController;

class FROSJointStateConfigurationClient final : public FROSBridgeSrvClient
{
public:
	FROSJointStateConfigurationClient(TArray<FString>* OutJointNames, const FString& InName, const FString& InType);

	void Callback( TSharedPtr<FROSBridgeSrv::SrvResponse> InResponse) override;
private:
	TArray<FString>* JointNames;
};

class FROSJointControllerConfigurationClient final : public FROSBridgeSrvClient
{
public:
  FROSJointControllerConfigurationClient(URJointController* InJointController, TMap<FString, float>* OutJointNames, const FString& InName, const FString& InType);

	void Callback( TSharedPtr<FROSBridgeSrv::SrvResponse> InResponse) override;
private:
  TMap<FString, float>* JointNames;
  URJointController* JointController;
};

class FROSJointLimitControllerConfigurationClient final : public FROSBridgeSrvClient
{
public:
  FROSJointLimitControllerConfigurationClient(TMap<FString, float>* OutJointNames, AActor* InModel, const FString& InName, const FString& InType);

	void Callback( TSharedPtr<FROSBridgeSrv::SrvResponse> InResponse) override;
private:
  TMap<FString, float>* JointNames;
  ARModel* Model;
};
