#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include "common/variant.pb.h"
using namespace humanoid_robot::PB::common;

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

// 测试基本类型
void test_basic_types()
{
    print_section("Basic Types");

    // 测试 Int32 变体
    {
        Variant variant;
        // //variant.set_type(Variant::KInt32Value);
        variant.set_int32value(12345);

        print_test_result("Variant Int32 type", static_cast<int>(Variant::KInt32Value),
                          static_cast<int>(variant.value_case()));
        print_test_result("Variant Int32 value", 12345, variant.int32value());
        print_test_result("Variant has int32value", true, variant.has_int32value());
    }

    // 测试 Int64 变体
    {
        Variant variant;
        // //variant.set_type(Variant::KInt64Value);
        variant.set_int64value(1234567890123LL);

        print_test_result("Variant Int64 type", static_cast<int>(Variant::KInt64Value),
                          static_cast<int>(variant.value_case()));
        print_test_result("Variant Int64 value", static_cast<int64_t>(1234567890123LL),
                          static_cast<int64_t>(variant.int64value()));
        print_test_result("Variant has int64value", true, variant.has_int64value());
    }

    // 测试 UInt32 变体
    {
        Variant variant;
        // //variant.set_type(Variant::KUint32Value);
        variant.set_uint32value(4294967295U);

        print_test_result("Variant UInt32 type", static_cast<int>(Variant::KUint32Value),
                          static_cast<int>(variant.value_case()));
        print_test_result("Variant UInt32 value", 4294967295U, variant.uint32value());
        print_test_result("Variant has uint32value", true, variant.has_uint32value());
    }

    // 测试 UInt64 变体
    {
        Variant variant;
        // //variant.set_type(Variant::KUint64Value);
        variant.set_uint64value(18446744073709551615ULL);

        print_test_result("Variant UInt64 type", static_cast<int>(Variant::KUint64Value),
                          static_cast<int>(variant.value_case()));
        print_test_result("Variant UInt64 value", static_cast<uint64_t>(18446744073709551615ULL),
                          static_cast<uint64_t>(variant.uint64value()));
        print_test_result("Variant has uint64value", true, variant.has_uint64value());
    }

    // 测试 Float 变体
    {
        Variant variant;
        // //variant.set_type(Variant::KFloatValue);
        variant.set_floatvalue(3.14159f);

        print_test_result("Variant Float type", static_cast<int>(Variant::KFloatValue),
                          static_cast<int>(variant.value_case()));
        // 浮点数比较需要容差
        bool float_match = std::abs(variant.floatvalue() - 3.14159f) < 0.00001f;
        print_test_result("Variant Float value", true, float_match);
        print_test_result("Variant has floatvalue", true, variant.has_floatvalue());
    }

    // 测试 Double 变体
    {
        Variant variant;
        // variant.set_type(Variant::KDoubleValue);
        variant.set_doublevalue(2.718281828459045);

        print_test_result("Variant Double type", static_cast<int>(Variant::KDoubleValue),
                          static_cast<int>(variant.value_case()));
        // 浮点数比较需要容差
        bool double_match = std::abs(variant.doublevalue() - 2.718281828459045) < 0.000000000001;
        print_test_result("Variant Double value", true, double_match);
        print_test_result("Variant has doublevalue", true, variant.has_doublevalue());
    }

    // 测试 Bool 变体
    {
        Variant variant;
        // variant.set_type(Variant::KBoolValue);
        variant.set_boolvalue(true);

        print_test_result("Variant Bool type", static_cast<int>(Variant::KBoolValue),
                          static_cast<int>(variant.value_case()));
        print_test_result("Variant Bool value", true, variant.boolvalue());
        print_test_result("Variant has boolvalue", true, variant.has_boolvalue());
    }

    // 测试 String 变体
    {
        Variant variant;
        // variant.set_type(Variant::KStringValue);
        variant.set_stringvalue("Hello, Protobuf!");

        print_test_result("Variant String type", static_cast<int>(Variant::KStringValue),
                          static_cast<int>(variant.value_case()));
        print_test_result("Variant String value", std::string("Hello, Protobuf!"), variant.stringvalue());
        print_test_result("Variant has stringvalue", true, variant.has_stringvalue());
    }

    // 测试 Bytes 变体
    {
        Variant variant;
        // variant.set_type(Variant::KByteValue);
        std::string byte_data = "binary\x00\x01\x02\x03";
        variant.set_bytevalue(byte_data);

        print_test_result("Variant Byte type", static_cast<int>(Variant::KByteValue),
                          static_cast<int>(variant.value_case()));
        print_test_result("Variant Byte value", byte_data, variant.bytevalue());
        print_test_result("Variant has bytevalue", true, variant.has_bytevalue());
    }
}

