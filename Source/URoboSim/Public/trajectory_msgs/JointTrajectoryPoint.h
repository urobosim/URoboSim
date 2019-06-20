#pragma once

#include "ROSBridgeMsg.h"


namespace trajectory_msgs
{
	class JointTrajectoryPoint : public FROSBridgeMsg
	{
		TArray<double> Positions;
		TArray<double> Velocities;
		TArray<double> Accelerations;
		FROSTime TimeFromStart;
	public:
		JointTrajectoryPoint()
		{
			MsgType = "trajectory_msgs/JointTrajectoryPoint";
		}

		JointTrajectoryPoint
		(
			const TArray<double>& InPositions,
			const TArray<double>& InVelocities,
			const TArray<double>& InAccelerations,
			FROSTime InTimeFromStart
		):
			Positions(InPositions),
			Velocities(InVelocities),
			Accelerations(InAccelerations),
			TimeFromStart(InTimeFromStart)
		{
			MsgType = "trajectory_msgs/JointTrajectoryPoint";
		}

		~JointTrajectoryPoint() override {}

		TArray<double> GetPositions() const
		{
			return Positions;
		}

		TArray<double> GetVelocities() const
		{
			return Velocities;
		}

		TArray<double> GetAccelerations() const
		{
			return Accelerations;
		}

		FROSTime GetTimeFromStart() const
		{
			return TimeFromStart;
		}

		void SetPositions(TArray<double>& InPositions)
		{
			Positions = InPositions;
		}

		void SetVelocities(TArray<double>& InVelocities)
		{
			Velocities = InVelocities;
		}

		void SetAccelerations(TArray<double>& InAccelerations)
		{
			Accelerations = InAccelerations;
		}

		void SetTimeFromStart(FROSTime InTimeFromStart)
		{
			TimeFromStart = InTimeFromStart;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			TArray<TSharedPtr<FJsonValue>> ValuesPtrArr;

			Positions.Empty();
			ValuesPtrArr = JsonObject->GetArrayField(TEXT("positions"));
			for (auto &ptr : ValuesPtrArr)
				Positions.Add(ptr->AsNumber());

			Velocities.Empty();
			ValuesPtrArr = JsonObject->GetArrayField(TEXT("velocities"));
			for (auto &ptr : ValuesPtrArr)
				Velocities.Add(ptr->AsNumber());

			Accelerations.Empty();
			ValuesPtrArr = JsonObject->GetArrayField(TEXT("accelerations"));
			for (auto &ptr : ValuesPtrArr)
				Accelerations.Add(ptr->AsNumber());

			TimeFromStart = FROSTime::GetFromJson(JsonObject->GetObjectField(TEXT("time_from_start")));

		}

		static JointTrajectoryPoint GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			JointTrajectoryPoint Result;
			Result.FromJson(JsonObject);
			return Result;
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

			TArray<TSharedPtr<FJsonValue>> PositionsArray;
			for (auto &val : Positions)
				PositionsArray.Add(MakeShareable(new FJsonValueNumber(val)));
			Object->SetArrayField(TEXT("positions"), PositionsArray);
			TArray<TSharedPtr<FJsonValue>> VelocitiesArray;
			for (auto &val : Velocities)
				VelocitiesArray.Add(MakeShareable(new FJsonValueNumber(val)));
			Object->SetArrayField(TEXT("velocities"), VelocitiesArray);
			TArray<TSharedPtr<FJsonValue>> AccelerationsArray;
			for (auto &val : Accelerations)
				AccelerationsArray.Add(MakeShareable(new FJsonValueNumber(val)));
			Object->SetArrayField(TEXT("accelerations"), AccelerationsArray);
			Object->SetObjectField(TEXT("time_from_start"), TimeFromStart.ToJsonObject());
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