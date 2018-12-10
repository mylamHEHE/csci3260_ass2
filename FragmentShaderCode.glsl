#version 430 //GLSL version your computer supports

out vec3 daColor;

in vec3 normalWorld;
in vec3 vertexPositionWorld;
in vec2 UV;

uniform vec3 lightPositionWorld;
uniform vec3 eyePositionWorld;
uniform sampler2D myTextureSampler;

uniform vec3 DiffuseLightColor;
uniform vec3 SpecularLightColor;

void main()
{
	vec3 MaterialAmbientColor = texture( myTextureSampler, UV ).rgb;
	vec3 MaterialDiffuseColor = texture( myTextureSampler, UV ).rgb;
	vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3)*3;

	vec3 AmbientLightColor = vec3(0.1f, 0.1f, 0.1f);

	vec3 lightVectorWorld = normalize(lightPositionWorld - vertexPositionWorld);
	float DiffuseBrightness = clamp(dot(lightVectorWorld, normalize(normalWorld)), 0.0, 1.0);

	
	vec3 reflectedLightVectorWorld = reflect(-lightVectorWorld, normalWorld);
	vec3 eyeVectorWorld = normalize(eyePositionWorld - vertexPositionWorld);
	
	float SpecularBrightness = clamp(dot(reflectedLightVectorWorld, eyeVectorWorld),0,1);

	daColor = MaterialAmbientColor * AmbientLightColor + 
			MaterialDiffuseColor * DiffuseLightColor * DiffuseBrightness+
			MaterialSpecularColor * SpecularLightColor * pow(SpecularBrightness,50);
}
