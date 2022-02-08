#pragma once

#include <string>
#include <vector>

#include "exqudens/vulkan/Export.hpp"

namespace exqudens::vulkan {

  class EXQUDENS_VULKAN_EXPORT StringVector {

    public:

      std::vector<std::string> values;
      std::vector<const char*> pointers;

      static std::vector<const char*> toPointers(const std::vector<std::string>& values);

      explicit StringVector(
          std::vector<std::string> values
      );
      StringVector();
      StringVector(const StringVector& object);
      StringVector(StringVector&& object) noexcept;

      StringVector& operator=(const StringVector& object);
      StringVector& operator=(StringVector&& object) noexcept;

      friend void swap(StringVector& first, StringVector& second);

      virtual ~StringVector();

  };

}
