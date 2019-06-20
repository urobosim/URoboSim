#pragma once

#include "ROSBridgeMsg.h"

#include "std_msgs/Header.h"
#include "actionlib_msgs/GoalID.h"
#include "pr2_controllers_msgs/PR2GripperCommandGoal.h"

namespace pr2_controllers_msgs
{
	class PR2GripperCommandActionGoal : public FROSBridgeMsg
	{
		std_msgs::Header Header;
		actionlib_msgs::GoalID GoalId;
		pr2_controllers_msgs::PR2GripperCommandGoal Goal;
	public:
		PR2GripperCommandActionGoal()
		{
			MsgType = "pr2_controllers_msgs/PR2GripperCommandActionGoal";
		}

		PR2GripperCommandActionGoal
		(
			std_msgs::Header InHeader,
			actionlib_msgs::GoalID InGoalId,
			pr2_controllers_msgs::PR2GripperCommandGoal InGoal
		):
			Header(InHeader),
			GoalId(InGoalId),
			Goal(InGoal)
		{
			MsgType = "pr2_controllers_msgs/PR2GripperCommandActionGoal";
		}

		~PR2GripperCommandActionGoal() override {}

		std_msgs::Header GetHeader() const
		{
			return Header;
		}

		actionlib_msgs::GoalID GetGoalId() const
		{
			return GoalId;
		}

		pr2_controllers_msgs::PR2GripperCommandGoal GetGoal() const
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

		void SetGoal(pr2_controllers_msgs::PR2GripperCommandGoal InGoal)
		{
			Goal = InGoal;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Header = std_msgs::Header::GetFromJson(JsonObject->GetObjectField(TEXT("header")));

			GoalId = actionlib_msgs::GoalID::GetFromJson(JsonObject->GetObjectField(TEXT("goal_id")));

			Goal = pr2_controllers_msgs::PR2GripperCommandGoal::GetFromJson(JsonObject->GetObjectField(TEXT("goal")));

		}

		static PR2GripperCommandActionGoal GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			PR2GripperCommandActionGoal Result;
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
