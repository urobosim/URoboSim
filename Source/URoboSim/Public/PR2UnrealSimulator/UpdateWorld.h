#pragma once

#include "ROSBridgeSrv.h"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/Vector3.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Quaternion.h"

namespace unreal_controller_manager
{
	class UpdateWorld : public FROSBridgeSrv
	{

   public:
	   UpdateWorld()
	   {
		   SrvType = TEXT("unreal_simulator_demo/UpdateWorld");
	   }

	   class Request : public SrvRequest
		{
	   private:
		   float SimTime;
		   TArray<FString> JointNames;
		   TArray<float> JointPositions;
		   TArray<float> JointVelocities;
		   geometry_msgs::Pose RobotPose;

	   public:
		   Request(){};
		   Request(const float& InSimTime, const TArray<FString>& InJointNames, const TArray<float>& InJointPositions, const TArray<float>& InJointVelocities, const FTransform& InRobotPose) : SimTime(InSimTime), JointNames(InJointNames), JointPositions(InJointPositions), JointVelocities(InJointVelocities){
			   RobotPose.SetPosition(geometry_msgs::Point(InRobotPose.GetTranslation()));
			   RobotPose.SetOrientation(geometry_msgs::Quaternion(InRobotPose.GetRotation()));
		   };

		   geometry_msgs::Pose GetRobotPose()
		   {
			   return RobotPose;
		   }

		   void SetRobotPose(const geometry_msgs::Pose& InRobotPose)
		   {
			   RobotPose = InRobotPose;
		   }

		   float GetSimTime()
		   {
			   return SimTime;
		   }

		   void SetSimTime(const float& InSimTime)
		   {
			   SimTime = InSimTime;
		   }

		   TArray<FString> GetJointNames()
		   {
				return JointNames;
		   }

		   void SetJointNames(const TArray<FString>& InJointNames)
		   {
			   JointNames = InJointNames;
		   }

		   TArray<float> GetJointVelocities()
		   {
				return JointVelocities;
		   }

		   void SetJointVelocities(const TArray<float>& InJointVelocities)
		   {
			   JointVelocities = InJointVelocities;
		   }

		   TArray<float> GetJointPositions()
		   {
				return JointPositions;
		   }

		   void SetJointPositions(const TArray<float>& InJointPositions)
		   {
			   JointPositions = InJointPositions;
		   }

			virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
			{

				SimTime = JsonObject->GetNumberField("sim_time");
				RobotPose = geometry_msgs::Pose::GetFromJson(JsonObject->GetObjectField(TEXT("robot_pose")));

				TArray<TSharedPtr<FJsonValue>> ValuesPtrArr;
				JointNames.Empty();
				ValuesPtrArr = JsonObject->GetArrayField(TEXT("joint_names"));
				for (auto &ptr : ValuesPtrArr)
					JointNames.Add(ptr->AsString());


				JointVelocities.Empty();
				ValuesPtrArr = JsonObject->GetArrayField(TEXT("joint_velocities"));
				for (auto &ptr : ValuesPtrArr)
					JointVelocities.Add(ptr->AsNumber());

				JointPositions.Empty();
				ValuesPtrArr = JsonObject->GetArrayField(TEXT("joint_positions"));
				for (auto &ptr : ValuesPtrArr)
					JointPositions.Add(ptr->AsNumber());
			}

			static Request GetFromJson(TSharedPtr<FJsonObject> JsonObject)
			{
				Request Req;
				Req.FromJson(JsonObject);
				return Req;
			}

			virtual FString ToString() const override
			{
				return TEXT("not implemented");
			}

			virtual TSharedPtr<FJsonObject> ToJsonObject() const
			{


				TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
				TArray<TSharedPtr<FJsonValue>> JointNameArray, JointPositionArray, JointVelocityArray;
				for (auto &str : JointNames)
					JointNameArray.Add(MakeShareable(new FJsonValueString(str))); // TODO check if FJsonValueString should be used

				for (auto &val : JointPositions)
					JointPositionArray.Add(MakeShareable(new FJsonValueNumber(val))); // TODO check if FJsonValueString should be used

				for (auto &val : JointVelocities)
					JointVelocityArray.Add(MakeShareable(new FJsonValueNumber(val))); // TODO check if FJsonValueString should be used


				Object->SetNumberField(TEXT("sim_time"), SimTime);
				Object->SetObjectField(TEXT("robot_pose"), RobotPose.ToJsonObject());
				Object->SetArrayField(TEXT("joint_names"), JointNameArray);
				Object->SetArrayField(TEXT("joint_positions"), JointPositionArray);
				Object->SetArrayField(TEXT("joint_velocities"), JointVelocityArray);
				return Object;
			}
		};

	   class Response : public SrvResponse
		{
	   private:
		   TArray<FString> JointNames;
		   TArray<float> JointPositions;
		   TArray<float> JointVelocities;
		   TArray<float> JointEfforts;
		   geometry_msgs::Twist BaseControllCommand;
		   TArray<FString> Commands;

