#include "ROSCommunication/Publisher/RPublisher.h"
// #include "geometry_msgs/TransformStamped.h"
// #include "geometry_msgs/PoseStamped.h"
// #include "geometry_msgs/Pose.h"
// #include "geometry_msgs/Point.h"
// #include "geometry_msgs/Quaternion.h"
// #include "control_msgs/JointTrajectoryControllerState.h"
// #include "actionlib_msgs/GoalStatusArray.h"

// #include "tf2_msgs/TFMessage.h"

URPublisher::URPublisher()
{
}

void URPublisher::DeInit()
{
  if (Handler.IsValid())
  {
    Handler->Disconnect();
  }
}

void URPublisher::Init(FString InHostIp, uint32 InPort, UObject* InOwner)
{
  Seq = 0;
  Handler = MakeShareable<FROSBridgeHandler>(new FROSBridgeHandler(InHostIp, InPort));
  Handler->Connect();
  SetMessageType();
  SetOwner(InOwner);
  CreatePublisher();
  Handler->AddPublisher(Publisher);
}

void URPublisher::Init(UObject* InOwner, TSharedPtr<FROSBridgeHandler> InHandler, FString InRosTopic)
{
  Seq = 0;
  Handler = InHandler;
  if(!InRosTopic.Equals(""))
    {
      Topic = InRosTopic;
    }
  SetMessageType();
  SetOwner(InOwner);
  CreatePublisher();
  Handler->AddPublisher(Publisher);
}

void URPublisher::CreatePublisher()
{
  Publisher = MakeShareable<FROSBridgePublisher>
    (new FROSBridgePublisher(Topic, MessageType));

  if(Publisher.IsValid())
    {
      UE_LOG(LogTemp, Log, TEXT("Publisher connected to RosBridge"));
    }
}

void URPublisher::SetPublishParameters(URPublisherParameter *&PublisherParameters)
{
  if (PublisherParameters)
  {
    Topic = PublisherParameters->Topic;
    ControllerName = PublisherParameters->ControllerName;
  }
}
