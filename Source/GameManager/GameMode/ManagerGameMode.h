// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SocketServer/Public/SocketServer.h"
#include "SocketClient/Public/SocketClient.h"
#include "GameManager/GameManager.h"
#include "ManagerGameMode.generated.h"





/**
 * 
 */
UCLASS(Config = Game)
class GAMEMANAGER_API AManagerGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:


	UPROPERTY(Config)
	FString ManagerIP;

	UPROPERTY(Config)
	int		ManagerPort;

	UPROPERTY(Config)
	int		ObserverPort;

	UPROPERTY(Config)
	FString GameServerPath;

	UPROPERTY(Config)
	FString CommmaPersistentMaps;

	USocketServerBPLibrary* SocketServer;
	USocketClientBPLibrary* SocketClient;
	FString AgentServerID;
	FString MasterServerID;

	FString ConnectionID;

	TArray<FString> GameServerSessionIDs;
	TArray<FString> AgentServerSessionIDs;

	TArray<FString> PersistentMaps;

	TMap<FGuid, FGameServerInfo> AgentGameServerInfos;
	TMap<FGuid, FGameServerInfo> MasterGameServerInfos;
	bool	CreateProcess;
public:
	AManagerGameMode();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	   
	UFUNCTION()
	void OnServerConnection(const EServerSocketConnectionEventType type, const bool success, const FString message, const FString sessionID, const FString serverID);
	
	UFUNCTION()
	void OnServerReceive(const FString sessionID, const FString message, const TArray<uint8>& byteArray, const FString serverID);

	UFUNCTION()
	void OnClientConnection(const bool success, const FString message, const FString clientConnectionIDP);

	UFUNCTION()
	void OnClientReceive(const FString message, const TArray<uint8>& byteArray, const FString clientConnectionIDP);


	void ConnectToMaster();

	void Test();



	FGameServerInfo* CreateGameServerProcess(FString MapPath);
	void CloseGameServerProcess(FGuid Guid);
};