	   public:
			Response() {}
			Response(const TArray<FString>& InJointNames, const TArray<float>& InJointPositions, const TArray<float>& InJointVelocities, const FVector& InLinear, const FVector& InAngular, const TArray<FString>& InCommands) : JointNames(InJointNames), JointPositions(InJointPositions), JointVelocities(InJointVelocities), Commands(InCommands)
			{
				BaseControllCommand.SetLinear(geometry_msgs::Vector3(InLinear));
				BaseControllCommand.SetAngular(geometry_msgs::Vector3(InAngular));
			};

			geometry_msgs::Twist GetBaseControllCommand()
		   {
				return BaseControllCommand;
		   }

		   void SetBaseControllCommand(const geometry_msgs::Twist InBaseControllCommand)
		   {
			   BaseControllCommand = InBaseControllCommand;
		   }

		   TArray<FString> GetCommands()
		   {
				return Commands;
		   }

		   void SetCommands(const TArray<FString>& InCommands)
		   {
			   Commands = InCommands;
		   }

		   TArray<FString> GetJointNames()
		   {
				return JointNames;
		   }

		   void SetJointNames(const TArray<FString>& InJointNames)
		   {
			   JointNames = InJointNames;
		   }

		   TArray<float> GetJointVelocities()
		   {
				return JointVelocities;
		   }

		   void SetJointVelocities(const TArray<float>& InJointVelocities)
		   {
			   JointVelocities = InJointVelocities;
		   }

		   TArray<float> GetJointPositions()
		   {
				return JointPositions;
		   }

		   void SetJointPositions(const TArray<float>& InJointPositions)
		   {
			   JointPositions = InJointPositions;
		   }

		   TArray<float> GetJointEfforts()
		   {
				return JointEfforts;
		   }

		   void SetJointEfforts(const TArray<float>& InJointEfforts)
		   {
			   JointEfforts = InJointEfforts;
		   }

			virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
			{
				BaseControllCommand = geometry_msgs::Twist::GetFromJson(JsonObject->GetObjectField(TEXT("base_control_command")));

				TArray<TSharedPtr<FJsonValue>> ValuesPtrArr;
				JointNames.Empty();
				ValuesPtrArr = JsonObject->GetArrayField(TEXT("joint_names"));
				for (auto &ptr : ValuesPtrArr)
					JointNames.Add(ptr->AsString());

				JointVelocities.Empty();
				ValuesPtrArr = JsonObject->GetArrayField(TEXT("joint_velocities"));
				for (auto &ptr : ValuesPtrArr)
					JointVelocities.Add(ptr->AsNumber());

				JointPositions.Empty();
				ValuesPtrArr = JsonObject->GetArrayField(TEXT("joint_positions"));
				for (auto &ptr : ValuesPtrArr)
					JointPositions.Add(ptr->AsNumber());

				JointEfforts.Empty();
				ValuesPtrArr = JsonObject->GetArrayField(TEXT("joint_efforts"));
				for (auto &ptr : ValuesPtrArr)
					JointEfforts.Add(ptr->AsNumber());

				Commands.Empty();
				ValuesPtrArr = JsonObject->GetArrayField(TEXT("commands"));
				for (auto &ptr : ValuesPtrArr)
					Commands.Add(ptr->AsString());
			}

			static Response GetFromJson(TSharedPtr<FJsonObject> JsonObject)
			{
				Response Res;
				Res.FromJson(JsonObject);
				return Res;
			}

			virtual FString ToString() const override
			{
				return TEXT("");
			}

			virtual TSharedPtr<FJsonObject> ToJsonObject() const
			{
				TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
				TArray<TSharedPtr<FJsonValue>> JointNameArray, JointPositionArray, JointVelocityArray, JointEffortArray, CommandsArray;
				for (auto &str : JointNames)
					JointNameArray.Add(MakeShareable(new FJsonValueString(str))); // TODO check if FJsonValueString should be used

				for (auto &val : JointPositions)
					JointPositionArray.Add(MakeShareable(new FJsonValueNumber(val))); // TODO check if FJsonValueString should be used

				for (auto &val : JointVelocities)
					JointVelocityArray.Add(MakeShareable(new FJsonValueNumber(val))); // TODO check if FJsonValueString should be used

				for (auto &val : JointEfforts)
					JointEffortArray.Add(MakeShareable(new FJsonValueNumber(val))); // TODO check if FJsonValueString should be used

				for (auto &str : Commands)
					CommandsArray.Add(MakeShareable(new FJsonValueString(str))); // TODO check if FJsonValueString should be used

				Object->SetArrayField(TEXT("joint_names"), JointNameArray);
				Object->SetArrayField(TEXT("joint_positions"), JointPositionArray);
				Object->SetArrayField(TEXT("joint_velocities"), JointVelocityArray);
				Object->SetArrayField(TEXT("joint_efforts"), JointEffortArray);
				Object->SetObjectField(TEXT("base_control_command"), BaseControllCommand.ToJsonObject());
				Object->SetArrayField(TEXT("commands"), CommandsArray);
				return Object;
			}
		};

	};
} // namespace rospy_tutorials " }")
