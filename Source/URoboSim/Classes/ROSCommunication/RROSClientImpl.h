#pragma once

#include "CoreMinimal.h"
#include "ROSBridgeHandler.h"
#include "srv/ConfigureJointStatePublisher.h"
#include "ROSBridgeSrvClient.h"


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
  FROSJointControllerConfigurationClient(TMap<FString, float>* OutJointNames, const FString& InName, const FString& InType);

	void Callback( TSharedPtr<FROSBridgeSrv::SrvResponse> InResponse) override;
private:
  TMap<FString, float>* JointNames;
};
