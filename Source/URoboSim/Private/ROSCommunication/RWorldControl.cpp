#include "ROSCommunication/RWorldControl.h"

AWorldController::AWorldController()
{
  WebsocketIPAddr = TEXT("127.0.0.1");
  WebsocketPort = 9090;
  PrimaryActorTick.bCanEverTick = true;
  PrimaryActorTick.TickGroup = TG_PrePhysics;
}

void AWorldController::BeginPlay()
{
  Super::BeginPlay();

  // Handler = MakeShareable<FROSBridgeHandler>(new FROSBridgeHandler(WebsocketIPAddr, WebsocketPort));
  // Handler->Connect();

  // Server = MakeShareable<FROSResetLevelServer>(new FROSResetLevelServer(TEXT("unreal"), TEXT("reset_level"), GetWorld(), this));
  // Handler->AddServiceServer(Server);
}

void AWorldController::TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
  // Handler->Process();
  // FTimerHandle UnusedHandle;
  // GetWorldTimerManager().SetTimer(
  //    UnusedHandle, this, &AWorldController::ResetLevel, 3, false);
}

void AWorldController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  // Handler->Disconnect();
}

void AWorldController::ResetLevel()
{
  UE_LOG(LogTemp, Error, TEXT("ResetLevel Function"));
  UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}
