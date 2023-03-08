#include "ROSCommunication/Service/Server/GraspObjectServer.h"
#include "Tags.h"
#include "urobosim_msgs/GraspObject.h"

DEFINE_LOG_CATEGORY_STATIC(LogRGraspObjectServer, Log, All)

void URGraspObjectServer::SetServiceServerParameters(URServiceServerParameter*& ServiceServerParameters)
{
  Super::SetServiceServerParameters(ServiceServerParameters);

  ControllerName = Cast<URGraspObjectServerParameter>(ServiceServerParameters)->ControllerName;
}

URGraspObjectServer::URGraspObjectServer()
{
  Type = TEXT("urobosim_msgs/GraspObject");
}

void URGraspObjectServer::CreateServiceServer()
{

  ARModel* Model = Cast<ARModel>(GetOwner());
  if(Model)
    {
      Controller = Cast<URGripperControllerBase>(Model->GetController(ControllerName));
    }

  ServiceServer = MakeShareable<FRGraspObjectServerCallback>(new FRGraspObjectServerCallback(Name, Type, GetWorld(), Controller));
}

FRGraspObjectServerCallback::FRGraspObjectServerCallback(FString Name, FString Type, UWorld *InWorld, URGripperControllerBase* InController) : FROSBridgeSrvServer(Name, Type)
{
  World = InWorld;
  Controller = InController;
  if(!Controller)
  {
    UE_LOG(LogRGraspObjectServer, Error, TEXT("Controller is nullptr"));
  }
}

TSharedPtr<FROSBridgeSrv::SrvRequest> FRGraspObjectServerCallback::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
  TSharedPtr<urobosim_msgs::GraspObject::Request> Request =
      MakeShareable(new urobosim_msgs::GraspObject::Request());
  Request->FromJson(JsonObject);
  return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FRGraspObjectServerCallback::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
  TSharedPtr<urobosim_msgs::GraspObject::Request> GraspObjectRequest =
      StaticCastSharedPtr<urobosim_msgs::GraspObject::Request>(Request);

  FString ObjectName = GraspObjectRequest->GetPose().GetHeader().GetFrameId();
  bool ServiceSuccess = false;
  if (!World)
  {
    UE_LOG(LogRGraspObjectServer, Error, TEXT("World not found"));
  }
  else
  {
    // Execute on game thread
    FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]() {
      TActorIterator<AStaticMeshActor> ActorItr =
          TActorIterator<AStaticMeshActor>(World);

      TArray<AActor*> AllMatchingActors = FTags::GetActorsWithKeyValuePair(World, TEXT("SemLog"), TEXT("Id"), ObjectName);
      AActor* Actor = nullptr;
      if(AllMatchingActors.Num() > 0)
        Actor = AllMatchingActors.Pop();
      if (!Actor)
        {
          // Couldn't find Actor for ID
          UE_LOG(LogTemp, Warning, TEXT("Actor with id:\"%s\" does not exist and can therefore not be moved."), *ObjectName);
          ServiceSuccess = false;
          return;
        }

      if(Controller->ReferenceLink)
            {
              FTransform Pose = Controller->ReferenceLink->GetComponentTransform();
              Pose.AddToTranslation(Pose.GetRotation().RotateVector(FConversions::ROSToU(GraspObjectRequest->GetPose().GetPose().GetPosition().GetVector())));
              Actor->SetActorTransform(Pose, false, nullptr, ETeleportType::TeleportPhysics);
              Controller->GraspComponent->OverrideObjectInReach(Actor);
              Controller->Grasp();
              ServiceSuccess = true;
            }
      else
        {
          UE_LOG(LogRGraspObjectServer, Error, TEXT("[%s] Controller not found"), *FString(__FUNCTION__));
        }
    },
                                                                         TStatId(), nullptr, ENamedThreads::GameThread);

    //wait code above to complete
    FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
  }

  return MakeShareable<FROSBridgeSrv::SrvResponse>(new urobosim_msgs::GraspObject::Response(ServiceSuccess));
}
