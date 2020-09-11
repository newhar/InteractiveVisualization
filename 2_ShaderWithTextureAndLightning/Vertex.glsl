#version 130
varying vec3 normal, lightDir, halfVector;

void main() {
	normal = normalize(gl_NormalMatrix*gl_Normal);
      /*  vertex normal to fragment shader  */
	lightDir = normalize(gl_LightSource[0].position.xyz);
      /*  Light Direction Vector to Fragment shader  */
	halfVector = normalize(gl_LightSource[0].halfVector.xyz);  
      /*  half Vector to Fragment shader  */ 
	gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;
      /*  Projected Position to Fragment shader     */    
		gl_TexCoord[0] = gl_MultiTexCoord0; //추가한 코드
      /*  texture coordinate to Fragment Shader     */    
      /*  vertex position on Clipping Space to Fragment Shader     */  
		
}