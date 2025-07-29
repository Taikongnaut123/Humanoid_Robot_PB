# PB - Protobuf 代码生成工具

## 目录结构

```
PB/
├── generate_proto.sh           # protobuf代码生成脚本
├── proto/                      # proto定义文件目录
│   ├── perception/             # perception模块目录
│   │   └── imgs.proto          # 图像处理相关proto定义
│   └── detection/              # detection模块目录
│       └── detection.proto     # 目标检测相关proto定义
├── include/                    # 生成的头文件目录
│   ├── pb_export.h            # 导出符号定义文件
│   ├── perception/            # perception模块头文件目录
│   │   ├── imgs.pb.h          # protobuf生成的头文件
│   │   └── imgs.grpc.pb.h     # gRPC生成的头文件
│   └── detection/             # detection模块头文件目录
│       ├── detection.pb.h     # protobuf生成的头文件
│       └── detection.grpc.pb.h # gRPC生成的头文件
└── source/                     # 生成的源文件目录
    ├── perception/            # perception模块源文件目录
    │   ├── imgs.pb.cc         # protobuf生成的源文件
    │   └── imgs.grpc.pb.cc    # gRPC生成的源文件
    └── detection/             # detection模块源文件目录
        ├── detection.pb.cc    # protobuf生成的源文件
        └── detection.grpc.pb.cc # gRPC生成的源文件
```

## 使用方法

### 基本用法

```bash
cd PB
./generate_proto.sh <proto_directory_path> <export_symbol>
```

**参数说明:**
- `proto_directory_path`: proto文件所在目录路径（必需）
- `export_symbol`: 导入导出符号名称（必需）

### 使用示例

#### 1. 处理perception模块（图像处理）

```bash
cd PB
./generate_proto.sh proto/perception PERCEPTION_API
```
这将为perception模块生成使用`PERCEPTION_API`导出符号的代码。

#### 2. 处理detection模块（目标检测）

```bash
cd PB
./generate_proto.sh proto/detection DETECTION_API
```
这将为detection模块生成使用`DETECTION_API`导出符号的代码。

#### 3. 使用原有的符号命名规范

```bash
cd PB
./generate_proto.sh proto/perception IMAGEPROCESSING_PB_API
./generate_proto.sh proto/detection DETECTION_PB_API
```

#### 4. 批量处理多个模块

```bash
cd PB
# 为不同模块使用不同的导出符号
./generate_proto.sh proto/perception PERCEPTION_API
./generate_proto.sh proto/detection DETECTION_API  
./generate_proto.sh proto/tracking TRACKING_API
./generate_proto.sh proto/navigation NAVIGATION_API
```

#### 5. 使用绝对路径

```bash
cd PB
./generate_proto.sh /absolute/path/to/your/proto CUSTOM_API_SYMBOL
```

### 添加新的模块

要添加新的模块（比如 `tracking`），只需：

1. 创建目录：`mkdir -p proto/tracking`
2. 放入proto文件：`tracking.proto`
3. 运行脚本：`./generate_proto.sh proto/tracking TRACKING_API`

生成的文件将自动放置在：
- `include/tracking/tracking.pb.h`（包含TRACKING_API符号）
- `include/tracking/tracking.grpc.pb.h`（包含TRACKING_API符号）
- `source/tracking/tracking.pb.cc`
- `source/tracking/tracking.grpc.pb.cc`

## 特性

- **🎯 自定义导出符号**: 为不同模块使用不同的导入导出符号，支持模块化构建
- **⚡ 自动化符号注入**: 自动为所有生成的protobuf和gRPC类添加指定的导出符号
- **📁 灵活的目录结构**: 支持任意深度的模块目录，自动创建对应的输出结构
- **🔄 批量处理**: 一次处理指定目录下的多个proto文件
- **🛠️ 路径自适应**: 自动处理相对路径和包含路径，支持绝对路径和相对路径
- **✅ 智能包含**: 根据目录深度自动调整`pb_export.h`的包含路径
- **📋 详细输出**: 显示处理进度、生成文件位置和完整目录结构

