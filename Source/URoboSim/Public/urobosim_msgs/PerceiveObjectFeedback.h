#pragma once

#include "ROSBridgeMsg.h"


namespace urobosim_msgs
{
	class PerceiveObjectFeedback : public FROSBridgeMsg
	{
	public:
		PerceiveObjectFeedback()
		{
			MsgType = "urobosim_msgs/PerceiveObjectFeedback";
		}

		~PerceiveObjectFeedback() override {}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
		}

		static PerceiveObjectFeedback GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			PerceiveObjectFeedback Result;
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
