#pragma once

#include "ROSBridgeSrv.h"


namespace urobosim_msgs
{
	class CheckVisibility : public FROSBridgeSrv
	{
	public:
		CheckVisibility()
		{
			SrvType = TEXT("urobosim_msgs/CheckVisibility");
		}

		class Request : public SrvRequest
		{
		private:
			FString ObjectName;
					
		public:
			Request(){ }
			Request(FString InObjectName)
				:
				ObjectName(InObjectName) { }
			
			
			// Getters 
			FString GetObjectName() const { return ObjectName; }
			
			
			// Setters 
			void SetObjectName(FString InObjectName) { ObjectName = InObjectName; }
			
			
			virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
			{
				ObjectName = JsonObject->GetStringField(TEXT("object_name"));

			}
			
			static Request GetFromJson(TSharedPtr<FJsonObject> JsonObject)
			{
				Request Req;
				Req.FromJson(JsonObject);
				return Req;
			}
			
			virtual FString ToString() const override
			{
									
				return TEXT("CheckVisibility::Request { object_name = ") + ObjectName +
					TEXT(" } ");

			}			

			virtual TSharedPtr<FJsonObject> ToJsonObject() const
			{
				TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

				Object->SetStringField(TEXT("object_name"), ObjectName);

				return Object;

			}
		};
		
		class Response : public SrvResponse
		{
		private:
			bool Visible;
			
			
		public:
			Response(){ }
			Response(bool InVisible)
				:
				Visible(InVisible) { }
			
			
			// Getters 
			bool GetVisible() const { return Visible; }
			
			
			// Setters 
			void SetVisible(bool InVisible) { Visible = InVisible; }
			
			
			virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
			{
				Visible = JsonObject->GetBoolField(TEXT("visible"));

			}
			
			static Response GetFromJson(TSharedPtr<FJsonObject> JsonObject)
			{
				Response Resp; 
				Resp.FromJson(JsonObject);
				return Resp;
			}			
			
			virtual FString ToString() const override
			{
									
				return TEXT("CheckVisibility::Response { visible = ") + FString::FromInt(Visible) +
					TEXT(" } ");

			}

			virtual TSharedPtr<FJsonObject> ToJsonObject() const
			{
				TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

				Object->SetBoolField(TEXT("visible"), Visible);

				return Object;

			}
		};
		
	};
	
}