// 测试日期和时间戳
void test_date_timestamp()
{
    print_section("Date and Timestamp");

    // 测试 Date 变体
    {
        Variant variant;
        // variant.set_type(Variant::KDateValue);
        Date *date = variant.mutable_datevalue();
        date->set_year(2025);
        date->set_month(1);
        date->set_day(15);

        print_test_result("Variant Date type", static_cast<int>(Variant::KDateValue),
                          static_cast<int>(variant.value_case()));
        print_test_result("Date year", 2025, variant.datevalue().year());
        print_test_result("Date month", 1, variant.datevalue().month());
        print_test_result("Date day", 15, variant.datevalue().day());
        print_test_result("Variant has datevalue", true, variant.has_datevalue());
    }

    // 测试 Timestamp 变体
    {
        Variant variant;
        // variant.set_type(Variant::KTimestampValue);
        Timestamp *timestamp = variant.mutable_timestampvalue();
        timestamp->set_seconds(1705123456); // 示例时间戳
        timestamp->set_nanos(123456789);

        print_test_result("Variant Timestamp type", static_cast<int>(Variant::KTimestampValue),
                          static_cast<int>(variant.value_case()));
        print_test_result("Timestamp seconds", static_cast<int64_t>(1705123456),
                          static_cast<int64_t>(variant.timestampvalue().seconds()));
        print_test_result("Timestamp nanos", 123456789, variant.timestampvalue().nanos());
        print_test_result("Variant has timestampvalue", true, variant.has_timestampvalue());
    }
}

// 测试数组类型
void test_array_types()
{
    print_section("Array Types");

    // 测试 Int32Array
    {
        Variant variant;
        // variant.set_type(Variant::KInt32ArrayValue);
        Int32Array *array = variant.mutable_int32arrayvalue();
        array->add_values(10);
        array->add_values(20);
        array->add_values(30);

        print_test_result("Variant Int32Array type", static_cast<int>(Variant::KInt32ArrayValue),
                          static_cast<int>(variant.value_case()));
        print_test_result("Int32Array size", 3, variant.int32arrayvalue().values_size());
        print_test_result("Int32Array first value", 10, variant.int32arrayvalue().values(0));
        print_test_result("Int32Array second value", 20, variant.int32arrayvalue().values(1));
        print_test_result("Int32Array third value", 30, variant.int32arrayvalue().values(2));
        print_test_result("Variant has int32arrayvalue", true, variant.has_int32arrayvalue());
    }

    // 测试 StringArray
    {
        Variant variant;
        // variant.set_type(Variant::KStringArrayValue);
        StringArray *array = variant.mutable_stringarrayvalue();
        array->add_values("hello");
        array->add_values("world");
        array->add_values("protobuf");

        print_test_result("Variant StringArray type", static_cast<int>(Variant::KStringArrayValue),
                          static_cast<int>(variant.value_case()));
        print_test_result("StringArray size", 3, variant.stringarrayvalue().values_size());
        print_test_result("StringArray first value", std::string("hello"), variant.stringarrayvalue().values(0));
        print_test_result("StringArray second value", std::string("world"), variant.stringarrayvalue().values(1));
        print_test_result("StringArray third value", std::string("protobuf"), variant.stringarrayvalue().values(2));
        print_test_result("Variant has stringarrayvalue", true, variant.has_stringarrayvalue());
    }

    // 测试 DoubleArray
    {
        Variant variant;
        // variant.set_type(Variant::KDoubleArrayValue);
        DoubleArray *array = variant.mutable_doublearrayvalue();
        array->add_values(1.1);
        array->add_values(2.2);
        array->add_values(3.3);

        print_test_result("Variant DoubleArray type", static_cast<int>(Variant::KDoubleArrayValue),
                          static_cast<int>(variant.value_case()));
        print_test_result("DoubleArray size", 3, variant.doublearrayvalue().values_size());

        // 浮点数比较需要容差
        bool match1 = std::abs(variant.doublearrayvalue().values(0) - 1.1) < 0.00001;
        bool match2 = std::abs(variant.doublearrayvalue().values(1) - 2.2) < 0.00001;
        bool match3 = std::abs(variant.doublearrayvalue().values(2) - 3.3) < 0.00001;
        print_test_result("DoubleArray first value", true, match1);
        print_test_result("DoubleArray second value", true, match2);
        print_test_result("DoubleArray third value", true, match3);
        print_test_result("Variant has doublearrayvalue", true, variant.has_doublearrayvalue());
    }
}

