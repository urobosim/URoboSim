#include "ROSCommunication/Service/Server/GetGraspedObjectServer.h"
#include "Controller/RController.h"
#include "EngineUtils.h"
#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "srv/GetGraspedObject.h"
#include "Tags.h"

DEFINE_LOG_CATEGORY_STATIC(LogRGetGraspedObjectServer, Log, All)

URGetGraspedObjectServer::URGetGraspedObjectServer()
{
  Type = TEXT("urobosim_msgs/get_grasped_object");
}

void URGetGraspedObjectServer::CreateServiceServer()
{
  ServiceServer = MakeShareable<FRGetGraspedObjectServerCallback>(new FRGetGraspedObjectServerCallback(Name, Type, GetWorld(), GetOwner()));
}

FRGetGraspedObjectServerCallback::FRGetGraspedObjectServerCallback(FString Name, FString Type, UWorld *InWorld, UObject *InOwner) : FROSBridgeSrvServer(Name, Type)
{
  World = InWorld;
  Owner = Cast<ARModel>(InOwner);
}

TSharedPtr<FROSBridgeSrv::SrvRequest> FRGetGraspedObjectServerCallback::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
  TSharedPtr<urobosim_msgs::GetGraspedObjectSrv::Request> Request =
      MakeShareable(new urobosim_msgs::GetGraspedObjectSrv::Request());
  Request->FromJson(JsonObject);
  return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FRGetGraspedObjectServerCallback::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
  TSharedPtr<urobosim_msgs::GetGraspedObjectSrv::Request> GetGraspedObjectRequest =
      StaticCastSharedPtr<urobosim_msgs::GetGraspedObjectSrv::Request>(Request);

  FString Gripper = GetGraspedObjectRequest->GetGripper();
  FString ObjectId = FString(TEXT("failure"));
  if (Owner)
  {
    URGripperControllerBase *GripperController = Cast<URGripperControllerBase>(Owner->GetController(Gripper));
      if (!World)
      {
        UE_LOG(LogRGetGraspedObjectServer, Error, TEXT("World not found"));
      }
      else
      {
        // Execute on game thread
        FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]() {
        if (GripperController)
        {
			AStaticMeshActor* FixatedObject = GripperController->GraspComponent->FixatedObject;
            UPrimitiveComponent* FixatedComponent = GripperController->GraspComponent->FixatedComponent ;
			if(FixatedObject)
				{	
                    if(FixatedComponent == FixatedObject->GetRootComponent())
                        {

                            FString SemlogId = FTags::GetValue(FixatedObject->Tags, TEXT("SemLog"), TEXT("Id"));
                            ObjectId = SemlogId;
                        }
                    else
                        {
                            FString SemlogId = FTags::GetValue(FixatedComponent->ComponentTags, TEXT("SemLog"), TEXT("Id"));
                            ObjectId = SemlogId;
                        }
                }
        }
        },
                                                                             TStatId(), nullptr, ENamedThreads::GameThread);

        //wait code above to complete
        FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
      }

  }
  return MakeShareable<FROSBridgeSrv::SrvResponse>(new urobosim_msgs::GetGraspedObjectSrv::Response(ObjectId));
}
