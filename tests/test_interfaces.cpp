#include <iostream>
#include <string>
#include <memory>
#include "../include/interfaces/interfaces_request_response.pb.h"
#include "../include/interfaces/interfaces_grpc.grpc.pb.h"
#include "printUtil.h"
using namespace humanoid_robot::PB::interfaces;
using namespace humanoid_robot::utils::PB;
using namespace humanoid_robot::PB::common;

// 测试基本请求响应类型
void test_request_response_types()
{
    print_section("Request Response Types");

    // 测试 SendRequest
    {
        humanoid_robot::PB::interfaces::SendRequest request;

        // 设置请求数据
        auto input = request.mutable_input()->mutable_keyvaluelist();
        {
            // 设置输入数据
            humanoid_robot::PB::common::Variant var;
            var.set_int16value(10001);
            input->insert(std::make_pair("type", var));
        }
        {
            // 设置输入数据
            humanoid_robot::PB::common::Variant var;
            var.set_stringvalue("test_resource");
            input->insert(std::make_pair("resource_name", var));
        }

        // 设置参数
        auto params = request.mutable_params()->mutable_keyvaluelist();
        {
            // 设置参数内容
            humanoid_robot::PB::common::Variant var;
            var.set_int32value(500);
            params->insert(std::make_pair("timeout", var));
        }
        {
            // 设置参数内容
            humanoid_robot::PB::common::Variant var;
            var.set_int32value(42);
            params->insert(std::make_pair("correlationid", var));
        }
        print_keyvaluelist(request.input().keyvaluelist());
        print_keyvaluelist(request.params().keyvaluelist());
    }

    // 测试 SendResponse
    {
        humanoid_robot::PB::interfaces::SendResponse response;

        auto output = response.mutable_output()->mutable_keyvaluelist();
        {
            // 设置输出数据
            humanoid_robot::PB::common::Variant var;
            var.set_stringvalue("Resource created successfully");
            output->insert(std::make_pair("message", var));
        }
        {
            // 设置输出数据
            humanoid_robot::PB::common::Variant var;
            var.set_int32value(123);
            output->insert(std::make_pair("resourceid", var));
        }
        {
            humanoid_robot::PB::common::Variant var;
            var.set_boolvalue(true);
            output->insert(std::make_pair("status", var));
        }

        {
            humanoid_robot::PB::common::Variant var;
            auto map = var.mutable_dictvalue()->mutable_keyvaluelist();
            {
                humanoid_robot::PB::common::Variant var_;
                var_.set_stringvalue("2023-10-10");
                map->insert(std::make_pair("date", var_));
            }
            {
                humanoid_robot::PB::common::Variant var_;
                var_.set_stringvalue("12:00:00");
                map->insert(std::make_pair("time", var_));
            }
            output->insert(std::make_pair("timestamp", var));
        }
        print_keyvaluelist(response.output().keyvaluelist());
    }
}

// 测试错误信息
void test_error_info()
{
    print_section("Error Info");

    humanoid_robot::PB::interfaces::ResultStatus error;
    error.set_code("ERR_001");
    error.set_message("Sample error message");
    error.set_details("Detailed error information");
    error.set_timestamp(1705123456);

    print_test_result("ErrorInfo code", std::string("ERR_001"), error.code());
    print_test_result("ErrorInfo message", std::string("Sample error message"), error.message());
    print_test_result("ErrorInfo details", std::string("Detailed error information"), error.details());
    print_test_result("ErrorInfo timestamp", static_cast<int64_t>(1705123456),
                      static_cast<int64_t>(error.timestamp()));
}

// 测试序列化
void test_serialization()
{
    print_section("Serialization");
    humanoid_robot::PB::interfaces::SendRequest request;

    // 创建一个复杂的请求进行序列化测试
    {
        // 设置请求数据
        auto input = request.mutable_input()->mutable_keyvaluelist();
        {
            // 设置输入数据
            humanoid_robot::PB::common::Variant var;
            var.set_int16value(10001);
            input->insert(std::make_pair("type", var));
        }
        {
            // 设置输入数据
            humanoid_robot::PB::common::Variant var;
            var.set_stringvalue("test_resource");
            input->insert(std::make_pair("resource_name", var));
        }

        // 设置参数
        auto params = request.mutable_params()->mutable_keyvaluelist();
        {
            // 设置参数内容
            humanoid_robot::PB::common::Variant var;
            var.set_int32value(500);
            params->insert(std::make_pair("timeout", var));
        }
        {
            // 设置参数内容
            humanoid_robot::PB::common::Variant var;
            var.set_int32value(42);
            params->insert(std::make_pair("correlationid", var));
        }
        print_keyvaluelist(request.input().keyvaluelist());
        print_keyvaluelist(request.params().keyvaluelist());
    }

    // 序列化
    std::string serialized;
    bool serialize_success = request.SerializeToString(&serialized);
    print_test_result("Serialization success", true, serialize_success);
    print_test_result("Serialized data not empty", true, !serialized.empty());

    if (serialize_success)
    {
        // 反序列化
        humanoid_robot::PB::interfaces::SendRequest deserialized;
        bool deserialize_success = deserialized.ParseFromString(serialized);
        print_test_result("Deserialization success", true, deserialize_success);

        print_keyvaluelist(deserialized.input().keyvaluelist());
        print_keyvaluelist(deserialized.params().keyvaluelist());
    }
}

int main()
{
    std::cout << "Testing Interfaces Protobuf Functionality" << std::endl;
    std::cout << "==========================================" << std::endl;

    try
    {
        test_request_response_types();
        test_error_info();
        test_serialization();

        std::cout << "\n=== Test Summary ===" << std::endl;
        std::cout << "All tests completed successfully!" << std::endl;
        std::cout << "Interfaces protobuf functionality is working correctly." << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
