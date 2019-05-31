//
// Created by Jonathan Leung
//

#ifndef WHEREISWALDOVR_GLFWAPP_H
#define WHEREISWALDOVR_GLFWAPP_H

// GLEW gives cross platform access to OpenGL 3.x+ functionality.
#include <GLFW/glfw3.h>
#include <GL/glew.h>

namespace glfw
{
  inline GLFWwindow* createWindow(const uvec2& size, const ivec2& position = ivec2(INT_MIN)) {
    GLFWwindow* window = glfwCreateWindow(size.x, size.y, "glfw", nullptr, nullptr);
    if (!window) {
      FAIL("Unable to create rendering window");
    }
    if ((position.x > INT_MIN) && (position.y > INT_MIN)) {
      glfwSetWindowPos(window, position.x, position.y);
    }
    return window;
  }
}


class GlfwApp {

protected:
  uvec2 windowSize;
  ivec2 windowPosition;
  GLFWwindow* window{nullptr};
  unsigned int frame{0};

public:
  GlfwApp() {
    // Initialize the GLFW system for creating and positioning windows
    if (!glfwInit()) {
      FAIL("Failed to initialize GLFW");
    }
    glfwSetErrorCallback(ErrorCallback);
  }

  virtual ~GlfwApp() {
    if (nullptr != window) {
      glfwDestroyWindow(window);
    }
    glfwTerminate();
  }

  virtual int run() {
    preCreate();

    window = createRenderingTarget(windowSize, windowPosition);

    if (!window) {
      std::cout << "Unable to create OpenGL window" << std::endl;
      return -1;
    }

    postCreate();

    initGl();

    while (!glfwWindowShouldClose(window)) {
      ++frame;
      glfwPollEvents();
      update();
      draw();
      finishFrame();
    }

    shutdownGl();

    return 0;
  }

protected:
  virtual GLFWwindow* createRenderingTarget(uvec2& size, ivec2& pos) = 0;

  virtual void draw() = 0;

  void preCreate() {
    glfwWindowHint(GLFW_DEPTH_BITS, 16);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
  }

  void postCreate() {
    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwMakeContextCurrent(window);

    // Initialize the OpenGL bindings
    // For some reason we have to set this experminetal flag to properly
    // init GLEW if we use a core context.
    glewExperimental = GL_TRUE;
    if (0 != glewInit()) {
      FAIL("Failed to initialize GLEW");
    }
    glGetError();

    if (GLEW_KHR_debug) {
      GLint v;
      glGetIntegerv(GL_CONTEXT_FLAGS, &v);
      if (v & GL_CONTEXT_FLAG_DEBUG_BIT) {
        //glDebugMessageCallback(glDebugCallbackHandler, this);
      }
    }
  }

  virtual void initGl() {
  }

  virtual void shutdownGl() {
  }

  virtual void finishFrame() {
    glfwSwapBuffers(window);
  }

  virtual void destroyWindow() {
    glfwSetKeyCallback(window, nullptr);
    glfwSetMouseButtonCallback(window, nullptr);
    glfwDestroyWindow(window);
  }

  virtual void onKey(int key, int scancode, int action, int mods) {
    if (GLFW_PRESS != action) {
      return;
    }

    switch (key) {
      case GLFW_KEY_ESCAPE:
        glfwSetWindowShouldClose(window, 1);
        return;
    }
  }

  virtual void update() {
  }

  virtual void onMouseButton(int button, int action, int mods) {
  }

protected:
  virtual void viewport(const ivec2& pos, const uvec2& size) {
    glViewport(pos.x, pos.y, size.x, size.y);
  }

private:

  static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    GlfwApp* instance = (GlfwApp *)glfwGetWindowUserPointer(window);
    instance->onKey(key, scancode, action, mods);
  }

  static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    GlfwApp* instance = (GlfwApp *)glfwGetWindowUserPointer(window);
    instance->onMouseButton(button, action, mods);
  }

  static void ErrorCallback(int error, const char* description) {
    FAIL(description);
  }
};


#endif //WHEREISWALDOVR_GLFWAPP_H
