// Fill out your copyright notice in the Description page of Project Settings.


#include "ManagerGameMode.h"
#include "GameManager/GameManager.h"
#include "GameManager/CommandPacket.h"

AManagerGameMode::AManagerGameMode()
{
	ManagerIP = FString("127.0.0.1");
	ManagerPort = 8888;
	ObserverPort = 8889;
	CreateProcess = false;
}

void AManagerGameMode::BeginPlay()
{
	Super::BeginPlay();



	SocketServer = USocketServerBPLibrary::socketServerBPLibrary;

	SocketServer->onsocketServerConnectionEventDelegate.AddDynamic(this, &AManagerGameMode::OnServerConnection);
	SocketServer->onserverReceiveTCPMessageEventDelegate.AddDynamic(this, &AManagerGameMode::OnServerReceive);

	SocketServer->startTCPServer(MasterServerID,FString("0.0.0.0"), ManagerPort);
	SocketServer->startTCPServer(AgentServerID, FString("0.0.0.0"), ObserverPort);

	SocketClient = USocketClientBPLibrary::socketClientBPLibrary;
	SocketClient->onsocketClientTCPConnectionEventDelegate.AddDynamic(this, &AManagerGameMode::OnClientConnection);
	SocketClient->onreceiveTCPMessageEventDelegate.AddDynamic(this, &AManagerGameMode::OnClientReceive);


	/*
	TArray<uint8> byteArrayRecv;
	TArray<uint8> byteArray;
	FNotifyGameServerPacket Packet;
	FNotifyGameServerPacket PacketRecv;

	FGameServerInfo Info;
	Info.URL = GetWorld()->URL;
	Info.ProcID = FPlatformProcess::GetCurrentProcessId();

	Packet.GameServerInfos.Add(Info);
	Packet.Pack(byteArray);
	PacketRecv.UnPack(byteArray);
	*/
	CommmaPersistentMaps.ParseIntoArray(PersistentMaps, TEXT(","));
	FGuid Guid = FGuid::NewGuid();
	//PersistentMaps[index];
	FString Param = FString::Format(TEXT("{0} -GUID={1}"), { PersistentMaps[0], Guid.ToString() });

	ConnectToMaster();
}



void AManagerGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	USocketServerBPLibrary::socketServerBPLibrary->stopTCPServer(MasterServerID);
	USocketServerBPLibrary::socketServerBPLibrary->stopTCPServer(AgentServerID);


	SocketClient->closeSocketClientConnectionTCP(ConnectionID);
	Super::EndPlay(EndPlayReason);
}

/*
	
*/
void AManagerGameMode::OnServerConnection(const EServerSocketConnectionEventType type, const bool success, const FString message, const FString sessionID, const FString serverID) 
{
	UE_LOG(LogTemp, Display, TEXT("%s success=%d %s ServerID=%s"), *FString(__FUNCTION__), success , *message , *serverID);
	if (serverID == MasterServerID )				
	{
		//Other -> Master Manager에 접속일경우
		if (success)
		{
			//1. 세션 ID를 보관하고
			AgentServerSessionIDs.Add(sessionID);
		}
		else if (!sessionID.IsEmpty())
		{
			AgentServerSessionIDs.Remove(sessionID);
		}
	}
	else if (serverID ==  AgentServerID)			//GameServer -> Observer
	{
		if (success)
		{
			GameServerSessionIDs.Add(sessionID);
		}
		else if (!sessionID.IsEmpty())
		{
			GameServerSessionIDs.Remove(sessionID);
		}
	}

}

void AManagerGameMode::OnServerReceive(const FString sessionID, const FString message, const TArray<uint8>& byteArray, const FString serverID) 
{
	UE_LOG(LogTemp, Display, TEXT("%s ServerID=%s Command=%d"), *FString(__FUNCTION__), *serverID, byteArray[0]);
	uint8 Command = byteArray[0];
	if (serverID == MasterServerID)
	{		
		if (Command == COMMAND_NOTIFYGAMESERVER)
		{
			FNotifyGameServerPacket Packet;
			Packet.UnPack(byteArray);

			FGameServerInfo* Info = MasterGameServerInfos.Find(Packet.GameServerInfo.GameServerGUID);
			if (Info != nullptr)
			{
				Info->ProcID = Packet.GameServerInfo.ProcID;	// 배치파일이면 또다른 프로세스일수있다
				Info->Port = Packet.GameServerInfo.Port;
				Info->Status = Packet.GameServerInfo.Status;
				UE_LOG(LogTemp, Display, TEXT("%s Found in MasterGameServerInfos. GUID %s ProcID %d Port %d Status %d"),
				*FString(__FUNCTION__), *Info->GameServerGUID.ToString(), Info->ProcID, Info->Port, Info->Status);
			}
			else
			{
				MasterGameServerInfos.Add(Packet.GameServerInfo.GameServerGUID, Packet.GameServerInfo);
			}
		}		
	}
	else if (serverID == AgentServerID)
	{
		if (Command == COMMAND_NOTIFYGAMESERVER)
		{
			// 게임서버가 자신의 Port,ProcID를 보내면 내부에서 ProcID로 찾아 Connected로 변경한다. 
			FNotifyGameServerPacket Packet;
			Packet.UnPack(byteArray);
			
			FGameServerInfo* Info = AgentGameServerInfos.Find(Packet.GameServerInfo.GameServerGUID);
			if (Info!=nullptr)
			{	
				Info->ProcID = Packet.GameServerInfo.ProcID;	// 배치파일이면 또다른 프로세스일수있다
				Info->Port = Packet.GameServerInfo.Port;
				Info->Status = Packet.GameServerInfo.Status;
				UE_LOG(LogTemp, Display, TEXT("%s Found in AgentGameServerInfos. GUID %s ProcID %d Port %d Status %d"), 
				*FString(__FUNCTION__),*Info->GameServerGUID.ToString(), Info->ProcID, Info->Port, Info->Status);
			}
			else
			{
				AgentGameServerInfos.Add(Packet.GameServerInfo.GameServerGUID,Packet.GameServerInfo);
			}

			// Master에 전달
			SocketClient->socketClientSendTCP(ConnectionID,FString(),byteArray);
		}
	}
}

