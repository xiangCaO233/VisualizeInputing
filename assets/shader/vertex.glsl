#version 330 core

// 当前VAO 描述信息编号0位置的信息输入(顶点坐标)
layout (location = 0) in vec3 vertex_pos;
// 当前VAO 描述信息编号1位置的信息输入(顶点颜色)
layout (location = 1) in vec3 vertex_color;
// 当前VAO 描述信息编号2位置的信息输入(顶点材质uv坐标)
layout (location = 2) in vec2 vertex_uv;

// 输出顶点颜色到渲染管线下一步骤
out vec3 color;
// 输出uv坐标到渲染管线下一步骤
out vec2 uv;

void main(){
	// 赋值全局gl位置(全管线通用)
	gl_Position = vec4(vertex_pos,1.0f);
	// 接收到的顶点颜色向后传输
	color = vertex_color;
	// 接收到的uv坐标向后传输
	uv = vertex_uv;
}
