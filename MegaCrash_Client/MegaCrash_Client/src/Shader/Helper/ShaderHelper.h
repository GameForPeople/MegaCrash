#pragma once

class CShaderHelper
{
public:
	static constexpr UINT MAX_NUM_PLAYER_DOT{ MAX_PLAYER };
	struct CB_INFO
	{
		float fTimer;
		UINT num_of_players;
		float player_hp;
		UINT padding1;
		XMFLOAT4 PlayerLookVector;
		XMFLOAT4 PlayerPosition;
		XMFLOAT4 OtherPlayerPosition[MAX_NUM_PLAYER_DOT];
	};

public:
	CShaderHelper(ID3D12Device * pd3dDevice);
	~CShaderHelper();

	void UpdateTimer(float fTimeElapsed) { m_Helper.fTimer += fTimeElapsed; }
	void CreateShaderVariables();
	void UpdateShaderVariables(ID3D12GraphicsCommandList* pd3dCmdLst);

	void SetPlayerPosition(const XMFLOAT3& player_position)
	{
		m_Helper.PlayerPosition.x = player_position.x;
		m_Helper.PlayerPosition.y = player_position.y;
		m_Helper.PlayerPosition.z = player_position.z;
	};
	void SetOtherPlayerPosition(const XMFLOAT3& other_player_position, int idx) 
	{
		m_Helper.OtherPlayerPosition[idx].x = other_player_position.x;
		m_Helper.OtherPlayerPosition[idx].y = other_player_position.y;
		m_Helper.OtherPlayerPosition[idx].z = other_player_position.z;
	};
	void SetPlayerLookVector(const XMFLOAT3& player_lookvector)
	{
		m_Helper.PlayerLookVector.x = player_lookvector.x;
		m_Helper.PlayerLookVector.y = player_lookvector.y;
		m_Helper.PlayerLookVector.z = player_lookvector.z;
	}
	void SetPlayerHP(float hp) { m_Helper.player_hp = hp; }

	float GetTimer() const {return m_Helper.fTimer;}
	void SetNumOfPlayers(UINT numofplayers) {m_Helper.num_of_players = numofplayers;}
private:
	CTexture*						m_pTexture;

	CB_INFO							m_Helper;

	CB_INFO*						m_pcbMapped;
	CB_DESC							m_cbDesc;
};