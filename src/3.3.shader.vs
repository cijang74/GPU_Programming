// '점(Vertex)'들의 위치를 계산하는 프로그램
#version 330 core
layout (location = 0) in vec3 aPos; // 0번 통로로 들어오는 x, y, z 위치 데이터
layout (location = 1) in vec3 aColor; // 1번 통로로 들어오는 R, G, B 색상 데이터
out vec3 ourColor; // 다음 단계(프래그먼트 셰이더)로 넘겨줄 색상 변수

void main()
{
    gl_Position = vec4(aPos, 1.0); // // 점의 최종 위치를 화면에 출력
    ourColor = aColor; // 입력받은 색상을 그대로 다음 단계로 토스
};