#pragma once
#define MAX_PLAYER			10
#define MAX_HP				100
#define MAX_SP				100
#define MAX_CP				180

#define DMG_BRICK			15
#define DMG_BULLET			25
#define DMG_HAMMER			50

#define DMGTHRESHOLD		350

#define MOVE_TYPE_NORMAL	0
#define MOVE_TYPE_DASH		1
#define MOVE_TYPE_JUMP		2

#define ATK_TYPE_ATTACK1	0
#define ATK_TYPE_ATTACK2	1
#define ATK_TYPE_SKILL		2


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
	BYTE		size{ sizeof(PK_MSG_KILLCOUNT) };
	BYTE		type{ MSGTYPE::MSGUPDATE::UPDATE_KILLCOUNT };
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
	BYTE		size;
	BYTE		type;
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
	BYTE		size{ sizeof(PK_ROOM_REQUEST) };
	BYTE		type{ MSGTYPE::MSGROOM::REFRESHROOMINFO };
};

#pragma pack(pop)