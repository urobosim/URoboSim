#pragma once

#include "ROSBridgeSrv.h"

namespace urobosim_msgs
{
	class CheckVisibilitySrv : public FROSBridgeSrv
	{

   public:
	   CheckVisibilitySrv()
	   {
		   SrvType = TEXT("urobosim_msgs/check_visibility");
	   }

	   class Request : public SrvRequest
		{
	   private:
                   FString ObjectName;
                   // geometry_msgs::Point LookAtPosition;
                   // geometry_msgs::Pose BasePose;
                   // TArray<trajectory_msgs::JointTrajectoryPoint> RobotPoses;
           public:
		   Request(){};
                  // Request(const FString& InObjectName, const geometry_msgs::Point& InLookAtPosition,
                  //         const geometry_msgs::Point& InBasePose, TArray<trajectory_msgs::JointTrajectoryPoint> InRobotPoses) : ObjectName(InObjectName),
                  //                                                                                                               LookAtPosition(InLookAtPosition),
                  //                                                                                                               BasePose(InBasePose),
                  //                                                                                                               RobotPoses(InRobotPoses){};
                  Request(const FString& InObjectName) : ObjectName(InObjectName){};

		   FString GetObjectName()
		   {
                     return ObjectName;
		   }

		   void SetObjectName(const FString& InName)
		   {
			   ObjectName= InName;
		   }

		  //  geometry_msgs::Pose GetBasePose()
		  //  {
		  //          return BasePose;
		  //  }

		  //  void SetBasePose(const geometry_msgs::Pose& InBasePose)
		  //  {
		  //          BasePose = InBasePose;
		  //  }

		  //  geometry_msgs::Point GetLookAtPosition()
		  //  {
		  //          return BasePose;
		  //  }

		  //  void SetLookAtPosition(const geometry_msgs::Point& InLookAtPosition)
		  //  {
		  //          LookAtPosition = InLookAtPosition;
		  //  }

                  // TArray<trajectory_msgs::JointTrajectoryPoint> GetRobotPoses() const
                  // {
                  //   return RobotPoses;
                  // }


                  virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
                  {

                    ObjectName = JsonObject->GetStringField("object_name");
                    // BasePose = geometry_msgs::Pose::GetFromJson(JsonObject->GetObjectField(TEXT("base_pose")));
                    // LookAtPosition = geometry_msgs::Point::GetFromJson(JsonObject->GetObjectField(TEXT("look_at_position")));

                    // TArray<TSharedPtr<FJsonValue>> ValuesPtrArr;
                    // RobotPoses.Empty();
                    // ValuesPtrArr = JsonObject->GetArrayField(TEXT("robot_poses"));
                    // for (auto &ptr : ValuesPtrArr)
                    //   RobotPoses.Add(trajectory_msgs::JointTrajectoryPoint::GetFromJson(ptr->AsObject()));
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

                    Object->SetStringField(TEXT("object_name"), ObjectName);
                    // Object->SetObjectField(TEXT("base_pose"), BasePose.ToJsonObject());
                    // Object->SetObjectField(TEXT("look_at_position"), LookAtPosition.ToJsonObject());

                    // TArray<TSharedPtr<FJsonValue>> PointsArray;
                    // for (auto &val : RobotPoses)
                    //   PointsArray.Add(MakeShareable(new FJsonValueObject(val.ToJsonObject())));
                    // Object->SetArrayField(TEXT("robot_poses"), PointsArray);

                    return Object;
                  }
           };

          class Response : public SrvResponse
          {
          private:
            bool bVisible;

          public:
            Response() {
            }
            Response(bool InVisible) : bVisible(InVisible) {}

            void SetVisible(bool S)
            {
              bVisible = S;
            }

            bool GetVisible()
            {
              return bVisible;
            }

            virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
            {
              bVisible = JsonObject->GetBoolField("visible");
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
              Object->SetBoolField("visible", bVisible);
              return Object;
            }
          };

	};
} // namespace rospy_tutorials " }")
