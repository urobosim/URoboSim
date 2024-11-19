#pragma once

#include "ROSBridgeSrv.h"

namespace urobosim_msgs
{
	class GetGraspedObjectSrv : public FROSBridgeSrv
	{

   public:
	   GetGraspedObjectSrv()
	   {
		   SrvType = TEXT("urobosim_msgs/get_grasped_object");
	   }

	   class Request : public SrvRequest
		{
	   private:
                   FString Gripper;
           public:
		   Request(){};
           Request(const FString& InGripper) : Gripper(InGripper){};

		   FString GetGripper()
		   {
                     return Gripper;
		   }

		   void SetGripper(const FString& InName)
		   {
			   Gripper= InName;
		   }
                  virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
                  {

                    Gripper = JsonObject->GetStringField("gripper");
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
                    Object->SetStringField(TEXT("gripper"), Gripper);
                    return Object;
                  }
           };

          class Response : public SrvResponse
          {
          private:
            FString ObjectId;

          public:
            Response() {
            }
            Response(const FString& InObjectId) : ObjectId(InObjectId){};

            void SetObjectI(FString S)
            {
              ObjectId = S;
            }

            FString GetObjectId()
            {
              return ObjectId;
            }

            virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
            {
              ObjectId = JsonObject->GetStringField("object_id");
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
                Object->SetStringField(TEXT("object_id"), ObjectId);
                return Object;
            }
          };

	};
} // namespace rospy_tutorials " }")
