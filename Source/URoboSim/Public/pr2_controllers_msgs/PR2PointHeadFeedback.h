#pragma once

#include "ROSBridgeMsg.h"


namespace pr2_controllers_msgs
{
	class PR2PointHeadFeedback : public FROSBridgeMsg
	{
		double PointingAngleError;
	public:
		PR2PointHeadFeedback()
		{
			MsgType = "pr2_controllers_msgs/PR2PointHeadFeedback";
		}

		PR2PointHeadFeedback
		(
			double InPointingAngleError
		):
			PointingAngleError(InPointingAngleError)
		{
			MsgType = "pr2_controllers_msgs/PR2PointHeadFeedback";
		}

		~PR2PointHeadFeedback() override {}

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

		static PR2PointHeadFeedback GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			PR2PointHeadFeedback Result;
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
