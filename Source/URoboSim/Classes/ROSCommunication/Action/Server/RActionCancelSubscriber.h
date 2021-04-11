#pragma once

#include "RActionServer.h"
#include "RActionServerParameter.h"
// clang-format off
#include "RActionCancelSubscriber.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URActionCancelSubscriber : public URActionSubscriber
{
	GENERATED_BODY()

public:
	URActionCancelSubscriber();
};