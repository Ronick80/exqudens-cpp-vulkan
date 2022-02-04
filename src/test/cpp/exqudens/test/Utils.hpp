#pragma once

#include <string>
#include <vector>
#include <stdexcept>

namespace exqudens {

  class TestUtils {

    public:

      static std::vector<std::string> toStringVector(
          const std::exception& exception,
          std::vector<std::string> previous = {}
      ) {
        previous.emplace_back(exception.what());
        try {
          std::rethrow_if_nested(exception);
          return previous;
        } catch (const std::exception& e) {
          return toStringVector(e, previous);
        } catch (...) {
          throw std::runtime_error("Unexpected error!");
        }
      }

  };

}
