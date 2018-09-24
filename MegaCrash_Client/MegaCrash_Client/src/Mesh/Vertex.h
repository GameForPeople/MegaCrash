#pragma once

enum class VertexTag 
{
	  Cube
	, Texture
	, Model
	, SkinnedModel
};

class CCubeVertex
{
public:
	XMFLOAT3						m_xmf3Pos;
	XMFLOAT3						m_xmf3Normal;

public:
	CCubeVertex()
		: m_xmf3Pos(XMFLOAT3(0.0f, 0.0f, 0.0f))
		, m_xmf3Normal(XMFLOAT3(0.0f, 0.0f, 0.0f))
	{}
	CCubeVertex(
		const XMFLOAT3& xmf3Pos
		, const XMFLOAT3& xmf3Normal)
		: m_xmf3Pos(xmf3Pos)
		, m_xmf3Normal(xmf3Normal)
	{}
	~CCubeVertex() { }
};

class CTexturedVertex
{
public:
	XMFLOAT3						m_xmf3Pos;
	XMFLOAT2						m_xmf2TexCoord;

public:
	CTexturedVertex()
		: m_xmf3Pos(XMFLOAT3(0.0f, 0.0f, 0.0f))
		, m_xmf2TexCoord(XMFLOAT2(0.0f, 0.0f))
	{}
	CTexturedVertex(
		  const XMFLOAT3& xmf3Pos
		, const XMFLOAT2& xmf2TexCoord)
		: m_xmf3Pos(xmf3Pos)
		, m_xmf2TexCoord(xmf2TexCoord)
	{}
	~CTexturedVertex() { }
};

class CStageVertex
{
public:
	XMFLOAT3						m_xmf3Pos;
	XMFLOAT3						m_xmf3Normal;
	XMFLOAT3						m_xmf3Tangent;
	XMFLOAT2						m_xmf2TexCoord;
	BYTE							m_iMaterialIdx;

public:
	CStageVertex()
		: m_xmf3Pos(XMFLOAT3(0.0f, 0.0f, 0.0f))
		, m_xmf3Normal(XMFLOAT3(0.0f, 0.0f, 0.0f))
		, m_xmf3Tangent(XMFLOAT3(0.0f, 0.0f, 0.0f))
		, m_xmf2TexCoord(XMFLOAT2(0.0f, 0.0f))
		, m_iMaterialIdx(0)
	{}
	CStageVertex(
		  const XMFLOAT3&	xmf3Pos
		, const XMFLOAT3&	xmf3Normal
		, const XMFLOAT3&	xmf3Tangent
		, const XMFLOAT2&	xmf2TexCoord
		, const BYTE		materialidx)
		: m_xmf3Pos(xmf3Pos)
		, m_xmf3Normal(xmf3Normal)
		, m_xmf3Tangent(xmf3Tangent)
		, m_xmf2TexCoord(xmf2TexCoord)
		, m_iMaterialIdx(materialidx)
	{}
	~CStageVertex() {}
};

class CModelVertex
{
public:
	XMFLOAT3						m_xmf3Pos;
	XMFLOAT3						m_xmf3Normal;
	XMFLOAT3						m_xmf3Tangent;
	XMFLOAT2						m_xmf2TexCoord;

public:
	CModelVertex()
		: m_xmf3Pos(XMFLOAT3(0.0f, 0.0f, 0.0f))
		, m_xmf3Normal(XMFLOAT3(0.0f, 0.0f, 0.0f))
		, m_xmf3Tangent(XMFLOAT3(0.0f, 0.0f, 0.0f))
		, m_xmf2TexCoord(XMFLOAT2(0.0f, 0.0f))
	{}
	CModelVertex(
		const XMFLOAT3& xmf3Pos
		, const XMFLOAT3& xmf3Normal
		, const XMFLOAT3& xmf3Tangent
		, const XMFLOAT2& xmf2TexCoord)
		: m_xmf3Pos(xmf3Pos)
		, m_xmf3Normal(xmf3Normal)
		, m_xmf3Tangent(xmf3Tangent)
		, m_xmf2TexCoord(xmf2TexCoord)
	{}
	~CModelVertex() {}
};

