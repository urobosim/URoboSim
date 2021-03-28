#pragma once

#include "ROSBridgeMsg.h"

#include "control_msgs/GripperCommand.h"

namespace control_msgs
{
	class GripperCommandGoal : public FROSBridgeMsg
	{
		control_msgs::GripperCommand Command;
	public:
		GripperCommandGoal()
		{
			MsgType = "control_msgs/GripperCommandGoal";
		}

		GripperCommandGoal
		(
			control_msgs::GripperCommand InCommand
		):
			Command(InCommand)
		{
			MsgType = "control_msgs/GripperCommandGoal";
		}

		~GripperCommandGoal() override {}

		control_msgs::GripperCommand GetCommand() const
		{
			return Command;
		}

		void SetCommand(control_msgs::GripperCommand InCommand)
		{
			Command = InCommand;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Command = control_msgs::GripperCommand::GetFromJson(JsonObject->GetObjectField(TEXT("command")));

		}

		static GripperCommandGoal GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			GripperCommandGoal Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

			Object->SetObjectField(TEXT("command"), Command.ToJsonObject());
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