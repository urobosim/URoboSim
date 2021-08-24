#include "URoboSimGameInstance.h"

void URoboSimGameInstance::OnStart()
{
  Super::OnStart();

  Topic = TEXT("/joint_states");
  MessageType = TEXT("sensor_msgs/JointState");
  Publisher = MakeShareable<FROSBridgePublisher>(new FROSBridgePublisher(Topic, MessageType));

  if (Publisher.IsValid())
  {
    ROSHandler->AddPublisher(Publisher);
  }
}


void URoboSimGameInstance::Tick(float DeltaTime)
{
  if (ROSHandler.IsValid() && Publisher.IsValid())
    {
      static int Seq = 0;
      TArray<FString> ListJointName;
      TArray<double> ListJointPosition;
      for(auto& Joint : Joints)
        {
          ListJointName.Add(Joint->GetName());
          ListJointPosition.Add(Joint->CurrentJointPos);
        }

      TSharedPtr<sensor_msgs::JointState> JointState =
        MakeShareable(new sensor_msgs::JointState());
      JointState->SetHeader(std_msgs::Header(Seq, FROSTime(), ""));
      JointState->SetName(ListJointName);
      JointState->SetPosition(ListJointPosition);

      ROSHandler->PublishMsg(Topic, JointState);

      Seq++;
      Super::Tick(DeltaTime);
    }
}
