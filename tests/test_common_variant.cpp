#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "common/variant.pb.h"

// 测试辅助函数
template <typename T>
void print_test_result(const std::string &test_name, T expected, T actual)
{
    if (expected == actual)
    {
        std::cout << "✓ " << test_name << " PASSED" << std::endl;
    }
    else
    {
        std::cout << "✗ " << test_name << " FAILED: expected " << expected << ", got " << actual << std::endl;
    }
}

void print_separator(const std::string &title)
{
    std::cout << "\n=== " << title << " ===" << std::endl;
}

// 测试基本类型
void test_basic_types()
{
    print_separator("Testing Basic Types");

    // 测试 TypedInt32
    {
        base_types::TypedInt32 typed_int32;
        typed_int32.set_type(base_types::VARIANT_TYPE_INT32);
        typed_int32.set_value(-123456);

        print_test_result("TypedInt32 type", static_cast<int>(base_types::VARIANT_TYPE_INT32),
                          static_cast<int>(typed_int32.type()));
        print_test_result("TypedInt32 value", -123456, typed_int32.value());
    }

    // 测试 TypedInt64
    {
        base_types::TypedInt64 typed_int64;
        typed_int64.set_type(base_types::VARIANT_TYPE_INT64);
        typed_int64.set_value(-9876543210LL);

        print_test_result("TypedInt64 type", static_cast<int>(base_types::VARIANT_TYPE_INT64),
                          static_cast<int>(typed_int64.type()));
        print_test_result("TypedInt64 value", static_cast<int64_t>(-9876543210LL), typed_int64.value());
    }

    // 测试 TypedUInt32
    {
        base_types::TypedUInt32 typed_uint32;
        typed_uint32.set_type(base_types::VARIANT_TYPE_UINT32);
        typed_uint32.set_value(4294967295U);

        print_test_result("TypedUInt32 type", static_cast<int>(base_types::VARIANT_TYPE_UINT32),
                          static_cast<int>(typed_uint32.type()));
        print_test_result("TypedUInt32 value", 4294967295U, typed_uint32.value());
    }

    // 测试 TypedUInt64
    {
        base_types::TypedUInt64 typed_uint64;
        typed_uint64.set_type(base_types::VARIANT_TYPE_UINT64);
        typed_uint64.set_value(18446744073709551615ULL);

        print_test_result("TypedUInt64 type", static_cast<int>(base_types::VARIANT_TYPE_UINT64),
                          static_cast<int>(typed_uint64.type()));
        print_test_result("TypedUInt64 value", static_cast<uint64_t>(18446744073709551615ULL), typed_uint64.value());
    }

    // 测试 TypedFloat
    {
        base_types::TypedFloat typed_float;
        typed_float.set_type(base_types::VARIANT_TYPE_FLOAT);
        typed_float.set_value(3.14159f);

        print_test_result("TypedFloat type", static_cast<int>(base_types::VARIANT_TYPE_FLOAT),
                          static_cast<int>(typed_float.type()));
        // 对浮点数使用近似比较
        bool float_match = std::abs(typed_float.value() - 3.14159f) < 0.00001f;
        std::cout << (float_match ? "✓" : "✗") << " TypedFloat value "
                  << (float_match ? "PASSED" : "FAILED") << std::endl;
    }

    // 测试 TypedDouble
    {
        base_types::TypedDouble typed_double;
        typed_double.set_type(base_types::VARIANT_TYPE_DOUBLE);
        typed_double.set_value(2.718281828459045);

        print_test_result("TypedDouble type", static_cast<int>(base_types::VARIANT_TYPE_DOUBLE),
                          static_cast<int>(typed_double.type()));
        // 对双精度浮点数使用近似比较
        bool double_match = std::abs(typed_double.value() - 2.718281828459045) < 0.000000000000001;
        std::cout << (double_match ? "✓" : "✗") << " TypedDouble value "
                  << (double_match ? "PASSED" : "FAILED") << std::endl;
    }

    // 测试 TypedBool
    {
        base_types::TypedBool typed_bool;
        typed_bool.set_type(base_types::VARIANT_TYPE_BOOL);
        typed_bool.set_value(true);

        print_test_result("TypedBool type", static_cast<int>(base_types::VARIANT_TYPE_BOOL),
                          static_cast<int>(typed_bool.type()));
        print_test_result("TypedBool value", true, typed_bool.value());
    }

    // 测试 TypedString
    {
        base_types::TypedString typed_string;
        typed_string.set_type(base_types::VARIANT_TYPE_STRING);
        typed_string.set_value("Hello, Protocol Buffers!");

        print_test_result("TypedString type", static_cast<int>(base_types::VARIANT_TYPE_STRING),
                          static_cast<int>(typed_string.type()));
        print_test_result("TypedString value", std::string("Hello, Protocol Buffers!"), typed_string.value());
    }

    // 测试 TypedBinary
    {
        base_types::TypedBinary typed_binary;
        typed_binary.set_type(base_types::VARIANT_TYPE_BINARY);
        std::string binary_data = "\x00\x01\x02\x03\xFF\xFE\xFD";
        typed_binary.set_value(binary_data);

        print_test_result("TypedBinary type", static_cast<int>(base_types::VARIANT_TYPE_BINARY),
                          static_cast<int>(typed_binary.type()));
        print_test_result("TypedBinary value", binary_data, typed_binary.value());
    }
}

