#pragma once

#define MAX_BUFF_SIZE		4096
#define MAX_PACKET_SIZE		255

#define INVALID_VALUE		-1

#define INVALID_OBJECT_ID	-1
#define START_PLAYER_ID		0
#define START_CUBE_ID		1'000'000
#define START_BULLET_ID		2'000'000
#define START_STAGE_ID		3'000'000

#define MAX_HP				100
#define MAX_SP				100
#define MAX_CP				180

#define	GAMEEND_KILLCOUNT	2

#define DMG_BRICK			15
#define DMG_BULLET			25 
#define DMG_NORMAL			50
#define DMG_SKILL			80
#define DMG_GROUND			100

#define DMGTHRESHOLD_CUB		100000
#define DMGTHRESHOLD_GRD_SOFT	800000
#define DMGTHRESHOLD_GRD_HARD	3000000


namespace MSGTYPE {
	enum MSGSTATE {
		CONNECT = 0
		, DISCONNECT
	};
	enum MSGACTION {
		MOVE = 10
		, ATTACK
		, SHOOT
		, ANIMATE
		, DAMAGE
		, SKILLUSE
		, SKILLCOOLDOWN
	};
	enum MSGUPDATE {
		CREATE_OBJECT = 20
		, DELETE_OBJECT
		, UPDATE_PLAYER
		, UPDATE_KILLCOUNT
		, UPDATE_OBJECT
		, PLAYER_DIED
		, RESET_OBJECT
	};
	enum MSGROOM {
		CREATEROOM = 30
		, STARTROOM
		, CLOSEROOM
		, ENDGAME
		, ROOMINFO
		, REFRESHROOMINFO
		, ROOM_CLIENTINFO
		, ACCEPTROOM
		, DISCONNECTROOM
		, CLIENT_TYPECHANGE
	};
	enum CHEAT {
		RESTART = 100
	};
}

using MSGWRAPPER = void*;


#pragma pack(push, 1)
struct PK_MSG_CONNECT {
	BYTE		size;
	BYTE		type;
	UINT		id;
};

struct PK_MSG_MOVE {
	BYTE		size;
	BYTE		type;
	BYTE		movetype;			// Invalid 값 통일해야됨
	BYTE		isLanded;
	UINT		id;
	XMFLOAT3	p;
	XMFLOAT4	q;
};

struct PK_MSG_ADD_PLAYER {
	BYTE		size;
	BYTE		type;
	UINT		id;
	WORD		HP;
	BYTE		ObjType;
};

struct PK_MSG_DELETE_PLAYER {
	BYTE		size;
	BYTE		type;
	UINT		id;
};

struct PK_MSG_ATTACK {
	BYTE		size;
	BYTE		type;
	BYTE		atktype;
	UINT		id;
	XMFLOAT3	p;
	XMFLOAT3	d;
};

struct PK_MSG_SKILL {
	BYTE		size;
	BYTE		type;
	UINT		id;
	BYTE		skilltype;
};

struct PK_MSG_SHOOT {
	BYTE		size;
	BYTE		type;
	UINT		shooterid;
	UINT		bulletid;
	BYTE		isSkill;
};

struct PK_MSG_DAMAGE {
	BYTE		size;
	BYTE		type;
	UINT		attackerid;
	UINT		victimid;
	BYTE		atktype;
	WORD		HP;
	XMFLOAT3	p;
};

struct PK_MSG_KILLCOUNT {
	BYTE		size		{ sizeof(PK_MSG_KILLCOUNT) };
	BYTE		type		{ MSGTYPE::MSGUPDATE::UPDATE_KILLCOUNT };
	UINT		attackerid;
	BYTE		GlobalKill;
	BYTE		PlayerKill;
};

struct PK_MSG_STATECHANGE {
	BYTE		size;
	BYTE		type;
	UINT		id;
	WORD		data1;
	WORD		data2;
	WORD		data3;
};

struct PK_MSG_PLAYERDIED {
	BYTE		size		{ sizeof(PK_MSG_PLAYERDIED) };
	BYTE		type		{ MSGTYPE::MSGUPDATE::PLAYER_DIED };
	UINT		id;
	XMFLOAT3	p;
};

struct CHEAT {
	BYTE		size;
	BYTE		type;
};

struct PK_ROOM_CREATE {
	BYTE		size;
	BYTE		type;
};

struct PK_ROOM_INFO {
	BYTE		size;
	BYTE		type;
	UINT		RoomID;
	UINT		ClientID;
	BYTE		ClientType;
};

struct PK_ROOM_START {
	BYTE		size;
	BYTE		type;
	UINT		RoomID;
	UINT		ClientID;
};

struct PK_ROOM_END {
	BYTE		size;
	BYTE		type;
	UINT		RoomID;
	UINT		WinnerID;
};

struct PK_ROOM_REQUEST {
	BYTE		size		{ sizeof(PK_ROOM_REQUEST) };
	BYTE		type		{ MSGTYPE::MSGROOM::REFRESHROOMINFO };
};

#pragma pack(pop)

