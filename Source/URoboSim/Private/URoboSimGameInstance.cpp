#include "URoboSimGameInstance.h"

URoboSimGameInstance::URoboSimGameInstance()
{
  JointStatePublishTopic = TEXT("/joint_states");
  JointStatePublisherMessageType = TEXT("sensor_msgs/JointState");
  SetJointStateServerType = TEXT("urobosim_msgs/SetEnvironmentJointState");
  KnowrobInterface = CreateDefaultSubobject<UKnowrobInterface>(FName(GetName() + "_KnowrobInterface"));
}

void URoboSimGameInstance::OnStart()
{
  Super::OnStart();

  if(ROSHandler.IsValid())
    {
      if(bEnableJointStatePublishing)
        {
          Publisher = MakeShareable<FROSBridgePublisher>(new FROSBridgePublisher(JointStatePublishTopic, JointStatePublisherMessageType));
          if (Publisher.IsValid())
            {
              ROSHandler->AddPublisher(Publisher);
            }
        }

      if(bEnableSettingJointState)
        {
          ServiceServer = MakeShareable<FSetEnvironmentJointStatesServerCallback>(new FSetEnvironmentJointStatesServerCallback(TEXT("UnrealSim/SetEnvJointState"), SetJointStateServerType, &Joints));
          ServiceServer->EnableDrive = EnableDrive;
          ServiceServer->ErrorTollerance = ErrorTollerance;
          ROSHandler->AddServiceServer(ServiceServer);
        }
      if(KnowrobInterface)
        {
          KnowrobInterface->QueryClient->Connect(ROSHandler);
        }
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("URoboSimGameInstance: Handler not valid"));
    }
}


void URoboSimGameInstance::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
  if (ROSHandler.IsValid() && Publisher.IsValid())
    {
      static int Seq = 0;
      TArray<FString> ListJointName;
      TArray<double> ListJointPosition;
      for(auto& Joint : Joints)
        {
          ListJointName.Add(Joint.Key);
          ListJointPosition.Add(Joint.Value->CurrentJointPos);
        }

      TSharedPtr<sensor_msgs::JointState> JointState =
        MakeShareable(new sensor_msgs::JointState());
      JointState->SetHeader(std_msgs::Header(Seq, FROSTime(), ""));
      JointState->SetName(ListJointName);
      JointState->SetPosition(ListJointPosition);

      ROSHandler->PublishMsg(JointStatePublishTopic, JointState);

      Seq++;
    }
}