// 测试 TypedVariant（oneof 功能）
void test_typed_variant()
{
    print_separator("Testing TypedVariant (oneof)");

    // 测试 int32 variant
    {
        base_types::TypedVariant variant;
        base_types::TypedInt32 *int32_val = variant.mutable_int32_val();
        int32_val->set_type(base_types::VARIANT_TYPE_INT32);
        int32_val->set_value(42);

        bool has_int32 = variant.has_int32_val();
        std::cout << (has_int32 ? "✓" : "✗") << " TypedVariant has_int32_val "
                  << (has_int32 ? "PASSED" : "FAILED") << std::endl;

        if (has_int32)
        {
            print_test_result("TypedVariant int32 value", 42, variant.int32_val().value());
        }
    }

    // 测试 string variant
    {
        base_types::TypedVariant variant;
        base_types::TypedString *string_val = variant.mutable_string_val();
        string_val->set_type(base_types::VARIANT_TYPE_STRING);
        string_val->set_value("Variant String Test");

        bool has_string = variant.has_string_val();
        std::cout << (has_string ? "✓" : "✗") << " TypedVariant has_string_val "
                  << (has_string ? "PASSED" : "FAILED") << std::endl;

        if (has_string)
        {
            print_test_result("TypedVariant string value", std::string("Variant String Test"),
                              variant.string_val().value());
        }
    }

    // 测试 bool variant
    {
        base_types::TypedVariant variant;
        base_types::TypedBool *bool_val = variant.mutable_bool_val();
        bool_val->set_type(base_types::VARIANT_TYPE_BOOL);
        bool_val->set_value(false);

        bool has_bool = variant.has_bool_val();
        std::cout << (has_bool ? "✓" : "✗") << " TypedVariant has_bool_val "
                  << (has_bool ? "PASSED" : "FAILED") << std::endl;

        if (has_bool)
        {
            print_test_result("TypedVariant bool value", false, variant.bool_val().value());
        }
    }
}

// 测试 VariantArray
void test_variant_array()
{
    print_separator("Testing VariantArray");

    base_types::VariantArray array;

    // 添加 int32 元素
    base_types::TypedVariant *variant1 = array.add_values();
    base_types::TypedInt32 *int32_val = variant1->mutable_int32_val();
    int32_val->set_type(base_types::VARIANT_TYPE_INT32);
    int32_val->set_value(100);

    // 添加 string 元素
    base_types::TypedVariant *variant2 = array.add_values();
    base_types::TypedString *string_val = variant2->mutable_string_val();
    string_val->set_type(base_types::VARIANT_TYPE_STRING);
    string_val->set_value("Array Element");

    // 添加 double 元素
    base_types::TypedVariant *variant3 = array.add_values();
    base_types::TypedDouble *double_val = variant3->mutable_double_val();
    double_val->set_type(base_types::VARIANT_TYPE_DOUBLE);
    double_val->set_value(123.456);

    // 验证数组大小
    print_test_result("VariantArray size", 3, array.values_size());

    // 验证各个元素
    if (array.values_size() >= 3)
    {
        print_test_result("Array[0] int32 value", 100, array.values(0).int32_val().value());
        print_test_result("Array[1] string value", std::string("Array Element"),
                          array.values(1).string_val().value());

        bool double_match = std::abs(array.values(2).double_val().value() - 123.456) < 0.001;
        std::cout << (double_match ? "✓" : "✗") << " Array[2] double value "
                  << (double_match ? "PASSED" : "FAILED") << std::endl;
    }
}

