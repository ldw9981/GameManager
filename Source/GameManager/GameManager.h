// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameManager.generated.h"

UENUM()
enum class EGameServerStatus : uint8
{
	Invalid = 0,
	Create = 1,
	Connected = 2,
	Disconnected =3
};

struct FGameServerInfo
{
	FGuid GameServerGUID;
	FString MapPath;
	uint32	ProcID;
	FProcHandle ProcHandle;
	FString IP;
	int32 Port;
	EGameServerStatus Status;


	FGameServerInfo()
	{
		ProcID = 0;
		Port = 0;
		Status = EGameServerStatus::Invalid;
	}

	friend FArchive& operator<<(FArchive& Ar, FGameServerInfo& GameServerInfo)
	{
		Ar << GameServerInfo.GameServerGUID;
		//Ar << GameServerInfo.MapPath;
		Ar << GameServerInfo.ProcID;
		// Handle은 로컬에서만 유효한 데이터
		//Ar << GameServerInfo.IP;
		Ar << GameServerInfo.Port;
		Ar << GameServerInfo.Status;
		return Ar;
	}
};

UCLASS()
class GAMEMANAGER_API UGameManager : public UObject
{
	GENERATED_BODY()
public:

public:
	UGameManager();
};