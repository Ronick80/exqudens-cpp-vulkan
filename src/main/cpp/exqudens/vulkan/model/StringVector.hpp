#pragma once

#include <string>
#include <utility>
#include <vector>
#include <utility>
#include <iterator>
#include <algorithm>

#include "exqudens/vulkan/Export.hpp"

namespace exqudens::vulkan {

  struct EXQUDENS_VULKAN_EXPORT StringVector {

    std::vector<std::string> values;
    std::vector<const char*> pointers;

    static std::vector<const char*> toPointers(const std::vector<std::string>& values) {
      std::vector<const char*> result;
      std::transform(
        values.begin(),
        values.end(),
        std::back_inserter(result),
        [](const std::string& s) -> const char* { return s.c_str(); }
      );
      return result;
    }

    explicit StringVector(
        std::vector<std::string> values
    ):
        values(std::move(values))
    {
      pointers = toPointers(this->values);
    }

    StringVector() = default;

    StringVector(
        const StringVector& object
    ):
        StringVector(object.values)
    {
    }

    StringVector(
        StringVector&& object
    ) noexcept:
        StringVector()
    {
      swap(*this, object);
    }

    StringVector& operator=(StringVector object) {
      swap(*this, object);
      return *this;
    }

    friend void swap(StringVector& first, StringVector& second) {
      std::swap(first.values, second.values);
      std::swap(first.pointers, second.pointers);
    }

    virtual ~StringVector() = default;

  };

}
