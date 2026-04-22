#version 410 core

in vec3 vColor;
in vec2 vUV;

out vec4 FragColor;

uniform sampler2D uTexture;
uniform float uTextureBlend;

void	main()
{
	vec4 textureColor = texture(uTexture, vUV);
	vec3 finalColor = mix(vColor, textureColor.rgb, uTextureBlend);
	FragColor = vec4(finalColor, 1.0);
}
