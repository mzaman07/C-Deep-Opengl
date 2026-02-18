#version 330 core
out vec4 FragColor;
// refactoring the properites to here 
// the point is to show that different objects have different reactions to light
// some reflect more, others scatter. We can simulate different types of objects.
struct Material {
	vec3 ambient;
	// sampler2D - is opaque type which means we can't instantiate these types but only
	// define in uniforms
	sampler2D diffuse;
	vec3 specular;
	float shininess;
};

// do the same thing for light
struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Material material;
uniform Light light;

// normal for light reflection off surface perpindicular - light diffusion
in vec3 Normal;
// use this position for lighting calculations
in vec3 FragPos;
// texture coords from shader
in vec2 TexCoords;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 viewPos;

// we will not do any inversing matrices is a costly operation for shaders
// we can do this on the CPU instead

// this should highlight the Phong light model 
// In the earlier days of lighting shaders people used to do this calc
// in the vertex shader. The main advantage of this was effeciency since
// there are generally a lot less vertices compared to fragments and it was 
// done less frequently. However the result was interpolated lighting colors and was 
// not realistic unless large amounts of vertices were used. - This was also called Gouraud shading
// The Phong shading gave much smoother results.

void main() {
	vec3 ambient = material.ambient * light.ambient * vec3(texture(material.diffuse, TexCoords));
	// diffusion light
	// Need normalized vectors that keep direction to calc
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);


	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	// specular stuff
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	// 32 is the shininess value of the highlight
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * material.specular);

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}

