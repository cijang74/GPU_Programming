#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader_s.h"

// 함수 미리 선언
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// 창 크기 800 x 600
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // GLFW 초기화 << OpenGL 기능들 사용하게 하는 라이브러리
    glfwInit();

    // GLFW 창 설정V
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // 버전 3.x 쓸거야
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // 버전 3.3 쓸거야
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 최신 기능들만 사용하도록 설정 (레거시 프로젝트가 아닌 이상 구 기능들은 쓰지 않아도 됨)

    // MAC일 경우 세팅
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw 창 생성
    // SCR_WIDTH x SCR_HEIGHT 해상도 크기로 "LearnOpenGL"라는 이름의 창을 생성하도록 객체 생성
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);

    // 객체 잘 생성되었는지 확인
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate(); // 생성 실패 시 메모리 정리 후 종료
        return -1;
    }

    // 방금 만든 창을 현재 스레드의 메인 작업 영역(Context)로 지정
    glfwMakeContextCurrent(window);

    // 창 크기가 변경될 때 마다 실행할 함수 등록 -> framebuffer_size_callback
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // GLAD 초기화 << 대충 OPENGL 함수들 플랫폼에 맞춰 매칭시켜주는 라이브러리인듯
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 셰이더 컴파일 및 링크 (프로그램 조립하기) -> 맨 위에 텍스트로 적은 쉐이더 프로그램들을 그래픽카드가 읽을 수 있도록 함
    // vertex shader 컴파일
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER); // 버텍스 셰이더 객체 빈 껍데기 생성
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // 빈 껍데기에 아까 쓴 소스코드 채워넣기
    glCompileShader(vertexShader); // 그래픽 카드가 이해할 수 있게 번역(컴파일)

    // 컴파일이 잘 되었는지, 오타는 없는지 확인
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader 컴파일(vertex shader 컴파일 과정과 동일함)
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // 컴파일 에러 체크
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // 셰이더 프로그램 링크(합체)
    unsigned int shaderProgram = glCreateProgram(); // 여러 셰이더를 묶을 '최종 프로그램' 껍데기 생성
    glAttachShader(shaderProgram, vertexShader); // 번역된 버텍스 셰이더 붙이기
    glAttachShader(shaderProgram, fragmentShader); // 번역된 프래그먼트 셰이더 붙이기
    glLinkProgram(shaderProgram); // 두 개를 하나로 연결(Link)해서 최종 파이프라인 완성!

    // 링크 에러 체크
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) 
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // 최종 프로그램에 다 연결했으니, 낱개로 있던 개별 셰이더들은 메모리 확보를 위해 삭제
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 정점 데이터(Vertex Data) 세팅 및 메모리 버퍼(Buffer) 설정
    // 그릴 도형의 정보(위치, 색상 등)를 그래픽 카드 메모리(VRAM)로 보내는 과정

    // 우리가 그릴 삼각형의 3개의 점(Vertex) 정보
    // 한 줄마다 [x, y, z 위치값]과 [R, G, B 색상값]이 순서대로 들어있음
    float vertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 

    };

    // VBO (Vertex Buffer Object): 그래픽 카드 메모리에 데이터를 담아둘 거대한 '택배 상자'
    // VAO (Vertex Array Object): 택배 상자 안의 데이터를 어떻게 읽어야 하는지 적어둔 '설명서'
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO); // VAO(설명서) 1개 생성
    glGenBuffers(1, &VBO); // VBO(택배 상자) 1개 생성

    // 1. 앞으로 할 모든 설정은 이 VAO(설명서)에 기록하겠다고 선언!
    glBindVertexArray(VAO);

    // 2. VBO(택배 상자)를 활성화하고, 아까 만든 vertices(삼각형 데이터)를 그래픽 카드 메모리로 복사해서 집어넣음
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 3. VAO(설명서)에 데이터 읽는 방법 기록하기
    // "0번 통로(위치) 데이터는 3개씩 묶어서 읽고, 전체 한 줄의 길이는 float 6개 크기이며, 시작점은 0번재부터야!"
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); // 0번 통로(위치) 열기

    // "1번 통로(색상) 데이터는 3개씩 묶어서 읽고, 전체 한 줄의 길이는 float 6개 크기이며, 시작점은 float 3개 건너뛴 지점부터야!"
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1); // 1번 통로(색상) 열기

    // 이제 설정 끝! 아까 합체시킨 '최종 셰이더 프로그램'을 사용하겠다고 선언
    glUseProgram(shaderProgram);

    // 더이상 사전에 변수 설정하지 않음
    // float timeValue = glfwGetTime();
    // float brightnessValue = static_cast<float>(sin(timeValue) / 2.0f + 0.5f);
    // int brightnessColorLocation = glGetUniformLocation(shaderProgram , "ourBrightness");
    // glUseProgram(shaderProgram);
    // glUniform1f(brightnessColorLocation, brightnessValue);

    // 4. 렌더 루프 (Render Loop) & 자원 해제
    while (!glfwWindowShouldClose(window))
    {
        processInput(window); // 키보드 입력 체크

        // 화면 지우기 (청록색 배경)
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 이제는 렌더링 과정에서 색과 밝기를 조절함
        float timeValue = glfwGetTime();
        float brightnessValue = static_cast<float>(sin(timeValue) / 2.0f + 0.5f);
        int brightnessColorLocation = glGetUniformLocation(shaderProgram , "ourBrightness");
        glUseProgram(shaderProgram);
        glUniform1f(brightnessColorLocation, brightnessValue);

        // 삼각형 그리기!
        // 아까 기록해둔 VAO(설명서)를 다시 꺼내서 활성화
        glBindVertexArray(VAO);

        // 그려라! (삼각형 모드로, 0번째 점부터 시작해서, 총 3개의 점을)
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window); // 더블 버퍼링 (화면 깜빡임 방지)
        glfwPollEvents();        // 마우스/키보드 이벤트 처리
    }

    // 루프가 끝나서 프로그램이 종료될 때, 그래픽 카드에 할당했던 메모리(택배 상자, 설명서, 프로그램)를 깨끗하게 지워줌
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

// window 창에서 입력받은 키보드/마우스 입력 처리 함수
void processInput(GLFWwindow* window)
{
    // ESC -> window 닫기 신호를 true로 설정 -> 메인루프 종료
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// 사용자가 마우스로 창 크기를 드래그해서 바꾸면 자동으로 이 함수가 실행됨
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // OpenGL이 그림을 그릴 영역(뷰포트)도 창 크기에 맞춰 동일하게 변경
    glViewport(0, 0, width, height);
}

// 1) uniform을 이용해 RGB로 보간된 삼각형의 밝기가 계속 바뀌도록 해 보세요.
// 2) learnopengl/shader_s.h를 include하고, 별도 셰이더 파일(3.3.shader.vs와 3.3.shader.fs)을 작성하여, 
// learnopengl의 shader class를 사용하는 방식으로 바꿔보세요