#pragma once

#include "ROSBridgeMsg.h"

#include "std_msgs/Header.h"
#include "actionlib_msgs/GoalID.h"
#include "urobosim_msgs/PerceiveObjectGoal.h"

namespace urobosim_msgs
{
	class PerceiveObjectActionGoal : public FROSBridgeMsg
	{
		std_msgs::Header Header;
		actionlib_msgs::GoalID GoalId;
		urobosim_msgs::PerceiveObjectGoal Goal;
	public:
		PerceiveObjectActionGoal()
		{
			MsgType = "urobosim_msgs/PerceiveObjectActionGoal";
		}

		PerceiveObjectActionGoal
		(
			std_msgs::Header InHeader,
			actionlib_msgs::GoalID InGoalId,
			urobosim_msgs::PerceiveObjectGoal InGoal
		):
			Header(InHeader),
			GoalId(InGoalId),
			Goal(InGoal)
		{
			MsgType = "urobosim_msgs/PerceiveObjectActionGoal";
		}

		~PerceiveObjectActionGoal() override {}

		std_msgs::Header GetHeader() const
		{
			return Header;
		}

		actionlib_msgs::GoalID GetGoalId() const
		{
			return GoalId;
		}

		urobosim_msgs::PerceiveObjectGoal GetGoal() const
		{
			return Goal;
		}

		void SetHeader(std_msgs::Header InHeader)
		{
			Header = InHeader;
		}

		void SetGoalId(actionlib_msgs::GoalID InGoalId)
		{
			GoalId = InGoalId;
		}

		void SetGoal(urobosim_msgs::PerceiveObjectGoal InGoal)
		{
			Goal = InGoal;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Header = std_msgs::Header::GetFromJson(JsonObject->GetObjectField(TEXT("header")));

			GoalId = actionlib_msgs::GoalID::GetFromJson(JsonObject->GetObjectField(TEXT("goal_id")));

			Goal = urobosim_msgs::PerceiveObjectGoal::GetFromJson(JsonObject->GetObjectField(TEXT("goal")));

		}

		static PerceiveObjectActionGoal GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			PerceiveObjectActionGoal Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

			Object->SetObjectField(TEXT("header"), Header.ToJsonObject());
			Object->SetObjectField(TEXT("goal_id"), GoalId.ToJsonObject());
			Object->SetObjectField(TEXT("goal"), Goal.ToJsonObject());
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