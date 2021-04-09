// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#include "SDFDataAssetFactory.h"
#include "SDFParserEditor.h"
#include "UnrealEd.h"

DEFINE_LOG_CATEGORY_STATIC(LogSDFDataAssetFactory, Log, All);

// Constructor
USDFDataAssetFactory::USDFDataAssetFactory(const FObjectInitializer &ObjectInitializer) : UFactory(ObjectInitializer)
{
	// Which asset type can the factory import
	SupportedClass = UDataAsset::StaticClass();

	// List of formats supported by the factory. Each entry is of the form "ext;Description" where ext is the file extension
	Formats.Add(TEXT("sdf;Simulation Description Format"));

	// Factory imports objects from files
	bEditorImport = true;

	// Required to allow other re importers do their CanReimport checks first
	ImportPriority = DefaultImportPriority - 1;
}

// Whether the specified file can be imported by this factory
bool USDFDataAssetFactory::FactoryCanImport(const FString &Filename)
{
	return Filename.EndsWith(".sdf", ESearchCase::IgnoreCase);
}

// Create a new object by importing it from a file name
UObject *USDFDataAssetFactory::FactoryCreateFile(UClass *InClass, UObject *InParent, FName InName, EObjectFlags Flags, const FString &Filename, const TCHAR *Parms, FFeedbackContext *Warn, bool &bOutOperationCanceled)
{
	UE_LOG(LogSDFDataAssetFactory, Log, TEXT("Create asset %s at %s"), *InName.ToString(), *InParent->GetName());

	Flags |= RF_Transactional;

	// Called when new assets are being (re-)imported
	GEditor->GetEditorSubsystem<UImportSubsystem>()->BroadcastAssetPreImport(this, InClass, InParent, InName, Parms);

	// Parse the .sdf buffer data into the data asset
	FSDFParserEditor Parser(Filename);

	// Create a new SDFDataAsset
	USDFDataAsset *NewDataAsset = Parser.ParseToDataAsset(InParent, InName, Flags);

	// Called when new assets have been (re-)imported
	GEditor->GetEditorSubsystem<UImportSubsystem>()->BroadcastAssetPostImport(this, NewDataAsset);
	return NewDataAsset;
}
/* End UFactory overrides */