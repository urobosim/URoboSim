#pragma once

#include "ROSBridgeMsg.h"

#include "geometry_msgs/PointStamped.h"
#include "geometry_msgs/Vector3.h"

namespace pr2_controllers_msgs
{
	class PR2PointHeadGoal : public FROSBridgeMsg
	{
		geometry_msgs::PointStamped Target;
		geometry_msgs::Vector3 PointingAxis;
		FString PointingFrame;
		FROSTime MinDuration;
		double MaxVelocity;
	public:
		PR2PointHeadGoal()
		{
			MsgType = "pr2_controllers_msgs/PR2PointHeadGoal";
		}

		PR2PointHeadGoal
		(
			geometry_msgs::PointStamped InTarget,
			geometry_msgs::Vector3 InPointingAxis,
			FString InPointingFrame,
			FROSTime InMinDuration,
			double InMaxVelocity
		):
			Target(InTarget),
			PointingAxis(InPointingAxis),
			PointingFrame(InPointingFrame),
			MinDuration(InMinDuration),
			MaxVelocity(InMaxVelocity)
		{
			MsgType = "pr2_controllers_msgs/PR2PointHeadGoal";
		}

		~PR2PointHeadGoal() override {}

		geometry_msgs::PointStamped GetTarget() const
		{
			return Target;
		}

		geometry_msgs::Vector3 GetPointingAxis() const
		{
			return PointingAxis;
		}

		FString GetPointingFrame() const
		{
			return PointingFrame;
		}

		FROSTime GetMinDuration() const
		{
			return MinDuration;
		}

		double GetMaxVelocity() const
		{
			return MaxVelocity;
		}

		void SetTarget(geometry_msgs::PointStamped InTarget)
		{
			Target = InTarget;
		}

		void SetPointingAxis(geometry_msgs::Vector3 InPointingAxis)
		{
			PointingAxis = InPointingAxis;
		}

		void SetPointingFrame(FString InPointingFrame)
		{
			PointingFrame = InPointingFrame;
		}

		void SetMinDuration(FROSTime InMinDuration)
		{
			MinDuration = InMinDuration;
		}

		void SetMaxVelocity(double InMaxVelocity)
		{
			MaxVelocity = InMaxVelocity;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Target = geometry_msgs::PointStamped::GetFromJson(JsonObject->GetObjectField(TEXT("target")));

			PointingAxis = geometry_msgs::Vector3::GetFromJson(JsonObject->GetObjectField(TEXT("pointing_axis")));

			PointingFrame = JsonObject->GetStringField(TEXT("pointing_frame"));

			MinDuration = FROSTime::GetFromJson(JsonObject->GetObjectField(TEXT("min_duration")));

			MaxVelocity = JsonObject->GetNumberField(TEXT("max_velocity"));

		}

		static PR2PointHeadGoal GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			PR2PointHeadGoal Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

			Object->SetObjectField(TEXT("target"), Target.ToJsonObject());
			Object->SetObjectField(TEXT("pointing_axis"), PointingAxis.ToJsonObject());
			Object->SetStringField(TEXT("pointing_frame"), PointingFrame);
			Object->SetObjectField(TEXT("min_duration"), MinDuration.ToJsonObject());
			Object->SetNumberField(TEXT("max_velocity"), MaxVelocity);
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
