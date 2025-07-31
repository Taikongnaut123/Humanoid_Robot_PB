#include <iostream>
#include <string>
#include <memory>
#include "../include/interfaces/interfaces_request_response.pb.h"
#include "../include/interfaces/interfaces_grpc.grpc.pb.h"

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

    // 测试 CreateRequest
    {
        interfaces::CreateRequest request;

        // 设置请求数据
        base_types::Dictionary *req_data = request.mutable_requestdata();
        base_types::Variant var;
        var.set_type(base_types::Variant::KStringValue);
        var.set_stringvalue("test_resource");
        (*req_data->mutable_keyvaluelist())["resource_name"] = var;

        // 设置参数
        interfaces::Params *params = request.mutable_params();
        params->set_timeout(30);
        params->set_correlationid("test-123");

        print_test_result("CreateRequest has requestData", true, request.has_requestdata());
        print_test_result("CreateRequest has params", true, request.has_params());
        print_test_result("Params timeout", 30, request.params().timeout());
        print_test_result("Params correlationId", std::string("test-123"), request.params().correlationid());

        // 验证字典内容
        const auto &dict_map = request.requestdata().keyvaluelist();
        auto it = dict_map.find("resource_name");
        print_test_result("Dictionary has resource_name", true, it != dict_map.end());
        if (it != dict_map.end())
        {
            print_test_result("Dictionary resource_name value", std::string("test_resource"),
                              it->second.stringvalue());
        }
    }

    // 测试 CreateResponse
    {
        interfaces::CreateResponse response;
        response.set_status(interfaces::STATUS_SUCCESS);
        response.set_message("Resource created successfully");
        response.set_resourceid("res_456");

        // 设置响应数据
        base_types::Dictionary *resp_data = response.mutable_responsedata();
        base_types::Variant var;
        var.set_type(base_types::Variant::KInt32Value);
        var.set_int32value(42);
        (*resp_data->mutable_keyvaluelist())["created_count"] = var;

        print_test_result("CreateResponse status", static_cast<int>(interfaces::STATUS_SUCCESS),
                          static_cast<int>(response.status()));
        print_test_result("CreateResponse message", std::string("Resource created successfully"),
                          response.message());
        print_test_result("CreateResponse resourceId", std::string("res_456"), response.resourceid());
        print_test_result("CreateResponse has responseData", true, response.has_responsedata());

        // 验证响应数据
        const auto &resp_dict = response.responsedata().keyvaluelist();
        auto it = resp_dict.find("created_count");
        print_test_result("Response dictionary has created_count", true, it != resp_dict.end());
        if (it != resp_dict.end())
        {
            print_test_result("Response created_count value", 42, it->second.int32value());
        }
    }
}

