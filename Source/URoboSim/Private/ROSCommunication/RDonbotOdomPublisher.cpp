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

static void CalculateOdomVelocity(TArray<double>& JointPositionStates, TArray<double>& JointVelocityStates)
{
  static TArray<double> JointPositionStatesOld = JointPositionStates;
  static TArray<double> JointVelocityStatesOld = JointVelocityStates;
   
  FROSTime frosTime;
  static FROSTime frosTimeOld = frosTime.Now();
  double deltaSecs = (double)(frosTime.NSecs - frosTimeOld.NSecs)/1000000000;
  double deltaSecsDelay = 0.001; //Set time delay to avoid noise and division by zeros

  double c_phi = cos(JointPositionStates[2]);
  double s_phi = sin(JointPositionStates[2]);
  double v_x = (JointPositionStates[0] - JointPositionStatesOld[0] + deltaSecsDelay * JointVelocityStatesOld[0])/(deltaSecs + deltaSecsDelay);
  double v_y = (JointPositionStates[1] - JointPositionStatesOld[1] + deltaSecsDelay * JointVelocityStatesOld[1])/(deltaSecs + deltaSecsDelay);
  
  JointVelocityStates[0] = v_x * c_phi + v_y * s_phi;
  JointVelocityStates[1] = -v_x * s_phi + v_y * c_phi;
  JointVelocityStates[2] = (JointPositionStates[2] - JointPositionStatesOld[2] + deltaSecsDelay * JointVelocityStatesOld[2])/(deltaSecs + deltaSecsDelay);

  //Update variables for next step
  JointPositionStatesOld = JointPositionStates;
  JointVelocityStatesOld = JointVelocityStates;
  frosTimeOld = frosTime.Now();
}

void URDonbotOdomPublisher::Publish()
{
  FVector BasePose =FConversions::UToROS(this->Owner->GetActorLocation());
  FQuat BaseQuaternion =FConversions::UToROS(this->Owner->GetActorRotation().Quaternion());
  FRotator BaseRotation = BaseQuaternion.Rotator();

  TSharedPtr<sensor_msgs::JointState> JointState =
    MakeShareable(new sensor_msgs::JointState());

  this->JointPositionStates[0]=BasePose.X;
  this->JointPositionStates[1]=BasePose.Y;
  static double deg2rad = acos(0.0)/90;
  this->JointPositionStates[2]=deg2rad*BaseRotation.Yaw;
  
  CalculateOdomVelocity(this->JointPositionStates, this->JointVelocityStates);

  JointState->SetHeader(std_msgs::Header(Seq, FROSTime(), TEXT("0")));
  JointState->SetName(FrameNames);
  JointState->SetPosition(this->JointPositionStates);
  JointState->SetVelocity(this->JointVelocityStates);

  Seq += 1;

  Handler->PublishMsg(Topic, JointState);
  Handler->Process();
}
