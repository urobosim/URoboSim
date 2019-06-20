#pragma once

#include "ROSBridgeMsg.h"

#include "geometry_msgs/PoseStamped.h"
#include "geometry_msgs/PoseStamped.h"

namespace urobosim_msgs
{
	class PerceiveObjectResult : public FROSBridgeMsg
	{
		FString Name;
		geometry_msgs::PoseStamped Pose;
		geometry_msgs::PoseStamped PoseWorld;
	public:
		PerceiveObjectResult()
		{
			MsgType = "urobosim_msgs/PerceiveObjectResult";
		}

		PerceiveObjectResult
		(
			FString InName,
			geometry_msgs::PoseStamped InPose,
			geometry_msgs::PoseStamped InPoseWorld
		):
			Name(InName),
			Pose(InPose),
			PoseWorld(InPoseWorld)
		{
			MsgType = "urobosim_msgs/PerceiveObjectResult";
		}

		~PerceiveObjectResult() override {}

		FString GetName() const
		{
			return Name;
		}

		geometry_msgs::PoseStamped GetPose() const
		{
			return Pose;
		}

		geometry_msgs::PoseStamped GetPoseWorld() const
		{
			return PoseWorld;
		}

		void SetName(FString InName)
		{
			Name = InName;
		}

		void SetPose(geometry_msgs::PoseStamped InPose)
		{
			Pose = InPose;
		}

		void SetPoseWorld(geometry_msgs::PoseStamped InPoseWorld)
		{
			PoseWorld = InPoseWorld;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Name = JsonObject->GetStringField(TEXT("name"));

			Pose = geometry_msgs::PoseStamped::GetFromJson(JsonObject->GetObjectField(TEXT("pose")));

			PoseWorld = geometry_msgs::PoseStamped::GetFromJson(JsonObject->GetObjectField(TEXT("pose_world")));

		}

		static PerceiveObjectResult GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			PerceiveObjectResult Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

			Object->SetStringField(TEXT("name"), Name);
			Object->SetObjectField(TEXT("pose"), Pose.ToJsonObject());
			Object->SetObjectField(TEXT("pose_world"), PoseWorld.ToJsonObject());
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