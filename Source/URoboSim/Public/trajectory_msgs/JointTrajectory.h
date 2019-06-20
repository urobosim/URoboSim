#pragma once

#include "ROSBridgeMsg.h"

#include "std_msgs/Header.h"
#include "trajectory_msgs/JointTrajectoryPoint.h"

namespace trajectory_msgs
{
	class JointTrajectory : public FROSBridgeMsg
	{
		std_msgs::Header Header;
		TArray<FString> JointNames;
		TArray<trajectory_msgs::JointTrajectoryPoint> Points;
	public:
		JointTrajectory()
		{
			MsgType = "trajectory_msgs/JointTrajectory";
		}

		JointTrajectory
		(
			std_msgs::Header InHeader,
			const TArray<FString>& InJointNames,
			const TArray<trajectory_msgs::JointTrajectoryPoint>& InPoints
		):
			Header(InHeader),
			JointNames(InJointNames),
			Points(InPoints)
		{
			MsgType = "trajectory_msgs/JointTrajectory";
		}

		~JointTrajectory() override {}

		std_msgs::Header GetHeader() const
		{
			return Header;
		}

		TArray<FString> GetJointNames() const
		{
			return JointNames;
		}

		TArray<trajectory_msgs::JointTrajectoryPoint> GetPoints() const
		{
			return Points;
		}

		void SetHeader(std_msgs::Header InHeader)
		{
			Header = InHeader;
		}

		void SetJointNames(TArray<FString>& InJointNames)
		{
			JointNames = InJointNames;
		}

		void SetPoints(TArray<trajectory_msgs::JointTrajectoryPoint>& InPoints)
		{
			Points = InPoints;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Header = std_msgs::Header::GetFromJson(JsonObject->GetObjectField(TEXT("header")));

			TArray<TSharedPtr<FJsonValue>> ValuesPtrArr;

			JointNames.Empty();
			ValuesPtrArr = JsonObject->GetArrayField(TEXT("joint_names"));
			for (auto &ptr : ValuesPtrArr)
				JointNames.Add(ptr->AsString());

			Points.Empty();
			ValuesPtrArr = JsonObject->GetArrayField(TEXT("points"));
			for (auto &ptr : ValuesPtrArr)
				Points.Add(trajectory_msgs::JointTrajectoryPoint::GetFromJson(ptr->AsObject()));

		}

		static JointTrajectory GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			JointTrajectory Result;
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
			TArray<TSharedPtr<FJsonValue>> PointsArray;
			for (auto &val : Points)
				PointsArray.Add(MakeShareable(new FJsonValueObject(val.ToJsonObject())));
			Object->SetArrayField(TEXT("points"), PointsArray);
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