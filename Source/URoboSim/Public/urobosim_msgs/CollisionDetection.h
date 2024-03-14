#pragma once

#include "ROSBridgeSrv.h"
#include "geometry_msgs/Vector3.h"


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
			geometry_msgs::Vector3 ImpactPoint;
			geometry_msgs::Vector3 ImpactNormal;
					
		public:
			Request(){ }
			Request(FString InObjectA,
				FString InComponentA,
				FString InObjectB,
				FString InComponentB,
				geometry_msgs::Vector3 InImpactPoint,
				geometry_msgs::Vector3 InImpactNormal)
				:
				ObjectA(InObjectA),
				ComponentA(InComponentA),
				ObjectB(InObjectB),
				ComponentB(InComponentB),
				ImpactPoint(InImpactPoint),
				ImpactNormal(InImpactNormal) { }
			
			
			// Getters 
			FString GetObjectA() const { return ObjectA; }
			FString GetComponentA() const { return ComponentA; }
			FString GetObjectB() const { return ObjectB; }
			FString GetComponentB() const { return ComponentB; }
			geometry_msgs::Vector3 GetImpactPoint() const { return ImpactPoint; }
			geometry_msgs::Vector3 GetImpactNormal() const { return ImpactNormal; }
			
			
			// Setters 
			void SetObjectA(FString InObjectA) { ObjectA = InObjectA; }
			void SetComponentA(FString InComponentA) { ComponentA = InComponentA; }
			void SetObjectB(FString InObjectB) { ObjectB = InObjectB; }
			void SetComponentB(FString InComponentB) { ComponentB = InComponentB; }
			void SetImpactPoint(geometry_msgs::Vector3 InImpactPoint) { ImpactPoint = InImpactPoint; }
			void SetImpactNormal(geometry_msgs::Vector3 InImpactNormal) { ImpactNormal = InImpactNormal; }
			
			
			virtual void FromJson(TSharedPtr<FJsonObject> JsonObject) override
			{
				ObjectA = JsonObject->GetStringField(TEXT("object_a"));

				ComponentA = JsonObject->GetStringField(TEXT("component_a"));

				ObjectB = JsonObject->GetStringField(TEXT("object_b"));

				ComponentB = JsonObject->GetStringField(TEXT("component_b"));

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