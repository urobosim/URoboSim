#include "ROSCommunication/Publisher/RDonbotOdomPublisher.h"
#include "sensor_msgs/JointState.h"

URDonbotOdomPublisher::URDonbotOdomPublisher()
{
  Topic = TEXT("/joint_states");
  MessageType = TEXT("sensor_msgs/JointState");
	OdomFrameXId = TEXT("odom_x_joint");
	OdomFrameYId = TEXT("odom_y_joint");
	OdomFrameZId = TEXT("odom_z_joint");
}

void URDonbotOdomPublisher::SetPublishParameters(URPublisherParameter *&PublisherParameters)
{
  if (URDonbotOdomPublisherParameter *DonbotOdomPublisherParameters = Cast<URDonbotOdomPublisherParameter>(PublisherParameters))
  {
    Super::SetPublishParameters(PublisherParameters);
    OdomFrameXId = DonbotOdomPublisherParameters->OdomFrameXId;
    OdomFrameYId = DonbotOdomPublisherParameters->OdomFrameYId;
    OdomFrameZId = DonbotOdomPublisherParameters->OdomFrameZId;
  }
}

void URDonbotOdomPublisher::Init()
{
  Super::Init();
  if (GetOwner())
  {
    BaseController = Cast<URBaseController>(GetOwner()->GetController(TEXT("BaseController")));
  }
}

void URDonbotOdomPublisher::Publish()
{
  if (GetOwner() && BaseController)
  {
    static int Seq = 0;
    TArray<FString> FrameNames;
    TSharedPtr<sensor_msgs::JointState> JointState =
        MakeShareable(new sensor_msgs::JointState());

    JointState->SetHeader(std_msgs::Header(Seq, FROSTime(), BaseFrameId));
    FrameNames.Add(OdomFrameXId);
    FrameNames.Add(OdomFrameYId);
    FrameNames.Add(OdomFrameZId);
    JointState->SetName(FrameNames);
    JointState->SetPosition(BaseController->GetOdomPositionStates());
    JointState->SetVelocity(BaseController->GetOdomVelocityStates());

    Seq += 1;

    Handler->PublishMsg(Topic, JointState);
    Handler->Process();
  }
}
