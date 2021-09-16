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

