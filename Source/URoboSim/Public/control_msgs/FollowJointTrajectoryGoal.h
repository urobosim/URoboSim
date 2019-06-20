#pragma once

#include "ROSBridgeMsg.h"

#include "trajectory_msgs/JointTrajectory.h"
#include "control_msgs/JointTolerance.h"
#include "control_msgs/JointTolerance.h"

namespace control_msgs
{
	class FollowJointTrajectoryGoal : public FROSBridgeMsg
	{
		trajectory_msgs::JointTrajectory Trajectory;
		TArray<control_msgs::JointTolerance> PathTolerance;
		TArray<control_msgs::JointTolerance> GoalTolerance;
		FROSTime GoalTimeTolerance;
	public:
		FollowJointTrajectoryGoal()
		{
			MsgType = "control_msgs/FollowJointTrajectoryGoal";
		}

		FollowJointTrajectoryGoal
		(
			trajectory_msgs::JointTrajectory InTrajectory,
			const TArray<control_msgs::JointTolerance>& InPathTolerance,
			const TArray<control_msgs::JointTolerance>& InGoalTolerance,
			FROSTime InGoalTimeTolerance
		):
			Trajectory(InTrajectory),
			PathTolerance(InPathTolerance),
			GoalTolerance(InGoalTolerance),
			GoalTimeTolerance(InGoalTimeTolerance)
		{
			MsgType = "control_msgs/FollowJointTrajectoryGoal";
		}

		~FollowJointTrajectoryGoal() override {}

		trajectory_msgs::JointTrajectory GetTrajectory() const
		{
			return Trajectory;
		}

		TArray<control_msgs::JointTolerance> GetPathTolerance() const
		{
			return PathTolerance;
		}

		TArray<control_msgs::JointTolerance> GetGoalTolerance() const
		{
			return GoalTolerance;
		}

		FROSTime GetGoalTimeTolerance() const
		{
			return GoalTimeTolerance;
		}

		void SetTrajectory(trajectory_msgs::JointTrajectory InTrajectory)
		{
			Trajectory = InTrajectory;
		}

		void SetPathTolerance(TArray<control_msgs::JointTolerance>& InPathTolerance)
		{
			PathTolerance = InPathTolerance;
		}

		void SetGoalTolerance(TArray<control_msgs::JointTolerance>& InGoalTolerance)
		{
			GoalTolerance = InGoalTolerance;
		}

		void SetGoalTimeTolerance(FROSTime InGoalTimeTolerance)
		{
			GoalTimeTolerance = InGoalTimeTolerance;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Trajectory = trajectory_msgs::JointTrajectory::GetFromJson(JsonObject->GetObjectField(TEXT("trajectory")));

			TArray<TSharedPtr<FJsonValue>> ValuesPtrArr;

			PathTolerance.Empty();
			ValuesPtrArr = JsonObject->GetArrayField(TEXT("path_tolerance"));
			for (auto &ptr : ValuesPtrArr)
				PathTolerance.Add(control_msgs::JointTolerance::GetFromJson(ptr->AsObject()));

			GoalTolerance.Empty();
			ValuesPtrArr = JsonObject->GetArrayField(TEXT("goal_tolerance"));
			for (auto &ptr : ValuesPtrArr)
				GoalTolerance.Add(control_msgs::JointTolerance::GetFromJson(ptr->AsObject()));

			GoalTimeTolerance = FROSTime::GetFromJson(JsonObject->GetObjectField(TEXT("goal_time_tolerance")));

		}

		static FollowJointTrajectoryGoal GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			FollowJointTrajectoryGoal Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

			Object->SetObjectField(TEXT("trajectory"), Trajectory.ToJsonObject());
			TArray<TSharedPtr<FJsonValue>> PathToleranceArray;
			for (auto &val : PathTolerance)
				PathToleranceArray.Add(MakeShareable(new FJsonValueObject(val.ToJsonObject())));
			Object->SetArrayField(TEXT("path_tolerance"), PathToleranceArray);
			TArray<TSharedPtr<FJsonValue>> GoalToleranceArray;
			for (auto &val : GoalTolerance)
				GoalToleranceArray.Add(MakeShareable(new FJsonValueObject(val.ToJsonObject())));
			Object->SetArrayField(TEXT("goal_tolerance"), GoalToleranceArray);
			Object->SetObjectField(TEXT("goal_time_tolerance"), GoalTimeTolerance.ToJsonObject());
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