#version 330

in vec3 ver_color;
in vec3 ver_normal;

out vec4 frag_color;

void main(void)
{
    //灯的位置
    vec3 lightPos = vec3(-2.0f,0.5f,10.0f);
    //漫反射反射光的亮度只和入射角度有关,小于0的光强无意义。
    float val = max(0.0, dot(normalize(ver_normal), normalize(lightPos)));
    
    frag_color = vec4(ver_color*val, 1.0);
}

