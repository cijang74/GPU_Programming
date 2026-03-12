#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// 함수 미리 선언
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// 창 크기 800 x 600
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 셰이더 소스코드 작성
// '점(Vertex)'들의 위치를 계산하는 프로그램
const char* vertexShaderSource = "#version 330 core\n" // OpenGL 3.3 코어 프로필 사용
"layout (location = 0) in vec3 aPos;\n" // 0번 통로로 들어오는 x, y, z 위치 데이터
"layout (location = 1) in vec3 aColor;\n" // 1번 통로로 들어오는 R, G, B 색상 데이터
"out vec3 ourColor;\n" // 다음 단계(프래그먼트 셰이더)로 넘겨줄 색상 변수

"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n" // // 점의 최종 위치를 화면에 출력
"   ourColor = aColor;\n " // 입력받은 색상을 그대로 다음 단계로 토스
"}\0";

// 화면에 찍힐 '픽셀(Fragment)'의 색상을 결정하는 프로그램
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n" // 최종적으로 화면에 칠해질 픽셀의 색상
"in vec3 ourColor;\n" // 이전 단계(버텍스 셰이더)에서 토스받은 색상 데이터

"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, 1.0f);\n" // // 토스받은 색상에 불투명도(1.0f)를 더해서 최종 색상으로 결정
"}\n\0";

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

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);

    // as we only have a single shader, we could also just activate our shader once beforehand if we want to 
    glUseProgram(shaderProgram);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render the triangle
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
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