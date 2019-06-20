#pragma once

#include "ROSBridgeMsg.h"

#include "std_msgs/Header.h"
#include "actionlib_msgs/GoalStatus.h"
#include "pr2_controllers_msgs/PR2GripperCommandFeedback.h"

namespace pr2_controllers_msgs
{
	class PR2GripperCommandActionFeedback : public FROSBridgeMsg
	{
		std_msgs::Header Header;
		actionlib_msgs::GoalStatus Status;
		pr2_controllers_msgs::PR2GripperCommandFeedback Feedback;
	public:
		PR2GripperCommandActionFeedback()
		{
			MsgType = "pr2_controllers_msgs/PR2GripperCommandActionFeedback";
		}

		PR2GripperCommandActionFeedback
		(
			std_msgs::Header InHeader,
			actionlib_msgs::GoalStatus InStatus,
			pr2_controllers_msgs::PR2GripperCommandFeedback InFeedback
		):
			Header(InHeader),
			Status(InStatus),
			Feedback(InFeedback)
		{
			MsgType = "pr2_controllers_msgs/PR2GripperCommandActionFeedback";
		}

		~PR2GripperCommandActionFeedback() override {}

		std_msgs::Header GetHeader() const
		{
			return Header;
		}

		actionlib_msgs::GoalStatus GetStatus() const
		{
			return Status;
		}

		pr2_controllers_msgs::PR2GripperCommandFeedback GetFeedback() const
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

		void SetFeedback(pr2_controllers_msgs::PR2GripperCommandFeedback InFeedback)
		{
			Feedback = InFeedback;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Header = std_msgs::Header::GetFromJson(JsonObject->GetObjectField(TEXT("header")));

			Status = actionlib_msgs::GoalStatus::GetFromJson(JsonObject->GetObjectField(TEXT("status")));

			Feedback = pr2_controllers_msgs::PR2GripperCommandFeedback::GetFromJson(JsonObject->GetObjectField(TEXT("feedback")));

		}

		static PR2GripperCommandActionFeedback GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			PR2GripperCommandActionFeedback Result;
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
