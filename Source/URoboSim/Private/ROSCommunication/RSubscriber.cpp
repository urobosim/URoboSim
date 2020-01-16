#include "ROSCommunication/RSubscriber.h"
#include "Controller/RController.h"
#include "geometry_msgs/Twist.h"
#include "Physics/RModel.h"

void URSubscriber::Init(UObject* InModel, TSharedPtr<FROSBridgeHandler> InHandler, FString InRosTopic )
{
	Model = Cast<ARModel>(InModel);
	ControllerComponent = Cast<URControllerComponent>(Model->Plugins["ControllerComponent"]);

	Init(InRosTopic);
	InHandler->AddSubscriber(Subscriber);

}

void URSubscriber::Init(FString RosTopic)
{
	if(!RosTopic.Equals(""))
	{
		Topic = RosTopic;
	}
	SetMessageType();
	CreateSubscriber();
}

void URJointStateSubscriber::SetMessageType()
{
	MessageType = TEXT("sensor_msgs/JointState");
}

void URJointStateSubscriber::CreateSubscriber()
{
	Subscriber = MakeShareable<FROSJointStateSubscriberCallback>(
			new FROSJointStateSubscriberCallback(Topic, TEXT("sensor_msgs/JointState"), Model));
	if(Subscriber.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Subscriber connected to RosBridge"));
	}
}

void URFollowJointTrajectoryActionGoalSubscriber::SetMessageType()
{
	MessageType = TEXT("control_msgs/FollowJointTrajectoryActionGoal");
}

void URFollowJointTrajectoryActionGoalSubscriber::CreateSubscriber()
{
	Subscriber = MakeShareable<FROSWholeBodyControllerCallback>(
			new FROSWholeBodyControllerCallback(Topic, MessageType, ControllerComponent->Controller.ControllerList["JointController"]));
	if(Subscriber.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("GiskardSubscriber connected to RosBridge"));
	}
}

void URActionCancelSubscriber::SetMessageType()
{
	MessageType = TEXT("actionlib_msgs/GoalID");
}

void URActionCancelSubscriber::CreateSubscriber()
{
	Subscriber = MakeShareable<FROSActionCancelCallback>(
			new FROSActionCancelCallback(Topic, MessageType, ControllerComponent->Controller.ControllerList[ControllerName]));
	if(Subscriber.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("GiskardSubscriber connected to RosBridge"));
	}
}

void URVelocityCommandSubscriber::SetMessageType()
{
  MessageType = TEXT("geometry_msgs/Twist");
  Topic = TEXT("/base_controller/command");
}

void URVelocityCommandSubscriber::CreateSubscriber()
{
	Subscriber = MakeShareable<FROSVelocityCommandSubscriberCallback>(
			new FROSVelocityCommandSubscriberCallback(Topic, MessageType, ControllerComponent->Controller.ControllerList["BaseController"]));
	if(Subscriber.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Subscriber connected to RosBridge"));
	}
}

void URPR2HeadActionGoalSubscriber::SetMessageType()
{
	MessageType = TEXT("pr2_controllers_msgs/PointHeadActionGoal");
}

void URPR2HeadActionGoalSubscriber::CreateSubscriber()
{
	Subscriber = MakeShareable<FROSPointHeadGoalSubscriberCallback>(
			new FROSPointHeadGoalSubscriberCallback(Topic, MessageType, ControllerComponent->Controller.ControllerList["HeadController"]));
	if(Subscriber.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Subscriber connected to RosBridge"));
	}
}

void URPR2GripperCommandActionGoalSubscriber::SetMessageType()
{
	MessageType = TEXT("pr2_controllers_msgs/Pr2GripperCommandActionGoal");
}

void URPR2GripperCommandActionGoalSubscriber::CreateSubscriber()
{
    UE_LOG(LogTemp, Warning, TEXT("%s"), *ControllerName);

	Subscriber = MakeShareable<FROSGripperCommandSubscriberCallback>(
			new FROSGripperCommandSubscriberCallback(Topic, MessageType, ControllerComponent->Controller.ControllerList[ControllerName]));
	if(Subscriber.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Subscriber connected to RosBridge"));
	}
}

void URPerceiveObjectActionGoalSubscriber::SetMessageType()
{
	MessageType = TEXT("urobosim_msgs/PerceiveObjectActionGoal");
}

void URPerceiveObjectActionGoalSubscriber::CreateSubscriber()
{
	Subscriber = MakeShareable<FROSPerceiveObjectGoalSubscriberCallback>(
			new FROSPerceiveObjectGoalSubscriberCallback(Topic, MessageType, ControllerComponent->Controller.ControllerList["CameraController"]));
	if(Subscriber.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Subscriber connected to RosBridge"));
	}
}

void URJointStateReplaySubscriber::SetMessageType()
{
	MessageType = TEXT("sensor_msgs/JointState");
}

void URJointStateReplaySubscriber::CreateSubscriber()
{
	Subscriber = MakeShareable<FROSJointStateReplaySubscriberCallback>(
			new FROSJointStateReplaySubscriberCallback(Topic, MessageType,  ControllerComponent->Controller.ControllerList[ControllerName]));
}

void UROdometrieReplaySubscriber::SetMessageType()
{
	MessageType = TEXT("tf2_msgs/TFMessage");
}

void UROdometrieReplaySubscriber::CreateSubscriber()
{
	Subscriber = MakeShareable<FROSOdomReplaySubscriberCallback>(
			new FROSOdomReplaySubscriberCallback(Topic, MessageType,  ControllerComponent->Controller.ControllerList[ControllerName]));
}

void URTFSubscriber::SetMessageType()
{
	MessageType = TEXT("tf2_msgs/TFMessage");
}

void URTFSubscriber::CreateSubscriber()
{
  Subscriber = MakeShareable<FROSTFReplaySubscriberCallback>(
                                                             new FROSTFReplaySubscriberCallback(Topic, MessageType,  ControllerComponent->Controller.ControllerList[ControllerName]));
}
