// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameManager.generated.h"

UCLASS(Config = Game)
class GAMEMANAGER_API UGameManager : public UObject
{
	GENERATED_BODY()
public:
	UPROPERTY(Config)
	FString ManagerIP;

	UPROPERTY(Config)
	int		ManagerPort;
	
	UPROPERTY(Config)
	FString ObserverIP;

	UPROPERTY(Config)
	int		ObserverPort;

public:
	UGameManager();
};