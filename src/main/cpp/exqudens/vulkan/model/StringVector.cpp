#include "exqudens/vulkan/model/StringVector.hpp"

#include <algorithm>
#include <iterator>
#include <utility>

namespace exqudens::vulkan {

  std::vector<const char*> StringVector::toPointers(const std::vector<std::string>& values) {
    std::vector<const char*> result;
    std::transform(
        values.begin(),
        values.end(),
        std::back_inserter(result),
        [](const std::string& s) -> const char* { return s.c_str(); }
    );
    return result;
  }

  StringVector::StringVector(
      std::vector<std::string> values
  ):
      values(std::move(values))
  {
    pointers = toPointers(this->values);
  }

  StringVector::StringVector() = default;

  StringVector::StringVector(const StringVector& object): StringVector(
      object.values
  ) {
  }

  StringVector::StringVector(StringVector&& object) noexcept: StringVector() {
    swap(*this, object);
  }

  StringVector& StringVector::operator=(const StringVector& object) {
    return *this = StringVector(object);
  }

  StringVector& StringVector::operator=(StringVector&& object) noexcept {
    swap(*this, object);
    return *this;
  }

  void swap(StringVector& first, StringVector& second) {
    std::swap(first.values, second.values);
    std::swap(first.pointers, second.pointers);
  }

  StringVector::~StringVector() = default;

}
