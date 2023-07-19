/*
  Contrary to expectation, rendering speed is not different when a
  double is used instead of a float.

  arm64:
    Build application using iTerm2 (arm64):
      $ cd ~/src/clone/apple-m1-opengl-double-precision-issue
      $ rm -rf build
      $ cmake -S . -B build
      $ cmake --build build
      $ file build/main
      build/main: Mach-O 64-bit executable arm64

    When coucou is a double:
      $ build/main
      FALLBACK (log once): Fallback to SW vertex processing because buildPipelineState failed
      FALLBACK (log once): Fallback to SW fragment processing because buildPipelineState failed
      FALLBACK (log once): Fallback to SW vertex processing, m_disable_code: 1000
      FALLBACK (log once): Fallback to SW fragment processing, m_disable_code: 1000000
      FALLBACK (log once): Fallback to SW vertex processing in drawCore, m_disable_code: 1000
      FALLBACK (log once): Fallback to SW fragment processing in drawCore, m_disable_code: 1000000
      8.928571 ms/frame
      9.345794 ms/frame

    When coucou is a float:
      $ build/main
      8.928571 ms/frame
      8.620690 ms/frame

  x86_64 Rosetta:
      $ file build/main
      build/main: Mach-O 64-bit executable x86_64

    When coucou is a double:
      $ build/main
      FALLBACK (log once): Fallback to SW vertex processing because buildPipelineState failed
      FALLBACK (log once): Fallback to SW fragment processing because buildPipelineState failed
      FALLBACK (log once): Fallback to SW vertex processing, m_disable_code: 1000
      FALLBACK (log once): Fallback to SW fragment processing, m_disable_code: 1000000
      FALLBACK (log once): Fallback to SW vertex processing in drawCore, m_disable_code: 1000
      FALLBACK (log once): Fallback to SW fragment processing in drawCore, m_disable_code: 1000000
      10.000000 ms/frame
      10.000000 ms/frame

    When coucou is a float:
      $ build/main
      9.174312 ms/frame
      8.849558 ms/frame
*/

#ifndef __APPLE__
#define __APPLE__
#endif
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

const char *vertexShaderSource =
    "#version 410 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource =
    "#version 410 core\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "   double coucou = 0.5;\n"
    "   color = vec4(1.0f, coucou, 0.2f, 1.0f);\n"
    "}\n\0";

int main()
{

  int success = glfwInit();
  if (!success)
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GLFWwindow *window = glfwCreateWindow(640, 480, "Hello", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (glewInit() != GLEW_OK)
    std::cout << "[GLEW Error][ GLEW could not be initialized ]\n";

  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "[OpenGL Error][ Vertez Shader compilation failed ]\n"
              << infoLog << '\n';
  }

  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "[OpenGL Error][ Fragment Shader compilation failed ]\n"
              << infoLog << '\n';
  }

  unsigned int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "[OpenGL Error][ Shader Program linking failed ]\n"
              << infoLog << '\n';
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};

  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // Initialize timing variables.
  double lastTime = glfwGetTime();
  int nbFrames = 0;
  int interval_sec = 1.0;

  while (!glfwWindowShouldClose(window))
  {
    glClearColor(0.25f, 0.1f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();

    // Measure speed
    // This code is a modification of code at
    // https://www.opengl-tutorial.org/miscellaneous/an-fps-counter/.
    double currentTime = glfwGetTime();
    nbFrames++;
    if (currentTime - lastTime >= interval_sec)
    {
      // If last prinf() was more than 1 sec ago
      // printf and reset timer
      printf("%f ms/frame\n", (1000.0 * (currentTime - lastTime)) / double(nbFrames));
      nbFrames = 0;
      lastTime = glfwGetTime();
    }
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);

  glfwTerminate();
  return 0;
}
