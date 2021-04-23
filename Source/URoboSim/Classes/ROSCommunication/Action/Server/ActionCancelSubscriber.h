
#include "ROSCommunication/Subscriber/RSubscriber.h"
#include "ActionCancelSubscriber.generated.h"

UCLASS()
class UROBOSIM_API URActionCancelSubscriber: public URSubscriber
{
    GENERATED_BODY()
public:

	virtual void SetMessageType();
	virtual void CreateSubscriber();
};
