#pragma once

#include "ROSBridgeSrv.h"

namespace rosapi
{
	class GetParam : public FROSBridgeSrv
	{

   public:
	   GetParam()
	   {
		   SrvType = TEXT("rosapi/get_param");
	   }

	   class Request : public SrvRequest
		{
	   private:
		   FString Name;
		   FString Default;
	   public:
		   Request(){};
		   Request(const FString& InName, const FString& InDefault) : Name(InName), Default(InDefault){};

		   FString GetName()
		   {
				return Name;
		   }

		   void SetName(const FString& InName)
		   {
			   Name = InName;
		   }

		   FString GetDefault()
		   {
				return Default;
		   }

		   void SetDefault(const FString& InDefault)
		   {
			   Default = InDefault;
		   }

			virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
			{

				Name = JsonObject->GetStringField("name");
				Default = JsonObject->GetStringField("default");
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

				Object->SetStringField(TEXT("name"), Name);
				Object->SetStringField(TEXT("default"), Default);
				UE_LOG(LogTemp, Error, TEXT("send request param server"));

				return Object;
			}
		};

	   class Response : public SrvResponse
		{
	   private:
			FString Value;

	   public:
			Response() {
			}
			Response(FString InValue) : Value(InValue) {}

			void SetValue(FString S)
			{
				Value = S;
			}

			FString GetValue()
			{
				return Value;
			}

			virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
			{
				Value = JsonObject->GetStringField("value");
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
				Object->SetStringField("value", Value);
				UE_LOG(LogTemp, Error, TEXT("recieve response param server"));
				return Object;
			}
		};

	};
} // namespace rospy_tutorials " }")
