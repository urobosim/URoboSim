#pragma once
#include "ROSBridgeSrv.h"
// #include "Pose.h"

class UROBOSIM_API FROSMoveObjectSrv : public FROSBridgeSrv
{
protected:
	FString Type;

public:
	FROSMoveObjectSrv(FString InType)
	{
		Type = InType;
	}

	class Request : public SrvRequest
	{
	private:
          FString Name;
          geometry_msgs::Pose Pose;


	public:
		Request() {}

		Request(FString InName, geometry_msgs::Pose InPose)
		{
			Name = InName;
			Pose = InPose;
		}

		FString GetName()
		{
			return Name;
		}

		geometry_msgs::Pose GetPose() {
			return Pose;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Name = JsonObject->GetStringField("name");
			Pose.FromJson(JsonObject->GetObjectField("pose"));
		}

		static Request GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Request Req;
			Req.FromJson(JsonObject);
			return Req;
		}

		FString ToString() const override
		{
                        return "FROSSetMaterialSrv:Request {name = " + Name + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetStringField(TEXT("name"), Name);
			Object->SetObjectField(TEXT("pose"), Pose.ToJsonObject());
			return Object;
		}

	};

	class Response : public SrvResponse
	{
	private:
		bool Success;

	public:
		Response() {}

		Response(bool InSuccess)
		{
			Success = InSuccess;
		}

		bool GetSuccess()
		{
			return Success;
		}

		virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
		{
			Success = JsonObject->GetBoolField("success");
		}

		static Response GetFromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			Response Res;
			Res.FromJson(JsonObject);
			return Res;
		}

		FString ToString() const override
		{
			return "FROSSetMaterialSrv:Response {success = " + (Success ? FString("True") : FString("False")) + "}";
		}

		virtual TSharedPtr<FJsonObject> ToJsonObject() const override
		{
			TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
			Object->SetBoolField(TEXT("success"), Success);
			return Object;
		}

	};

};
