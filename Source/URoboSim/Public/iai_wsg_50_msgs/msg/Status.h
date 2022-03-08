#pragma once

#include "ROSBridgeMsg.h"

#include "std_msgs/Header.h"

namespace iai_wsg_50_msgs
{
	class Status : public FROSBridgeMsg
	{
		std_msgs::Header Header;
		FString Status;
		float Width;
		float Speed;
		float Acc;
		float Force;
		float ForceFinger0;
		float ForceFinger1;
	public:
		Status()
		{
			MsgType = "iai_wsg_50_msgs/Status";
		}

		Status
		(
			std_msgs::Header InHeader,
			FString InStatus,
			float InWidth,
			float InSpeed,
			float InAcc,
			float InForce,
			float InForceFinger0,
			float InForceFinger1
		):
			Header(InHeader),
			Status(InStatus),
			Width(InWidth),
			Speed(InSpeed),
			Acc(InAcc),
			Force(InForce),
			ForceFinger0(InForceFinger0),
			ForceFinger1(InForceFinger1)
		{
			MsgType = "iai_wsg_50_msgs/Status";
		}

		~Status() override {}

		std_msgs::Header GetHeader() const
		{
			return Header;
		}

		FString GetStatus() const
		{
			return Status;
		}

		float GetWidth() const
		{
			return Width;
		}

		float GetSpeed() const
		{
			return Speed;
		}

		float GetAcc() const
		{
			return Acc;
		}

		float GetForce() const
		{
			return Force;
		}

		float GetForceFinger0() const
		{
			return ForceFinger0;
		}

		float GetForceFinger1() const
		{
			return ForceFinger1;
		}

		void SetHeader(std_msgs::Header InHeader)
		{
			Header = InHeader;
		}

		void SetStatus(FString InStatus)
		{
			Status = InStatus;
		}

		void SetWidth(float InWidth)
		{
			Width = InWidth;
		}

		void SetSpeed(float InSpeed)
		{
			Speed = InSpeed;
		}

		void SetAcc(float InAcc)
		{
			Acc = InAcc;
		}

		void SetForce(float InForce)
		{
			Force = InForce;
		}

		void SetForceFinger0(float InForceFinger0)
		{
			ForceFinger0 = InForceFinger0;
		}

		void SetForceFinger1(float InForceFinger1)
		{
			ForceFinger1 = InForceFinger1;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Header = std_msgs::Header::GetFromJson(JsonObject->GetObjectField(TEXT("header")));

			Status = JsonObject->GetStringField(TEXT("status"));

			Width = JsonObject->GetNumberField(TEXT("width"));

			Speed = JsonObject->GetNumberField(TEXT("speed"));

			Acc = JsonObject->GetNumberField(TEXT("acc"));

			Force = JsonObject->GetNumberField(TEXT("force"));

			ForceFinger0 = JsonObject->GetNumberField(TEXT("force_finger0"));

			ForceFinger1 = JsonObject->GetNumberField(TEXT("force_finger1"));

		}

		static Status GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Status Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

			Object->SetObjectField(TEXT("header"), Header.ToJsonObject());
			Object->SetStringField(TEXT("status"), Status);
			Object->SetNumberField(TEXT("width"), Width);
			Object->SetNumberField(TEXT("speed"), Speed);
			Object->SetNumberField(TEXT("acc"), Acc);
			Object->SetNumberField(TEXT("force"), Force);
			Object->SetNumberField(TEXT("force_finger0"), ForceFinger0);
			Object->SetNumberField(TEXT("force_finger1"), ForceFinger1);
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