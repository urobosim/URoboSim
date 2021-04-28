#include "ROSCommunication/Publisher/JointStatePublisher.h"
#include "sensor_msgs/JointState.h"
#include "Physics/RJoint.h"

URJointStatePublisher::URJointStatePublisher()
{
  Topic = TEXT("/joint_states");
  JointParamTopic = TEXT("/hardware_interface/joints");
}

void URJointStatePublisher::SetPublishParameters(URPublisherParameter *&PublisherParameters)
{
  Super::SetPublishParameters(PublisherParameters);
  URJointStatePublisherParameter *JointStatePublisherParameter = Cast<URJointStatePublisherParameter>(PublisherParameters);
  if (JointStatePublisherParameter)
  {
    JointParamTopic = JointStatePublisherParameter->JointParamTopic;
  }  
}

void URJointStatePublisher::SetMessageType()
{
  MessageType = TEXT("sensor_msgs/JointState");
}

void URJointStatePublisher::SetOwner(UObject* InOwner)
{
  Owner = Cast<ARModel>(InOwner);
  ConfigClient = NewObject<URJointStateConfigurationClient>(this);
  ConfigClient->JointParamTopic = JointParamTopic;
  ConfigClient->URServiceClient::Init(InOwner, &ListJointName, Handler);
}

void URJointStatePublisher::CreatePublisher()
{
  Super::CreatePublisher();
}

void URJointStatePublisher::Publish()
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
