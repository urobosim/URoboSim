#pragma once

#include "ROSBridgeMsg.h"

#include "std_msgs/Header.h"
#include "actionlib_msgs/GoalStatus.h"
#include "urobosim_msgs/PerceiveObjectFeedback.h"

namespace urobosim_msgs
{
	class PerceiveObjectActionFeedback : public FROSBridgeMsg
	{
		std_msgs::Header Header;
		actionlib_msgs::GoalStatus Status;
		urobosim_msgs::PerceiveObjectFeedback Feedback;
	public:
		PerceiveObjectActionFeedback()
		{
			MsgType = "urobosim_msgs/PerceiveObjectActionFeedback";
		}

		PerceiveObjectActionFeedback
		(
			std_msgs::Header InHeader,
			actionlib_msgs::GoalStatus InStatus,
			urobosim_msgs::PerceiveObjectFeedback InFeedback
		):
			Header(InHeader),
			Status(InStatus),
			Feedback(InFeedback)
		{
			MsgType = "urobosim_msgs/PerceiveObjectActionFeedback";
		}

		~PerceiveObjectActionFeedback() override {}

		std_msgs::Header GetHeader() const
		{
			return Header;
		}

		actionlib_msgs::GoalStatus GetStatus() const
		{
			return Status;
		}

		urobosim_msgs::PerceiveObjectFeedback GetFeedback() const
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

		void SetFeedback(urobosim_msgs::PerceiveObjectFeedback InFeedback)
		{
			Feedback = InFeedback;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Header = std_msgs::Header::GetFromJson(JsonObject->GetObjectField(TEXT("header")));

			Status = actionlib_msgs::GoalStatus::GetFromJson(JsonObject->GetObjectField(TEXT("status")));

			Feedback = urobosim_msgs::PerceiveObjectFeedback::GetFromJson(JsonObject->GetObjectField(TEXT("feedback")));

		}

		static PerceiveObjectActionFeedback GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			PerceiveObjectActionFeedback Result;
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