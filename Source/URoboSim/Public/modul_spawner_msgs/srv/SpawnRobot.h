// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ROSBridgeSrv.h"

/**
 * 
 */
class UROSBRIDGE_API FROSRobotModelSrv : public FROSBridgeSrv
{
protected:
    FString Type;

public:
    FROSRobotModelSrv(FString InType)
    {
        Type=InType;
    }
    class Request:public SrvRequest
    {
    private:
        FString Name;
        FString Id;

    public:
        Request() {}
        Request(FString InName,FString InId)
        {
            Name=InName;
            Id=InId;
        }

        FString GetId()
        {
            return Id;
        }

        FString GetName()
        {
            return Name;
        }

        virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
        {
            Name=JsonObject->GetStringField("name");
            Id=JsonObject->GetStringField("id");
        }

        static Request GetFromJson(TSharedPtr<FJsonObject> JsonObject)
        {
            Request Req;
            Req.FromJson(JsonObject);
            return Req;
        }

        FString ToString() const override
        {
            return "FROSRobotModelSrv::Request {name = "+Name+"}";
        }

        virtual TSharedPtr<FJsonObject> ToJsonObject() const override
        {
            TSharedPtr<FJsonObject> Object= MakeShareable<FJsonObject>(new FJsonObject());
            Object->SetStringField(TEXT("name"),Name);
            return Object;
        }
    };

    class Response : public SrvResponse
    {
    private:
        FString Id;
        FString Name;
        bool Success;

    public:
        Response(){}

        Response(FString InId, FString InName, bool InSuccess)
        {
            Id=InId;
            Name=InName;
            Success=InSuccess;
        }
        FString GetId()
        {
            return Id;
        }

        FString GetName()
        {
            return Name;
        }

        bool GetSuccess()
        {
            return Success;
        }

        virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
        {
            Id = JsonObject->GetStringField("id");
            Name = JsonObject->GetStringField("Name");
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
            return "FROSSpawnModelSrv:Response {id = " + Id +
                ", name = " + Name + ", success = " + (Success ? FString("True") : FString("False")) + "}";
        }

        virtual TSharedPtr<FJsonObject> ToJsonObject() const override
        {
            TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());
            Object->SetStringField(TEXT("id"), Id);
            Object->SetStringField(TEXT("name"), Name);
            Object->SetBoolField(TEXT("success"), Success);
            return Object;
        }
    };
};