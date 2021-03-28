#pragma once

#include "ROSBridgeMsg.h"


namespace control_msgs
{
	class PointHeadFeedback : public FROSBridgeMsg
	{
		double PointingAngleError;
	public:
		PointHeadFeedback()
		{
			MsgType = "control_msgs/PointHeadFeedback";
		}

		PointHeadFeedback
		(
			double InPointingAngleError
		):
			PointingAngleError(InPointingAngleError)
		{
			MsgType = "control_msgs/PointHeadFeedback";
		}

		~PointHeadFeedback() override {}

		double GetPointingAngleError() const
		{
			return PointingAngleError;
		}

		void SetPointingAngleError(double InPointingAngleError)
		{
			PointingAngleError = InPointingAngleError;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			PointingAngleError = JsonObject->GetNumberField(TEXT("pointing_angle_error"));

		}

		static PointHeadFeedback GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			PointHeadFeedback Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

			Object->SetNumberField(TEXT("pointing_angle_error"), PointingAngleError);
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