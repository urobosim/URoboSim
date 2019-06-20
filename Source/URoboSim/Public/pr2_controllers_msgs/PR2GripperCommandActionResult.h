#pragma once

#include "ROSBridgeMsg.h"

#include "std_msgs/Header.h"
#include "actionlib_msgs/GoalStatus.h"
#include "pr2_controllers_msgs/PR2GripperCommandResult.h"

namespace pr2_controllers_msgs
{
	class PR2GripperCommandActionResult : public FROSBridgeMsg
	{
		std_msgs::Header Header;
		actionlib_msgs::GoalStatus Status;
		pr2_controllers_msgs::PR2GripperCommandResult Result;
	public:
		PR2GripperCommandActionResult()
		{
			MsgType = "pr2_controllers_msgs/PR2GripperCommandActionResult";
		}

		PR2GripperCommandActionResult
		(
			std_msgs::Header InHeader,
			actionlib_msgs::GoalStatus InStatus,
			pr2_controllers_msgs::PR2GripperCommandResult InResult
		):
			Header(InHeader),
			Status(InStatus),
			Result(InResult)
		{
			MsgType = "pr2_controllers_msgs/PR2GripperCommandActionResult";
		}

		~PR2GripperCommandActionResult() override {}

		std_msgs::Header GetHeader() const
		{
			return Header;
		}

		actionlib_msgs::GoalStatus GetStatus() const
		{
			return Status;
		}

		pr2_controllers_msgs::PR2GripperCommandResult GetResult() const
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

		void SetResult(pr2_controllers_msgs::PR2GripperCommandResult InResult)
		{
			Result = InResult;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Header = std_msgs::Header::GetFromJson(JsonObject->GetObjectField(TEXT("header")));

			Status = actionlib_msgs::GoalStatus::GetFromJson(JsonObject->GetObjectField(TEXT("status")));

			Result = pr2_controllers_msgs::PR2GripperCommandResult::GetFromJson(JsonObject->GetObjectField(TEXT("result")));

		}

		static PR2GripperCommandActionResult GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			PR2GripperCommandActionResult Result;
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
