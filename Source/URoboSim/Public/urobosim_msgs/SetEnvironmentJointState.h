#pragma once

#include "ROSBridgeSrv.h"


namespace urobosim_msgs
{
	class SetEnvironmentJointState : public FROSBridgeSrv
	{
	public:
		SetEnvironmentJointState()
		{
			SrvType = TEXT("urobosim_msgs/SetEnvironmentJointState");
		}

		class Request : public SrvRequest
		{
		private:
			TArray<FString> JointNames;
			TArray<float> JointStates;
					
		public:
			Request(){ }
			Request(TArray<FString> InJointNames,
				TArray<float> InJointStates)
				:
				JointNames(InJointNames),
				JointStates(InJointStates) { }
			
			
			// Getters 
			TArray<FString> GetJointNames() const { return JointNames; }
			TArray<float> GetJointStates() const { return JointStates; }
			
			
			// Setters 
			void SetJointNames(TArray<FString> InJointNames) { JointNames = InJointNames; }
			void SetJointStates(TArray<float> InJointStates) { JointStates = InJointStates; }
			
			
			virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
			{
				TArray<TSharedPtr<FJsonValue>> ValuesPtrArr;

				JointNames.Empty();
				ValuesPtrArr = JsonObject->GetArrayField(TEXT("joint_names"));
				for (auto &ptr : ValuesPtrArr)
					JointNames.Add(ptr->AsString());

				JointStates.Empty();
				ValuesPtrArr = JsonObject->GetArrayField(TEXT("joint_states"));
				for (auto &ptr : ValuesPtrArr)
					JointStates.Add(ptr->AsNumber());

			}
			
			static Request GetFromJson(TSharedPtr<FJsonObject> JsonObject)
			{
				Request Req;
				Req.FromJson(JsonObject);
				return Req;
			}
			
			virtual FString ToString() const override
			{
									
				FString JointNamesString = "[ ";
				for (auto &value : JointNames)
					JointNamesString += value + TEXT(", ");
				JointNamesString += " ] ";
				FString JointStatesString = "[ ";
				for (auto &value : JointStates)
					JointStatesString += FString::SanitizeFloat(value) + TEXT(", ");
				JointStatesString += " ] ";
				return TEXT("SetEnvironmentJointState::Request { joint_names =") + JointNamesString +
					TEXT(", joint_states =") + JointStatesString +
					TEXT(" } ");

			}			

			virtual TSharedPtr<FJsonObject> ToJsonObject() const
			{
				TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

				TArray<TSharedPtr<FJsonValue>> JointNamesArray;
				for (auto &val : JointNames)
					JointNamesArray.Add(MakeShareable(new FJsonValueString(val)));
				Object->SetArrayField(TEXT("joint_names"), JointNamesArray);

				TArray<TSharedPtr<FJsonValue>> JointStatesArray;
				for (auto &val : JointStates)
					JointStatesArray.Add(MakeShareable(new FJsonValueNumber(val)));
				Object->SetArrayField(TEXT("joint_states"), JointStatesArray);

				return Object;

			}
		};
		
		class Response : public SrvResponse
		{
		private:
			bool Success;
			
			
		public:
			Response(){ }
			Response(bool InSuccess)
				:
				Success(InSuccess) { }
			
			
			// Getters 
			bool GetSuccess() const { return Success; }
			
			
			// Setters 
			void SetSuccess(bool InSuccess) { Success = InSuccess; }
			
			
			virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
			{
				Success = JsonObject->GetBoolField(TEXT("success"));

			}
			
			static Response GetFromJson(TSharedPtr<FJsonObject> JsonObject)
			{
				Response Resp; 
				Resp.FromJson(JsonObject);
				return Resp;
			}			
			
			virtual FString ToString() const override
			{
									
				return TEXT("SetEnvironmentJointState::Response { success = ") + FString::FromInt(Success) +
					TEXT(" } ");

			}

			virtual TSharedPtr<FJsonObject> ToJsonObject() const
			{
				TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

				Object->SetBoolField(TEXT("success"), Success);

				return Object;

			}
		};
		
	};
	
}