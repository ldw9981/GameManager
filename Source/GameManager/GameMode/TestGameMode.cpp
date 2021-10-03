// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGameMode.h"
#include "HAL/PlatformProcess.h"

#include "GameManager/CommandPacket.h"

void ATestGameMode::BeginPlay()
{
	Super::BeginPlay();
	SocketClient = USocketClientBPLibrary::socketClientBPLibrary;
	SocketClient->onsocketClientTCPConnectionEventDelegate.AddDynamic(this, &ATestGameMode::OnClientConnection);
	SocketClient->onreceiveTCPMessageEventDelegate.AddDynamic(this, &ATestGameMode::OnClientReceive);

	SocketClient->connectSocketClientTCP(FString("127.0.0.1"), 8889, EReceiveFilterClient::E_B, ConnectionID);

	FString CommandLine = FCommandLine::Get();
	TArray<FString> StringArray; 
	//CommandLine.ParseIntoArray(StringArray,TEXT(' '));

	
	FParse::Value(FCommandLine::Get(), TEXT("-GUID="), GuidString);
	UE_LOG(LogTemp, Display, TEXT("%s %s"), *FString(__FUNCTION__), *GuidString);
}

void ATestGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	SocketClient->closeSocketClientConnectionTCP(ConnectionID);
	Super::EndPlay(EndPlayReason);
}

void ATestGameMode::OnClientConnection(const bool success, const FString message, const FString clientConnectionIDP)
{
	UE_LOG(LogTemp, Display, TEXT("%s %d %s"), *FString(__FUNCTION__), success, *message);
	
	if (success)
	{
		TArray<uint8> byteArray;
		FNotifyGameServerPacket Packet;

		FGuid::Parse(GuidString, Packet.GameServerInfo.GameServerGUID);
		Packet.GameServerInfo.Port = GetWorld()->URL.Port;
		Packet.GameServerInfo.ProcID = FPlatformProcess::GetCurrentProcessId();
		Packet.GameServerInfo.Status = EGameServerStatus::Connected;
		Packet.Pack(byteArray);
		UE_LOG(LogTemp, Display, TEXT("%s GUID %s ProcID %d, Port %d"), *FString(__FUNCTION__), *Packet.GameServerInfo.GameServerGUID.ToString(),Packet.GameServerInfo.ProcID, Packet.GameServerInfo.Port);
		SocketClient->socketClientSendTCP(ConnectionID, FString(), byteArray);
	}
	else
	{
		FGenericPlatformMisc::RequestExit(true);
	}
}

void ATestGameMode::OnClientReceive(const FString message, const TArray<uint8>& byteArray, const FString clientConnectionIDP)
{

}
