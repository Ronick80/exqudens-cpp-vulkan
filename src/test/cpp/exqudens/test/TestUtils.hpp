#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <filesystem>

#include <lodepng.h>

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
          throw std::runtime_error("Unexpected error!");
        }
      }

      static std::vector<std::vector<std::vector<unsigned char>>> readPng(const std::string& path) {
        std::vector<std::vector<std::vector<unsigned char>>> image;
        std::vector<unsigned char> pixels;
        unsigned int width, height, depth, error;
        depth = 4;
        error = lodepng::decode(pixels, width, height, path);
        if (error) {
          throw std::runtime_error(
              std::string(__FUNCTION__)
              + "("
              + std::filesystem::path(__FILE__).make_preferred().string()
              + ":"
              + std::to_string(__LINE__)
              + "): "
              + "failed to read image '"
              + std::to_string(error)
              + "': "
              + lodepng_error_text(error)
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
      }

      static void writePng(const std::vector<std::vector<std::vector<unsigned char>>>& image, const std::string& path) {
        if (image.empty() || image[0].empty() || image[0][0].empty()) {
          throw std::runtime_error(
              std::string(__FUNCTION__)
              + "("
              + std::filesystem::path(__FILE__).make_preferred().string()
              + ":"
              + std::to_string(__LINE__)
              + "): "
              + "image.empty() || image[0].empty() || image[0][0].empty()"
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
              std::string(__FUNCTION__)
              + "("
              + std::filesystem::path(__FILE__).make_preferred().string()
              + ":"
              + std::to_string(__LINE__)
              + "): "
              + "failed to write image '"
              + std::to_string(error)
              + "': "
              + lodepng_error_text(error)
          );
        }
      }

  };

}
