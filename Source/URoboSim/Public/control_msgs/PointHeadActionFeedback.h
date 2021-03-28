#pragma once

#include "ROSBridgeMsg.h"

#include "std_msgs/Header.h"
#include "actionlib_msgs/GoalStatus.h"
#include "control_msgs/PointHeadFeedback.h"

namespace control_msgs
{
	class PointHeadActionFeedback : public FROSBridgeMsg
	{
		std_msgs::Header Header;
		actionlib_msgs::GoalStatus Status;
		control_msgs::PointHeadFeedback Feedback;
	public:
		PointHeadActionFeedback()
		{
			MsgType = "control_msgs/PointHeadActionFeedback";
		}

		PointHeadActionFeedback
		(
			std_msgs::Header InHeader,
			actionlib_msgs::GoalStatus InStatus,
			control_msgs::PointHeadFeedback InFeedback
		):
			Header(InHeader),
			Status(InStatus),
			Feedback(InFeedback)
		{
			MsgType = "control_msgs/PointHeadActionFeedback";
		}

		~PointHeadActionFeedback() override {}

		std_msgs::Header GetHeader() const
		{
			return Header;
		}

		actionlib_msgs::GoalStatus GetStatus() const
		{
			return Status;
		}

		control_msgs::PointHeadFeedback GetFeedback() const
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

		void SetFeedback(control_msgs::PointHeadFeedback InFeedback)
		{
			Feedback = InFeedback;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Header = std_msgs::Header::GetFromJson(JsonObject->GetObjectField(TEXT("header")));

			Status = actionlib_msgs::GoalStatus::GetFromJson(JsonObject->GetObjectField(TEXT("status")));

			Feedback = control_msgs::PointHeadFeedback::GetFromJson(JsonObject->GetObjectField(TEXT("feedback")));

		}

		static PointHeadActionFeedback GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			PointHeadActionFeedback Result;
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