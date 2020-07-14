#include "ROSCommunication/RDonbotOdomPublisher.h"
#include "sensor_msgs/JointState.h"

URDonbotOdomPublisher::URDonbotOdomPublisher()
{
  Topic = TEXT("/joint_state");
}

void URDonbotOdomPublisher::SetMessageType()
{
  MessageType = TEXT("sensor_msgs/JointState");
}

void URDonbotOdomPublisher::SetOwner(UObject* InOwner)
{
  Owner = Cast<ARModel>(InOwner);
  FrameNames.Add(OdomFrameXId);
  FrameNames.Add(OdomFrameYId);
  FrameNames.Add(OdomFrameZId);
  JointStates.Add(0);
  JointStates.Add(0);
  JointStates.Add(0);
}

void URDonbotOdomPublisher::Publish()
{

  FVector BasePose =FConversions::UToROS(Owner->GetActorLocation());
  FRotator BaseRotation =FConversions::UToROS(Owner->GetActorRotation());

  TSharedPtr<sensor_msgs::JointState> JointState =
    MakeShareable(new sensor_msgs::JointState());


  JointStates[0]=BasePose.X;
  JointStates[1]=BasePose.Y;
  JointStates[2]=BaseRotation.Yaw;

  JointState->SetHeader(std_msgs::Header(Seq, FROSTime(), TEXT("0")));
  JointState->SetName(FrameNames);
  JointState->SetPosition(JointStates);


  Seq += 1;

  Handler->PublishMsg(Topic, JointState);
  Handler->Process();
}
