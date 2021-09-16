// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameManager.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, GameManager, "GameManager" );

UGameManager::UGameManager()
{
	ManagerIP = FString("127.0.0.1");
	ManagerPort = 8888;

	ObserverIP = FString("127.0.0.1");
	ObserverPort = 8889;
}