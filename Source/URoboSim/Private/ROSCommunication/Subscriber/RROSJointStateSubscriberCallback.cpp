#include "ROSCommunication/Subscriber/RROSJointStateSubscriberCallback.h"
#include "sensor_msgs/JointState.h"

FROSJointStateSubscriberCallback::FROSJointStateSubscriberCallback(
	const FString& InTopic, const FString& InType, ARModel* InModel) :
	FROSBridgeSubscriber(InTopic, InType)
{
	Model = InModel;
}

FROSJointStateSubscriberCallback::~FROSJointStateSubscriberCallback()
{
}

TSharedPtr<FROSBridgeMsg> FROSJointStateSubscriberCallback::ParseMessage
(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<sensor_msgs::JointState> JointStateMessage =
		MakeShareable<sensor_msgs::JointState>(new sensor_msgs::JointState());

	JointStateMessage->FromJson(JsonObject);

	return StaticCastSharedPtr<FROSBridgeMsg>(JointStateMessage);
}

void FROSJointStateSubscriberCallback::Callback(TSharedPtr<FROSBridgeMsg> Msg)
{
	// Downcast to subclass using StaticCastSharedPtr function
	TSharedPtr<sensor_msgs::JointState> JointStateMessage = StaticCastSharedPtr<sensor_msgs::JointState>(Msg);

	for(int i = 0; i < JointStateMessage->Names.Num();i++)
	{
		if(Model->Joints.Contains(JointStateMessage->Names[i]))
		{
			// Model->Joints[JointStateMessage->Names[i]]->SetJointVelocity(JointStateMessage->Velocities[i]);
			Model->Joints[JointStateMessage->Names[i]]->SetJointEffort(JointStateMessage->Efforts[i]);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("No Joint Named %s in Model"), *JointStateMessage->Names[i]);
		}

		// Model->Joints[JointStateMessage->Names[i]]->SetJointPosition(JointStateMessage->Positions[i]);
	}

	// UE_LOG(LogTemp, Log, TEXT("[] Message received by ! Content: "));
	// Do something with the message
	// UE_LOG(LogTemp, Log, TEXT("[%s] Message received by %s! Content: %s"),
	// 	*FString(__FUNCTION__), *Owner->GetName(), *JointStateMessage->GetData());
}
