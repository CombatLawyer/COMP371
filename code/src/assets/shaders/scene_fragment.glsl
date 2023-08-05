#version 330 core

const float PI = 3.1415926535897932384626433832795;

uniform vec3 light_color;
uniform vec3 light_position;
uniform vec3 light_direction;

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
uniform int useSpotlight = 0;
uniform float transparency = 1.0f;

out vec4 result;

vec3 ambient_color(vec3 light_color_arg) {
    return shading_ambient_strength * light_color_arg;
}

vec3 diffuse_color(vec3 light_color_arg, vec3 light_position_arg) {
    vec3 light_direction = normalize(light_position_arg - fragment_position);
    return shading_diffuse_strength * light_color_arg * max(dot(normalize(fragment_normal), light_direction), 0.0f);
}

vec3 specular_color(vec3 light_color_arg, vec3 light_position_arg) {
    vec3 light_direction = normalize(light_position_arg - fragment_position);
    vec3 view_direction = normalize(view_position - fragment_position);
    vec3 reflect_light_direction = reflect(-light_direction, normalize(fragment_normal));
    return shading_specular_strength * light_color_arg * pow(max(dot(reflect_light_direction, view_direction), 0.0f),32);
}

float spotlight_scalar() {
    float theta = dot(normalize(light_position - fragment_position), normalize(-light_direction));
    float epsilon = (light_cutoff_inner - light_cutoff_outer);
    float intensity = clamp((theta - light_cutoff_outer) / epsilon, 0.0, 1.0);
	return intensity;
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
    vec3 lightDir = normalize(light_position - fragment_position);
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
	scalar = (useSpotlight == 0) ? scalar * 1.0f : scalar * spotlight_scalar();
    ambient = ambient_color(light_color);
    diffuse = scalar * diffuse_color(light_color, light_position);
    specular = scalar * specular_color(light_color, light_position);
	
    vec4 textureColor = texture(textureSampler, vertexUV);
    vec3 light = (specular + diffuse + ambient);
    
	if (useTexture == 0)
	{
		result = vec4(light * object_color, transparency);
	}
	else
	{
		result = textureColor * vec4(light * object_color, transparency);
	}
}

