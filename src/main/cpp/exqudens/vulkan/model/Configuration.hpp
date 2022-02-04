#pragma once

#include "exqudens/vulkan/Export.hpp"
#include "exqudens/vulkan/model/StringVector.hpp"

namespace exqudens::vulkan {

  struct EXQUDENS_VULKAN_EXPORT Configuration {

    StringVector extensions;

    Configuration() = default;

    explicit Configuration(
        const StringVector& extensions
    ):
        extensions(StringVector(extensions.values))
    {
    }

    Configuration(
        const Configuration& object
    ):
        Configuration(object.extensions)
    {
    }

    Configuration(
        Configuration&& object
    ) noexcept:
        Configuration()
    {
      swap(*this, object);
    }

    Configuration& operator=(Configuration object) {
      swap(*this, object);
      return *this;
    }

    friend void swap(Configuration& first, Configuration& second) {
      std::swap(first.extensions, second.extensions);
    }

    virtual ~Configuration() = default;

  };

}
