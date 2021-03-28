#pragma once

#include "ROSBridgeMsg.h"

#include "std_msgs/Header.h"
#include "actionlib_msgs/GoalStatus.h"
#include "control_msgs/FollowJointTrajectoryFeedback.h"

namespace control_msgs
{
	class FollowJointTrajectoryActionFeedback : public FROSBridgeMsg
	{
		std_msgs::Header Header;
		actionlib_msgs::GoalStatus Status;
		control_msgs::FollowJointTrajectoryFeedback Feedback;
	public:
		FollowJointTrajectoryActionFeedback()
		{
			MsgType = "control_msgs/FollowJointTrajectoryActionFeedback";
		}

		FollowJointTrajectoryActionFeedback
		(
			std_msgs::Header InHeader,
			actionlib_msgs::GoalStatus InStatus,
			control_msgs::FollowJointTrajectoryFeedback InFeedback
		):
			Header(InHeader),
			Status(InStatus),
			Feedback(InFeedback)
		{
			MsgType = "control_msgs/FollowJointTrajectoryActionFeedback";
		}

		~FollowJointTrajectoryActionFeedback() override {}

		std_msgs::Header GetHeader() const
		{
			return Header;
		}

		actionlib_msgs::GoalStatus GetStatus() const
		{
			return Status;
		}

		control_msgs::FollowJointTrajectoryFeedback GetFeedback() const
		{
			return Feedback;
		}

		void SetHeader(std_msgs::Header InHeader)
		{
			Header = InHeader;
		}

		void SetStatus(actionlib_msgs::GoalStatus InStatus)
		{
			Status = InStatus;
		}

		void SetFeedback(control_msgs::FollowJointTrajectoryFeedback InFeedback)
		{
			Feedback = InFeedback;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Header = std_msgs::Header::GetFromJson(JsonObject->GetObjectField(TEXT("header")));

			Status = actionlib_msgs::GoalStatus::GetFromJson(JsonObject->GetObjectField(TEXT("status")));

			Feedback = control_msgs::FollowJointTrajectoryFeedback::GetFromJson(JsonObject->GetObjectField(TEXT("feedback")));

		}

		static FollowJointTrajectoryActionFeedback GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			FollowJointTrajectoryActionFeedback Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

			Object->SetObjectField(TEXT("header"), Header.ToJsonObject());
			Object->SetObjectField(TEXT("status"), Status.ToJsonObject());
			Object->SetObjectField(TEXT("feedback"), Feedback.ToJsonObject());
			return Object;
		}
		virtual FString ToYamlString() const override
		{
			FString OutputString;
			TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
			FJsonSerializer::Serialize(ToJsonObject().ToSharedRef(), Writer);
			return OutputString;
		}
	};
}