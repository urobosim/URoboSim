// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "CoreMinimal.h"
#include "EditorReimportHandler.h"
#include "Factories/DataAssetFactory.h"
#include "SDFDataAssetFactory.generated.h"

// Forward declaration
class USDFDataAsset;

/**
 *
 */
UCLASS()
class UROBOSIMEDITOR_API USDFDataAssetFactory : public UFactory/*, public FReimportHandler*/
{
	GENERATED_BODY()

public:
	// Constructor
	USDFDataAssetFactory(const FObjectInitializer& ObjectInitializer);

private:
	//////// Begin UFactory Interface
	// Whether the specified file can be imported by this factory
	virtual bool FactoryCanImport(const FString& Filename) override;

	// Create a new object by importing it from a file name
	virtual UObject* FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, const FString & Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled) override;
	//////// End UFactory Interface
};
