#pragma once

#include "ROSBridgeMsg.h"


namespace iai_wsg_50_msgs
{
	class PositionCmd : public FROSBridgeMsg
	{
		float Pos;
		float Speed;
		float Force;
	public:
		PositionCmd()
		{
			MsgType = "iai_wsg_50_msgs/PositionCmd";
		}

		PositionCmd
		(
			float InPos,
			float InSpeed,
			float InForce
		):
			Pos(InPos),
			Speed(InSpeed),
			Force(InForce)
		{
			MsgType = "iai_wsg_50_msgs/PositionCmd";
		}

		~PositionCmd() override {}

		float GetPos() const
		{
			return Pos;
		}

		float GetSpeed() const
		{
			return Speed;
		}

		float GetForce() const
		{
			return Force;
		}

		void SetPos(float InPos)
		{
			Pos = InPos;
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
			Pos = JsonObject->GetNumberField(TEXT("pos"));

			Speed = JsonObject->GetNumberField(TEXT("speed"));

			Force = JsonObject->GetNumberField(TEXT("force"));

		}

		static PositionCmd GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			PositionCmd Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

			Object->SetNumberField(TEXT("pos"), Pos);
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