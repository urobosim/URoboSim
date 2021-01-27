#include "ROSCommunication/Publisher/EnvironmentStatePublisher.h"
#include "sensor_msgs/JointState.h"
#include "Physics/RJoint.h"

UREnvironmentStatePublisher::UREnvironmentStatePublisher()
{
  Topic = TEXT("/joint_states");
  JointParamTopic = TEXT("/hardware_interface/joints");
}

void UREnvironmentStatePublisher::SetMessageType()
{
  MessageType = TEXT("sensor_msgs/JointState");
}

void UREnvironmentStatePublisher::SetOwner(UObject* InOwner)
{
  Owner = Cast<ARModel>(InOwner);
  ConfigClient = NewObject<URJointStateConfigurationClient>(this);
  ConfigClient->JointParamTopic = JointParamTopic;
  ConfigClient->URROSClient::Init(InOwner, &ListJointName, Handler);
}

void UREnvironmentStatePublisher::CreatePublisher()
{
  Super::CreatePublisher();
}

void UREnvironmentStatePublisher::Publish()
{
  ListJointPosition.Empty();
  ListJointPosition.Reserve(ListJointName.Num());
  ListJointVelocity.Empty();
  ListJointVelocity.Reserve(ListJointName.Num());
  ListJointEffort.Empty();
  ListJointEffort.Reserve(ListJointName.Num());

  for (auto &JointName : ListJointName)
    {
      if(Owner->Joints.Contains(JointName))
        {
          URJoint* Joint = Owner->Joints[JointName];
          float JointPosition = Joint->GetEncoderValue();
          float JointVelocity = Joint->GetJointVelocity();

          ListJointPosition.Add(JointPosition);
          ListJointVelocity.Add(0.0);
          ListJointEffort.Add(0.0);
        }
      else
        {
          UE_LOG(LogTemp, Log, TEXT("Joint not in robot: %s"), *JointName);
        }
    }

  TSharedPtr<sensor_msgs::JointState> JointState =
    MakeShareable(new sensor_msgs::JointState());
  JointState->SetHeader(std_msgs::Header(Seq, FROSTime(), TEXT("0")));
  JointState->SetName(ListJointName);
  JointState->SetPosition(ListJointPosition);
  JointState->SetVelocity(ListJointVelocity);
  JointState->SetEffort(ListJointEffort);

  Handler->PublishMsg(Topic, JointState);

  Handler->Process();
  Seq++;
  // UE_LOG(LogTemp, Log, TEXT("JointState = %s"), *JointState->ToString());
}
