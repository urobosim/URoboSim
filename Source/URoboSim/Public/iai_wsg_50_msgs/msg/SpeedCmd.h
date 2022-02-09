#pragma once

#include "ROSBridgeMsg.h"


namespace iai_wsg_50_msgs
{
	class SpeedCmd : public FROSBridgeMsg
	{
		float Speed;
		float Force;
	public:
		SpeedCmd()
		{
			MsgType = "iai_wsg_50_msgs/SpeedCmd";
		}

		SpeedCmd
		(
			float InSpeed,
			float InForce
		):
			Speed(InSpeed),
			Force(InForce)
		{
			MsgType = "iai_wsg_50_msgs/SpeedCmd";
		}

		~SpeedCmd() override {}

		float GetSpeed() const
		{
			return Speed;
		}

		float GetForce() const
		{
			return Force;
		}

		void SetSpeed(float InSpeed)
		{
			Speed = InSpeed;
		}

		void SetForce(float InForce)
		{
			Force = InForce;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Speed = JsonObject->GetNumberField(TEXT("speed"));

			Force = JsonObject->GetNumberField(TEXT("force"));

		}

		static SpeedCmd GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			SpeedCmd Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

			Object->SetNumberField(TEXT("speed"), Speed);
			Object->SetNumberField(TEXT("force"), Force);
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