#pragma once

#include "ROSBridgeSrv.h"

namespace unreal_controller_manager
{
	class RegisterRobot : public FROSBridgeSrv
	{

   public:
	   RegisterRobot()
	   {
		   SrvType = TEXT("unreal_simulator_demo/RegisterRobot");
	   }

	   class Request : public SrvRequest
		{
	   private:
		   FString RobotName;
		   TArray<FString> JointNames;
	   public:
		   Request(){};
		   Request(const FString& InRobotName, const TArray<FString>& InJointNames) : RobotName(InRobotName), JointNames(InJointNames){};

		   FString GetRobotName()
		   {
				return RobotName;
		   }

		   void SetRobotName(const FString& InRobotName)
		   {
			   RobotName = InRobotName;
		   }

		   TArray<FString> GetJointNames()
		   {
				return JointNames;
		   }

		   void SetJointNames(const TArray<FString>& InJointNames)
		   {
			   JointNames = InJointNames;
		   }

			virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
			{
				TArray<TSharedPtr<FJsonValue>> ValuesPtrArr;
				JointNames.Empty();
				ValuesPtrArr = JsonObject->GetArrayField(TEXT("joint_names"));
				for (auto &ptr : ValuesPtrArr)
					JointNames.Add(ptr->AsString());

				RobotName = JsonObject->GetStringField("robot_name");
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
				TArray<TSharedPtr<FJsonValue>> JointNameArray;
				for (auto &str : JointNames)
					JointNameArray.Add(MakeShareable(new FJsonValueString(str))); // TODO check if FJsonValueString should be used

				Object->SetStringField(TEXT("robot_name"), RobotName);
				Object->SetArrayField(TEXT("joint_names"), JointNameArray);
				return Object;
			}
		};

	   class Response : public SrvResponse
		{
	   private:
			bool bSuccess;
			FString StatusMessage;

	   public:
			Response() {
				bSuccess = false;
			}
			Response(bool InSuccess) : bSuccess(InSuccess) {}

			void SetSuccess(bool S)
			{
				bSuccess = S;
			}

			bool GetSuccess()
			{
				return bSuccess;
			}

			void SetStatusMessage(FString S)
			{
				StatusMessage = S;
			}

			FString GetStatusMessage()
			{
				return StatusMessage;
			}

			virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
			{
				bSuccess = JsonObject->GetBoolField("success");
				StatusMessage = JsonObject->GetStringField("status_message");
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
				Object->SetBoolField("sucess", bSuccess);
				Object->SetStringField("status_message", StatusMessage);
				return Object;
			}
		};

	};
} // namespace rospy_tutorials " }")
