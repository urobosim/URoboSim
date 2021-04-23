#pragma once

#include "CoreMinimal.h"
#include "Physics/RModel.h"
#include "ROSCommunication/RROSService.h"
#include "CheckVisibilityService.generated.h"

UCLASS()
class UROBOSIM_API UCheckVisibilityService : public URROSService
{
    GENERATED_BODY()

protected:

    virtual void SetOwner(UObject* InOwner) override;
    virtual void SetType() override;
    virtual void CreateServiceServer() override;


    UPROPERTY()
      ARModel* Owner;

};
