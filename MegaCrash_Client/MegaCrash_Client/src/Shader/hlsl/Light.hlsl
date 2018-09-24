//--------------------------------------------------------------------------------------
#define MAX_LIGHTS			8 
#define MAX_MATERIALS		64 

#define POINT_LIGHT			1
#define SPOT_LIGHT			2
#define DIRECTIONAL_LIGHT	3

#define _WITH_LOCAL_VIEWER_HIGHLIGHTING
#define _WITH_THETA_PHI_CONES
//#define _WITH_REFLECT
#define _WITH_COOK_TORRANCE

struct MATERIAL
{
	float4				m_cAmbient;
	float4				m_cDiffuse;
	float4				m_cSpecular; //a = power
	float4				m_cEmissive;
};

struct LIGHT
{
	float4				m_cAmbient;
	float4				m_cDiffuse;
	float4				m_cSpecular;
	float3				m_vPosition;
	float 				m_fFalloff;
	float3				m_vDirection;
	float 				m_fTheta; //cos(m_fTheta)
	float3				m_vAttenuation;
	float				m_fPhi; //cos(m_fPhi)
	bool				m_bEnable;
	int 				m_nType;
	float				m_fRange;
	float				padding;
};

cbuffer cbMaterial : register(b0)
{
	MATERIAL			gMaterials[MAX_MATERIALS];
};

cbuffer cbLights : register(b1)
{
	LIGHT				gLights[MAX_LIGHTS];
	float4				gcGlobalAmbientLight;
};

// Schlick gives an approximation to Fresnel reflectance
// (see pg. 233 "Real-Time Rendering 3rd Ed.").
// R0 = ( (n-1)/(n+1) )^2, where n is the index of refraction.
float4 SchlickFresnel(float4 R0, float3 normal, float3 lightVec)
{
    float cosIncidentAngle = saturate(dot(normal, lightVec));

    float f0 = 1.0f - cosIncidentAngle;
    float f0f0 = f0 * f0;
    float4 reflectPercent = R0 + (1.0f - R0) * (f0f0 * f0f0 * f0);

    return reflectPercent;
}


float4 BDRFDirectionalLight(int nIndex, float3 vPosition, float3 vNormal, float3 vToCamera, float4 diffuseColor, float4 specularColor, float glossinessColor, float4 ReflectionColor)
{
    float4 ResultColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

    float3 vToLight = -gLights[nIndex].m_vDirection;
    float fDiffuseFactor = dot(vToLight, vNormal);

    //---------------

    float3 halfVec = normalize(vToCamera + vToLight);

    //---------------

    float Roughness = 1 - glossinessColor;
    float m = glossinessColor.r * 512.f;


    float roughnessFactor = (m + 8.0f) * pow(max(dot(halfVec, vNormal), 0.0f), m) / 8.0f;
    float4 fresnelFactor = SchlickFresnel(specularColor, halfVec, vToLight);

    //---------------
    
    float fSpecularFactor = 0.0f;
    float4 specAlbedo = float4(0.0f, 0.0f, 0.0f, 0.0f);

    if (fDiffuseFactor > 0.0f)
    {
        specAlbedo = fresnelFactor * roughnessFactor;
        specAlbedo = specAlbedo / (specAlbedo + 1.0f) ;
    }
    else
    {
        fDiffuseFactor = 0.0f;
    }

    float4 ResultReflectColor = glossinessColor * fresnelFactor * float4(ReflectionColor.rgb, 1);
    
    ResultColor += gLights[nIndex].m_cAmbient;
    ResultColor += (gLights[nIndex].m_cDiffuse * fDiffuseFactor * diffuseColor);
    ResultColor += (gLights[nIndex].m_cSpecular * specAlbedo);
    ResultColor += lerp(ResultReflectColor, ResultReflectColor* ResultColor, 0.1);
    return ResultColor;


}

float4 BDRFPointLight(int nIndex, float3 vPosition, float3 vNormal, float3 vToCamera, float4 diffuseColor, float4 specularColor, float glossinessColor, float4 ReflectionColor)
{
    float3 vToLight = gLights[nIndex].m_vPosition - vPosition;
    float fDistance = length(vToLight);
    
    float Roughness = 1 - glossinessColor;
    float4 specAlbedo = float4(0.0f, 0.0f, 0.0f, 0.0f);

    if (fDistance <= gLights[nIndex].m_fRange)
    {
        vToLight /= fDistance;
        float fDiffuseFactor = dot(vToLight, vNormal);
        if (fDiffuseFactor > 0.0f)
        {
            float3 halfVec = normalize(vToCamera + vToLight);

            float m = glossinessColor.r * 512.f;

            float roughnessFactor = (m + 8.0f) * pow(max(dot(halfVec, vNormal), 0.0f), m) / 8.0f;
            float4 fresnelFactor = SchlickFresnel(specularColor, halfVec, vToLight);

            specAlbedo = fresnelFactor * roughnessFactor;
            specAlbedo = specAlbedo / (specAlbedo + 1.0f);
        }
        else
        {
            fDiffuseFactor = 0.0f;
        }

        float fAttenuationFactor = 1.0f / dot(gLights[nIndex].m_vAttenuation, float3(1.0f, fDistance, fDistance * fDistance));

        return (gLights[nIndex].m_cAmbient 
            + (gLights[nIndex].m_cDiffuse * fDiffuseFactor * diffuseColor)
            + (gLights[nIndex].m_cSpecular * specAlbedo)
            ) * fAttenuationFactor;
    }
    return (float4(0.0f, 0.0f, 0.0f, 0.0f));
}

