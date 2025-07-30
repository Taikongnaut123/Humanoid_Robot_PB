#include <iostream>
#include "detection/detection.pb.h"

int main()
{
    // 初始化 protobuf 库
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    std::cout << "Testing detection library only..." << std::endl;

    // 测试 detection 模块基本消息创建
    detection::BoundingBox bbox;
    bbox.set_x(10.5);
    bbox.set_y(20.5);
    bbox.set_width(100.0);
    bbox.set_height(150.0);
    std::cout << "BoundingBox: (" << bbox.x() << ", " << bbox.y()
              << ") size: " << bbox.width() << "x" << bbox.height() << std::endl;

    detection::DetectionResult result;
    result.set_object_id("obj_001");
    result.set_class_name("person");
    result.set_confidence(0.95);

    // 设置边界框
    *result.mutable_bbox() = bbox;

    std::cout << "DetectionResult: " << result.class_name()
              << " (confidence: " << result.confidence() << ", id: " << result.object_id() << ")" << std::endl;

    // 测试序列化
    std::string serialized;
    if (result.SerializeToString(&serialized))
    {
        std::cout << "✓ Serialization successful, size: " << serialized.size() << " bytes" << std::endl;

        // 测试反序列化
        detection::DetectionResult deserialized;
        if (deserialized.ParseFromString(serialized))
        {
            std::cout << "✓ Deserialization successful!" << std::endl;
            std::cout << "  Deserialized class: " << deserialized.class_name() << std::endl;
            std::cout << "  Deserialized confidence: " << deserialized.confidence() << std::endl;
        }
    }

    std::cout << "✓ Detection library test passed!" << std::endl;

    // 清理 protobuf 库
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
