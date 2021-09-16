// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SocketClient/Public/SocketClient.h"
#include "ObserverGameMode.generated.h"


class USocketClientBPLibrary;

struct FGameServerProcess
{
	FString MapPath;
	uint32	ProcID;
	FProcHandle ProcHandle;
};

/**
 * 
 */
UCLASS(Config = Game)
class GAMEMANAGER_API AObserverGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(Config)
	FString GameServerPath;

	FString ConnectionID;
	USocketClientBPLibrary* SocketClient;


	TMap<FGuid, FGameServerProcess> GameServerProcessMap;

public:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void Connect();

	UFUNCTION()
	void socketClientTCPConnectionEventDelegate(const bool success, const FString message, const FString clientConnectionIDP);
	
	UFUNCTION()
	void receiveTCPMessageEventDelegate(const FString message, const TArray<uint8>& byteArray, const FString clientConnectionIDP);
	
	FGuid CreateGameServerProcess(FString MapPath);
	void KillGameServerProcess(FGuid Giud);
};
