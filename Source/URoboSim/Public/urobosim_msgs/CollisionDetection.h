#pragma once

#include "ROSBridgeSrv.h"
#include "geometry_msgs/Vector3.h"
#include "geometry_msgs/Pose.h"


namespace urobosim_msgs
{
	class CollisionDetection : public FROSBridgeSrv
	{
	public:
		CollisionDetection()
		{
			SrvType = TEXT("urobosim_msgs/CollisionDetection");
		}

		class Request : public SrvRequest
		{
		private:
			FString ObjectA;
			FString ComponentA;
			FString ObjectB;
			FString ComponentB;
			geometry_msgs::Vector3 VelocityA;
			geometry_msgs::Vector3 VelocityB;
			geometry_msgs::Pose PoseA;
			geometry_msgs::Pose PoseB;
			geometry_msgs::Vector3 ImpactPoint;
			geometry_msgs::Vector3 ImpactNormal;
					
		public:
			Request(){ }
			Request(FString InObjectA,
				FString InComponentA,
				FString InObjectB,
				FString InComponentB,
				geometry_msgs::Vector3 InVelocityA,
				geometry_msgs::Vector3 InVelocityB,
				geometry_msgs::Pose InPoseA,
				geometry_msgs::Pose InPoseB,
				geometry_msgs::Vector3 InImpactPoint,
				geometry_msgs::Vector3 InImpactNormal)
				:
				ObjectA(InObjectA),
				ComponentA(InComponentA),
				ObjectB(InObjectB),
				ComponentB(InComponentB),
				VelocityA(InVelocityA),
				VelocityB(InVelocityB),
				PoseA(InPoseA),
				PoseB(InPoseB),
				ImpactPoint(InImpactPoint),
				ImpactNormal(InImpactNormal) { }
			
			
			// Getters 
			FString GetObjectA() const { return ObjectA; }
			FString GetComponentA() const { return ComponentA; }
			FString GetObjectB() const { return ObjectB; }
			FString GetComponentB() const { return ComponentB; }
			geometry_msgs::Vector3 GetVelocityA() const { return VelocityA; }
			geometry_msgs::Vector3 GetVelocityB() const { return VelocityB; }
			geometry_msgs::Pose GetPoseA() const { return PoseA; }
			geometry_msgs::Pose GetPoseB() const { return PoseB; }
			geometry_msgs::Vector3 GetImpactPoint() const { return ImpactPoint; }
			geometry_msgs::Vector3 GetImpactNormal() const { return ImpactNormal; }
			
			
			// Setters 
			void SetObjectA(FString InObjectA) { ObjectA = InObjectA; }
			void SetComponentA(FString InComponentA) { ComponentA = InComponentA; }
			void SetObjectB(FString InObjectB) { ObjectB = InObjectB; }
			void SetComponentB(FString InComponentB) { ComponentB = InComponentB; }
			void SetVelocityA(geometry_msgs::Vector3 InVelocityA) { VelocityA = InVelocityA; }
			void SetVelocityB(geometry_msgs::Vector3 InVelocityB) { VelocityB = InVelocityB; }
			void SetPoseA(geometry_msgs::Pose InPoseA) { PoseA = InPoseA; }
			void SetPoseB(geometry_msgs::Pose InPoseB) { PoseB = InPoseB; }
			void SetImpactPoint(geometry_msgs::Vector3 InImpactPoint) { ImpactPoint = InImpactPoint; }
			void SetImpactNormal(geometry_msgs::Vector3 InImpactNormal) { ImpactNormal = InImpactNormal; }
			
			
			virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
			{
				ObjectA = JsonObject->GetStringField(TEXT("object_a"));

				ComponentA = JsonObject->GetStringField(TEXT("component_a"));

				ObjectB = JsonObject->GetStringField(TEXT("object_b"));

				ComponentB = JsonObject->GetStringField(TEXT("component_b"));

				VelocityA = geometry_msgs::Vector3::GetFromJson(JsonObject->GetObjectField(TEXT("velocity_a")));

				VelocityB = geometry_msgs::Vector3::GetFromJson(JsonObject->GetObjectField(TEXT("velocity_b")));

				PoseA = geometry_msgs::Pose::GetFromJson(JsonObject->GetObjectField(TEXT("pose_a")));

				PoseB = geometry_msgs::Pose::GetFromJson(JsonObject->GetObjectField(TEXT("pose_b")));

				ImpactPoint = geometry_msgs::Vector3::GetFromJson(JsonObject->GetObjectField(TEXT("impact_point")));

				ImpactNormal = geometry_msgs::Vector3::GetFromJson(JsonObject->GetObjectField(TEXT("impact_normal")));

			}
			
			static Request GetFromJson(TSharedPtr<FJsonObject> JsonObject)
			{
				Request Req;
				Req.FromJson(JsonObject);
				return Req;
			}
			
			virtual FString ToString() const override
			{
									
				return TEXT("CollisionDetection::Request { object_a = ") + ObjectA +
					TEXT(", component_a = ") + ComponentA +
					TEXT(", object_b = ") + ObjectB +
					TEXT(", component_b = ") + ComponentB +
					TEXT(", velocity_a = ") + VelocityA.ToString() +
					TEXT(", velocity_b = ") + VelocityB.ToString() +
					TEXT(", pose_a = ") + PoseA.ToString() +
					TEXT(", pose_b = ") + PoseB.ToString() +
					TEXT(", impact_point = ") + ImpactPoint.ToString() +
					TEXT(", impact_normal = ") + ImpactNormal.ToString() +
					TEXT(" } ");

			}			

			virtual TSharedPtr<FJsonObject> ToJsonObject() const
			{
				TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

				Object->SetStringField(TEXT("object_a"), ObjectA);

				Object->SetStringField(TEXT("component_a"), ComponentA);

				Object->SetStringField(TEXT("object_b"), ObjectB);

				Object->SetStringField(TEXT("component_b"), ComponentB);

				Object->SetObjectField(TEXT("velocity_a"), VelocityA.ToJsonObject());

				Object->SetObjectField(TEXT("velocity_b"), VelocityB.ToJsonObject());

				Object->SetObjectField(TEXT("pose_a"), PoseA.ToJsonObject());

				Object->SetObjectField(TEXT("pose_b"), PoseB.ToJsonObject());

				Object->SetObjectField(TEXT("impact_point"), ImpactPoint.ToJsonObject());

				Object->SetObjectField(TEXT("impact_normal"), ImpactNormal.ToJsonObject());

				return Object;

			}
		};
		
		class Response : public SrvResponse
		{
		private:
			FString Success;
			
			
		public:
			Response(){ }
			Response(FString InSuccess)
				:
				Success(InSuccess) { }
			
			
			// Getters 
			FString GetSuccess() const { return Success; }
			
			
			// Setters 
			void SetSuccess(FString InSuccess) { Success = InSuccess; }
			
			
			virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
			{
				Success = JsonObject->GetStringField(TEXT("success"));

			}
			
			static Response GetFromJson(TSharedPtr<FJsonObject> JsonObject)
			{
				Response Resp; 
				Resp.FromJson(JsonObject);
				return Resp;
			}			
			
			virtual FString ToString() const override
			{
									
				return TEXT("CollisionDetection::Response { success = ") + Success +
					TEXT(" } ");

			}

			virtual TSharedPtr<FJsonObject> ToJsonObject() const
			{
				TSharedPtr<FJsonObject> Object = MakeShareable<FJsonObject>(new FJsonObject());

				Object->SetStringField(TEXT("success"), Success);

				return Object;

			}
		};
		
	};
	
}