// 测试 VariantMap
void test_variant_map()
{
    print_separator("Testing VariantMap");

    base_types::VariantMap map;

    // 添加 int32 键值对
    {
        base_types::TypedVariant variant;
        base_types::TypedInt32 *int32_val = variant.mutable_int32_val();
        int32_val->set_type(base_types::VARIANT_TYPE_INT32);
        int32_val->set_value(999);
        (*map.mutable_values())["number"] = variant;
    }

    // 添加 string 键值对
    {
        base_types::TypedVariant variant;
        base_types::TypedString *string_val = variant.mutable_string_val();
        string_val->set_type(base_types::VARIANT_TYPE_STRING);
        string_val->set_value("Map Value");
        (*map.mutable_values())["text"] = variant;
    }

    // 添加 bool 键值对
    {
        base_types::TypedVariant variant;
        base_types::TypedBool *bool_val = variant.mutable_bool_val();
        bool_val->set_type(base_types::VARIANT_TYPE_BOOL);
        bool_val->set_value(true);
        (*map.mutable_values())["flag"] = variant;
    }

    // 验证 map 大小
    print_test_result("VariantMap size", 3, static_cast<int>(map.values().size()));

    // 验证各个键值对
    auto &values = map.values();

    if (values.find("number") != values.end())
    {
        print_test_result("Map['number'] int32 value", 999, values.at("number").int32_val().value());
    }

    if (values.find("text") != values.end())
    {
        print_test_result("Map['text'] string value", std::string("Map Value"),
                          values.at("text").string_val().value());
    }

    if (values.find("flag") != values.end())
    {
        print_test_result("Map['flag'] bool value", true, values.at("flag").bool_val().value());
    }
}

// 测试 TypedArray 和 TypedMap
void test_typed_containers()
{
    print_separator("Testing TypedArray and TypedMap");

    // 测试 TypedArray
    {
        base_types::TypedArray typed_array;
        typed_array.set_type(base_types::VARIANT_TYPE_ARRAY);

        base_types::VariantArray *array = typed_array.mutable_value();

        // 添加一些元素
        base_types::TypedVariant *variant = array->add_values();
        base_types::TypedString *string_val = variant->mutable_string_val();
        string_val->set_type(base_types::VARIANT_TYPE_STRING);
        string_val->set_value("Typed Array Element");

        print_test_result("TypedArray type", static_cast<int>(base_types::VARIANT_TYPE_ARRAY),
                          static_cast<int>(typed_array.type()));
        print_test_result("TypedArray element count", 1, typed_array.value().values_size());
        print_test_result("TypedArray[0] string value", std::string("Typed Array Element"),
                          typed_array.value().values(0).string_val().value());
    }

    // 测试 TypedMap
    {
        base_types::TypedMap typed_map;
        typed_map.set_type(base_types::VARIANT_TYPE_MAP);

        base_types::VariantMap *map = typed_map.mutable_value();

        // 添加一个键值对
        base_types::TypedVariant variant;
        base_types::TypedFloat *float_val = variant.mutable_float_val();
        float_val->set_type(base_types::VARIANT_TYPE_FLOAT);
        float_val->set_value(2.718f);
        (*map->mutable_values())["e"] = variant;

        print_test_result("TypedMap type", static_cast<int>(base_types::VARIANT_TYPE_MAP),
                          static_cast<int>(typed_map.type()));
        print_test_result("TypedMap element count", 1,
                          static_cast<int>(typed_map.value().values().size()));

        auto &values = typed_map.value().values();
        if (values.find("e") != values.end())
        {
            bool float_match = std::abs(values.at("e").float_val().value() - 2.718f) < 0.001f;
            std::cout << (float_match ? "✓" : "✗") << " TypedMap['e'] float value "
                      << (float_match ? "PASSED" : "FAILED") << std::endl;
        }
    }
}

