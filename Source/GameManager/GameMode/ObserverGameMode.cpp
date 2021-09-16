// Fill out your copyright notice in the Description page of Project Settings.


#include "ObserverGameMode.h"
#include "GameManager/GameManager.h"

void AObserverGameMode::BeginPlay()
{
	Super::BeginPlay();
	FString ServerID;

	SocketClient = USocketClientBPLibrary::socketClientBPLibrary;

	SocketClient->onsocketClientTCPConnectionEventDelegate.AddDynamic(this, &AObserverGameMode::socketClientTCPConnectionEventDelegate);
	SocketClient->onreceiveTCPMessageEventDelegate.AddDynamic(this, &AObserverGameMode::receiveTCPMessageEventDelegate);

	Connect();

}



void AObserverGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	SocketClient->closeSocketClientConnectionTCP(ConnectionID);
	Super::EndPlay(EndPlayReason);
}

void AObserverGameMode::Connect()
{
	UGameManager* GameManager = NewObject<UGameManager>(UGameManager::StaticClass());

	SocketClient->connectSocketClientTCP(GameManager->ManagerIP, GameManager->ManagerPort, EReceiveFilterClient::E_B, ConnectionID);
}

void AObserverGameMode::socketClientTCPConnectionEventDelegate(const bool success, const FString message, const FString clientConnectionIDP) 
{
	UE_LOG(LogTemp, Display, TEXT("%s %d %s"), *FString(__FUNCTION__), success, *message);
	if (!success)
	{
		Connect();
	}
}

void AObserverGameMode::receiveTCPMessageEventDelegate(const FString message, const TArray<uint8>& byteArray, const FString clientConnectionIDP) 
{
	UE_LOG(LogTemp, Display, TEXT("%s"), *FString(__FUNCTION__));
}

FGuid AObserverGameMode::CreateGameServerProcess(FString MapPath)
{
	if (GameServerPath.IsEmpty())
	{
		UE_LOG(LogTemp, Display, TEXT("%s Failed. GameServerPath Empty"), *FString(__FUNCTION__));
		return FGuid();
	}
		

	FGuid Guid  = FGuid::NewGuid();
	
	uint32 ProcID =0;
	FProcHandle ProcHandle = FPlatformProcess::CreateProc(
		*GameServerPath,	// const TCHAR* URL
		nullptr,	// const TCHAR* Parms
		false,		// bool bLaunchDetached
		false,		// bool bLaunchHidden
		false,		// bool bLaunchReallyHidden
		&ProcID,	// uint32* OutProcessID
		0,			// PriorityModifier
		*FPaths::GetPath(GameServerPath),			// const TCHAR* OptionalWorkingDirectory
		0,			// void* PipeWriteChild
		0);			// void * PipeReadChild = nullptr
		
	if (ProcHandle.IsValid() == false )
	{
		UE_LOG(LogTemp, Display, TEXT("%s OutProcessID is 0."), *FString(__FUNCTION__));
		return FGuid();
	}
	FGameServerProcess Item;
	Item.MapPath = MapPath;
	Item.ProcID = ProcID;
	Item.ProcHandle = ProcHandle;
	GameServerProcessMap.Add(Guid,Item);
	return Guid;
}

void AObserverGameMode::KillGameServerProcess(FGuid Giud)
{
	FGameServerProcess* Item = GameServerProcessMap.Find(Giud);
	if (Item != nullptr)
	{
		FPlatformProcess::CloseProc(Item->ProcHandle);
	}
}