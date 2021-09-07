
#pragma once

#include "CoreMinimal.h"
#include "ROSCommunication/Service/Client/PrologQueryClient.h"
#include "KnowRobInterface.generated.h"



UCLASS()
class UKnowrobInterface : public UObject
{
  GENERATED_BODY()

public:

  UKnowrobInterface();

  UPROPERTY()
  UPrologQueryClient* QueryClient;
};
