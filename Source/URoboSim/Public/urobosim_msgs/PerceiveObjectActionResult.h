#pragma once

#include "ROSBridgeMsg.h"

#include "std_msgs/Header.h"
#include "actionlib_msgs/GoalStatus.h"
#include "urobosim_msgs/PerceiveObjectResult.h"

namespace urobosim_msgs
{
	class PerceiveObjectActionResult : public FROSBridgeMsg
	{
		std_msgs::Header Header;
		actionlib_msgs::GoalStatus Status;
		urobosim_msgs::PerceiveObjectResult Result;
	public:
		PerceiveObjectActionResult()
		{
			MsgType = "urobosim_msgs/PerceiveObjectActionResult";
		}

		PerceiveObjectActionResult
		(
			std_msgs::Header InHeader,
			actionlib_msgs::GoalStatus InStatus,
			urobosim_msgs::PerceiveObjectResult InResult
		):
			Header(InHeader),
			Status(InStatus),
			Result(InResult)
		{
			MsgType = "urobosim_msgs/PerceiveObjectActionResult";
		}

		~PerceiveObjectActionResult() override {}

		std_msgs::Header GetHeader() const
		{
			return Header;
		}

		actionlib_msgs::GoalStatus GetStatus() const
		{
			return Status;
		}

		urobosim_msgs::PerceiveObjectResult GetResult() const
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

		void SetResult(urobosim_msgs::PerceiveObjectResult InResult)
		{
			Result = InResult;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Header = std_msgs::Header::GetFromJson(JsonObject->GetObjectField(TEXT("header")));

			Status = actionlib_msgs::GoalStatus::GetFromJson(JsonObject->GetObjectField(TEXT("status")));

			Result = urobosim_msgs::PerceiveObjectResult::GetFromJson(JsonObject->GetObjectField(TEXT("result")));

		}

		static PerceiveObjectActionResult GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			PerceiveObjectActionResult Result;
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