#pragma once

#include "ROSBridgeMsg.h"


namespace control_msgs
{
	class FollowJointTrajectoryResult : public FROSBridgeMsg
	{
		int32 ErrorCode;
	public:
		FollowJointTrajectoryResult()
		{
			MsgType = "control_msgs/FollowJointTrajectoryResult";
		}

		FollowJointTrajectoryResult
		(
			int32 InErrorCode
		):
			ErrorCode(InErrorCode)
		{
			MsgType = "control_msgs/FollowJointTrajectoryResult";
		}

		~FollowJointTrajectoryResult() override {}

		int32 GetErrorCode() const
		{
			return ErrorCode;
		}

		void SetErrorCode(int32 InErrorCode)
		{
			ErrorCode = InErrorCode;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			ErrorCode = JsonObject->GetNumberField(TEXT("error_code"));

		}

		static FollowJointTrajectoryResult GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			FollowJointTrajectoryResult Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

			Object->SetNumberField(TEXT("error_code"), ErrorCode);
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