// Fill out your copyright notice in the Description page of Project Settings.


#include "URoboSimSubsystem.h"
#include "URoboSimSettings.h"

UURoboSimSubsystem::UURoboSimSubsystem()
{
  JointStatePublisherMessageType = TEXT("sensor_msgs/JointState");
  SetJointStateServerType = TEXT("urobosim_msgs/SetEnvironmentJointState");
  KnowrobInterface = CreateDefaultSubobject<UKnowrobInterface>(FName(GetName() + "_KnowrobInterface"));
}

void UURoboSimSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
  // Super::Initialize(Collection);
  const UURoboSimSettings* Settings = GetDefault<UURoboSimSettings>();

  // if(!GIsEditor || GIsPlayInEditorWorld)
    // {
      if (Settings->bEnableURoboSim)
        {
          Super::Initialize(Collection);
          if(ROSHandler.IsValid())
            {
              if(Settings->bEnableJointStatePublishing)
                {
                  Publisher = MakeShareable<FROSBridgePublisher>(new FROSBridgePublisher(Settings->JointStatePublishTopic, JointStatePublisherMessageType));
                  if (Publisher.IsValid())
                    {
                      ROSHandler->AddPublisher(Publisher);
                    }
                }

              if(Settings->bEnableSettingJointState)
                {
                  ServiceServer = MakeShareable<FSetEnvironmentJointStatesServerCallback>(new FSetEnvironmentJointStatesServerCallback(TEXT("UnrealSim/SetEnvJointState"), SetJointStateServerType, &Joints));
                  ServiceServer->EnableDrive = Settings->EnableDrive;
                  ServiceServer->ErrorTollerance = Settings->ErrorTollerance;
                  ROSHandler->AddServiceServer(ServiceServer);
                }
              if(KnowrobInterface)
                {
                  KnowrobInterface->QueryClient->Connect(ROSHandler);
                }
            }
          else
            {
              UE_LOG(LogTemp, Error, TEXT("URoboSimSubsystem: Handler not valid"));
            }
        }

    // }
}
