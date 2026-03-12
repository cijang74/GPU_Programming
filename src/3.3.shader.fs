// 화면에 찍힐 '픽셀(Fragment)'의 색상을 결정하는 프로그램
#version 330 core
out vec4 FragColor; // 최종적으로 화면에 칠해질 픽셀의 색상
in vec3 ourColor; // 이전 단계(버텍스 셰이더)에서 토스받은 색상 데이터
uniform float ourBrightness; // 매 초 마다 밝기 조절하기 위해 생성한 변수

void main()
{
   FragColor = ourBrightness*vec4(ourColor, 1.0f); // // 토스받은 색상에 불투명도(1.0f)를 더해서 최종 색상으로 결정
};