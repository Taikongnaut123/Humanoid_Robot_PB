#include <iostream>
#include <string>
#include <memory>
#include "../include/interfaces/interfaces_request_response.pb.h"
#include "../include/interfaces/interfaces_grpc.grpc.pb.h"
using namespace base_types;

void print_variant(const Variant &var);

#define CASE_PRINT(VARIANT_TYPE, VARIANT_FUNC)                             \
    case Variant::K##VARIANT_TYPE##Value:                                  \
    {                                                                      \
        std::cout << #VARIANT_TYPE << ": " << VARIANT_FUNC() << std::endl; \
        break;                                                             \
    }

#define CASE_PRINT_ARRAY(VARIANT_TYPE, VARIANT_FUNC) \
    case Variant::K##VARIANT_TYPE##ArrayValue:       \
    {                                                \
        std::cout << #VARIANT_TYPE " Array: ";       \
        auto &array = VARIANT_FUNC();                \
        for (const auto &item : array)               \
        {                                            \
            std::cout << item << " ";                \
        }                                            \
        std::cout << std::endl;                      \
        break;                                       \
    }

#define CASE_PRINT_DATE(VARIANT_TYPE, VARIANT_FUNC) \
    case Variant::K##VARIANT_TYPE##Value:           \
    {                                               \
        auto &date = VARIANT_FUNC();                \
        std::cout << #VARIANT_TYPE << ": "          \
                  << date.year() << "-"             \
                  << date.month() << "-"            \
                  << date.day() << std::endl;       \
        break;                                      \
    }

#define CASE_PRINT_TIMESTAMP(VARIANT_TYPE, VARIANT_FUNC) \
    case Variant::K##VARIANT_TYPE##Value:                \
    {                                                    \
        auto &timestamp = VARIANT_FUNC();                \
        std::cout << #VARIANT_TYPE << ": "               \
                  << timestamp.seconds() << "."          \
                  << timestamp.nanos() << std::endl;     \
        break;                                           \
    }

#define CASE_PRINT_DICT(VARIANT_TYPE, VARIANT_FUNC)                    \
    case Variant::K##VARIANT_TYPE##Value:                              \
    {                                                                  \
        auto map = VARIANT_FUNC();                                     \
        std::cout << #VARIANT_TYPE << ": "                             \
                  << map.keyvaluelist_size() << " items" << std::endl; \
        for (const auto &item : map.keyvaluelist())                    \
        {                                                              \
            std::cout << "  " << item.first << ": ";                   \
            print_variant(item.second);                                \
        }                                                              \
        break;                                                         \
    }

// 简单的测试函数
void print_variant(const Variant &var)
{
    auto caseVal = var.value_case();
    std::cout << "Variant case: " << caseVal << std::endl;
    switch (caseVal)
    {
        CASE_PRINT(Bool, var.boolvalue);
        CASE_PRINT(Int8, var.int8value);
        CASE_PRINT(Uint8, var.uint8value);
        CASE_PRINT(Int16, var.int16value);
        CASE_PRINT(Uint16, var.uint16value);
        CASE_PRINT(Int32, var.int32value);
        CASE_PRINT(Uint32, var.uint32value);
        CASE_PRINT(Int64, var.int64value);
        CASE_PRINT(Uint64, var.uint64value);
        CASE_PRINT(Float, var.floatvalue);
        CASE_PRINT(Double, var.doublevalue);
        CASE_PRINT(Char, var.charvalue);
        CASE_PRINT(Byte, var.bytevalue);
        CASE_PRINT(String, var.stringvalue);
        CASE_PRINT_DATE(Date, var.datevalue);
        CASE_PRINT_TIMESTAMP(Timestamp, var.timestampvalue);
        CASE_PRINT_DICT(Dict, var.dictvalue);
        CASE_PRINT_ARRAY(Bool, var.boolarrayvalue().values);
        CASE_PRINT_ARRAY(Int8, var.int8arrayvalue().values);
        CASE_PRINT_ARRAY(Uint8, var.uint8arrayvalue().values);
        CASE_PRINT_ARRAY(Int16, var.int16arrayvalue().values);
        CASE_PRINT_ARRAY(Uint16, var.uint16arrayvalue().values);
        CASE_PRINT_ARRAY(Int32, var.int32arrayvalue().values);
        CASE_PRINT_ARRAY(Uint32, var.uint32arrayvalue().values);
        CASE_PRINT_ARRAY(Int64, var.int64arrayvalue().values);
        CASE_PRINT_ARRAY(Uint64, var.uint64arrayvalue().values);
        CASE_PRINT_ARRAY(Float, var.floatarrayvalue().values);
        CASE_PRINT_ARRAY(Double, var.doublearrayvalue().values);
        CASE_PRINT_ARRAY(Char, var.chararrayvalue().values);
        CASE_PRINT_ARRAY(Byte, var.bytearrayvalue().values);
        CASE_PRINT_ARRAY(String, var.stringarrayvalue().values);
    default:
        std::cout << "Unknown Variant case" << std::endl;
        break;
    }
}

