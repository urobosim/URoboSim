#pragma once

#include "ROSBridgeMsg.h"

#include "std_msgs/Header.h"
#include "actionlib_msgs/GoalStatus.h"
#include "control_msgs/PointHeadResult.h"

namespace control_msgs
{
	class PointHeadActionResult : public FROSBridgeMsg
	{
		std_msgs::Header Header;
		actionlib_msgs::GoalStatus Status;
		control_msgs::PointHeadResult Result;
	public:
		PointHeadActionResult()
		{
			MsgType = "control_msgs/PointHeadActionResult";
		}

		PointHeadActionResult
		(
			std_msgs::Header InHeader,
			actionlib_msgs::GoalStatus InStatus,
			control_msgs::PointHeadResult InResult
		):
			Header(InHeader),
			Status(InStatus),
			Result(InResult)
		{
			MsgType = "control_msgs/PointHeadActionResult";
		}

		~PointHeadActionResult() override {}

		std_msgs::Header GetHeader() const
		{
			return Header;
		}

		actionlib_msgs::GoalStatus GetStatus() const
		{
			return Status;
		}

		control_msgs::PointHeadResult GetResult() const
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

		void SetResult(control_msgs::PointHeadResult InResult)
		{
			Result = InResult;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Header = std_msgs::Header::GetFromJson(JsonObject->GetObjectField(TEXT("header")));

			Status = actionlib_msgs::GoalStatus::GetFromJson(JsonObject->GetObjectField(TEXT("status")));

			Result = control_msgs::PointHeadResult::GetFromJson(JsonObject->GetObjectField(TEXT("result")));

		}

		static PointHeadActionResult GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			PointHeadActionResult Result;
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