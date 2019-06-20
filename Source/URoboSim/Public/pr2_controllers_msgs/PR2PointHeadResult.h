#pragma once

#include "ROSBridgeMsg.h"


namespace pr2_controllers_msgs
{
	class PR2PointHeadResult : public FROSBridgeMsg
	{
	public:
		PR2PointHeadResult()
		{
			MsgType = "pr2_controllers_msgs/PR2PointHeadResult";
		}

		~PR2PointHeadResult() override {}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
		}

		static PR2PointHeadResult GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			PR2PointHeadResult Result;
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