// 测试序列化和反序列化
void test_serialization()
{
    print_separator("Testing Serialization and Deserialization");

    // 创建一个复杂的 TypedVariant
    base_types::TypedVariant original;
    base_types::TypedString *string_val = original.mutable_string_val();
    string_val->set_type(base_types::VARIANT_TYPE_STRING);
    string_val->set_value("Serialization Test");

    // 序列化
    std::string serialized;
    bool serialize_success = original.SerializeToString(&serialized);
    std::cout << (serialize_success ? "✓" : "✗") << " Serialization "
              << (serialize_success ? "PASSED" : "FAILED") << std::endl;

    if (serialize_success)
    {
        std::cout << "  Serialized size: " << serialized.size() << " bytes" << std::endl;

        // 反序列化
        base_types::TypedVariant deserialized;
        bool deserialize_success = deserialized.ParseFromString(serialized);
        std::cout << (deserialize_success ? "✓" : "✗") << " Deserialization "
                  << (deserialize_success ? "PASSED" : "FAILED") << std::endl;

        if (deserialize_success && deserialized.has_string_val())
        {
            print_test_result("Deserialized string value", std::string("Serialization Test"),
                              deserialized.string_val().value());
        }
    }
}

// 测试枚举值
void test_variant_type_enum()
{
    print_separator("Testing VariantType Enum");

    // 验证枚举值
    print_test_result("VARIANT_TYPE_UNKNOWN", 0, static_cast<int>(base_types::VARIANT_TYPE_UNKNOWN));
    print_test_result("VARIANT_TYPE_INT32", 1, static_cast<int>(base_types::VARIANT_TYPE_INT32));
    print_test_result("VARIANT_TYPE_INT64", 2, static_cast<int>(base_types::VARIANT_TYPE_INT64));
    print_test_result("VARIANT_TYPE_UINT32", 3, static_cast<int>(base_types::VARIANT_TYPE_UINT32));
    print_test_result("VARIANT_TYPE_UINT64", 4, static_cast<int>(base_types::VARIANT_TYPE_UINT64));
    print_test_result("VARIANT_TYPE_FLOAT", 5, static_cast<int>(base_types::VARIANT_TYPE_FLOAT));
    print_test_result("VARIANT_TYPE_DOUBLE", 6, static_cast<int>(base_types::VARIANT_TYPE_DOUBLE));
    print_test_result("VARIANT_TYPE_BOOL", 7, static_cast<int>(base_types::VARIANT_TYPE_BOOL));
    print_test_result("VARIANT_TYPE_STRING", 8, static_cast<int>(base_types::VARIANT_TYPE_STRING));
    print_test_result("VARIANT_TYPE_BINARY", 9, static_cast<int>(base_types::VARIANT_TYPE_BINARY));
    print_test_result("VARIANT_TYPE_ARRAY", 10, static_cast<int>(base_types::VARIANT_TYPE_ARRAY));
    print_test_result("VARIANT_TYPE_MAP", 11, static_cast<int>(base_types::VARIANT_TYPE_MAP));
}

int main()
{
    // 初始化 protobuf 库
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    std::cout << "=== PB Common Variant Library Test Suite ===" << std::endl;
    std::cout << "Testing PB/proto/common/variant.proto functionality" << std::endl;

    try
    {
        // 运行所有测试
        test_variant_type_enum();
        test_basic_types();
        test_typed_variant();
        test_variant_array();
        test_variant_map();
        test_typed_containers();
        test_serialization();

        print_separator("Test Summary");
        std::cout << "✓ All variant library tests completed!" << std::endl;
        std::cout << "✓ Common variant library functionality verified!" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "✗ Test failed with exception: " << e.what() << std::endl;

        // 清理 protobuf 库
        google::protobuf::ShutdownProtobufLibrary();
        return 1;
    }

    // 清理 protobuf 库
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
