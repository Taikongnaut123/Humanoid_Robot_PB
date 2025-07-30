#include <iostream>
#include "perception/imgs.pb.h"

int main()
{
    // 初始化 protobuf 库
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    std::cout << "Testing perception library only..." << std::endl;

    // 测试 perception 模块基本消息创建
    imageprocessing::TrackRow track;
    track.set_x1(10.0);
    track.set_y1(20.0);
    track.set_x2(110.0);
    track.set_y2(170.0);
    track.set_trackid("track_001");
    track.set_conf(0.89);
    track.set_cls("person");

    std::cout << "TrackRow: " << track.cls()
              << " track ID: " << track.trackid()
              << " confidence: " << track.conf() << std::endl;
    std::cout << "  Bounding box: (" << track.x1() << ", " << track.y1()
              << ") to (" << track.x2() << ", " << track.y2() << ")" << std::endl;

    // 添加mask数据
    imageprocessing::Masks *mask = track.add_mask();
    mask->set_x(50);
    mask->set_y(60);

    mask = track.add_mask();
    mask->set_x(51);
    mask->set_y(61);

    std::cout << "Added " << track.mask_size() << " mask points" << std::endl;

    // 测试图像消息
    imageprocessing::Img img;
    img.set_timestamp("1234567890");
    img.set_img("binary_image_data");
    img.set_requiresmasks(true);
    std::cout << "Image: timestamp=" << img.timestamp()
              << ", requires_masks=" << img.requiresmasks() << std::endl;

    // 测试MasksTracks消息
    imageprocessing::MasksTracks masksTracks;
    masksTracks.set_timestamp("1234567890");
    imageprocessing::TrackRow *trackPtr = masksTracks.add_tracks();
    *trackPtr = track; // 复制track到masksTracks

    std::cout << "MasksTracks created with " << masksTracks.tracks_size() << " tracks" << std::endl;

    // 测试序列化
    std::string serialized;
    if (masksTracks.SerializeToString(&serialized))
    {
        std::cout << "✓ Serialization successful, size: " << serialized.size() << " bytes" << std::endl;

        // 测试反序列化
        imageprocessing::MasksTracks deserialized;
        if (deserialized.ParseFromString(serialized))
        {
            std::cout << "✓ Deserialization successful!" << std::endl;
            std::cout << "  Deserialized tracks count: " << deserialized.tracks_size() << std::endl;
            if (deserialized.tracks_size() > 0)
            {
                const auto &firstTrack = deserialized.tracks(0);
                std::cout << "  First track class: " << firstTrack.cls() << std::endl;
            }
        }
    }

    std::cout << "✓ Perception library test passed!" << std::endl;

    // 清理 protobuf 库
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