class CSkinnedAnimationVertex
{
public:
	XMFLOAT3						m_xmf3Pos;
	XMFLOAT3						m_xmf3Normal;
	XMFLOAT3						m_xmf3Tangent;
	XMFLOAT2						m_xmf2TexCoord;
	XMFLOAT3						m_xmf3BoneWeights;
	BYTE							m_iBoneIndices[4];
	BYTE							m_iMaterialIdx;

public:
	CSkinnedAnimationVertex()
		: m_xmf3Pos(XMFLOAT3(0.0f, 0.0f, 0.0f))
		, m_xmf3Normal(XMFLOAT3(0.0f, 0.0f, 0.0f))
		, m_xmf3Tangent(XMFLOAT3(0.0f, 0.0f, 0.0f))
		, m_xmf2TexCoord(XMFLOAT2(0.0f, 0.0f))
		, m_xmf3BoneWeights(XMFLOAT3())
		, m_iMaterialIdx(0)
	{
		::ZeroMemory(m_iBoneIndices, sizeof(BYTE) * 4);
	}
	CSkinnedAnimationVertex(
		const XMFLOAT3& xmf3Pos
		, const XMFLOAT3& xmf3Normal
		, const XMFLOAT3& xmf3Tangent
		, const XMFLOAT2& xmf2TexCoord
		, const XMFLOAT3& xmf3BoneWeight
		, const BYTE boneidx0
		, const BYTE boneidx1
		, const BYTE boneidx2
		, const BYTE boneidx3
		, const BYTE materialidx)
		: m_xmf3Pos(xmf3Pos)
		, m_xmf3Normal(xmf3Normal)
		, m_xmf3Tangent(xmf3Tangent)
		, m_xmf2TexCoord(xmf2TexCoord)
		, m_xmf3BoneWeights(xmf3BoneWeight)
		, m_iMaterialIdx(materialidx)
	{
		m_iBoneIndices[0] = boneidx0;
		m_iBoneIndices[1] = boneidx1;
		m_iBoneIndices[2] = boneidx2;
		m_iBoneIndices[3] = boneidx3;
	}
	~CSkinnedAnimationVertex() {}
};

class CBillboardVertex
{
public:
	XMFLOAT3						m_xmf3Pos;
	XMFLOAT2						m_xmf2Size;

public:
	CBillboardVertex()
		: m_xmf3Pos(XMFLOAT3(0.0f, 0.0f, 0.0f))
		, m_xmf2Size(XMFLOAT2(0.0f, 0.0f))
	{}
	CBillboardVertex(
		const XMFLOAT3& xmf3Pos
		, const XMFLOAT2& xmf2Size)
		: m_xmf3Pos(xmf3Pos)
		, m_xmf2Size(xmf2Size)
	{}
	~CBillboardVertex() {}
};

class CParticleBillboardVertex
{
public:
	XMFLOAT3						m_xmf3Pos;
	XMFLOAT2						m_xmf2Size;

	XMFLOAT2						m_xmf2CurrTex;
	XMFLOAT2						m_xmf2DividedTexCoord;

public:
	CParticleBillboardVertex()
		: m_xmf3Pos(XMFLOAT3(0.0f, 0.0f, 0.0f))
		, m_xmf2Size(XMFLOAT2(0.0f, 0.0f))
		, m_xmf2CurrTex(XMFLOAT2(0.0f, 0.0f))
		, m_xmf2DividedTexCoord(XMFLOAT2(0.0f, 0.0f))
	{}
	CParticleBillboardVertex(
		const XMFLOAT3& xmf3Pos
		, const XMFLOAT2& xmf2Size
		, const XMFLOAT2& xmf2CurrTex
		, const XMFLOAT2& xmf2DividedTexCoord)
		: m_xmf3Pos(xmf3Pos)
		, m_xmf2Size(xmf2Size)
		, m_xmf2CurrTex(xmf2CurrTex)
		, m_xmf2DividedTexCoord(xmf2DividedTexCoord)
	{}
	~CParticleBillboardVertex() {}
};

