#pragma once

#include "ROSBridgeSrv.h"

namespace u_robo_sim_communication
{
	class SimulationCommands : public FROSBridgeSrv
	{

   public:
	   SimulationCommands()
	   {
		   SrvType = TEXT("ROSCommunication/SimulationCommands");
	   }

	   class Request : public SrvRequest
		{
	   private:
		   FString Command;
	   public:
		   Request(){};
		   Request(const FString& InCommand) : Command(InCommand){};

		   FString GetCommand()
		   {
				return Command;
		   }

		   void SetCommand(const FString& InCommand)
		   {
			   Command = InCommand;
		   }

			virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
			{
				Command = JsonObject->GetStringField("command");
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
				Object->SetStringField(TEXT("command"), Command);
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
				Object->SetBoolField("success", bSuccess);
				Object->SetStringField("status_message", StatusMessage);
				return Object;
			}
		};

	};
} // namespace rospy_tutorials " }")
