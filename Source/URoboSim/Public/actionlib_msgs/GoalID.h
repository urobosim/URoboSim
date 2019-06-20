#pragma once

#include "ROSBridgeMsg.h"


namespace actionlib_msgs
{
	class GoalID : public FROSBridgeMsg
	{
		FROSTime Stamp;
		FString Id;
	public:
		GoalID()
		{
			MsgType = "actionlib_msgs/GoalID";
		}

		GoalID
		(
			FROSTime InStamp,
			FString InId
		):
			Stamp(InStamp),
			Id(InId)
		{
			MsgType = "actionlib_msgs/GoalID";
		}

		~GoalID() override {}

		FROSTime GetStamp() const
		{
			return Stamp;
		}

		FString GetId() const
		{
			return Id;
		}

		void SetStamp(FROSTime InStamp)
		{
			Stamp = InStamp;
		}

		void SetId(FString InId)
		{
			Id = InId;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Stamp = FROSTime::GetFromJson(JsonObject->GetObjectField(TEXT("stamp")));

			Id = JsonObject->GetStringField(TEXT("id"));

		}

		static GoalID GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			GoalID Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

			Object->SetObjectField(TEXT("stamp"), Stamp.ToJsonObject());
			Object->SetStringField(TEXT("id"), Id);
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