// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SocketServer/Public/SocketServer.h"
#include "ManagerGameMode.generated.h"

/**
 * 
 */
UCLASS()
class GAMEMANAGER_API AManagerGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	FString ServerID;
public:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	   
	UFUNCTION()
	void socketServerConnectionEventDelegate(const EServerSocketConnectionEventType type, const bool success, const FString message, const FString sessionID, const FString serverID);
	
	UFUNCTION()
	void serverReceiveTCPMessageEventDelegate(const FString sessionID, const FString message, const TArray<uint8>& byteArray, const FString serverID);
	
	UFUNCTION()
	void serverReceiveTCPFileDownloadEventDelegate(const int32 statusCode, const FString statusMessage, const FString sessionID, const FString serverID, const FString fullPath);
	
	UFUNCTION()
	void serverReceiveTCPFileDownloadProgressEventDelegate(const FSocketServerDownloadFileInfo fileInfo);
	
	UFUNCTION()
	void serverSendTCPFileUploadEventDelegate(const int32 statusCode, const FString statusMessage, const FString sessionID, const FString serverID, const FString fullPath);
	
	UFUNCTION()
	void serverSendTCPFileUploadProgressEventDelegate(const FSocketServerUploadFileInfo fileInfo);
	
	UFUNCTION()
	void socketServerUDPConnectionEventDelegate(const bool success, const FString message, const FString serverID);
	
	UFUNCTION()
	void serverReceiveUDPMessageEventDelegate(const FString sessionID, const FString message, const TArray<uint8>& byteArray, const FString serverID);
};
