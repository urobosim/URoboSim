
#include "AJointActor.h"
#include "URoboSimGameInstance.h"

#if WITH_EDITOR
#include "Editor/EditorEngine.h"
#endif // WITH_EDITOR
AJointActor::AJointActor()
{
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
  JointDescription = CreateDefaultSubobject<USDFJoint>(FName(*(GetName() + TEXT("_JointDescription"))));
  Axis = CreateDefaultSubobject<USDFJointAxis>(TEXT("Axis"));
}

void AJointActor::BeginPlay()
{
  Super::BeginPlay();
  URoboSimGameInstance* GI = Cast<URoboSimGameInstance>(UGameplayStatics::GetGameInstance(this));
  if(GI)
    {
      GI->Joints.Add(GetName(), this);
    }
}

void AJointActor::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
  if(Joint)
    {
      CurrentJointPos = Joint->GetEncoderValue();
    }
}

bool AJointActor::CreateJoint()
{
  if(JointDescription)
    {
      JointDescription->Name = GetName();
      JointDescription->Axis = Axis;
      if(!ConstraintActor1 || !ConstraintActor1)
        {
          return false;
        }
      JointDescription->Parent=ConstraintActor1->GetName();
      JointDescription->Child=ConstraintActor2->GetName();
    }

  if(Joint)
    {
      Joint->ConditionalBeginDestroy();
      Joint = NULL;
      GEngine->ForceGarbageCollection(true);
    }

  if(JointType)
    {
      JointType->Init(this, JointDescription);
      Joint = JointType->NewJoint();
      Joint->SetParentChild(ConstraintActor1, ConstraintActor2);
      Joint->Constraint->ConnectToComponents();
      RootComponent = Joint->Constraint;
    }
  if(Joint)
    {
      return true;
    }
  else
    {
      return false;
    }
}

#if WITH_EDITOR
void AJointActor::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
  Super::PostEditChangeChainProperty(PropertyChangedEvent);
}

void AJointActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
  Super::PostEditChangeProperty(PropertyChangedEvent);

  const static FName JointTypeName("JointType");

  const FName PropertyName = PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : FName();
  const FName MemberPropertyName = PropertyChangedEvent.MemberProperty ? PropertyChangedEvent.MemberProperty->GetFName() : FName();

  if(PropertyName == JointTypeName || MemberPropertyName == JointTypeName)
    {
      CreateJoint();
    }
}

#endif // WITH_EDITOR
