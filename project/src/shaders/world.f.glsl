#version 120
varying vec4 texcoord;
uniform sampler2D texture;

void main(void) {
  //gl_FragColor = texture2D(texture, vec2(fract(fract(texcoord.w / 16384) * 1024.0f) + float(texcoord.w >= 0x80) / 16.0f, floor(fract(texcoord.w / 16384) * 1024.0f) + float((texcoord.w >= 0x40 && texcoord.w < 0x80) || (texcoord.w >= 0xC0)) / 16.0f));
  if(texcoord.w < 0)
    gl_FragColor = texture2D(texture, vec2((fract(texcoord.x) + texcoord.w) / 16.0, texcoord.z));
  else
    gl_FragColor = texture2D(texture, vec2((fract(texcoord.x + texcoord.z) + texcoord.w) / 16.0, texcoord.y));
}