class CAdvancedParticleBillboardVertex
{
public:
	XMFLOAT3						m_xmf3Pos;
	XMFLOAT3						m_xmf3Velocity;
	XMFLOAT4						m_xmf4Color;
	XMFLOAT2						m_xmf2Size;
	XMFLOAT3						m_xmf3Opt; // isForcedByGravity , isStretchedBB, isAlignedByMovDir 
	XMFLOAT4						m_xmf4Value; // SpinRate, SeedValue, CurrLife, GravityForce
	XMFLOAT4						m_xmf4CustomColor; // CustomColoe
	XMFLOAT2						m_xmf2SizeMultiplier; // SizeMultiplier
public:
	CAdvancedParticleBillboardVertex()
		: m_xmf3Pos(XMFLOAT3(0.0f, 0.0f, 0.0f))
		, m_xmf3Velocity(XMFLOAT3(0.0f, 0.0f, 0.0f))
		, m_xmf4Color(XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f))
		, m_xmf2Size(XMFLOAT2(0.0f, 0.0f))
		, m_xmf3Opt(XMFLOAT3(0, 0, 0))
		, m_xmf4Value(XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f))
		, m_xmf4CustomColor(XMFLOAT4(1.0f,1.0f,1.0f,1.0f))
		, m_xmf2SizeMultiplier(XMFLOAT2(1.0f,1.0f))
	{}
	
	CAdvancedParticleBillboardVertex(XMFLOAT3 xmf3Pos, XMFLOAT3 xmf3Velocity, XMFLOAT4	 xmf4Color,
	XMFLOAT2	xmf2Size, XMFLOAT3 xmf3Opt, XMFLOAT4 xmf4Value, XMFLOAT4 xmf4CustomColor, XMFLOAT2 xmf2SizeMultiplier) :
		m_xmf3Pos(xmf3Pos)
		, m_xmf3Velocity(xmf3Velocity)
		, m_xmf4Color(xmf4Color)
		, m_xmf2Size(xmf2Size)
		, m_xmf3Opt(xmf3Opt)
		, m_xmf4Value(xmf4Value)
		, m_xmf4CustomColor(xmf4CustomColor)
		, m_xmf2SizeMultiplier(xmf2SizeMultiplier)
	{}
	~CAdvancedParticleBillboardVertex() {};
};

class CTextVertex {
public:
	XMFLOAT4						m_xmf4Pos;
	XMFLOAT4						m_xmf4TexCoord;
	XMFLOAT4						m_xmf4Color;

public:
	CTextVertex()
		: m_xmf4Color(XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f))
		, m_xmf4TexCoord(XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f))
		, m_xmf4Pos(XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f))
	{}

	CTextVertex(
		const XMFLOAT4& xmf4Pos,
		const XMFLOAT4& xmf4TexCoord,
		const XMFLOAT4& xmf4Color
	)
		: m_xmf4Pos(xmf4Pos)
		, m_xmf4TexCoord(xmf4TexCoord)
		, m_xmf4Color(xmf4Color) 
	{}

	void SetVertexData(
		const XMFLOAT4& xmf4Pos,
		const XMFLOAT4& xmf4TexCoord,
		const XMFLOAT4& xmf4Color
	)
	{
		m_xmf4Pos = xmf4Pos;
		m_xmf4TexCoord = xmf4TexCoord;
		m_xmf4Color = xmf4Color;
	}

	~CTextVertex() {}
};