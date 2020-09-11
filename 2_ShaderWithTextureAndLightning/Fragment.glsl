#version 130
varying vec3 normal, lightDir, halfVector;
uniform sampler2D tex; //set 2D Texture@Frament Shader //추가
uniform sampler2D normal_tex;
void main() {
	vec4 color;
	vec3 n, h, t, b ;
	float NdotL, NdotH;
	
	//Coupute TBN Matrix
	n = normalize(normal);
	b = normalize(vec3(0,0,1));
	t = normalize(cross(b, n));
	b = cross(t, n);
	mat3 TBN_Matrix = transpose(mat3(t,b,n));

	n = (2*texture2D(normal_tex,gl_TexCoord[0].st).rgb-vec3(1.0,1.0,1.0))*TBN_Matrix;
	
	//Compute Lights	
	color = gl_FrontMaterial.ambient * gl_LightSource[0].ambient +gl_FrontMaterial.ambient * gl_LightModel.ambient;
	
	NdotL = max(dot(n,lightDir),0.0);
	if (NdotL > 0.0) {
		
		color += gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse * NdotL;
		h = normalize(halfVector);
		NdotH = max(dot(n,h),0.0);
		color += gl_FrontMaterial.specular * gl_LightSource[0].specular *pow(NdotH, gl_FrontMaterial.shininess);
	}

	gl_FragColor = color;	
}