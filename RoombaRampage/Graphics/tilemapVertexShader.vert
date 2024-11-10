R"( #version 460 core

layout (location=0) in vec2 vertexPosition;
layout (location=2) in vec2 vertexTexCoords;


uniform mat3 projection;
uniform mat3 view;
uniform int tilemapIndex;
uniform int tilemapRows;
uniform int tilemapColumns;
uniform mat3 modelMatrix;
uniform vec4 modelColor;
uniform int layer;
uniform int tilePicSizeX;
uniform int tilePicSizeY;

layout (location=0) out vec4 color; 
layout (location=1) out vec2 texCoords;

void main()
{

	mat3 matrix = projection * view * modelMatrix;

	float tileWidth = 1.0 / tilePicSizeX;

	float tileHeight = 1.0 / tilePicSizeY;

	int y = tilemapIndex / tilePicSizeX; 

	int x = tilemapIndex % tilePicSizeY;

	float frameXOffset = tileWidth * x ;

	float frameYOffset = tileHeight * y;

	int xStep = gl_InstanceID % tilemapRows;

	int yStep = gl_InstanceID / tilemapRows;

	//vec2 translate = vertexPosition + vec2(xStep,yStep);

	

	gl_Position = vec4(vec2(matrix * vec3(vertexPosition, 1.f)),
					   -0.0001 * layer, 1.0);

	

	//gl_Position.x = gl_Position.x + tileWidth * gl_InstanceID;
	//gl_Position.y = gl_Position.y - gl_Position.y * gl_InstanceID;

	texCoords = vec2(vertexTexCoords.x * tileWidth + frameXOffset, vertexTexCoords.y * tileHeight + frameYOffset);
	
	color = modelColor;
}

)"