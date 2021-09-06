#pragma once

#include "ROSBridgeSrv.h"
// #include "std_msgs/Byte.h"

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
                  // std_msgs::Byte mode;
                  FString Id;
                  FString Query;
           public:
                  Request(){};
                  Request(const FString& InId, const FString& InQuery): Id(InId), Query(InQuery){};

                  //  TArray<FString> GetJointNames()
		  //  {
                  //    return JointNames;
		  //  }

		  //  void SetJointNames(const TArray<FString>& InNames)
		  //  {
		  //          JointNames= InNames;
		  //  }

                  //  TArray<float> GetJointStates()
		  //  {
                  //    return JointStates;
		  //  }

		  //  void SetJointStates(const TArray<float>& InStates)
		  //  {
		  //          JointStates = InStates;
		  //  }

                  virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
                  {

                  //   TArray<TSharedPtr<FJsonValue>> ValuesPtrArr;
                  //   JointNames.Empty();
                  //   ValuesPtrArr = JsonObject->GetArrayField(TEXT("joint_names"));
                  //   for (auto &ptr : ValuesPtrArr)
                  //     {
                  //       FString Names = ptr->AsString();
                  //       JointNames.Add(Names);
                  //     }

                  //   ValuesPtrArr = JsonObject->GetArrayField(TEXT("joint_states"));
                  //   JointStates.Empty();
                  //   for (auto &ptr : ValuesPtrArr)
                  //     {
                  //       float States = ptr->AsNumber();
                  //       JointStates.Add(States);
                  //     }
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

                  //   TArray<TSharedPtr<FJsonValue>> JointNamesArray;
                  //   for (auto &Name : JointNames)
                  //     {
                  //       TSharedPtr<FJsonValue> Ptr = MakeShareable(new FJsonValueString(Name));
                  //       JointNamesArray.Add(Ptr);
                  //     }
                  //   Object->SetArrayField("joint_names", JointNamesArray);

                  //   TArray<TSharedPtr<FJsonValue>> JointStatesArray;
                  //   for (auto &States : JointStates)
                  //     {
                  //       TSharedPtr<FJsonValue> Ptr = MakeShareable(new FJsonValueNumber(States));
                  //       JointStatesArray.Add(Ptr);
                  //     }
                  //   Object->SetArrayField("joint_states", JointStatesArray);

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

          //   void SetSuccess(bool S)
          //   {
          //     bSuccess = S;
          //   }

          //   bool GetSuccess()
          //   {
          //     return bSuccess;
          //   }

            virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
            {
          //     bSuccess = JsonObject->GetBoolField("success");
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
          //     Object->SetBoolField("success", bSuccess);
              return Object;
            }
          };

	};
} // namespace rospy_tutorials " }")
