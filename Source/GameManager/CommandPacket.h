// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameManager/GameManager.h"
/**
 * 
 */


#define COMMAND_TEST1 1 
#define COMMAND_CREATEGAMESERVER 2 
#define COMMAND_DESTROYGAMESERVER 3 
#define COMMAND_NOTIFYGAMESERVER 4
#define COMMAND_REQ_GAMESERVERLIST 5
#define COMMAND_REP_GAMESERVERLIST 6


class GAMEMANAGER_API CommandPacket
{
public:
	CommandPacket();
	~CommandPacket();
};


struct FCommandPacket
{
public:
	uint8 Command;	
	virtual ~FCommandPacket() {	}
	virtual void Serialize(FArchive& Ar);
	void Pack(TArray<uint8>& byteArray);
	void UnPack(const TArray<uint8>& byteArray);
};

struct FTest1Packet : public FCommandPacket
{
public:
	uint8 Data1;
	int Data2;
	TArray<int> MyArray;

	FTest1Packet()
	{
		Command = COMMAND_TEST1;
	}
	virtual void Serialize(FArchive& Ar) override;
};

struct FCreateGameServerPacket : public FCommandPacket
{
public:
	FString MapName;

	FCreateGameServerPacket()
	{
		Command = COMMAND_CREATEGAMESERVER;
	}
	virtual void Serialize(FArchive& Ar) override;
};

struct FDestroyGameServerPacket : public FCommandPacket
{
public:
	FGuid Guid;
	
	FDestroyGameServerPacket()
	{
		Command = COMMAND_DESTROYGAMESERVER;
	}

};

struct FNotifyGameServerPacket : public FCommandPacket
{
public:
	FGameServerInfo GameServerInfo;

	FNotifyGameServerPacket()
	{
		Command = COMMAND_NOTIFYGAMESERVER;
	}
	virtual void Serialize(FArchive& Ar) override;
};


struct FReqGameServerListPacket : public FCommandPacket
{
public:

	FReqGameServerListPacket()
	{
		Command = COMMAND_REQ_GAMESERVERLIST;
	}
	virtual void Serialize(FArchive& Ar) override;
};

struct FRepGameServerListPacket : public FCommandPacket
{
public:
	TArray<FGameServerInfo> List;

	FRepGameServerListPacket()
	{
		Command = COMMAND_REP_GAMESERVERLIST;
	}
	virtual void Serialize(FArchive& Ar) override;
};



