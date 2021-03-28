#pragma once

#include "ROSBridgeMsg.h"

#include "std_msgs/Header.h"
#include "trajectory_msgs/JointTrajectoryPoint.h"
#include "trajectory_msgs/JointTrajectoryPoint.h"
#include "trajectory_msgs/JointTrajectoryPoint.h"

namespace control_msgs
{
	class FollowJointTrajectoryFeedback : public FROSBridgeMsg
	{
		std_msgs::Header Header;
		TArray<FString> JointNames;
		trajectory_msgs::JointTrajectoryPoint Desired;
		trajectory_msgs::JointTrajectoryPoint Actual;
		trajectory_msgs::JointTrajectoryPoint Error;
	public:
		FollowJointTrajectoryFeedback()
		{
			MsgType = "control_msgs/FollowJointTrajectoryFeedback";
		}

		FollowJointTrajectoryFeedback
		(
			std_msgs::Header InHeader,
			const TArray<FString>& InJointNames,
			trajectory_msgs::JointTrajectoryPoint InDesired,
			trajectory_msgs::JointTrajectoryPoint InActual,
			trajectory_msgs::JointTrajectoryPoint InError
		):
			Header(InHeader),
			JointNames(InJointNames),
			Desired(InDesired),
			Actual(InActual),
			Error(InError)
		{
			MsgType = "control_msgs/FollowJointTrajectoryFeedback";
		}

		~FollowJointTrajectoryFeedback() override {}

		std_msgs::Header GetHeader() const
		{
			return Header;
		}

		TArray<FString> GetJointNames() const
		{
			return JointNames;
		}

		trajectory_msgs::JointTrajectoryPoint GetDesired() const
		{
			return Desired;
		}

		trajectory_msgs::JointTrajectoryPoint GetActual() const
		{
			return Actual;
		}

		trajectory_msgs::JointTrajectoryPoint GetError() const
		{
			return Error;
		}

		void SetHeader(std_msgs::Header InHeader)
		{
			Header = InHeader;
		}

		void SetJointNames(TArray<FString>& InJointNames)
		{
			JointNames = InJointNames;
		}

		void SetDesired(trajectory_msgs::JointTrajectoryPoint InDesired)
		{
			Desired = InDesired;
		}

		void SetActual(trajectory_msgs::JointTrajectoryPoint InActual)
		{
			Actual = InActual;
		}

		void SetError(trajectory_msgs::JointTrajectoryPoint InError)
		{
			Error = InError;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Header = std_msgs::Header::GetFromJson(JsonObject->GetObjectField(TEXT("header")));

			TArray<TSharedPtr<FJsonValue>> ValuesPtrArr;

			JointNames.Empty();
			ValuesPtrArr = JsonObject->GetArrayField(TEXT("joint_names"));
			for (auto &ptr : ValuesPtrArr)
				JointNames.Add(ptr->AsString());

			Desired = trajectory_msgs::JointTrajectoryPoint::GetFromJson(JsonObject->GetObjectField(TEXT("desired")));

			Actual = trajectory_msgs::JointTrajectoryPoint::GetFromJson(JsonObject->GetObjectField(TEXT("actual")));

			Error = trajectory_msgs::JointTrajectoryPoint::GetFromJson(JsonObject->GetObjectField(TEXT("error")));

		}

		static FollowJointTrajectoryFeedback GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			FollowJointTrajectoryFeedback Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

			Object->SetObjectField(TEXT("header"), Header.ToJsonObject());
			TArray<TSharedPtr<FJsonValue>> JointNamesArray;
			for (auto &val : JointNames)
				JointNamesArray.Add(MakeShareable(new FJsonValueString(val)));
			Object->SetArrayField(TEXT("joint_names"), JointNamesArray);
			Object->SetObjectField(TEXT("desired"), Desired.ToJsonObject());
			Object->SetObjectField(TEXT("actual"), Actual.ToJsonObject());
			Object->SetObjectField(TEXT("error"), Error.ToJsonObject());
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