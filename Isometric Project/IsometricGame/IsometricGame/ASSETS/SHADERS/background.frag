#ifdef GL_ES
precision mediump float;
#endif

#extension GL_OES_standard_derivatives : enable

uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;
const float aoinParam1 = 0.7;

float stars(vec2 uv,float scale)
{
	uv.x+= sin(time+scale)*.01;
	float w=smoothstep(9.,0.,-uv.y*(scale/10.));
	if(w<.1)return 0.;
	uv+=(time*aoinParam1)/scale;uv.y+=time*0./scale;
	uv.x+=sin(uv.y+time*.05)/scale;
	uv*=scale;
	vec2 s=floor(uv),f=fract(uv),p;
	float k=3.,d;
	p=.5+.35*sin(53.*fract(sin((s+p+scale)*mat2(7,3,6,5))*5.))-f;d=length(p);k=min(d,k);
	k=smoothstep(0.,k,sin(f.x+f.y)*0.007);
    return k*w;
}

void main(void){
	vec2 uv=(gl_FragCoord.xy*1.-resolution.xy)/min(resolution.x,resolution.y); 
	vec3 finalColor=vec3(0);
	float c=smoothstep(0.68,0.,clamp(uv.y*.2+.8,0.,.85));
	c+=stars(uv,6.);
	finalColor=(vec3(c));
	gl_FragColor = vec4(finalColor,1);
}