/*

   Copyright [2008] [Trevor Hogan]

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

	   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

   CODE PORTED FROM THE ORIGINAL GHOST PROJECT: http://ghost.pwner.org/

*/

#ifndef REPLAY_H
#define REPLAY_H

#include "gameslot.h"
#include <QString>

//
// CReplay
//

class CIncomingAction;

class CReplay : public CPacked
{
public:
	enum BlockID {
		REPLAY_LEAVEGAME		= 0x17,
		REPLAY_FIRSTSTARTBLOCK	= 0x1A,
		REPLAY_SECONDSTARTBLOCK	= 0x1B,
		REPLAY_THIRDSTARTBLOCK	= 0x1C,
		REPLAY_TIMESLOT2		= 0x1E,		// corresponds to W3GS_INCOMING_ACTION2
		REPLAY_TIMESLOT			= 0x1F,		// corresponds to W3GS_INCOMING_ACTION
		REPLAY_CHATMESSAGE		= 0x20,
		REPLAY_CHECKSUM			= 0x22,		// corresponds to W3GS_OUTGOING_KEEPALIVE
		REPLAY_DESYNC			= 0x23
	};

private:
	unsigned char m_HostPID;
	QString m_HostName;
	QString m_GameName;
	QString m_StatString;
	uint32_t m_PlayerCount;
	uint32_t m_MapGameType;
	QVector<PIDPlayer> m_Players;
	QVector<CGameSlot> m_Slots;
	uint32_t m_RandomSeed;
	unsigned char m_SelectMode;				// also known as the "layout style" elsewhere in this project
	unsigned char m_StartSpotCount;
	QQueue<QByteArray> m_LoadingBlocks;
	QQueue<QByteArray> m_Blocks;
	QQueue<uint32_t> m_CheckSums;
	QString m_CompiledBlocks;

public:
	CReplay( );
	virtual ~CReplay( );

	unsigned char GetHostPID( )				{ return m_HostPID; }
	QString GetHostName( )					{ return m_HostName; }
	QString GetGameName( )					{ return m_GameName; }
	QString GetStatString( )					{ return m_StatString; }
	uint32_t GetPlayerCount( )				{ return m_PlayerCount; }
	uint32_t GetMapGameType( )				{ return m_MapGameType; }
	QVector<PIDPlayer> GetPlayers( )			{ return m_Players; }
	QVector<CGameSlot> GetSlots( )			{ return m_Slots; }
	uint32_t GetRandomSeed( )				{ return m_RandomSeed; }
	unsigned char GetSelectMode( )			{ return m_SelectMode; }
	unsigned char GetStartSpotCount( )		{ return m_StartSpotCount; }
	QQueue<QByteArray> *GetLoadingBlocks( )	{ return &m_LoadingBlocks; }
	QQueue<QByteArray> *GetBlocks( )			{ return &m_Blocks; }
	QQueue<uint32_t> *GetCheckSums( )		{ return &m_CheckSums; }

	void AddPlayer( unsigned char nPID, QString nName )		{ m_Players.push_back( PIDPlayer( nPID, nName ) ); }
	void SetSlots( QVector<CGameSlot> nSlots )				{ m_Slots = nSlots; }
	void SetRandomSeed( uint32_t nRandomSeed )				{ m_RandomSeed = nRandomSeed; }
	void SetSelectMode( unsigned char nSelectMode )			{ m_SelectMode = nSelectMode; }
	void SetStartSpotCount( unsigned char nStartSpotCount )	{ m_StartSpotCount = nStartSpotCount; }
	void SetMapGameType( uint32_t nMapGameType )			{ m_MapGameType = nMapGameType; }
	void SetHostPID( unsigned char nHostPID )				{ m_HostPID = nHostPID; }
	void SetHostName( QString nHostName )					{ m_HostName = nHostName; }

	void AddLeaveGame( uint32_t reason, unsigned char PID, uint32_t result );
	void AddLeaveGameDuringLoading( uint32_t reason, unsigned char PID, uint32_t result );
	void AddTimeSlot2( QQueue<CIncomingAction *> actions );
	void AddTimeSlot( uint16_t timeIncrement, QQueue<CIncomingAction *> actions );
	void AddChatMessage( unsigned char PID, unsigned char flags, uint32_t chatMode, QString message );
	void AddLoadingBlock( QByteArray &loadingBlock );
	void BuildReplay( QString gameName, QString statString, uint32_t war3Version, uint16_t buildNumber );

	void ParseReplay( bool parseBlocks );
};

#endif