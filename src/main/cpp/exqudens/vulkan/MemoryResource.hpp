#pragma once

#include "exqudens/vulkan/Resource.hpp"

namespace exqudens::vulkan {

  template <typename T, typename M>
  struct MemoryResource: Resource<T> {

    std::shared_ptr<M> memory;

    M& memoryReference() {
      try {
        if (!memory) {
          throw std::runtime_error(CALL_INFO() + ": memory is not initialized!");
        }
        return *memory;
      } catch (...) {
        std::throw_with_nested(std::runtime_error(CALL_INFO()));
      }
    }

  };

}
