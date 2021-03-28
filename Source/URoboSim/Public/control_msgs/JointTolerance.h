#pragma once

#include "ROSBridgeMsg.h"


namespace control_msgs
{
	class JointTolerance : public FROSBridgeMsg
	{
		FString Name;
		double Position;
		double Velocity;
		double Acceleration;
	public:
		JointTolerance()
		{
			MsgType = "control_msgs/JointTolerance";
		}

		JointTolerance
		(
			FString InName,
			double InPosition,
			double InVelocity,
			double InAcceleration
		):
			Name(InName),
			Position(InPosition),
			Velocity(InVelocity),
			Acceleration(InAcceleration)
		{
			MsgType = "control_msgs/JointTolerance";
		}

		~JointTolerance() override {}

		FString GetName() const
		{
			return Name;
		}

		double GetPosition() const
		{
			return Position;
		}

		double GetVelocity() const
		{
			return Velocity;
		}

		double GetAcceleration() const
		{
			return Acceleration;
		}

		void SetName(FString InName)
		{
			Name = InName;
		}

		void SetPosition(double InPosition)
		{
			Position = InPosition;
		}

		void SetVelocity(double InVelocity)
		{
			Velocity = InVelocity;
		}

		void SetAcceleration(double InAcceleration)
		{
			Acceleration = InAcceleration;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Name = JsonObject->GetStringField(TEXT("name"));

			Position = JsonObject->GetNumberField(TEXT("position"));

			Velocity = JsonObject->GetNumberField(TEXT("velocity"));

			Acceleration = JsonObject->GetNumberField(TEXT("acceleration"));

		}

		static JointTolerance GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			JointTolerance Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

			Object->SetStringField(TEXT("name"), Name);
			Object->SetNumberField(TEXT("position"), Position);
			Object->SetNumberField(TEXT("velocity"), Velocity);
			Object->SetNumberField(TEXT("acceleration"), Acceleration);
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