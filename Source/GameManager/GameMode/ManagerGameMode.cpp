// Fill out your copyright notice in the Description page of Project Settings.


#include "ManagerGameMode.h"
#include "GameManager/GameManager.h"

void AManagerGameMode::BeginPlay()
{
	Super::BeginPlay();


	USocketServerBPLibrary* Server = USocketServerBPLibrary::socketServerBPLibrary;

	Server->onsocketServerConnectionEventDelegate.AddDynamic(this, &AManagerGameMode::socketServerConnectionEventDelegate);
	Server->onserverReceiveTCPMessageEventDelegate.AddDynamic(this, &AManagerGameMode::serverReceiveTCPMessageEventDelegate);
	Server->onsocketServerUDPConnectionEventDelegate.AddDynamic(this, &AManagerGameMode::socketServerUDPConnectionEventDelegate);
	Server->onserverReceiveUDPMessageEventDelegate.AddDynamic(this, &AManagerGameMode::serverReceiveUDPMessageEventDelegate);

	
	UGameManager* GameManager = NewObject<UGameManager>(UGameManager::StaticClass());
	Server->startTCPServer(ServerID,FString("0.0.0.0"), GameManager->ManagerPort);
}



void AManagerGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	USocketServerBPLibrary::socketServerBPLibrary->stopTCPServer(ServerID);
	Super::EndPlay(EndPlayReason);
}

void AManagerGameMode::socketServerConnectionEventDelegate(const EServerSocketConnectionEventType type, const bool success, const FString message, const FString sessionID, const FString serverID) 
{
	UE_LOG(LogTemp, Display, TEXT("%s"), *FString(__FUNCTION__));
}

void AManagerGameMode::serverReceiveTCPMessageEventDelegate(const FString sessionID, const FString message, const TArray<uint8>& byteArray, const FString serverID) 
{
	UE_LOG(LogTemp, Display, TEXT("%s"), *FString(__FUNCTION__));
}

void AManagerGameMode::serverReceiveTCPFileDownloadEventDelegate(const int32 statusCode, const FString statusMessage, const FString sessionID, const FString serverID, const FString fullPath) 
{
	UE_LOG(LogTemp, Display, TEXT("%s"), *FString(__FUNCTION__));
}

void AManagerGameMode::serverReceiveTCPFileDownloadProgressEventDelegate(const FSocketServerDownloadFileInfo fileInfo) 
{
	UE_LOG(LogTemp, Display, TEXT("%s"), *FString(__FUNCTION__));
}

void AManagerGameMode::serverSendTCPFileUploadEventDelegate(const int32 statusCode, const FString statusMessage, const FString sessionID, const FString serverID, const FString fullPath) 
{
	UE_LOG(LogTemp, Display, TEXT("%s"), *FString(__FUNCTION__));
}

void AManagerGameMode::serverSendTCPFileUploadProgressEventDelegate(const FSocketServerUploadFileInfo fileInfo) 
{
	UE_LOG(LogTemp, Display, TEXT("%s"),* FString(__FUNCTION__));
}

void AManagerGameMode::socketServerUDPConnectionEventDelegate(const bool success, const FString message, const FString serverID) 
{
	UE_LOG(LogTemp, Display, TEXT("%s"),* FString(__FUNCTION__));
}

void AManagerGameMode::serverReceiveUDPMessageEventDelegate(const FString sessionID, const FString message, const TArray<uint8>& byteArray, const FString serverID) 
{
	UE_LOG(LogTemp, Display, TEXT("%s"), *FString(__FUNCTION__));
}