#version 430

uniform mat4 uProjection;
uniform mat4 uView;

uniform vec3 uQuad1;
uniform vec3 uQuad2;

layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 4) out;

in vec3 vColorPass[];
in float vLifeTimePass[];
in float vSizePass[];
in int vTypePass[];

smooth out vec2 vTexCoord;
flat out vec4 vColorPart;

void main()
{
  if(vTypePass[0] != 0)
  {
    vec3 vPosOld = gl_in[0].gl_Position.xyz;
    float fSize = vSizePass[0];
    mat4 mVP = uProjection*uView;
    
    vColorPart = vec4(vColorPass[0], vLifeTimePass[0]);
       
    vec3 vPos = vPosOld+(-uQuad1-uQuad2)*fSize;
    vTexCoord = vec2(0.0, 0.0);
    gl_Position = mVP*vec4(vPos, 1.0);
    EmitVertex();
    
    vPos = vPosOld+(-uQuad1+uQuad2)*fSize;
    vTexCoord = vec2(0.0, 1.0);
    gl_Position = mVP*vec4(vPos, 1.0);
    EmitVertex();
    
    vPos = vPosOld+(uQuad1-uQuad2)*fSize;
    vTexCoord = vec2(1.0, 0.0);
    gl_Position = mVP*vec4(vPos, 1.0);
    EmitVertex();
    
    vPos = vPosOld+(uQuad1+uQuad2)*fSize;
    vTexCoord = vec2(1.0, 1.0);
    gl_Position = mVP*vec4(vPos, 1.0);
    EmitVertex();
      
    EndPrimitive();
  }
}