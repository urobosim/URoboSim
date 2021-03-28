#pragma once

#include "ROSBridgeMsg.h"

#include "actionlib_msgs/GoalID.h"

namespace actionlib_msgs
{
	class GoalStatus : public FROSBridgeMsg
	{
		actionlib_msgs::GoalID GoalId;
		uint8 Status;
		FString Text;
	public:
		GoalStatus()
		{
			MsgType = "actionlib_msgs/GoalStatus";
		}

		GoalStatus
		(
			actionlib_msgs::GoalID InGoalId,
			uint8 InStatus,
			FString InText
		):
			GoalId(InGoalId),
			Status(InStatus),
			Text(InText)
		{
			MsgType = "actionlib_msgs/GoalStatus";
		}

		~GoalStatus() override {}

		actionlib_msgs::GoalID GetGoalId() const
		{
			return GoalId;
		}

		uint8 GetStatus() const
		{
			return Status;
		}

		FString GetText() const
		{
			return Text;
		}

		void SetGoalId(actionlib_msgs::GoalID InGoalId)
		{
			GoalId = InGoalId;
		}

		void SetStatus(uint8 InStatus)
		{
			Status = InStatus;
		}

		void SetText(FString InText)
		{
			Text = InText;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			GoalId = actionlib_msgs::GoalID::GetFromJson(JsonObject->GetObjectField(TEXT("goal_id")));

			Status = JsonObject->GetNumberField(TEXT("status"));

			Text = JsonObject->GetStringField(TEXT("text"));

		}

		static GoalStatus GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			GoalStatus Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

			Object->SetObjectField(TEXT("goal_id"), GoalId.ToJsonObject());
			Object->SetNumberField(TEXT("status"), Status);
			Object->SetStringField(TEXT("text"), Text);
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