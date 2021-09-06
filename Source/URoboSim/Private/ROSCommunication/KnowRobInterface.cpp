
#include "KnowRobInterface.h"

UKnowrobInterface::UKnowrobInterface()
{
  QueryClient = CreateDefaultSubobject<UPrologQueryClient>(FName(*GetOuter()->GetName() + "_PrologQueryClient"));
}
