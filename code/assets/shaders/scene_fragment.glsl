#version 330 core

const float PI = 3.1415926535897932384626433832795;

uniform int numberOfSpotlights;

uniform vec3 light_color;
uniform vec3 pointLight_Pos;

uniform vec3 zSpotLight_pos;
uniform vec3 zSpotLight_dir;
uniform vec3 zSpotLight_color;

uniform vec3 ySpotLight_pos;
uniform vec3 ySpotLight_dir;
uniform vec3 ySpotLight_color;

uniform vec3 object_color;

const float shading_ambient_strength    = 0.1;
const float shading_diffuse_strength    = 0.6;
const float shading_specular_strength   = 0.3;

uniform float light_cutoff_outer;
uniform float light_cutoff_inner;
uniform float light_near_plane;
uniform float light_far_plane;

uniform vec3 view_position;

uniform sampler2D diffuseTexture;
uniform sampler2D shadow_map;

in vec3 fragment_position;
in vec4 fragment_position_light_space;
in vec3 fragment_normal;

in vec4 gl_FragCoord;

in vec3 vertexColor;
in vec2 vertexUV;
uniform sampler2D textureSampler;

uniform int useTexture = 0;
uniform int useColor = 0;
uniform int useShadow = 0;
uniform int useSpotlightZ = 0;
uniform int useSpotlightY = 0;

uniform float transparency = 1.0f;

out vec4 result;

vec3 calcPointLight(vec3 normal, vec3 fragPos, vec3 viewDir, vec3 color)
{
    vec3 lightDir = normalize(pointLight_Pos - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);    
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // combine results
    vec3 ambient = shading_ambient_strength * color;
    vec3 diffuse = shading_diffuse_strength * diff * color;
    vec3 specular = shading_specular_strength * spec * color;
    return (ambient + diffuse + specular);
}

vec3 calcSpotLight(vec3 spotlightPos, vec3 spotlightdir, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 color)
{
    vec3 lightDir = normalize(spotlightPos - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);	
    // spotlight intensity
    float theta = dot(lightDir, normalize(-spotlightdir)); 
    float epsilon = (light_cutoff_inner - light_cutoff_outer);
    float intensity = clamp((theta - light_cutoff_outer) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = shading_ambient_strength * intensity * color;
    vec3 diffuse = shading_diffuse_strength * diff * intensity * color;
    vec3 specular = shading_specular_strength * spec * intensity * color;
    return (ambient + diffuse + specular);
}

float shadow_scalar() {	
	// perform perspective divide
    vec3 projCoords = fragment_position_light_space.xyz / fragment_position_light_space.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadow_map, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(fragment_normal);
    vec3 lightDir = normalize(pointLight_Pos - fragment_position);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadow_map, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadow_map, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

void main()
{

    vec3 ambient = vec3(0.0f);
    vec3 diffuse = vec3(0.0f);
    vec3 specular = vec3(0.0f);

    float scalar = (useShadow == 0) ? 1.0f : 1.0f - shadow_scalar();
	
    vec4 textureColor = texture(textureSampler, vertexUV);
	
	vec3 norm = normalize(fragment_normal);
	vec3 viewDir = normalize(view_position - fragment_position);
    
	// First calculate point light
	vec3 light = scalar * calcPointLight(norm, fragment_position, viewDir, light_color);
	
	// Then the spotlights
	if (useSpotlightZ == 1)
	{
		light += calcSpotLight(zSpotLight_pos, zSpotLight_dir, norm, fragment_position, viewDir, zSpotLight_color);
	}
	if (useSpotlightY == 1)
	{
		light += calcSpotLight(ySpotLight_pos, ySpotLight_dir, norm, fragment_position, viewDir, ySpotLight_color);
	}
    
	if (useTexture == 0)
	{
		result = vec4(light * object_color, transparency);
	}
	else
	{
		result = textureColor * vec4(light * object_color, transparency);
	}
}

