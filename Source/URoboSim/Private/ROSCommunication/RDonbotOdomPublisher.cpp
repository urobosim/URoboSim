#include "ROSCommunication/RDonbotOdomPublisher.h"
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
  Owner = Cast<ARModel>(InOwner);

  this->FrameNames.Add(this->OdomFrameXId);
  this->FrameNames.Add(this->OdomFrameYId);
  this->FrameNames.Add(this->OdomFrameZId);

  this->JointPositionStates.Add(0);
  this->JointPositionStates.Add(0);
  this->JointPositionStates.Add(0);

  this->JointVelocityStates.Add(0);
  this->JointVelocityStates.Add(0);
  this->JointVelocityStates.Add(0);
}

void URDonbotOdomPublisher::Publish()
{
  static TArray<double> JointPositionStatesOld = this->JointPositionStates;
  static TArray<double> JointVelocityStatesOld = this->JointVelocityStates;

  FVector BasePose =FConversions::UToROS(this->Owner->GetActorLocation());
  FQuat BaseQuaternion =FConversions::UToROS(this->Owner->GetActorRotation().Quaternion());
  FRotator BaseRotation = BaseQuaternion.Rotator();

  TSharedPtr<sensor_msgs::JointState> JointState =
    MakeShareable(new sensor_msgs::JointState());

  this->JointPositionStates[0]=BasePose.X;
  this->JointPositionStates[1]=BasePose.Y;
  this->JointPositionStates[2]=BaseRotation.Yaw;
  
  FROSTime frosTime;
  static FROSTime frosTimeOld;
  float deltaSecs = (float)(frosTime.NSecs - frosTimeOld.NSecs)/1000000000;
  float deltaSecsDelay = 0.01;

  this->JointVelocityStates[0]=(this->JointPositionStates[0] - JointPositionStatesOld[0] + deltaSecsDelay * JointVelocityStatesOld[0])/(deltaSecs + deltaSecsDelay);
  this->JointVelocityStates[1]=(this->JointPositionStates[1] - JointPositionStatesOld[1] + deltaSecsDelay * JointVelocityStatesOld[1])/(deltaSecs + deltaSecsDelay);
  this->JointVelocityStates[2]=(this->JointPositionStates[2] - JointPositionStatesOld[2] + deltaSecsDelay * JointVelocityStatesOld[2])/(deltaSecs + deltaSecsDelay);

  JointState->SetHeader(std_msgs::Header(Seq, frosTime, TEXT("0")));
  JointState->SetName(FrameNames);
  JointState->SetPosition(this->JointPositionStates);
  JointState->SetVelocity(this->JointVelocityStates);

  Seq += 1;

  Handler->PublishMsg(Topic, JointState);
  Handler->Process();

  JointPositionStatesOld = this->JointPositionStates;
  JointVelocityStatesOld = this->JointVelocityStates;
  frosTimeOld = frosTime.Now();
}
