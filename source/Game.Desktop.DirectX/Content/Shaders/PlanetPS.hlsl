cbuffer CBufferPerFrame
{
	float3 AmbientColor;
	float3 LightColor;
	float3 LightPosition;
	float3 CameraPosition;
}

cbuffer CBufferPerObject
{
	float3 SpecularColor;
	float SpecularPower;
}

Texture2D ColorMap;
Texture2D SpecularMap;
SamplerState TextureSampler;

struct VS_OUTPUT
{
	float4 Position: SV_Position;
	float3 WorldPosition : WORLDPOS;
	float Attenuation : ATTENUATION;
	float2 TextureCoordinate : TEXCOORD;
	float3 Normal : NORMAL;
};

float4 main(VS_OUTPUT IN) : SV_TARGET
{
	float3 viewDirection = normalize(CameraPosition - IN.WorldPosition);
	float3 lightDirection = normalize(LightPosition - IN.WorldPosition);

	float3 normal = normalize(IN.Normal);
	float n_dot_l = dot(normal, lightDirection);
	float3 halfVector = normalize(lightDirection + viewDirection);
	float n_dot_h = dot(normal, halfVector);

	float4 color = ColorMap.Sample(TextureSampler, IN.TextureCoordinate);
	float specularClamp = SpecularMap.Sample(TextureSampler, IN.TextureCoordinate).x;
	float2 lightCoefficients = lit(n_dot_l, n_dot_h, SpecularPower).yz;

	float3 ambient = color.rgb * AmbientColor;
	float3 diffuse = color.rgb * lightCoefficients.x * LightColor * IN.Attenuation;
	float3 specular = min(lightCoefficients.y, specularClamp) * SpecularColor * IN.Attenuation;

	return float4(saturate(ambient + (diffuse + specular)), color.a);
}