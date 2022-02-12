#pragma once

#include <cstdlib>
#include <string>
#include <vector>
#include <stdexcept>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "exqudens/TestMacros.hpp"

namespace exqudens::vulkan {

  class TestUiApplication {

    public:

      std::vector<std::string> arguments = {};
      GLFWwindow* window = nullptr;

      TestUiApplication(const int& argc, char** argv) {
        try {
          for (std::size_t i = 0; i < argc; i++) {
            arguments.emplace_back(std::string(argv[i]));
          }
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      int run() {
        try {
          int width = 800;
          int height = 600;
          std::string title = "Vulkan";

          glfwInit();
          glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

          window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

          glfwSetWindowUserPointer(window, this);
          glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);

          //vulkanEnvironment = new VulkanEnvironment();

          while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            int width = 0;
            int height = 0;
            glfwGetFramebufferSize(window, &width, &height);
            while (width == 0 || height == 0) {
              glfwGetFramebufferSize(window, &width, &height);
              glfwWaitEvents();
            }
            //vulkanEnvironment->setFrameBufferSize(width, height);
            //vulkanEnvironment->drawFrame();
          }
          //vulkanEnvironment->waitIdle();

          //delete vulkanEnvironment;
          glfwDestroyWindow(window);
          glfwTerminate();

          return EXIT_SUCCESS;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

    private:

      static void frameBufferResizeCallback(GLFWwindow* window, int width, int height) {
        try {
          //TestUiApplication* app = reinterpret_cast<TestUiApplication*>(glfwGetWindowUserPointer(window));
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

}