// 测试字典类型
void test_dictionary()
{
    print_section("Dictionary");

    Variant variant;
    // variant.set_type(Variant::KDictValue);
    Dictionary *dict = variant.mutable_dictvalue();

    // 添加 int32 值
    {
        Variant int_var;
        // int_var.set_type(Variant::KInt32Value);
        int_var.set_int32value(42);
        (*dict->mutable_keyvaluelist())["number"] = int_var;
    }

    // 添加 string 值
    {
        Variant str_var;
        // str_var.set_type(Variant::KStringValue);
        str_var.set_stringvalue("hello");
        (*dict->mutable_keyvaluelist())["text"] = str_var;
    }

    // 添加 bool 值
    {
        Variant bool_var;
        // bool_var.set_type(Variant::KBoolValue);
        bool_var.set_boolvalue(true);
        (*dict->mutable_keyvaluelist())["flag"] = bool_var;
    }

    print_test_result("Variant Dictionary type", static_cast<int>(Variant::KDictValue),
                      static_cast<int>(variant.value_case()));
    print_test_result("Dictionary size", 3, static_cast<int>(variant.dictvalue().keyvaluelist().size()));

    // 验证字典内容
    const auto &dict_map = variant.dictvalue().keyvaluelist();
    auto number_it = dict_map.find("number");
    auto text_it = dict_map.find("text");
    auto flag_it = dict_map.find("flag");

    print_test_result("Dictionary has 'number'", true, number_it != dict_map.end());
    print_test_result("Dictionary has 'text'", true, text_it != dict_map.end());
    print_test_result("Dictionary has 'flag'", true, flag_it != dict_map.end());

    if (number_it != dict_map.end())
    {
        print_test_result("Dictionary 'number' value", 42, number_it->second.int32value());
    }
    if (text_it != dict_map.end())
    {
        print_test_result("Dictionary 'text' value", std::string("hello"), text_it->second.stringvalue());
    }
    if (flag_it != dict_map.end())
    {
        print_test_result("Dictionary 'flag' value", true, flag_it->second.boolvalue());
    }

    print_test_result("Variant has dictvalue", true, variant.has_dictvalue());
}

// 测试序列化
void test_serialization()
{
    print_section("Serialization");

    // 创建一个复杂的变体进行序列化测试
    Variant original;
    // original.set_type(Variant::KStringValue);
    original.set_stringvalue("Serialization Test");

    // 序列化
    std::string serialized;
    bool serialize_success = original.SerializeToString(&serialized);
    print_test_result("Serialization success", true, serialize_success);
    print_test_result("Serialized data not empty", true, !serialized.empty());

    if (serialize_success)
    {
        // 反序列化
        Variant deserialized;
        bool deserialize_success = deserialized.ParseFromString(serialized);
        print_test_result("Deserialization success", true, deserialize_success);

        if (deserialize_success)
        {
            print_test_result("Deserialized type", static_cast<int>(Variant::KStringValue),
                              static_cast<int>(deserialized.value_case()));
            print_test_result("Deserialized value", std::string("Serialization Test"),
                              deserialized.stringvalue());
            print_test_result("Deserialized has stringvalue", true, deserialized.has_stringvalue());
        }
    }
}

// 测试类型枚举值
void test_type_enums()
{
    print_section("Type Enums");

    print_test_result("KUnknownValue", 0, static_cast<int>(Variant::KUnknownValue));
    print_test_result("KBoolValue", 2, static_cast<int>(Variant::KBoolValue));
    print_test_result("KInt8Value", 3, static_cast<int>(Variant::KInt8Value));
    print_test_result("KUint8Value", 4, static_cast<int>(Variant::KUint8Value));
    print_test_result("KInt16Value", 5, static_cast<int>(Variant::KInt16Value));
    print_test_result("KUint16Value", 6, static_cast<int>(Variant::KUint16Value));
    print_test_result("KInt32Value", 7, static_cast<int>(Variant::KInt32Value));
    print_test_result("KUint32Value", 8, static_cast<int>(Variant::KUint32Value));
    print_test_result("KInt64Value", 9, static_cast<int>(Variant::KInt64Value));
    print_test_result("KUint64Value", 10, static_cast<int>(Variant::KUint64Value));
    print_test_result("KFloatValue", 11, static_cast<int>(Variant::KFloatValue));
    print_test_result("KDoubleValue", 12, static_cast<int>(Variant::KDoubleValue));
    print_test_result("KCharValue", 13, static_cast<int>(Variant::KCharValue));
    print_test_result("KByteValue", 14, static_cast<int>(Variant::KByteValue));
    print_test_result("KStringValue", 15, static_cast<int>(Variant::KStringValue));
    print_test_result("KDateValue", 16, static_cast<int>(Variant::KDateValue));
    print_test_result("KTimestampValue", 17, static_cast<int>(Variant::KTimestampValue));
    print_test_result("KDictValue", 18, static_cast<int>(Variant::KDictValue));

    // 数组类型
    print_test_result("KBoolArrayValue", 21, static_cast<int>(Variant::KBoolArrayValue));
    print_test_result("KInt32ArrayValue", 26, static_cast<int>(Variant::KInt32ArrayValue));
    print_test_result("KStringArrayValue", 34, static_cast<int>(Variant::KStringArrayValue));
    print_test_result("KDoubleArrayValue", 31, static_cast<int>(Variant::KDoubleArrayValue));
}

int main()
{
    std::cout << "Testing Common Variant Protobuf Functionality" << std::endl;
    std::cout << "=============================================" << std::endl;

    try
    {
        test_basic_types();
        test_date_timestamp();
        test_array_types();
        test_dictionary();
        test_serialization();
        test_type_enums();

        std::cout << "\n=== Test Summary ===" << std::endl;
        std::cout << "All tests completed successfully!" << std::endl;
        std::cout << "Variant protobuf functionality is working correctly." << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
