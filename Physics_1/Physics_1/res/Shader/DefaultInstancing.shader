#vertex

#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in vec4 vertexColor;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;
out vec4 VertexColor;

uniform vec3 textureTiling;  //x, y

uniform mat4 instancedModels[115];

uniform mat4 projection;
uniform mat4 view;
uniform mat4 inverseModel;

void main()
{
	//gl_Position = projection * view * model * vec4(position, 1);
	//TexCoord = vec2(texCoord.x * textureTiling.x, texCoord.y * textureTiling.y);
	
	//vec4 worlNormal = inverseModel * vec4(normal, 1.0f);
	//Normal = normalize(worlNormal.xyz);
	//Normal = normal;
	
	//FragPos = vec3(model * vec4(position, 1.0f));
	VertexColor = vertexColor;

	mat4 model = instancedModels[gl_InstanceID];
    gl_Position = projection * view * model * vec4(position, 1);
    TexCoord = vec2(texCoord.x * textureTiling.x, texCoord.y * textureTiling.y);
    
    Normal = mat3(transpose(inverse(model))) * normal;
    FragPos = vec3(model * vec4(position, 1.0f));
    VertexColor = vertexColor;

};


#fragment

#version 330 core


out vec4 color;

in vec4 VertexColor;
in vec3 Normal;
in vec2 TexCoord;

uniform float alphaCutoffThreshold;

uniform vec4 solidColor;

uniform sampler2D texture_opacity;


void main()
{
	
	float alphaCutOut = texture(texture_opacity, TexCoord).r;

	if(alphaCutOut < alphaCutoffThreshold)
	{
		discard;
	}

    color = vec4(solidColor.xyz, 1.0);
	
};

