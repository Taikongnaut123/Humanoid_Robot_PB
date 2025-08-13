#include "printUtil.h"

using namespace humanoid_robot::utils::PB;
using namespace humanoid_robot::PB::common;

#define CASE_PRINT(VARIANT_TYPE, VARIANT_FUNC)                             \
    case humanoid_robot::PB::common::Variant::K##VARIANT_TYPE##Value:      \
    {                                                                      \
        std::cout << #VARIANT_TYPE << ": " << VARIANT_FUNC() << std::endl; \
        break;                                                             \
    }

#define CASE_PRINT_ARRAY(VARIANT_TYPE, VARIANT_FUNC)                       \
    case humanoid_robot::PB::common::Variant::K##VARIANT_TYPE##ArrayValue: \
    {                                                                      \
        std::cout << #VARIANT_TYPE " Array: ";                             \
        auto &array = VARIANT_FUNC();                                      \
        for (const auto &item : array)                                     \
        {                                                                  \
            std::cout << item << " ";                                      \
        }                                                                  \
        std::cout << std::endl;                                            \
        break;                                                             \
    }

#define CASE_PRINT_DATE(VARIANT_TYPE, VARIANT_FUNC)                   \
    case humanoid_robot::PB::common::Variant::K##VARIANT_TYPE##Value: \
    {                                                                 \
        auto &date = VARIANT_FUNC();                                  \
        std::cout << #VARIANT_TYPE << ": "                            \
                  << date.year() << "-"                               \
                  << date.month() << "-"                              \
                  << date.day() << std::endl;                         \
        break;                                                        \
    }

#define CASE_PRINT_TIMESTAMP(VARIANT_TYPE, VARIANT_FUNC)              \
    case humanoid_robot::PB::common::Variant::K##VARIANT_TYPE##Value: \
    {                                                                 \
        auto &timestamp = VARIANT_FUNC();                             \
        std::cout << #VARIANT_TYPE << ": "                            \
                  << timestamp.seconds() << "."                       \
                  << timestamp.nanos() << std::endl;                  \
        break;                                                        \
    }

#define CASE_PRINT_DICT(VARIANT_TYPE, VARIANT_FUNC)                    \
    case humanoid_robot::PB::common::Variant::K##VARIANT_TYPE##Value:  \
    {                                                                  \
        auto map = VARIANT_FUNC();                                     \
        std::cout << #VARIANT_TYPE << ": "                             \
                  << map.keyvaluelist_size() << " items" << std::endl; \
        for (const auto &item : map.keyvaluelist())                    \
        {                                                              \
            std::cout << "  " << item.first << ": ";                   \
            ::humanoid_robot::utils::PB::print_variant(item.second);   \
        }                                                              \
        break;                                                         \
    }

namespace humanoid_robot::utils::PB
{
    // 用于打印Variant的不同类型
    void print_variant(const humanoid_robot::PB::common::Variant &var)
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

    void print_keyvaluelist(const ::google::protobuf::Map<std::string, ::humanoid_robot::PB::common::Variant> &keyvaluelist)
    {
        for (const auto &pair : keyvaluelist)
        {
            std::cout << pair.first << ": ";
            // 显式转换类型，避免重载歧义
            print_variant(pair.second);
        }
    }

    void print_section(const std::string &section_name)
    {
        std::cout << "\n=== " << section_name << " ===" << std::endl;
    }
} // namespace humanoid_robot::utils::PB