float4 BDRFSpotLight(int nIndex, float3 vPosition, float3 vNormal, float3 vToCamera, float4 diffuseColor, float4 specularColor, float glossinessColor, float4 ReflectionColor)
{
    float3 vToLight = gLights[nIndex].m_vPosition - vPosition;
    float fDistance = length(vToLight);

    float Roughness = 1 - glossinessColor;
    float4 specAlbedo = float4(0.0f, 0.0f, 0.0f, 0.0f);

    if (fDistance <= gLights[nIndex].m_fRange)
    {
        vToLight /= fDistance;
        float fDiffuseFactor = dot(vToLight, vNormal);
        if (fDiffuseFactor > 0.0f)
        {
            float3 halfVec = normalize(vToCamera + vToLight);

            float m = glossinessColor.r * 512.f ;

            float roughnessFactor = (m + 8.0f) * pow(max(dot(halfVec, vNormal), 0.0f), m) / 8.0f;
            float4 fresnelFactor = SchlickFresnel(specularColor, halfVec, vToLight);

            specAlbedo = fresnelFactor * roughnessFactor;
            specAlbedo = specAlbedo / (specAlbedo + 1.0f) ;
        }
        else
        {
            fDiffuseFactor = 0.0f;
        }
#ifdef _WITH_THETA_PHI_CONES
        float fAlpha = max(dot(-vToLight, gLights[nIndex].m_vDirection), 0.0f);
        float fSpotFactor = pow(max(((fAlpha - gLights[nIndex].m_fPhi) / (gLights[nIndex].m_fTheta - gLights[nIndex].m_fPhi)), 0.0f), gLights[nIndex].m_fFalloff);
#else
		float fSpotFactor = pow(max(dot(-vToLight, gLights[i].m_vDirection), 0.0f), gLights[i].m_fFalloff);
#endif
        float fAttenuationFactor = 1.0f / dot(gLights[nIndex].m_vAttenuation, float3(1.0f, fDistance, fDistance * fDistance));

        return (gLights[nIndex].m_cAmbient 
            + (gLights[nIndex].m_cDiffuse * fDiffuseFactor * diffuseColor)
            + (gLights[nIndex].m_cSpecular * specAlbedo)
            ) * fAttenuationFactor * fSpotFactor;
    }
    return (float4(0.0f, 0.0f, 0.0f, 0.0f));
}

float4 BRDFLighting(
       float3       vPosition
     , float3       vNormal
     , float4       diffuseColor
     , float4       specularColor
     , float        glossiness
     , float4       ReflectionColor
     , float        shadowFactor)
{
    float3 vCameraPosition = gvCameraPosition.xyz;
    float3 vToCamera = normalize(vCameraPosition - vPosition);
    float3 viewDir = normalize(vPosition - vCameraPosition);

    float4 cColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

    for (int i = 0; i < MAX_LIGHTS; i++)
    {
        if (gLights[i].m_bEnable)
        {
            if (gLights[i].m_nType == DIRECTIONAL_LIGHT)
            {
                float4 clr = BDRFDirectionalLight(i, vPosition, vNormal, vToCamera, diffuseColor, specularColor, glossiness, ReflectionColor);
                cColor += lerp(clr, shadowFactor * clr, 0.7f);
            }
            else if (gLights[i].m_nType == POINT_LIGHT)
            {
                cColor += BDRFPointLight(i, vPosition, vNormal, vToCamera, diffuseColor, specularColor, glossiness, ReflectionColor);
            }
            else if (gLights[i].m_nType == SPOT_LIGHT)
            {
                cColor += BDRFSpotLight(i, vPosition, vNormal, vToCamera, diffuseColor, specularColor, glossiness, ReflectionColor);
            }
        }
    }
    
    cColor += diffuseColor * (gcGlobalAmbientLight * gMaterials[gnMaterial].m_cAmbient);
    cColor.a = diffuseColor.a;

    return saturate(cColor);
}