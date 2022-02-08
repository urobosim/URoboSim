#pragma once

#include "ROSBridgeMsg.h"


namespace iai_wsg_50_msgs
{
	class Cmd : public FROSBridgeMsg
	{
		FString Mode;
		float Pos;
		float Speed;
	public:
		Cmd()
		{
			MsgType = "iai_wsg_50_msgs/Cmd";
		}

		Cmd
		(
			FString InMode,
			float InPos,
			float InSpeed
		):
			Mode(InMode),
			Pos(InPos),
			Speed(InSpeed)
		{
			MsgType = "iai_wsg_50_msgs/Cmd";
		}

		~Cmd() override {}

		FString GetMode() const
		{
			return Mode;
		}

		float GetPos() const
		{
			return Pos;
		}

		float GetSpeed() const
		{
			return Speed;
		}

		void SetMode(FString InMode)
		{
			Mode = InMode;
		}

		void SetPos(float InPos)
		{
			Pos = InPos;
		}

		void SetSpeed(float InSpeed)
		{
			Speed = InSpeed;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Mode = JsonObject->GetStringField(TEXT("mode"));

			Pos = JsonObject->GetNumberField(TEXT("pos"));

			Speed = JsonObject->GetNumberField(TEXT("speed"));

		}

		static Cmd GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Cmd Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

			Object->SetStringField(TEXT("mode"), Mode);
			Object->SetNumberField(TEXT("pos"), Pos);
			Object->SetNumberField(TEXT("speed"), Speed);
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