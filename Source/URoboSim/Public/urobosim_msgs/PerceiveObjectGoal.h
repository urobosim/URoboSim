#pragma once

#include "ROSBridgeMsg.h"


namespace urobosim_msgs
{
	class PerceiveObjectGoal : public FROSBridgeMsg
	{
		FString Type;
	public:
		PerceiveObjectGoal()
		{
			MsgType = "urobosim_msgs/PerceiveObjectGoal";
		}

		PerceiveObjectGoal
		(
			FString InType
		):
			Type(InType)
		{
			MsgType = "urobosim_msgs/PerceiveObjectGoal";
		}

		~PerceiveObjectGoal() override {}

		FString GetType() const
		{
			return Type;
		}

		void SetType(FString InType)
		{
			Type = InType;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Type = JsonObject->GetStringField(TEXT("type"));

		}

		static PerceiveObjectGoal GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			PerceiveObjectGoal Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

			Object->SetStringField(TEXT("type"), Type);
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