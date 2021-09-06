#pragma once

#include "ROSBridgeSrvServer.h"
#include "ROSCommunication/RROSCommunication.h"
#include "RobotInterface.h"
// clang-format off
#include "RServiceServer.generated.h"
// clang-format on

UCLASS(BlueprintType, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URServiceServerParameter : public UObject
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere)
	FString Name;

	UPROPERTY(EditAnywhere)
	FString Type;
};

UCLASS()
class UROBOSIM_API UServiceServer : public UROSCommunication
{
	GENERATED_BODY()
};

UCLASS()
class UROBOSIM_API URServiceServer : public UServiceServer, public IRobotInterface
{
	GENERATED_BODY()

public:
	virtual void SetServiceServerParameters(URServiceServerParameter *&ServiceServerParameters);

public:
	UPROPERTY(EditAnywhere, Category = "ROS Service Server")
	FString Name;

	UPROPERTY(VisibleAnywhere)
	FString Type;

protected:
  virtual void Init() override;

	virtual void CreateServiceServer(){};

protected:
	TSharedPtr<FROSBridgeSrvServer> ServiceServer;
};


// UCLASS()
// class UROBOSIM_API URServiceServerFactory: public UObject
// {
//     GENERATED_BODY()
// public:
// 	static URServiceServer* CreateInstance(FString Type, UObject* Owner);
//
// };
