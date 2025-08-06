#ifndef PRINT_UTIL_H
#define PRINT_UTIL_H

#include <iostream>
#include <string>
#include <google/protobuf/util/json_util.h>
#include "common/variant.pb.h"

namespace humanoid_robot
{
    namespace utils
    {
        namespace PB
        {

            void print_variant(const base_types::Variant &var);

            void print_keyvaluelist(const ::google::protobuf::Map<std::string, ::base_types::Variant> &keyvaluelist);

            void print_section(const std::string &section_name);

            // 简单的测试函数
            template <typename T>
            void print_test_result(const std::string &test_name, const T &expected, const T &actual)
            {
                bool passed = (expected == actual);
                std::cout << "[" << (passed ? "PASS" : "FAIL") << "] " << test_name
                          << " - Expected: " << expected << ", Actual: " << actual << std::endl;
            }

        } // namespace PB
    } // namespace utils
} // namespace humanoid_robot

#endif // PRINT_UTIL_H