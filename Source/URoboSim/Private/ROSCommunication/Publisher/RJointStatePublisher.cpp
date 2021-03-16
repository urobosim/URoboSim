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

URJointStatePublisher::URJointStatePublisher()
{
  Topic = TEXT("/joint_states");
}

void URJointStatePublisher::SetMessageType()
{
  MessageType = TEXT("sensor_msgs/JointState");
}

void URJointStatePublisher::SetOwner(UObject *&InOwner)
{
  URPublisher::SetOwner(InOwner);
  ConfigClient = NewObject<URJointStateConfigurationClient>(this);
  TArray<FString> JointNames;
  ConfigClient->URServiceClient::Init(InOwner, &JointNames, Handler);
  UE_LOG(LogRJointStatePublisher, Error, TEXT("JointName:"))
  for (const FString &JointName : JointNames)
  {
    UE_LOG(LogRJointStatePublisher, Warning, TEXT("%s"), *JointName)
  }
  for (const URJoint *Joint : GetOwner()->GetJoints())
  {
    JointStates.Add(Joint->GetName());
  }
}

void URJointStatePublisher::Publish()
{
  for (TPair<FString, FJointState> &JointState : JointStates)
  {
    URJoint *Joint = GetOwner()->GetJoint(JointState.Key);
    if (Joint)
    {
      JointState.Value = Joint->GetJointState();
    }
    else
    {
      UE_LOG(LogRJointStatePublisher, Error, TEXT("Joint %s is not in %s"), *JointState.Key, *GetOwner()->GetName())
    }
  }

  TSharedPtr<sensor_msgs::JointState> JointStateMsg =
      MakeShareable(new sensor_msgs::JointState());
  JointStateMsg->SetHeader(std_msgs::Header(Seq, FROSTime(), FrameId));
  TArray<FString> JointNames;
  TArray<double> JointPositions;
  TArray<double> JointVelocities;
  GenerateMsg(JointStates, JointNames, JointPositions, JointVelocities);
  JointStateMsg->SetName(JointNames);
  JointStateMsg->SetPosition(JointPositions);
  JointStateMsg->SetVelocity(JointVelocities);

  Handler->PublishMsg(Topic, JointStateMsg);

  Handler->Process();
  Seq++;
}