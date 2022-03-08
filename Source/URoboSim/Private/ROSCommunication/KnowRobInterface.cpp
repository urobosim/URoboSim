
#include "ROSCommunication/KnowRobInterface.h"

UKnowrobInterface::UKnowrobInterface()
{
  QueryClient = CreateDefaultSubobject<UPrologQueryClient>(FName(*(GetOuter()->GetName() + TEXT("_PrologQueryClient"))));
}
