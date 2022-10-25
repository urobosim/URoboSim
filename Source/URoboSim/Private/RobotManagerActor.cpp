// Fill out your copyright notice in the Description page of Project Settings.


#include "RobotManagerActor.h"
#include "ROSBridgeGameInstance.h"
#include "RosSettings.h"
#include "SDFParser.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnRobotServer.h"
#include "Controller/RControllerDataAsset.h"
#include "Factory/RControllerBuilder.h"
#include "Factory/RModelBuilder.h"
#include "Factory/RROSCommunicationBuilder.h"

URobotManager::URobotManager()
{
	// ROSBridge server default values
	ServerIP = "127.0.0.1";
	ServerPORT = 9090;

	SDFClient = CreateDefaultSubobject<USDFClient>(TEXT("SDFClient"));
	SDFClient->RobotManager = this;

	SpawnRobotServer = CreateDefaultSubobject<USpawnRobotServer>(TEXT("SpawnRobotServer"));
	SpawnRobotServer->Controller = this;
	SpawnRobotServer->SDFClient = SDFClient;
}

// Sets default values
ARobotManagerActor::ARobotManagerActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RobotManager = CreateDefaultSubobject<URobotManager>(TEXT("RobotManager"));
}

// Called when the game starts or when spawned
void ARobotManagerActor::BeginPlay()
{
	Super::BeginPlay();
	RobotManager->Init(TEXT("Default"), GetWorld());
}

// Called when destroyed or game stopped
void ARobotManagerActor::EndPlay(const EEndPlayReason::Type Reason)
{
	RobotManager->EndPlay(Reason);
	Super::EndPlay(Reason);
}

// Called every frame
void ARobotManagerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void URobotManager::ParseSDF()
{
	FString SDFFile = SDFClient->SDFFile;

	FSDFParser Parser = FSDFParser(SDFFile);

	FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
	{
		EObjectFlags Flags = EObjectFlags();
		Flags |= RF_Transactional;
		USDFDataAsset* SDFAsset = Parser.ParseToNewDataAsset(GetWorld(), TEXT(""), Flags);

		if (SDFAsset)
		{
			for (USDFModel* Model : SDFAsset->Models)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Name = FName(Model->Name);


				// Creates RRobot Actor.
				URModelBuilder* ModelBuilder = NewObject<URModelBuilder>(this);
				ARModel* NewRobot = Cast<ARModel>(
					GetWorld()->SpawnActor(ARModel::StaticClass(), &Model->Pose, SpawnParams));
				ModelBuilder->Load(Model, NewRobot, Model->Pose.GetLocation(), SDFAsset->Version);


				FString ControllerConfigPathShort = TEXT("/Game/ControllerConfig");
				FString Reason = "";
				FString ControllerConfigPathLong;

				if (!FPackageName::TryConvertFilenameToLongPackageName(ControllerConfigPathShort,
				                                                       ControllerConfigPathLong,
				                                                       &Reason))
				{
					UE_LOG(LogTemp, Error, TEXT("Packacke name invlaide because : %s"), *Reason);
				}
				else
				{
					URControllerDataAsset* ControllerConfig = Cast<URControllerDataAsset>(StaticLoadObject(
						URControllerDataAsset::StaticClass(), nullptr, *ControllerConfigPathLong));
					if (ControllerConfig)
					{
						TArray<ARModel*> Robots;
						Robots.Add(NewRobot);
						URControllerBuilder* ControllerBuilder = NewObject<URControllerBuilder>();
						ControllerBuilder->Init(Robots, ControllerConfig->ControllerConfiguration);
						ControllerBuilder->Build();
						NewRobot->GetPlugin(TEXT("ControllerComponent"))->Init();
					}
				}
				FString ROSConfigPathShort = TEXT("/Game/ROSCom");
				Reason = "";
				FString ROSConfigPathLong;

				if (!FPackageName::TryConvertFilenameToLongPackageName(ROSConfigPathShort,
				                                                       ROSConfigPathLong,
				                                                       &Reason))
				{
					UE_LOG(LogTemp, Error, TEXT("Packacke name invlaide because : %s"), *Reason);
				}
				else
				{
					URROSCommunicationDataAsset* ROSConfig = Cast<URROSCommunicationDataAsset>(StaticLoadObject(
						URROSCommunicationDataAsset::StaticClass(), nullptr, *ROSConfigPathLong));
					if (ROSConfig)
					{
						URROSCommunicationBuilder* ROSCommunicationBuilder = NewObject<URROSCommunicationBuilder>();
						ROSCommunicationBuilder->Init(NewRobot, ROSConfig->ROSCommunicationConfiguration);
						ROSCommunicationBuilder->Build(ROSConfig->PublisherConfiguration,
						                               ROSConfig->SubscriberConfiguration,
						                               ROSConfig->ServiceServerConfiguration,
						                               ROSConfig->ServiceClientConfiguration,
						                               ROSConfig->ActionServerConfiguration);
						NewRobot->GetPlugin(TEXT("ROSCommunicationComponent"))->Init();
					}
				}
			}
		}
	}, TStatId(), nullptr, ENamedThreads::GameThread);
	FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
				// PostSpawnActor(InAsset, NewRobot);
}

void URobotManager::Init(FString DefaultNamespace, UWorld* InWorld)
{
	Namespace = DefaultNamespace;

	if (!GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s]: GetWorld returned, NULL."), *FString(__FUNCTION__));
		return;
	}

	if (bUseGlobalIP)
	{
          const URosSettings* Settings = GetDefault<URosSettings>();
          ServerIP = Settings->ROSBridgeServerHost;
          ServerPORT = Settings->ROSBridgeServerPort;
        }

        SDFClient->Connect(ServerIP, ServerPORT);
	SpawnRobotServer->Connect(ServerIP, ServerPORT);

	//Connect SDFClient to SpawnRobotServer
}

void URobotManager::EndPlay(const EEndPlayReason::Type Reason)
{
	// Disconnect before parent ends
	SDFClient->Disconnect();
	SpawnRobotServer->Disconnect();
}
