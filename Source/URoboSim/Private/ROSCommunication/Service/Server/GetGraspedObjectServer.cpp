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
  Type = TEXT("urobosim_msgs/GetGraspedObject");
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
  FString ObjectId = FString(TEXT("NoObject"));
  TArray<FString> RightGripperController;
  RightGripperController.Add(TEXT("RGripperController"));
  RightGripperController.Add(TEXT("R1GripperController"));
  RightGripperController.Add(TEXT("R2GripperController"));
  RightGripperController.Add(TEXT("R3GripperController"));
  RightGripperController.Add(TEXT("R4GripperController"));
  TArray<FString> LeftGripperController;
  LeftGripperController.Add(TEXT("LGripperController"));
  LeftGripperController.Add(TEXT("L1GripperController"));
  LeftGripperController.Add(TEXT("L2GripperController"));
  LeftGripperController.Add(TEXT("L3GripperController"));
  LeftGripperController.Add(TEXT("L4GripperController"));
  TArray<FString> ActiveControllerList;
  if(Gripper.Equals(TEXT("right")))
    {
      ActiveControllerList = RightGripperController;
    }
  else if(Gripper.Equals(TEXT("left")))
    {
      ActiveControllerList = LeftGripperController;
    }

  if (Owner)
  {

      if (!World)
      {
        UE_LOG(LogRGetGraspedObjectServer, Error, TEXT("World not found"));
      }
      else
      {
        // Execute on game thread
        FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]() {

        for(auto& GC: ActiveControllerList)
          {
            URGripperControllerBase *GripperController = Cast<URGripperControllerBase>(Owner->GetController(GC));
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
            if(!ObjectId.Equals(TEXT("NoObject")))
              {
                break;
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
