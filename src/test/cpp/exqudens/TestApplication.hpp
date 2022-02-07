#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "exqudens/TestConfiguration.hpp"
#include "exqudens/test/TestUtilsTests.hpp"
#include "exqudens/test/Tests.hpp"
#include "exqudens/test/UiTests.hpp"

namespace exqudens::vulkan {

  class TestApplication {

    public:

      static int run(int* argc, char** argv) {
        TestConfiguration::setExecutableFile(argv[0]);
        testing::InitGoogleMock(argc, argv);
        testing::InitGoogleTest(argc, argv);
        //testing::GTEST_FLAG(filter) = "*-UiTests.*";
        for (int i = 0; i < *argc; i++) {
          std::cout << "AAA: '" << argv[i] << "'" << std::endl;
        }
        return RUN_ALL_TESTS();
      }

  };

}
