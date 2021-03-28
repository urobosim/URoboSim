#pragma once

#include "ROSBridgeMsg.h"

#include "std_msgs/Header.h"
#include "actionlib_msgs/GoalStatus.h"
#include "control_msgs/GripperCommandResult.h"

namespace control_msgs
{
	class GripperCommandActionResult : public FROSBridgeMsg
	{
		std_msgs::Header Header;
		actionlib_msgs::GoalStatus Status;
		control_msgs::GripperCommandResult Result;
	public:
		GripperCommandActionResult()
		{
			MsgType = "control_msgs/GripperCommandActionResult";
		}

		GripperCommandActionResult
		(
			std_msgs::Header InHeader,
			actionlib_msgs::GoalStatus InStatus,
			control_msgs::GripperCommandResult InResult
		):
			Header(InHeader),
			Status(InStatus),
			Result(InResult)
		{
			MsgType = "control_msgs/GripperCommandActionResult";
		}

		~GripperCommandActionResult() override {}

		std_msgs::Header GetHeader() const
		{
			return Header;
		}

		actionlib_msgs::GoalStatus GetStatus() const
		{
			return Status;
		}

		control_msgs::GripperCommandResult GetResult() const
		{
			return Result;
		}

		void SetHeader(std_msgs::Header InHeader)
		{
			Header = InHeader;
		}

		void SetStatus(actionlib_msgs::GoalStatus InStatus)
		{
			Status = InStatus;
		}

		void SetResult(control_msgs::GripperCommandResult InResult)
		{
			Result = InResult;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Header = std_msgs::Header::GetFromJson(JsonObject->GetObjectField(TEXT("header")));

			Status = actionlib_msgs::GoalStatus::GetFromJson(JsonObject->GetObjectField(TEXT("status")));

			Result = control_msgs::GripperCommandResult::GetFromJson(JsonObject->GetObjectField(TEXT("result")));

		}

		static GripperCommandActionResult GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			GripperCommandActionResult Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

			Object->SetObjectField(TEXT("header"), Header.ToJsonObject());
			Object->SetObjectField(TEXT("status"), Status.ToJsonObject());
			Object->SetObjectField(TEXT("result"), Result.ToJsonObject());
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