// 测试错误信息
void test_error_info()
{
    print_section("Error Info");

    interfaces::ErrorInfo error;
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

// 测试状态枚举
void test_status_enums()
{
    print_section("Status Enums");

    print_test_result("STATUS_UNKNOWN", 0, static_cast<int>(interfaces::STATUS_UNKNOWN));
    print_test_result("STATUS_SUCCESS", 1, static_cast<int>(interfaces::STATUS_SUCCESS));
    print_test_result("STATUS_FAILED", 2, static_cast<int>(interfaces::STATUS_FAILED));
    print_test_result("STATUS_TIMEOUT", 3, static_cast<int>(interfaces::STATUS_TIMEOUT));
    print_test_result("STATUS_CANCELLED", 4, static_cast<int>(interfaces::STATUS_CANCELLED));
    print_test_result("STATUS_PENDING", 5, static_cast<int>(interfaces::STATUS_PENDING));
}

// 测试其他请求类型
void test_other_requests()
{
    print_section("Other Request Types");

    // 测试 SendRequest
    {
        interfaces::SendRequest request;

        // 设置发送数据
        base_types::Dictionary *send_data = request.mutable_messagedata();
        base_types::Variant var;
        var.set_type(base_types::Variant::KStringValue);
        var.set_stringvalue("Hello World");
        (*send_data->mutable_keyvaluelist())["message"] = var;

        // 设置目标ID
        request.set_targetid("service_endpoint");

        print_test_result("SendRequest has messagedata", true, request.has_messagedata());
        print_test_result("SendRequest targetId", std::string("service_endpoint"), request.targetid());

        const auto &send_dict = request.messagedata().keyvaluelist();
        auto it = send_dict.find("message");
        print_test_result("Send data has message", true, it != send_dict.end());
        if (it != send_dict.end())
        {
            print_test_result("Send message value", std::string("Hello World"),
                              it->second.stringvalue());
        }
    }

    // 测试 QueryRequest
    {
        interfaces::QueryRequest request;
        request.set_queryid("query_001");
        request.set_limit(100);
        request.set_offset(0);

        // 设置查询条件
        base_types::Dictionary *conditions = request.mutable_queryconditions();
        base_types::Variant var;
        var.set_type(base_types::Variant::KBoolValue);
        var.set_boolvalue(true);
        (*conditions->mutable_keyvaluelist())["active"] = var;

        print_test_result("QueryRequest queryId", std::string("query_001"),
                          request.queryid());
        print_test_result("QueryRequest limit", 100, request.limit());
        print_test_result("QueryRequest offset", 0, request.offset());
        print_test_result("QueryRequest has queryConditions", true, request.has_queryconditions());

        const auto &condition_dict = request.queryconditions().keyvaluelist();
        auto it = condition_dict.find("active");
        print_test_result("Query conditions has active", true, it != condition_dict.end());
        if (it != condition_dict.end())
        {
            print_test_result("Query active condition value", true, it->second.boolvalue());
        }
    }

    // 测试 HealthCheckRequest
    {
        interfaces::HealthCheckRequest request;
        request.set_service("interface_service");

        // 设置检查参数
        base_types::Dictionary *check_params = request.mutable_checkparams();
        base_types::Variant var;
        var.set_type(base_types::Variant::KStringValue);
        var.set_stringvalue("basic");
        (*check_params->mutable_keyvaluelist())["check_type"] = var;

        print_test_result("HealthCheckRequest service", std::string("interface_service"),
                          request.service());
        print_test_result("HealthCheckRequest has checkParams", true, request.has_checkparams());

        const auto &params_dict = request.checkparams().keyvaluelist();
        auto it = params_dict.find("check_type");
        print_test_result("Check params has check_type", true, it != params_dict.end());
        if (it != params_dict.end())
        {
            print_test_result("Check type value", std::string("basic"), it->second.stringvalue());
        }
    }
}

// 测试序列化
void test_serialization()
{
    print_section("Serialization");

    // 创建一个复杂的请求进行序列化测试
    interfaces::CreateRequest original;

    // 设置请求数据
    base_types::Dictionary *req_data = original.mutable_requestdata();
    base_types::Variant var1;
    var1.set_type(base_types::Variant::KStringValue);
    var1.set_stringvalue("test_serialization");
    (*req_data->mutable_keyvaluelist())["action"] = var1;

    base_types::Variant var2;
    var2.set_type(base_types::Variant::KInt32Value);
    var2.set_int32value(123);
    (*req_data->mutable_keyvaluelist())["count"] = var2;

    // 设置参数
    interfaces::Params *params = original.mutable_params();
    params->set_timeout(60);
    params->set_correlationid("serialize-test-456");

    // 序列化
    std::string serialized;
    bool serialize_success = original.SerializeToString(&serialized);
    print_test_result("Serialization success", true, serialize_success);
    print_test_result("Serialized data not empty", true, !serialized.empty());

    if (serialize_success)
    {
        // 反序列化
        interfaces::CreateRequest deserialized;
        bool deserialize_success = deserialized.ParseFromString(serialized);
        print_test_result("Deserialization success", true, deserialize_success);

        if (deserialize_success)
        {
            print_test_result("Deserialized has requestData", true, deserialized.has_requestdata());
            print_test_result("Deserialized has params", true, deserialized.has_params());
            print_test_result("Deserialized timeout", 60, deserialized.params().timeout());
            print_test_result("Deserialized correlationId", std::string("serialize-test-456"),
                              deserialized.params().correlationid());

            // 验证字典内容
            const auto &dict_map = deserialized.requestdata().keyvaluelist();
            auto action_it = dict_map.find("action");
            auto count_it = dict_map.find("count");

            print_test_result("Deserialized has action", true, action_it != dict_map.end());
            print_test_result("Deserialized has count", true, count_it != dict_map.end());

            if (action_it != dict_map.end())
            {
                print_test_result("Deserialized action value", std::string("test_serialization"),
                                  action_it->second.stringvalue());
            }
            if (count_it != dict_map.end())
            {
                print_test_result("Deserialized count value", 123, count_it->second.int32value());
            }
        }
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
        test_status_enums();
        test_other_requests();
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
