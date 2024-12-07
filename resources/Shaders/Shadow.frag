/*
	Author: IVY
*/

uniform sampler2D texture;
uniform float time;

float rand(float n) 
{ 
    return fract(sin(n) * 43758.5453); 
}

float generateNoise(vec2 coord, float noiseResolution)
{
    coord /= noiseResolution;
    
    vec2 p = floor(coord);
    vec2 f = fract(coord);
    
    f = smoothstep(vec2(0.0), vec2(1.0), f);
    
    float n = p.x + p.y * noiseResolution;
    
    float p1 = mix(rand(n-1.0), rand(n), f.x);
    float p2 = mix(rand(n+(noiseResolution - 1.0)), rand(n+noiseResolution), f.x);
    
    return mix(p1, p2, f.y);
}

void main() 
{
	float cover = 0.05;
	float height = 0.1;

	float noiseResolution = 8.0;
	
	float noise = 0.0;
	float frequency = 0.95;
	float amplify = 0.85;
	
	float speed = (time * noiseResolution) * 0.5;
	
	vec2 position = (gl_FragCoord.xy * height) + speed;
	
	for (int i = 0; i < 8; i++) 
	{
		noise += generateNoise((position * frequency) - speed, noiseResolution) * amplify; 
		frequency *= 2.0;
		amplify *= 0.5;
	}

	noise = noise - cover;
	
	gl_FragColor = 1.1 * texture2D(texture, gl_TexCoord[0].xy) - 0.15 * vec4(noise);
}