/*
	Slaver GameManager가 Master GameManager에 연결 이벤트
*/
void AManagerGameMode::OnClientConnection(const bool success, const FString message, const FString clientConnectionIDP)
{
	UE_LOG(LogTemp, Display, TEXT("%s %d %s"), *FString(__FUNCTION__), success, *message);
	if (success)
	{	
		if (!CreateProcess)
		{
			CreateProcess = true;
			CommmaPersistentMaps.ParseIntoArray(PersistentMaps, TEXT(","));
		
			for (TArray<FString>::SizeType index = 0; index < PersistentMaps.Num(); index++)
			{
				CreateGameServerProcess(PersistentMaps[index]);		
			}
		}
	

		// Master에 GameServer 정보 알림
		if (AgentGameServerInfos.Num() > 0)
		{

			/*
			for (const TPair<uint32, FGameServerInfo>& pair : SlaveGameServerInfos)
			{
				//pair.Key;				//pair.Value;
				Packet.GameServerInfos.Add(pair.Value);
			}			
			*/

			for (const TPair<FGuid, FGameServerInfo>& pair : AgentGameServerInfos)
			{
				TArray<uint8> byteArray;
				FNotifyGameServerPacket Packet;
				Packet.GameServerInfo = pair.Value;
				Packet.Pack(byteArray);
				SocketClient->socketClientSendTCP(ConnectionID, FString(), byteArray);
			}
		}
	}
	else
	{		
		ConnectToMaster();	// 다시 시도
	}
}

void AManagerGameMode::OnClientReceive(const FString message, const TArray<uint8>& byteArray, const FString clientConnectionIDP)
{

}

void AManagerGameMode::ConnectToMaster()
{
	SocketClient->connectSocketClientTCP(ManagerIP, ManagerPort, EReceiveFilterClient::E_B, ConnectionID);
}

void AManagerGameMode::Test()
{
	if (GameServerSessionIDs.Num() == 0)
	{
		return;
	}
	int index = 0;
	FString osID = GameServerSessionIDs[index];

	TArray<uint8> byteArray;
	FTest1Packet Packet;
	Packet.Data1 = 3;
	Packet.Data2 = 4;
	Packet.Pack(byteArray);

	SocketServer->socketServerSendTCPMessageToClient(osID,FString("DDD"), byteArray);
}

FGameServerInfo* AManagerGameMode::CreateGameServerProcess(FString MapPath)
{

	// 1. 프로세스 생성 시도
	if (GameServerPath.IsEmpty())
	{
		UE_LOG(LogTemp, Display, TEXT("%s Failed. GameServerPath Empty"), *FString(__FUNCTION__));
		return nullptr;
	}

	FGuid Guid = FGuid::NewGuid();
	
	FString Param = FString::Format(TEXT("{0} \"-GUID={1}\""), { MapPath, Guid.ToString() });
	
	uint32 ProcID = 0;
	FProcHandle ProcHandle = FPlatformProcess::CreateProc(
		*GameServerPath,	// const TCHAR* URL
		*Param,	// const TCHAR* Parms
		false,		// bool bLaunchDetached
		false,		// bool bLaunchHidden
		false,		// bool bLaunchReallyHidden
		&ProcID,	// uint32* OutProcessID
		0,			// PriorityModifier
		*FPaths::GetPath(GameServerPath),			// const TCHAR* OptionalWorkingDirectory
		0,			// void* PipeWriteChild
		0);			// void * PipeReadChild = nullptr

	if (ProcHandle.IsValid() == false)
	{
		UE_LOG(LogTemp, Display, TEXT("%s ProcHandle is invalid."), *FString(__FUNCTION__));
		return nullptr;
	}

	FGameServerInfo Item;
	Item.MapPath = MapPath;
	Item.ProcID = 0;
	Item.ProcHandle = ProcHandle;
	Item.Status = EGameServerStatus::Create;
	Item.GameServerGUID = Guid;
	UE_LOG(LogTemp, Display, TEXT("%s , Guid %d"), *FString(__FUNCTION__), *Guid.ToString());
	return &AgentGameServerInfos.Add(Guid, Item);
	//2. Master GameManager에 게임서버 상태 알림.
	//* 이후 게임서버에서 Observer Port에 접속하여 패킷을 보내 ProcID를 검색하여 Status를 Connected로 변경해야한다.
}

void AManagerGameMode::CloseGameServerProcess(FGuid Guid)
{

}

