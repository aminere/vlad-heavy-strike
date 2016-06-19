uniform mat4 WorldMatrix = mat4(1.0);
uniform vec4 LightColor = vec4(1.0);

vec4 Light_Point_Diffuse(in vec4 vertPos, in vec4 vertNorm, in vec4 lightPos, in vec4 lightColor, in vec3 lightAttenuation)
{
	vec4 lightDir = lightPos - vertPos;
	float dist = length(lightDir);
	lightDir = lightDir / dist;
	
	// compute distance based attenuation
	// attenuation = 1 / (LA.x + LA.y*dist + LA.z*dist*dist)
	float distAttn = clamp(1.0/(lightAttenuation.x + lightAttenuation.y*dist + lightAttenuation.z*dist*dist), 0.0, 1.0);	
	
	// compute surface/light angle based attenuation
	float angleAttn = clamp(dot(vertNorm, lightDir), 0.0, 1.0);
	
	return lightColor * distAttn * angleAttn;
}

void main()
{
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * WorldMatrix * gl_Vertex;	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	
	vec4 lightPos = vec4(0.0, 0.0, 8.0, 0.0);
	//vec4 lightColor = vec4(1.0, 1.0, 1.0, 1.0);
	vec3 lightAttenuation = vec3(0.0, 0.0, 0.005);
	
	vec4 normal = WorldMatrix * vec4(gl_Normal, 0.0);
	normal = normalize(normal);
	
	vec4 color = Light_Point_Diffuse(WorldMatrix * gl_Vertex, normal, lightPos, LightColor, lightAttenuation);
	gl_FrontColor = color;
}
