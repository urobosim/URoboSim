#pragma once

#include "ROSBridgeMsg.h"


namespace control_msgs
{
	class GripperCommand : public FROSBridgeMsg
	{
		double Position;
		double MaxEffort;
	public:
		GripperCommand()
		{
			MsgType = "control_msgs/GripperCommand";
		}

		GripperCommand
		(
			double InPosition,
			double InMaxEffort
		):
			Position(InPosition),
			MaxEffort(InMaxEffort)
		{
			MsgType = "control_msgs/GripperCommand";
		}

		~GripperCommand() override {}

		double GetPosition() const
		{
			return Position;
		}

		double GetMaxEffort() const
		{
			return MaxEffort;
		}

		void SetPosition(double InPosition)
		{
			Position = InPosition;
		}

		void SetMaxEffort(double InMaxEffort)
		{
			MaxEffort = InMaxEffort;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Position = JsonObject->GetNumberField(TEXT("position"));

			MaxEffort = JsonObject->GetNumberField(TEXT("max_effort"));

		}

		static GripperCommand GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			GripperCommand Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

			Object->SetNumberField(TEXT("position"), Position);
			Object->SetNumberField(TEXT("max_effort"), MaxEffort);
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