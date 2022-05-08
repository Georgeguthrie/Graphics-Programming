//Version number
#version 400
//Layout Qualifer
layout( location = 0 ) out vec4 fragcolor;
//Uniform variable

uniform sampler2D texture;
in vec2 TexCoords;
 
void main()
{
//Setting each vector component to uniform varaible then setting final colour

    fragcolor = texture2D(texture, TexCoords);
}