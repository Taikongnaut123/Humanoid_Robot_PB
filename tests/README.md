# PB 测试模块

## 概述
此目录包含 PB 库的所有测试程序。测试程序通过 CMake 管理，输出到项目根目录定义的 `OUTPUT_BIN_DIR` 路径。

## 测试文件

### test_detection_only.cpp
- **功能**: 测试 detection 子模块的基本功能
- **测试内容**: BoundingBox、DetectionResult 消息的创建、序列化和反序列化
- **依赖库**: libdetection.so

### test_perception_only.cpp  
- **功能**: 测试 perception 子模块的基本功能
- **测试内容**: TrackRow、Img、MasksTracks 消息的创建、序列化和反序列化
- **依赖库**: libperception.so

### test_pb_combined.cpp
- **功能**: 综合测试两个子模块的基本功能
- **测试内容**: 同时验证 detection 和 perception 模块的核心功能
- **依赖库**: libdetection.so, libperception.so

## 构建配置

### CMake 选项
- `BUILD_PB_TESTS`: 控制是否构建测试程序（默认 ON）

### 输出路径
测试可执行文件输出到: `${OUTPUT_BIN_DIR}`
- 通常为: `build/bin/linux_x64/release/`

## 构建方法

### 1. 完整构建（包含测试）
```bash
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=/home/ubuntu/vcpkg/scripts/buildsystems/vcpkg.cmake
make
```

### 2. 禁用测试构建
```bash
cmake .. -DBUILD_PB_TESTS=OFF -DCMAKE_TOOLCHAIN_FILE=/home/ubuntu/vcpkg/scripts/buildsystems/vcpkg.cmake
make
```

### 3. 只构建特定测试
```bash
make test_detection_only
make test_perception_only  
make test_pb_combined
```

## 运行测试

### 设置环境
```bash
export LD_LIBRARY_PATH=build/lib/linux_x64/release:build/vcpkg_installed/x64-linux/lib
```

### 运行单个测试
```bash
cd build/bin/linux_x64/release
./test_detection_only
./test_perception_only
./test_pb_combined
```

### 运行所有测试
```bash
make test  # 如果配置了 CTest
```

## 测试输出示例

### 成功输出
```
=== detection 模块测试 ===
✓ BoundingBox 创建成功
✓ DetectionResult 创建成功
✓ 序列化成功，大小: 45 字节
✓ 反序列化成功
✓ 所有测试通过
```

### 失败输出
```
✗ detection 模块测试失败: [错误信息]
```

## 添加新测试

### 1. 创建测试文件
在此目录下创建新的 `.cpp` 文件，遵循命名规范：
- `test_[模块名]_[功能].cpp`

### 2. 自动识别
CMake 会自动识别所有 `*.cpp` 文件并创建对应的可执行文件。

### 3. 库链接规则
- 文件名包含 `detection`: 自动链接 libdetection.so
- 文件名包含 `perception`: 自动链接 libperception.so  
- 其他: 链接所有可用库

## 故障排除

### 常见问题
1. **找不到动态库**: 检查 `LD_LIBRARY_PATH` 设置
2. **编译错误**: 确认 vcpkg 工具链正确配置
3. **链接错误**: 确认 PB 库已成功构建

### 调试方法
```bash
# 检查库依赖
ldd test_detection_only

# 检查库搜索路径
echo $LD_LIBRARY_PATH

# 详细构建信息
make VERBOSE=1
```
