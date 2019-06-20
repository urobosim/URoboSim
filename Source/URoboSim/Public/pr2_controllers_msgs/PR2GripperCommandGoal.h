#pragma once

#include "ROSBridgeMsg.h"

#include "pr2_controllers_msgs/PR2GripperCommand.h"

namespace pr2_controllers_msgs
{
	class PR2GripperCommandGoal : public FROSBridgeMsg
	{
		pr2_controllers_msgs::PR2GripperCommand Command;
	public:
		PR2GripperCommandGoal()
		{
			MsgType = "pr2_controllers_msgs/PR2GripperCommandGoal";
		}

		PR2GripperCommandGoal
		(
			pr2_controllers_msgs::PR2GripperCommand InCommand
		):
			Command(InCommand)
		{
			MsgType = "pr2_controllers_msgs/PR2GripperCommandGoal";
		}

		~PR2GripperCommandGoal() override {}

		pr2_controllers_msgs::PR2GripperCommand GetCommand() const
		{
			return Command;
		}

		void SetCommand(pr2_controllers_msgs::PR2GripperCommand InCommand)
		{
			Command = InCommand;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Command = pr2_controllers_msgs::PR2GripperCommand::GetFromJson(JsonObject->GetObjectField(TEXT("command")));

		}

		static PR2GripperCommandGoal GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			PR2GripperCommandGoal Result;
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
