#pragma once

#include "ROSBridgeSrv.h"
#include "std_msgs/Byte.h"

namespace json_prolog_msgs
{
	class PrologQuerySrv : public FROSBridgeSrv
	{

   public:
	   PrologQuerySrv()
	   {
		   SrvType = TEXT("json_prolog_msgs/prolog_query");
	   }

	   class Request : public SrvRequest
		{
	   private:
                  std_msgs::Byte Mode;
                  FString Id;
                  FString Query;
           public:
                  Request(){};
                  Request(const std_msgs::Byte& InMode, const FString& InId, const FString& InQuery): Mode(InMode), Id(InId), Query(InQuery){};

                  std_msgs::Byte GetMode()
                  {
                    return Mode;
                  }

                  void SetMode(const std_msgs::Byte& InMode)
                  {
                    Mode = InMode;
                  }

                   FString GetId()
		   {
                     return Id;
		   }

		   void SetId(const FString& InId)
		   {
		           Id= InId;
		   }

                   FString GetQuery()
		   {
                     return Query;
		   }

		   void SetQuery(const FString& InQuery)
		   {
		           Query= InQuery;
		   }

                  virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
                  {
                    // Mode = std_msgs::Byte::GetFromJson(JsonObject->GetObjectField(TEXT("mode")));
                    Id = JsonObject->GetStringField("id");
                    Query = JsonObject->GetStringField("query");
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

                    // Object->SetObjectField(TEXT("mode"), Mode.ToJsonObject());
                    UE_LOG(LogTemp, Error, TEXT("Query %s"), *Query);
                    Object->SetStringField(TEXT("query"), Query);
                    Object->SetStringField(TEXT("id"), Id);
                    return Object;
                  }
           };

          class Response : public SrvResponse
          {
          private:
            bool bOk;
            FString Message;

          public:
            Response() {
            }
            Response(bool bOutOk, const FString& OutMessage): bOk(bOutOk), Message(OutMessage)  {}

            void SetbOk(bool Ok)
            {
              bOk = Ok;
            }

            bool GetbOk()
            {
              return bOk;
            }

            FString GetMessage()
            {
              return Message;
            }

            void SetMessage(const FString& InMessage)
            {
              Message = InMessage;
            }
            virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
            {
              bOk = JsonObject->GetBoolField("ok");
              Message = JsonObject->GetStringField("message");
              if(bOk)
                {
                  UE_LOG(LogTemp, Log, TEXT("Query ok"));
                }
              else
                {
                  UE_LOG(LogTemp, Error, TEXT("Message: %s"), *Message);
                }
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
              Object->SetBoolField("ok", bOk);
              Object->SetStringField(TEXT("message"), Message);
              return Object;
            }
          };

        };
}