## 支持的proto功能

- **标准protobuf消息**: 自动为所有message类添加导出符号
- **gRPC服务**: 为Service、Stub、StubInterface类添加导出符号
- **嵌套消息**: 支持嵌套消息和枚举的符号注入
- **流式RPC**: 支持单向流和双向流的gRPC服务

## 前置条件

1. 确保 `grpc_project` 已经通过cmake配置，vcpkg依赖已安装
2. `pb_export.h` 文件存在于 `include/` 目录

## 输出

脚本执行后会显示：
- 找到的proto文件数量和文件名
- 每个文件的详细处理进度
- 导出符号注入状态
- 生成的文件位置
- 完整的目录结构

### 输出示例

```
=== 通用 Protobuf 代码生成脚本 ===
PB 根目录: /path/to/PB
Proto目录参数: proto/perception
导出符号: PERCEPTION_API
搜索 Proto 文件目录: /path/to/PB/proto/perception
找到 1 个 proto 文件:
  - imgs.proto

=== 处理: /path/to/PB/proto/perception/imgs.proto ===
  相对路径: perception/imgs.proto
  目录: perception
  文件名: imgs
  1. 生成protobuf代码...
  ✓ 已移动: include/perception/imgs.pb.h
  ✓ 已移动: include/perception/imgs.grpc.pb.h
  ✓ 已移动: source/perception/imgs.pb.cc
  ✓ 已移动: source/perception/imgs.grpc.pb.cc
  2. 添加导出符号到头文件...
    处理: include/perception/imgs.pb.h
      ✓ 已添加导出符号(PERCEPTION_API)到protobuf类
    处理: include/perception/imgs.grpc.pb.h
      ✓ 已添加导出符号(PERCEPTION_API)到gRPC类
  ✓ 处理完成: imgs

=== 生成完成! ===
```

## 故障排除

### 常见错误

1. **错误: protoc工具不存在**
   ```
   解决方案: 确保grpc_project已经通过cmake配置，运行以下命令：
   cd ../grpc_project/build && cmake ..
   ```

2. **错误: Proto搜索目录不存在**
   ```
   解决方案: 检查proto目录路径是否正确，确保目录存在
   mkdir -p proto/your_module
   ```

3. **参数错误**
   ```
   解决方案: 确保提供两个参数：
   ./generate_proto.sh proto/module_name EXPORT_SYMBOL
   ```

### 验证生成结果

检查导出符号是否正确注入：
```bash
# 检查protobuf类
grep -n "YOUR_EXPORT_SYMBOL" include/module/file.pb.h

# 检查gRPC类  
grep -n "YOUR_EXPORT_SYMBOL" include/module/file.grpc.pb.h
```

## 完整工作流程示例

### 1. 准备工作
```bash
# 确保grpc_project已配置
cd ../grpc_project/build
cmake ..

# 回到PB目录
cd ../PB
```

### 2. 创建新模块
```bash
# 创建tracking模块
mkdir -p proto/tracking

# 创建proto文件（示例）
cat > proto/tracking/tracking.proto << 'EOF'
syntax = "proto3";
package tracking;

message Track {
    string id = 1;
    float x = 2;
    float y = 3;
}

service TrackingService {
    rpc UpdateTrack(Track) returns (Track);
}
EOF
```

### 3. 生成代码
```bash
# 为tracking模块生成代码
./generate_proto.sh proto/tracking TRACKING_API

# 检查生成结果
find include/tracking source/tracking -name "*.h" -o -name "*.cc"
```

### 4. 验证符号
```bash
# 验证导出符号
grep "TRACKING_API" include/tracking/tracking.pb.h
grep "TRACKING_API" include/tracking/tracking.grpc.pb.h
```

现在您就有了一个完整的、带有自定义导出符号的protobuf代码生成系统！
