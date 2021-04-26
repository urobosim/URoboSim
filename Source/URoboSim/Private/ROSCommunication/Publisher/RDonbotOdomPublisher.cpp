#include "ROSCommunication/Publisher/RDonbotOdomPublisher.h"
#include "sensor_msgs/JointState.h"

URDonbotOdomPublisher::URDonbotOdomPublisher()
{
  Topic = TEXT("/joint_states");
}

void URDonbotOdomPublisher::SetMessageType()
{
  MessageType = TEXT("sensor_msgs/JointState");
}

void URDonbotOdomPublisher::SetOwner(UObject* InOwner)
{
  URControllerComponent* ControllerComp = Cast<URControllerComponent>(Cast<ARModel>(InOwner)->Plugins["ControllerComponent"]);
  this->Owner = Cast<URBaseController>(ControllerComp->GetController(TEXT("BaseController")));

  this->FrameNames.Add(this->OdomFrameXId);
  this->FrameNames.Add(this->OdomFrameYId);
  this->FrameNames.Add(this->OdomFrameZId);
}

void URDonbotOdomPublisher::Publish()
{
  TSharedPtr<sensor_msgs::JointState> JointState =
    MakeShareable(new sensor_msgs::JointState());

  JointState->SetHeader(std_msgs::Header(Seq, FROSTime(), TEXT("0")));
  JointState->SetName(FrameNames);
  if(this->Owner)
  {
    JointState->SetPosition(this->Owner->GetOdomPositionStates());
    JointState->SetVelocity(this->Owner->GetOdomVelocityStates());
  }

  Seq += 1;

  Handler->PublishMsg(Topic, JointState);
  Handler->Process();
}
