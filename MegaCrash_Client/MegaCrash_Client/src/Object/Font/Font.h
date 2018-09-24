//------------------------------------------------------- ----------------------
// File: Font.h
//-----------------------------------------------------------------------------
#pragma once
#include "Object/Object.h"

struct FontChar
{
	// 유니코드 아이디값
	int id;

	// 텍스쳐 좌표로 변환필요 , 0~1 사이
	// uv 좌표
	float u;
	float v;
	// 텍스쳐에서의 폰트 너비와 높이
	float twidth;
	float theight;

	// 스크린 좌표계상에서의 폰트의 높이와 너비
	float width;
	float height;

	// 폰트 사이즈를 기반으로 정규화되어야한다.
	float xoffset; // 현재 커서 위치에서 오른쪽으로 얼마나 떨어져 있는가
	float yoffset; // 문장의 윗부분에서부터 얼마나 글씨가 떨어져있나
	float xadvance; // 다음 글자까지 얼마나 떨어져 있는가
};

struct FontKerning
{
	int firstid; // 첫번째 글자의 아이디
	int secondid; // 두번째 글자의 아이디
	float amount; // 두번째 글자가 얼마나 추가적으로 떨어지는지, 또는 붙이는지
};

class CFont : public CGameObject
{
public:
	static constexpr wchar_t* DefaultFontPath{ L"../../../Assets/Textures/Font/Default32.fnt" };

	struct VertexController
	{
		CTextVertex& textvertex;
		VertexController(CTextVertex& vertex) : textvertex(vertex) {}
	};

private:
	std::wstring						name;			// 폰트 이름
	std::wstring						fontImage;
	int									size;			// 폰트 사이즈,
	float								lineHeight;		// 다음줄과 떨어진 높이
	float								baseHeight;		// 모든 글자의 높이
	int									textureWidth;	// 폰트 텍스쳐의 가로 길이
	int									textureHeight;	// 폰트 텍스쳐의 세로 길이
	int									numCharacters;	// 폰트 글자 갯수
	FontChar*							CharList;		// 글자 리스트
	int									numKernings;	// 글자 사이 간격 수
	FontKerning*						KerningsList;	// 커닝값 보유 목록

	// 얼마나 글자간 패딩이 이루어져 있는지?
	// 텍스쳐상에서 양 옆에 있는 다른 글자의 일부가 잘린상태로 렌더링되는것을 방지하고자 함
	float								leftpadding;
	float								toppadding;
	float								rightpadding;
	float								bottompadding;

	UINT								m_nControllers;
	VertexController**					m_ppControllers;

	UINT								m_currentVertexIndex;

public:
	CFont(
		  ID3D12Device*					pd3dDevice
		, ID3D12GraphicsCommandList*	pd3dCmdLst
		, eTex::Font					eTexture
		, LPCWSTR						filename
		, UINT							maxControlVertex
		, int							windowWidth
		, int							windowHeight);
	virtual ~CFont();

	void LoadFont(LPCWSTR filename, int windowWidth, int windowHeight);

	float		GetKerning(wchar_t first, wchar_t second);	// 두 글자간 사이 간격을 반환
	FontChar*	GetChar(wchar_t c);							// wchar_t와 대응되는 FontChar를 반환.

	// 매 프레임 처음에 호출할 것.
	void ResetTextData();
	void AppendTextData(std::wstring	text
		, XMFLOAT2						pos
		, XMFLOAT2						scale = XMFLOAT2(1.0f, 1.0f)
		, XMFLOAT2						padding = XMFLOAT2(0.5f, 0.0f)
		, XMFLOAT4						color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	virtual void Render(
		  ID3D12GraphicsCommandList*	pd3dCmdLst
		, CCamera*						pCamera);
};