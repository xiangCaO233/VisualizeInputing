#version 330 core

// 光栅化后的颜色rgb结果(默认线形插值)(类型,名称必须与顶点着色器中声明完全相同)
in vec3 color;
// 光栅化后的材质uv坐标
in vec2 uv;

// 图片采样器(与cpu侧交互设置当前采样材质信息)
uniform sampler2D sampler;

// 最后运算渲染色彩结果(向后输出到渲染管线的显示部分)
out vec4 FragColor;

void main(){
	// 直接使用顶点颜色插值结果作为最终颜色
	// FragColor = vec4(color,1.0f);
	// 从当前采样器指定的材质的光栅化uv坐标结果中采集颜色
	FragColor = texture(sampler,uv);
}
