#version 110

attribute vec2 position;
attribute vec2 texcoord;

varying vec2 texcoord_;

void main()
{
        gl_Position = vec4(position, 0.0, 1.0);
        texcoord_ = texcoord;
}