
#include "AJointActor.h"
#include "URoboSimGameInstance.h"
#include "Editor/EditorEngine.h"

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
      GI->Joints.Add(this);
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
      if(JointDescription)
        {
          JointDescription->Name = GetName();
          JointDescription->Axis = Axis;
          if(!ConstraintActor1 || !ConstraintActor1)
            {
              return;
            }
          JointDescription->Parent=ConstraintActor1->GetName();
          JointDescription->Child=ConstraintActor2->GetName();
        }

      if(Joint)
        {
          Joint->BeginDestroy();
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
    }
}

#endif // WITH_EDITOR
