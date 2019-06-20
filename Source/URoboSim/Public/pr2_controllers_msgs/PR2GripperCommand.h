#pragma once

#include "ROSBridgeMsg.h"


namespace pr2_controllers_msgs
{
	class PR2GripperCommand : public FROSBridgeMsg
	{
		double Position;
		double MaxEffort;
	public:
		PR2GripperCommand()
		{
			MsgType = "pr2_controllers_msgs/PR2GripperCommand";
		}

		PR2GripperCommand
		(
			double InPosition,
			double InMaxEffort
		):
			Position(InPosition),
			MaxEffort(InMaxEffort)
		{
			MsgType = "pr2_controllers_msgs/PR2GripperCommand";
		}

		~PR2GripperCommand() override {}

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

		static PR2GripperCommand GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			PR2GripperCommand Result;
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
