#version 330 core

uniform vec3 lightColour;
uniform vec3 objectColour;
uniform vec3 lightPosition;
uniform vec3 cameraPosition;

out vec4 fragmentColour;

in vec3 position;
in vec3 fragmentPosition;

void main() 
{ 
    //ambient code
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColour;
	
	//diffuse code
	vec3 norm = normalize(position);
	vec3 lightDirection = normalize(lightPosition - fragmentPosition);
	float diff = max(dot(norm, lightDirection), 0.0);
	vec3 diffuse = diff * lightColour;

	//specular code
	float specularStrength = 0.5;
	vec3 viewDirection = normalize(cameraPosition - fragmentPosition);
	vec3 reflectDirection = reflect(-lightDirection, norm);
	float spec = pow(max(dot(viewDirection, reflectDirection), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColour;

	vec3 finalResult = (ambient + diffuse + specular) * objectColour;
	fragmentColour = vec4(finalResult, 1.0);
}