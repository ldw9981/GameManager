// Fill out your copyright notice in the Description page of Project Settings.


#include "CommandPacket.h"
#include "Serialization/BufferArchive.h"
//#include "Containers/Array.h"

CommandPacket::CommandPacket()
{
}

CommandPacket::~CommandPacket()
{
}

void FCommandPacket::Serialize(FArchive& Ar)
{
	Ar << Command;
}

void FCommandPacket::Pack(TArray<uint8>& byteArray)
{
	FMemoryWriter Ar(byteArray);
	Serialize(Ar);
}

void FCommandPacket::UnPack(const TArray<uint8>& byteArray)
{
	FMemoryReader Ar(byteArray);
	Serialize(Ar);
}




void FTest1Packet::Serialize(FArchive& Ar)
{
	Ar << Command;
	Ar << Data1;
	Ar << Data2;
	Ar << MyArray;
}

void FCreateGameServerPacket::Serialize(FArchive& Ar)
{
	Ar << Command;

}

void FNotifyGameServerPacket::Serialize(FArchive& Ar)
{
	Ar << Command;
	Ar << GameServerInfo;
}

void FReqGameServerListPacket::Serialize(FArchive& Ar)
{
	Ar << Command;
}

void FRepGameServerListPacket::Serialize(FArchive& Ar)
{
	Ar << Command;
	Ar << List;
}