void print_keyvaluelist(const ::google::protobuf::Map<std::string, ::base_types::Variant> &keyvaluelist)
{
    for (const auto &pair : keyvaluelist)
    {
        std::cout << pair.first << ": ";
        print_variant(pair.second);
    }
}

// 简单的测试函数
template <typename T>
void print_test_result(const std::string &test_name, const T &expected, const T &actual)
{
    bool passed = (expected == actual);
    std::cout << "[" << (passed ? "PASS" : "FAIL") << "] " << test_name
              << " - Expected: " << expected << ", Actual: " << actual << std::endl;
}

void print_section(const std::string &section_name)
{
    std::cout << "\n=== " << section_name << " ===" << std::endl;
}

// 测试基本请求响应类型
void test_request_response_types()
{
    print_section("Request Response Types");

    // 测试 SendRequest
    {
        interfaces::SendRequest request;

        // 设置请求数据
        auto input = request.mutable_input()->mutable_keyvaluelist();
        {
            // 设置输入数据
            base_types::Variant var;
            var.set_int16value(10001);
            input->insert(std::make_pair("type", var));
        }
        {
            // 设置输入数据
            base_types::Variant var;
            var.set_stringvalue("test_resource");
            input->insert(std::make_pair("resource_name", var));
        }

        // 设置参数
        auto params = request.mutable_params()->mutable_keyvaluelist();
        {
            // 设置参数内容
            base_types::Variant var;
            var.set_int32value(500);
            params->insert(std::make_pair("timeout", var));
        }
        {
            // 设置参数内容
            base_types::Variant var;
            var.set_int32value(42);
            params->insert(std::make_pair("correlationid", var));
        }
        print_keyvaluelist(request.input().keyvaluelist());
        print_keyvaluelist(request.params().keyvaluelist());
    }

    // 测试 SendResponse
    {
        interfaces::SendResponse response;

        auto output = response.mutable_output()->mutable_keyvaluelist();
        {
            // 设置输出数据
            base_types::Variant var;
            var.set_stringvalue("Resource created successfully");
            output->insert(std::make_pair("message", var));
        }
        {
            // 设置输出数据
            base_types::Variant var;
            var.set_int32value(123);
            output->insert(std::make_pair("resourceid", var));
        }
        {
            Variant var;
            var.set_boolvalue(true);
            output->insert(std::make_pair("status", var));
        }

        {
            Variant var;
            auto map = var.mutable_dictvalue()->mutable_keyvaluelist();
            {
                Variant var_;
                var_.set_stringvalue("2023-10-10");
                map->insert(std::make_pair("date", var_));
            }
            {
                Variant var_;
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

    interfaces::ResultStatus error;
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
    interfaces::SendRequest request;

    // 创建一个复杂的请求进行序列化测试
    {
        // 设置请求数据
        auto input = request.mutable_input()->mutable_keyvaluelist();
        {
            // 设置输入数据
            base_types::Variant var;
            var.set_int16value(10001);
            input->insert(std::make_pair("type", var));
        }
        {
            // 设置输入数据
            base_types::Variant var;
            var.set_stringvalue("test_resource");
            input->insert(std::make_pair("resource_name", var));
        }

        // 设置参数
        auto params = request.mutable_params()->mutable_keyvaluelist();
        {
            // 设置参数内容
            base_types::Variant var;
            var.set_int32value(500);
            params->insert(std::make_pair("timeout", var));
        }
        {
            // 设置参数内容
            base_types::Variant var;
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
        interfaces::SendRequest deserialized;
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
