#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <filesystem>
#include <stdexcept>

#include <lodepng.h>

#include "exqudens/TestMacros.hpp"

namespace exqudens::vulkan {

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
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      static std::vector<std::string> toStackTrace(const std::exception& exception) {
        try {
          std::vector<std::string> elements = toStringVector(exception);
          if (elements.size() > 1) {
            std::ranges::reverse(elements);
          }
          return elements;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      static std::string toString(const std::exception& e) {
        try {
          std::vector<std::string> stackTrace = toStackTrace(e);
          std::ostringstream out;
          for (size_t i = 0; i < stackTrace.size(); i++) {
            out << stackTrace[i];
            if (i < stackTrace.size() - 1) {
              out << std::endl;
            }
          }
          return out.str();
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      static std::vector<std::vector<std::vector<unsigned char>>> readPng(const std::string& path) {
        try {
          std::vector<std::vector<std::vector<unsigned char>>> image;
          std::vector<unsigned char> pixels;
          unsigned int width, height, depth, error;
          depth = 4;
          error = lodepng::decode(pixels, width, height, path);
          if (error) {
            throw std::runtime_error(
                CALL_INFO() + ": failed to read image '" + std::to_string(error) + "': " + lodepng_error_text(error)
            );
          }
          unsigned int yMultiplier = width * depth;
          image.resize(height);
          for (unsigned int y = 0; y < height; y++) {
            image[y].resize(width);
            for (unsigned int x = 0; x < width; x++) {
              image[y][x].resize(depth);
              for (unsigned int z = 0; z < depth; z++) {
                unsigned int yOffset = y * yMultiplier;
                unsigned int xOffset = x * depth;
                unsigned int zOffset = z;
                unsigned int offset = yOffset + xOffset + zOffset;
                unsigned int i = offset;
                image[y][x][z] = pixels[i];
              }
            }
          }
          return image;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      static void writePng(const std::vector<std::vector<std::vector<unsigned char>>>& image, const std::string& path) {
        try {
          if (image.empty() || image[0].empty() || image[0][0].empty()) {
            throw std::runtime_error(
                CALL_INFO() + ": image.empty() || image[0].empty() || image[0][0].empty()"
            );
          }
          unsigned int height = image.size();
          unsigned int width = image[0].size();
          unsigned int depth = image[0][0].size();
          std::vector<unsigned char> pixels;
          pixels.resize(width * height * depth);
          unsigned int i = 0;
          for (unsigned int y = 0; y < height; y++) {
            for (unsigned int x = 0; x < width; x++) {
              for (unsigned int z = 0; z < depth; z++) {
                pixels[i] = image[y][x][z];
                i++;
              }
            }
          }
          unsigned int error = lodepng::encode(path, pixels, width, height);
          if (error) {
            throw std::runtime_error(
                CALL_INFO() + ": failed to write image '" + std::to_string(error) + "': " + lodepng_error_text(error)
            );
          }
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

}
