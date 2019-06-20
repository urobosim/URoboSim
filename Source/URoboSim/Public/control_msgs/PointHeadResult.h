#pragma once

#include "ROSBridgeMsg.h"


namespace control_msgs
{
	class PointHeadResult : public FROSBridgeMsg
	{
	public:
		PointHeadResult()
		{
			MsgType = "control_msgs/PointHeadResult";
		}

		~PointHeadResult() override {}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
		}

		static PointHeadResult GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			PointHeadResult Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

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
