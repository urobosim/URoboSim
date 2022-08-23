#pragma once

#include "SDFParser.h"

class FXmlFile;

class UROBOSIMEDITOR_API FSDFParserEditor : public FSDFParser //: public FSDFParserBase
{
public:
    // Default constructor
    FSDFParserEditor();

    // Constructor with load from path
    FSDFParserEditor(const FString& InFilename);

    // Clear parser
    virtual void Clear() override;

    // Load sdf from path
    virtual bool LoadSDF(const FString& InFilename) override;
protected:
    // Fbx factory
    UFbxFactory* FbxFactory;
    // Import .fbx meshes from data asset
    UStaticMesh* ImportMesh(const FString& Uri, ESDFType Type);
    UStaticMesh* CreateMesh(ESDFType InType, ESDFGeometryType InShape, FString InName, TArray<float> InParameters);

    // Parse <link> node
    virtual void ParseLink(const FXmlNode* InNode, USDFModel*& OutModel) override;
    // Parse <visual> node
    virtual void ParseVisualChild(const FXmlNode* InNode, USDFVisual*& OutVisual) override;
    virtual void ParseCollisionChild(const FXmlNode* InNode, USDFCollision*& OutCollision) override;

    virtual UStaticMesh* GetMesh(const FString& Uri, ESDFType Type) override;


    virtual bool CreateCollisionForMesh(UStaticMesh* OutMesh, ESDFGeometryType Type);
    virtual USDFCollision* CreateVirtualCollision(USDFLink* OutLink);
};
