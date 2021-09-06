
#pragma once

#include "CoreMinimal.h"
#include "ROSCommunication/Service/Client/PrologQueryClient.h"
#include "KnowRobInterface.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FKnowrobEvent, UObject * /*PerceivedObject*/);

UCLASS()
class UKnowrobInterface : public UObject
{
  GENERATED_BODY()

public:

  UKnowrobInterface();

  UPROPERTY()
  UPrologQueryClient* QueryClient;

  FKnowrobEvent OnKnowrobEvent;

};
