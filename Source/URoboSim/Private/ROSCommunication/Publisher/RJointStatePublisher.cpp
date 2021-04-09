#include "ROSCommunication/Publisher/RJointStatePublisher.h"
#include "Physics/RJoint.h"
#include "sensor_msgs/JointState.h"

DEFINE_LOG_CATEGORY_STATIC(LogRJointStatePublisher, Log, All);

static void GenerateMsg(const TMap<FString, FJointState> &JointStates, TArray<FString> &JointNames, TArray<double> &JointPositions, TArray<double> &JointVelocities)
{
  for (const TPair<FString, FJointState> &JointState : JointStates)
  {
    JointNames.Add(JointState.Key);
    JointPositions.Add(JointState.Value.JointPosition);
    JointVelocities.Add(JointState.Value.JointVelocity);
  }
}

void URJointStatePublisher::Init()
{
  if (!PublisherParameters)
  {
    PublisherParameters = CreateDefaultSubobject<URJointStatePublisherParameter>(TEXT("JointStatePublisherParameters"));
  }
  
  if (GetOwner())
  {
    for (const URJoint *Joint : GetOwner()->GetJoints())
    {
      JointStates.Add(Joint->GetName());
    }
  }
}

void URJointStatePublisher::Publish()
{
  for (TPair<FString, FJointState> &JointState : JointStates)
  {
    URJoint *Joint = GetOwner()->GetJoint(JointState.Key);
    if (Joint)
    {
      JointState.Value = Joint->GetJointStateInROSUnit();
    }
    else
    {
      UE_LOG(LogRJointStatePublisher, Error, TEXT("Joint %s is not in %s"), *JointState.Key, *GetOwner()->GetName())
    }
  }

  static int Seq = 0;
  const URJointStatePublisherParameter *JointStatePublisherParameters = GetJointStatePublisherParameters();
  if (JointStatePublisherParameters)
  {
    static TSharedPtr<sensor_msgs::JointState> JointStateMsg = MakeShareable(new sensor_msgs::JointState());
    JointStateMsg->SetHeader(std_msgs::Header(Seq++, FROSTime(), JointStatePublisherParameters->FrameId));

    TArray<FString> JointNames;
    TArray<double> JointPositions;
    TArray<double> JointVelocities;
    GenerateMsg(JointStates, JointNames, JointPositions, JointVelocities);
    JointStateMsg->SetName(JointNames);
    JointStateMsg->SetPosition(JointPositions);
    JointStateMsg->SetVelocity(JointVelocities);

    Handler->PublishMsg(JointStatePublisherParameters->Topic, JointStateMsg);

    Handler->Process();
  }
}