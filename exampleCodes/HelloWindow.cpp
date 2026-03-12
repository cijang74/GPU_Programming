#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// 해당 매크로 설정하면 키를 누르고있는 동안 연속해서 값이 변하도록 함
#define ALLOW_REPEAT_KEYSTROKE 1

// 함수 미리 선언
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// 창 크기 800 x 600
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 배경 색 RGB값
float rgb[3] = { 0.2f, 0.3f, 1.0f };

// clamp 함수 만들어서 value를 받으면 min_value과 max_value 사이에서만 값을 뱉도록 조절
void clamp(float &value, float min_value, float max_value)
{
    if (value < min_value) value = min_value;
    if (value > max_value) value = max_value;
}

int main()
{
    // GLFW 초기화 << OpenGL 기능들 사용하게 하는 라이브러리
    glfwInit();

    // GLFW 창 설정
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

    // 렌더링 루프(해당 반복문에서 실제로 화면에 그리는 과정이 진행됨)
    while (!glfwWindowShouldClose(window))
    {
        // window창에서 키보드/마우스 입력 감지 및 반응 (input)
        processInput(window);

        glClearColor(rgb[0], rgb[1], rgb[2], 1.0f); // 화면을 지울 색상 결정
        glClear(GL_COLOR_BUFFER_BIT); // 화면의 색상 버퍼를 싹 덮어씌움

        // glfw: 버퍼(뒷면 도화지라고 생각하면 편함)에 그림이 다 그려졌으면, 프론트 버퍼(실제 window 창)과 교체하여 화면에 보여줌 (더블 버퍼링)
        glfwSwapBuffers(window);

        // 키보드/마우스에 입력이 발생하였는지 확인하고 그에 맞는 콜백 함수 실행
        glfwPollEvents();
    }

    // glfw: 렌더링 다 끝났으면 메모리 정리 후 종료
    glfwTerminate();
    return 0;
}

// window 창에서 입력받은 키보드/마우스 입력 처리 함수
void processInput(GLFWwindow* window)
{
    // ESC -> window 닫기 신호를 true로 설정 -> 메인루프 종료
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

#if ALLOW_REPEAT_KEYSTROKE // 꾹 누르면 발생하도록 하는 매크로 설정되어있으면
    // F1~F6을 누르면 RGB값이 달라짐
    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
    {
        rgb[0] += 0.001f;
        clamp(rgb[0], 0.0f, 1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
    {
        rgb[0] -= 0.001f;
        clamp(rgb[0], 0.0f, 1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
    {
        rgb[1] += 0.001f;
        clamp(rgb[1], 0.0f, 1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS)
    {
        rgb[1] -= 0.001f;
        clamp(rgb[1], 0.0f, 1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS)
    {
        rgb[2] += 0.001f;
        clamp(rgb[2], 0.0f, 1.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_F6) == GLFW_PRESS)
    {
        rgb[2] -= 0.001f;
        clamp(rgb[2], 0.0f, 1.0f);
    }

#else // 꾹 누르면 발생하도록 하는 매크로 설정되어있지 않아있다면
    static bool pressed_f1_f6[6] = {false, }; // F1~F6 키가 눌려있는 상태 기억

    for (int i = 0; i < 6; i++)
    {
        // 키가 눌린 상태라면 true로 저장
        if (glfwGetKey(window, GLFW_KEY_F1 + i) == GLFW_PRESS)
            pressed_f1_f6[i] = true;
  
        // 키를 눌렀다가 뗀 순간이거나, 이전 상태가 눌러져있던 상태라면
        if (glfwGetKey(window, GLFW_KEY_F1 + i) == GLFW_RELEASE && pressed_f1_f6[i] == true)
        {
            // RGB값 변경
            if (i % 2 == 0) rgb[i / 2] += 0.1f;
            else rgb[i / 2] -= 0.1f;
            clamp(rgb[i / 2], 0.0f, 1.0f);
            pressed_f1_f6[i] = false;

            printf("backgroundColor : %.2f %.2f %.2f\n", rgb[0], rgb[1], rgb[2]);
        }
    }
#endif
}

// 사용자가 마우스로 창 크기를 드래그해서 바꾸면 자동으로 이 함수가 실행됨
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // OpenGL이 그림을 그릴 영역(뷰포트)도 창 크기에 맞춰 동일하게 변경
    glViewport(0, 0, width, height);
}