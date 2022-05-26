#pragma once

#include <memory>
#include <stdexcept>

#include "exqudens/vulkan/Macros.hpp"

namespace exqudens::vulkan {

  template <typename T>
  struct Resource {

    unsigned int id;
    std::shared_ptr<T> value;

    T& reference() {
      try {
        if (!value) {
          throw std::runtime_error(CALL_INFO() + ": value is not initialized!");
        }
        return *value;
      } catch (...) {
        std::throw_with_nested(std::runtime_error(CALL_INFO()));
      }
    }

  };

}
