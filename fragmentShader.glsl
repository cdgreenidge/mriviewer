#version 110

uniform sampler2D texture;
uniform vec4 overlayColor;
uniform float overlayAlpha;

varying vec2 texcoord_;

void main()
{
	gl_FragColor = mix(texture2D(texture, texcoord_),
		           overlayColor, overlayAlpha);
}