#pragma once

#include "ROSBridgeSrv.h"

namespace urobosim_msgs
{
	class GraspObject : public FROSBridgeSrv
	{
	public:
		GraspObject()
		{
			SrvType = TEXT("urobosim_msgs/GraspObject");
		}

		class Request : public SrvRequest
		{
		private:
			geometry_msgs::PoseStamped Pose;

		public:
			Request(){ }
			Request(geometry_msgs::PoseStamped InPose)
				:
				Pose(InPose) { }


			// Getters
			geometry_msgs::PoseStamped GetPose() const { return Pose; }


			// Setters
			void SetPose(geometry_msgs::PoseStamped InPose) { Pose = InPose; }


			virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
			{
				Pose = geometry_msgs::PoseStamped::GetFromJson(JsonObject->GetObjectField(TEXT("pose")));

			}

			static Request GetFromJson(TSharedPtr<FJsonObject> JsonObject)
			{
				Request Req;
				Req.FromJson(JsonObject);
				return Req;
			}

//			### TOSTRING ###

			virtual TSharedPtr<FJsonObject> ToJsonObject() const
			{
				TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

				Object->SetObjectField(TEXT("pose"), Pose.ToJsonObject());

				return Object;

			}
		};

		class Response : public SrvResponse
		{
                private:
                        bool Success;

		public:
                    Response(){ }
                    Response(bool InSuccess) :
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

//			### TOSTRING ###

			virtual TSharedPtr<FJsonObject> ToJsonObject() const
			{
				TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

				Object->SetBoolField(TEXT("success"), Success);

				return Object;

			}
		};

	};

}
