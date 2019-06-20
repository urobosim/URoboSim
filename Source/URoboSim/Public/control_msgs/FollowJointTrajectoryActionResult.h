#pragma once

#include "ROSBridgeMsg.h"

#include "std_msgs/Header.h"
#include "actionlib_msgs/GoalStatus.h"
#include "control_msgs/FollowJointTrajectoryResult.h"

namespace control_msgs
{
	class FollowJointTrajectoryActionResult : public FROSBridgeMsg
	{
		std_msgs::Header Header;
		actionlib_msgs::GoalStatus Status;
		control_msgs::FollowJointTrajectoryResult Result;
	public:
		FollowJointTrajectoryActionResult()
		{
			MsgType = "control_msgs/FollowJointTrajectoryActionResult";
		}

		FollowJointTrajectoryActionResult
		(
			std_msgs::Header InHeader,
			actionlib_msgs::GoalStatus InStatus,
			control_msgs::FollowJointTrajectoryResult InResult
		):
			Header(InHeader),
			Status(InStatus),
			Result(InResult)
		{
			MsgType = "control_msgs/FollowJointTrajectoryActionResult";
		}

		~FollowJointTrajectoryActionResult() override {}

		std_msgs::Header GetHeader() const
		{
			return Header;
		}

		actionlib_msgs::GoalStatus GetStatus() const
		{
			return Status;
		}

		control_msgs::FollowJointTrajectoryResult GetResult() const
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

		void SetResult(control_msgs::FollowJointTrajectoryResult InResult)
		{
			Result = InResult;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Header = std_msgs::Header::GetFromJson(JsonObject->GetObjectField(TEXT("header")));

			Status = actionlib_msgs::GoalStatus::GetFromJson(JsonObject->GetObjectField(TEXT("status")));

			Result = control_msgs::FollowJointTrajectoryResult::GetFromJson(JsonObject->GetObjectField(TEXT("result")));

		}

		static FollowJointTrajectoryActionResult GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			FollowJointTrajectoryActionResult Result;
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