#include "ROSCommunication/Service/Server/SetEnvironmentJointStatesServer.h"
#include "Controller/RController.h"
#include "EngineUtils.h"
#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "srv/SetEnvironmentJointState.h"

DEFINE_LOG_CATEGORY_STATIC(LogSetEnvironmentJointStatesServer, Log, All)

FSetEnvironmentJointStatesServerCallback::FSetEnvironmentJointStatesServerCallback(FString Name, FString Type, TMap<FString,AJointActor*>* InJointList ) : FROSBridgeSrvServer(Name, Type)
{
  JointList = InJointList;
}

TSharedPtr<FROSBridgeSrv::SrvRequest> FSetEnvironmentJointStatesServerCallback::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
  TSharedPtr<urobosim_msgs::SetEnvironmentJointStateSrv::Request> Request =
      MakeShareable(new urobosim_msgs::SetEnvironmentJointStateSrv::Request());
  Request->FromJson(JsonObject);
  return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FSetEnvironmentJointStatesServerCallback::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
  TSharedPtr<urobosim_msgs::SetEnvironmentJointStateSrv::Request> SetEnvironmentJointStatesRequest =
    StaticCastSharedPtr<urobosim_msgs::SetEnvironmentJointStateSrv::Request>(Request);

  bool ServiceSuccess = false;
  TArray<FString> JointNames = SetEnvironmentJointStatesRequest->GetJointNames();
  TArray<float> JointStates = SetEnvironmentJointStatesRequest->GetJointStates();
  if(JointNames.Num() != JointStates.Num())
    {
      return MakeShareable<FROSBridgeSrv::SrvResponse>(new urobosim_msgs::SetEnvironmentJointStateSrv::Response(ServiceSuccess));
    }

  // Execute on game thread
    FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]() {
        for(int i = 0; i < JointNames.Num(); i++)
          {
            AJointActor* JointActor = (*JointList)[JointNames[i]];
            if(JointActor)
              {
                JointActor->Joint->SetDrive(EnableDrive);
                FJointState JointState(JointStates[i], 0.0);
                JointActor->Joint->SetMotorJointState(JointState);
              }
          }

    },
      TStatId(), nullptr, ENamedThreads::GameThread);

    //wait code above to complete
    FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);



    float ServiceDuration = 0;
    float SleepTime = 0.01;
    while(!ServiceSuccess && ServiceDuration < 3.0)
      {
        //set to false if one Joint did not reach its goal
        ServiceSuccess = true;
        for(int i = 0; i < JointNames.Num(); i++)
          {
            AJointActor* JointActor = (*JointList)[JointNames[i]];
            if(JointActor)
              {
                float Error= FMath::Abs(JointStates[i] - JointActor->CurrentJointPos);
                UE_LOG(LogTemp, Error, TEXT("ServiceDuration %f ErrorPer %f toll %f"), ServiceDuration, Error, ErrorTollerance);

                if(Error <= ErrorTollerance)
                  {
                    FEnableDrive TempDrive = EnableDrive;
                    TempDrive.bPositionDrive = false;
                    TempDrive.bVelocityDrive = false;
                    JointActor->Joint->SetDrive(TempDrive);
                  }
                else
                  {
                    ServiceSuccess = false;
                  }
              }
          }

        ServiceDuration+=SleepTime;
        FPlatformProcess::Sleep(SleepTime);
      }


    return MakeShareable<FROSBridgeSrv::SrvResponse>(new urobosim_msgs::SetEnvironmentJointStateSrv::Response(ServiceSuccess));
}
