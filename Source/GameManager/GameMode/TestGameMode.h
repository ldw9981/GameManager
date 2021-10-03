// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SocketClient/Public/SocketClient.h"
#include "TestGameMode.generated.h"

/**
 *  게임서버를 테스트하기 위한 모드
 */
UCLASS()
class GAMEMANAGER_API ATestGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	FString GuidString;
	FString ConnectionID;
	USocketClientBPLibrary* SocketClient;
public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnClientConnection(const bool success, const FString message, const FString clientConnectionIDP);

	UFUNCTION()
	void OnClientReceive(const FString message, const TArray<uint8>& byteArray, const FString clientConnectionIDP);
};
