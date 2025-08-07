# PB - Protobuf 模块

人形机器人项目的协议缓冲区（Protocol Buffers）模块，包含消息定义、代码生成工具、实用工具库和测试套件。

## 概述

PB 模块是一个完整的 Protobuf/gRPC 代码生成和管理系统，支持：
- 自动化 proto 代码生成
- 模块化的 CMake 构建系统
- 类型安全的打印和调试工具
- 完整的测试覆盖

## 目录结构

```
PB/
├── generate_proto.sh           # protobuf代码生成脚本
├── CMakeLists.txt             # 主 CMake 配置文件
├── cmake/                     # CMake 配置文件
│   └── PBConfig.cmake.in      # PB 配置模板
├── proto/                     # proto定义文件目录
│   ├── common/                # 公共类型定义
│   │   └── variant.proto      # 通用变体类型定义
│   ├── interfaces/            # 接口服务定义
│   │   ├── interfaces_request_response.proto
│   │   ├── interfaces_grpc.proto
│   │   └── interfaces_callback.proto
│   ├── perception/            # 感知模块定义
│   │   └── imgs.proto         # 图像处理相关proto定义
│   └── detection/             # 检测模块定义
│       └── detection.proto    # 目标检测相关proto定义
├── include/                   # 生成的头文件目录
│   ├── pb_export.h           # 导出符号定义文件
│   ├── common/               # 公共类型头文件
│   ├── interfaces/           # 接口服务头文件
│   ├── perception/           # 感知模块头文件
│   └── detection/            # 检测模块头文件
├── source/                   # 生成的源文件目录
│   ├── common/               # 公共类型源文件
│   ├── interfaces/           # 接口服务源文件
│   ├── perception/           # 感知模块源文件
│   └── detection/            # 检测模块源文件
├── utils/                    # 实用工具库
│   ├── CMakeLists.txt        # utils CMake 配置
│   ├── include/              # 工具库头文件
│   │   └── printUtil.h       # 打印和调试工具
│   └── source/               # 工具库源文件
│       └── printUtil.cpp     # 打印工具实现
└── tests/                    # 测试套件
    ├── CMakeLists.txt        # 测试 CMake 配置
    ├── test_common_variant.cpp   # 通用变体类型测试
    └── test_interfaces.cpp   # 接口服务测试
```

## 快速开始

### 1. 构建 PB 模块

```bash
# 在项目根目录下
mkdir -p build
cd build
cmake ..
make -j$(nproc) PB  # 只构建 PB 模块
```

### 2. 运行测试

```bash
# 运行所有 PB 测试
make test_common_variant test_interfaces

# 执行测试
./bin/linux_x64/release/test_common_variant
./bin/linux_x64/release/test_interfaces
```

### 3. 使用 PB 库

在你的 CMakeLists.txt 中：

```cmake
# 链接 PB 库
target_link_libraries(your_target
    libcommonPB
    libinterfacesPB
    libdetectionPB
    libperceptionPB
    libPBUtils  # 可选：调试和打印工具
)
```

在你的代码中：

```cpp
#include "common/variant.pb.h"
#include "interfaces/interfaces_request_response.pb.h"
#include "printUtil.h"  // 可选：调试工具

using namespace base_types;
using namespace humanoid_robot::utils::PB;

// 创建和使用 Variant
base_types::Variant var;
var.set_stringvalue("Hello, PB!");
print_variant(var);  // 使用调试工具

// 创建和使用接口请求
interfaces::SendRequest request;
auto input = request.mutable_input()->mutable_keyvaluelist();
// ...
```

## Proto 代码生成

### 基本用法

```bash
cd PB
./generate_proto.sh <proto_directory_path> <export_symbol>
```

**参数说明:**
- `proto_directory_path`: proto文件所在目录路径（必需）
- `export_symbol`: 导入导出符号名称（必需）

### 使用示例

#### 1. 处理 interfaces 模块

```bash
cd PB
./generate_proto.sh proto/interfaces INTERFACES_API
```
这将为 interfaces 模块生成使用 `INTERFACES_API` 导出符号的代码。

#### 2. 处理 common 模块（通用类型）

```bash
cd PB
./generate_proto.sh proto/common COMMON_API
```
这将为 common 模块生成使用 `COMMON_API` 导出符号的代码。

#### 3. 处理 perception 模块（图像处理）

```bash
cd PB
./generate_proto.sh proto/perception PERCEPTION_API
这将为 perception 模块生成使用 `PERCEPTION_API` 导出符号的代码。

#### 4. 处理 detection 模块（目标检测）

```bash
cd PB
./generate_proto.sh proto/detection DETECTION_API
```
这将为 detection 模块生成使用 `DETECTION_API` 导出符号的代码。

#### 5. 批量处理所有模块

```bash
cd PB
# 按照当前项目结构处理所有模块
./generate_proto.sh proto/common COMMON_API
./generate_proto.sh proto/interfaces INTERFACES_API
./generate_proto.sh proto/perception PERCEPTION_API
./generate_proto.sh proto/detection DETECTION_API
```

#### 6. 使用绝对路径

```bash
cd PB
./generate_proto.sh /absolute/path/to/your/proto CUSTOM_API_SYMBOL
```

## CMake 构建系统

PB 模块使用现代化的 CMake 构建系统，支持：

### 构建选项

```cmake
# 主要构建选项
option(BUILD_PB_TESTS "Build PB tests" ON)
option(BUILD_PB_UTILS "Build PB utils" ON)
```

### 库目标

PB 模块会自动为每个子模块创建库目标：

- `libcommonPB` - 通用类型库（Variant 等）
- `libinterfacesPB` - 接口服务库
- `libperceptionPB` - 感知模块库  
- `libdetectionPB` - 检测模块库
- `libPBUtils` - 实用工具库
- `test_common_variant` - 通用类型测试
- `test_interfaces` - 接口服务测试

### 安装支持

```cmake
# 安装 PB 库和头文件
make install

# 在其他项目中使用
find_package(PB CONFIG REQUIRED)
target_link_libraries(your_target PB::commonPB PB::interfacesPB)
```

## 调试和工具

### printUtil 工具库

PB 模块提供了强大的调试和打印工具：

```cpp
#include "printUtil.h"
using namespace humanoid_robot::utils::PB;

// 打印 Variant 类型
base_types::Variant var;
var.set_stringvalue("Hello");
print_variant(var);

// 打印键值对列表
print_keyvaluelist(keyvaluelist);

// 打印测试结果
print_test_result("Test name", expected_value, actual_value);

// 打印分节标题
print_section("Test Section");
```

### 支持的打印类型

- ✅ **基本类型**: int8, uint8, int16, uint16, int32, uint32, int64, uint64
- ✅ **浮点类型**: float, double
- ✅ **其他类型**: bool, string, bytes
- ✅ **时间类型**: Date, Timestamp
- ✅ **容器类型**: 各种数组类型
- ✅ **复合类型**: Dictionary, KeyValueList
- ✅ **测试辅助**: 结果比较和格式化输出

## 测试套件

### 运行测试

```bash
# 编译测试
make test_common_variant test_interfaces

# 运行通用类型测试
./bin/linux_x64/release/test_common_variant

# 运行接口服务测试
./bin/linux_x64/release/test_interfaces

# 运行所有测试
make run_all_tests
```

### 测试覆盖

#### test_common_variant.cpp
- ✅ 基本数据类型（bool, 整型, 浮点型）
- ✅ 字符串和字节类型
- ✅ 日期和时间戳类型
- ✅ 数组类型（所有基本类型的数组）
- ✅ 字典类型（嵌套键值对）
- ✅ 序列化和反序列化
- ✅ 类型枚举验证

#### test_interfaces.cpp
- ✅ 请求响应类型（SendRequest/SendResponse）
- ✅ 错误信息类型（ResultStatus）
- ✅ 复杂嵌套结构
- ✅ 序列化和反序列化
- ✅ 键值对列表操作

## 模块架构

### 依赖关系

```
    ┌─────────────┐
    │    tests    │ (test_*, 依赖所有库)
    └─────────────┘
           │
    ┌─────────────┐
    │    utils    │ (libPBUtils, 调试工具)
    └─────────────┘
           │
    ┌─────────────┐    ┌─────────────┐    ┌─────────────┐
    │ interfaces  │    │ perception  │    │ detection   │
    └─────────────┘    └─────────────┘    └─────────────┘
           │                   │                   │
    ┌─────────────┐    ┌─────────────┐    ┌─────────────┐
    │   common    │    │   common    │    │   common    │
    └─────────────┘    └─────────────┘    └─────────────┘
           │                   │                   │
    ┌─────────────────────────────────────────────────────┐
    │            protobuf + gRPC (vcpkg)                  │
    └─────────────────────────────────────────────────────┘
```

### 核心类型

#### base_types::Variant
通用变体类型，支持所有基本数据类型和复合类型：

```cpp
// 创建不同类型的 Variant
Variant int_var;
int_var.set_int32value(42);

Variant string_var; 
string_var.set_stringvalue("Hello");

Variant dict_var;
auto dict = dict_var.mutable_dictvalue()->mutable_keyvaluelist();
// 添加键值对...
```

#### interfaces::SendRequest/SendResponse
标准的请求响应模式：

```cpp
interfaces::SendRequest request;
auto input = request.mutable_input()->mutable_keyvaluelist();
// 设置输入参数...

interfaces::SendResponse response;
auto output = response.mutable_output()->mutable_keyvaluelist();
// 处理输出结果...
```

### 添加新的模块

要添加新的模块（比如 `tracking`），需要：

1. **在 pb_export.h 中定义导出符号**:
```cpp
// 在 PB/include/pb_export.h 中为新模块添加导出符号定义
// Windows 平台：
#ifdef TRACKING_EXPORTS
#define TRACKING_API __declspec(dllexport)
#else
#define TRACKING_API __declspec(dllimport)
#endif

// Linux 平台：
#ifdef TRACKING_EXPORTS
#define TRACKING_API __attribute__((visibility("default")))
#else
#define TRACKING_API __attribute__((visibility("default")))
#endif

// 通用平台：
#define TRACKING_API
```

2. **创建目录**: `mkdir -p proto/tracking`
3. **放入proto文件**: `tracking.proto`
4. **运行脚本**: `./generate_proto.sh proto/tracking TRACKING_API`

生成的文件将自动放置在：
- `include/tracking/tracking.pb.h`（包含TRACKING_API符号）
- `include/tracking/tracking.grpc.pb.h`（包含TRACKING_API符号）
- `source/tracking/tracking.pb.cc`
- `source/tracking/tracking.grpc.pb.cc`

**注意**: 必须先在 `pb_export.h` 中定义导出符号，否则生成的代码将无法正确编译。

## 特性

- **🎯 模块化架构**: 清晰的模块分离，每个子模块独立构建
- **🛠️ CMake 集成**: 现代化的 CMake 构建系统，支持 vcpkg 依赖管理
- **🔧 自定义导出符号**: 为不同模块使用不同的导入导出符号，支持模块化构建
- **⚡ 自动化符号注入**: 自动为所有生成的 protobuf 和 gRPC 类添加指定的导出符号
- **📁 灵活的目录结构**: 支持任意深度的模块目录，自动创建对应的输出结构
- **🛠️ 路径自适应**: 自动处理相对路径和包含路径，支持绝对路径和相对路径
- **✅ 智能包含**: 根据目录深度自动调整 `pb_export.h` 的包含路径
- **📋 详细输出**: 显示处理进度、生成文件位置和完整目录结构
- **🧪 完整测试**: 包含全面的测试套件，确保代码质量
- **🔍 调试工具**: 提供强大的打印和调试实用工具

## 支持的 proto 功能

- **标准 protobuf 消息**: 自动为所有 message 类添加导出符号
- **gRPC 服务**: 为 Service、Stub、StubInterface 类添加导出符号
- **嵌套消息**: 支持嵌套消息和枚举的符号注入
- **流式 RPC**: 支持单向流和双向流的 gRPC 服务
- **复合类型**: 支持复杂的嵌套结构和键值对映射

## 环境要求

### 构建依赖
- **CMake**: >= 3.8
- **C++**: >= C++17
- **vcpkg**: 用于依赖管理
- **protobuf**: >= 3.21.0 (通过 vcpkg 安装)
- **gRPC**: >= 1.50.0 (通过 vcpkg 安装)

### 运行时依赖
- **protobuf 运行时库**
- **gRPC 运行时库**

## 性能特性

- **零拷贝**: 支持高效的消息传递
- **内存安全**: 使用智能指针和 RAII
- **类型安全**: 强类型的 proto 定义
- **序列化优化**: 高效的二进制序列化

## 前置条件

1. **vcpkg 配置**: 确保项目配置了 vcpkg 工具链
2. **pb_export.h**: 文件存在于 `include/` 目录，并包含所需的导出符号定义
3. **构建环境**: 已安装必要的编译工具链

### pb_export.h 配置说明

`pb_export.h` 文件定义了所有模块的导出符号，目前支持的模块包括：
- `COMMON_API` - 通用类型模块
- `INTERFACES_API` - 接口服务模块  
- `PERCEPTION_API` - 感知模块
- `DETECTION_API` - 检测模块

如果需要添加新模块，必须先在 `pb_export.h` 中定义对应的导出符号，然后才能使用 `generate_proto.sh` 生成代码。

## 故障排除

### 常见问题

#### 1. 编译错误

**问题**: `libprotobufd.a: hidden symbol ... is referenced by DSO`

**原因**: debug 和 release 版本的 protobuf 库混用

**解决方案**:
```bash
# 清理并重新构建
cd build
rm -rf vcpkg_installed CMakeCache.txt CMakeFiles/
CMAKE_BUILD_TYPE=Release cmake ..
make -j$(nproc)
```

#### 2. 链接错误

**问题**: `target_include_directories` 中包含了库目标

**解决方案**: 检查 CMakeLists.txt，确保库目标在 `target_link_libraries` 中：
```cmake
# ❌ 错误
target_include_directories(target PRIVATE protobuf::libprotobuf)

# ✅ 正确  
target_link_libraries(target PRIVATE protobuf::libprotobuf)
```

#### 3. Proto 生成错误

**问题**: `protoc工具不存在`

**解决方案**: 修改脚本中的工具路径：
```bash
# 在 generate_proto.sh 中修改（约第 67-68 行）
PROTOC_PATH="你的protoc工具路径"
GRPC_PLUGIN_PATH="你的grpc_cpp_plugin工具路径"
```

#### 4. 测试运行失败

**问题**: 找不到共享库

**解决方案**: 设置库路径：
```bash
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/path/to/build/lib/linux_x64/release
./bin/linux_x64/release/test_interfaces
```

#### 5. 导出符号未定义错误

**问题**: `'NEW_MODULE_API' was not declared in this scope`

**原因**: 在 `pb_export.h` 中没有定义对应的导出符号

**解决方案**: 编辑 `PB/include/pb_export.h`，按照以下模板添加新模块的导出符号：
```cpp
// Windows 平台部分
#ifdef NEW_MODULE_EXPORTS
#define NEW_MODULE_API __declspec(dllexport)
#else
#define NEW_MODULE_API __declspec(dllimport)
#endif

// Linux 平台部分  
#ifdef NEW_MODULE_EXPORTS
#define NEW_MODULE_API __attribute__((visibility("default")))
#else
#define NEW_MODULE_API __attribute__((visibility("default")))
#endif

// 通用平台部分
#define NEW_MODULE_API
```

### 验证生成结果

检查导出符号是否正确注入：
```bash
# 检查 protobuf 类
grep -n "YOUR_EXPORT_SYMBOL" include/module/file.pb.h

# 检查 gRPC 类  
grep -n "YOUR_EXPORT_SYMBOL" include/module/file.grpc.pb.h

# 验证库文件
nm -D build/lib/linux_x64/release/libcommonPB.so | grep Variant
```

### 调试技巧

```bash
# 查看库依赖
ldd build/lib/linux_x64/release/libinterfacesPB.so

# 检查符号表
objdump -T build/lib/linux_x64/release/libPBUtils.so

# 运行测试时启用详细输出
VERBOSE=1 make test
```

## 输出日志

脚本执行后会显示：
- 找到的 proto 文件数量和文件名
- 每个文件的详细处理进度
- 导出符号注入状态
- 生成的文件位置
- 完整的目录结构

### 输出示例

```
=== 通用 Protobuf 代码生成脚本 ===
PB 根目录: /path/to/PB
Proto目录参数: proto/interfaces
导出符号: INTERFACES_API
搜索 Proto 文件目录: /path/to/PB/proto/interfaces
找到 3 个 proto 文件:
  - interfaces_callback.proto
  - interfaces_grpc.proto  
  - interfaces_request_response.proto

=== 处理: /path/to/PB/proto/interfaces/interfaces_request_response.proto ===
  相对路径: interfaces/interfaces_request_response.proto
  目录: interfaces
  文件名: interfaces_request_response
  1. 生成protobuf代码...
  ✓ 已移动: include/interfaces/interfaces_request_response.pb.h
  ✓ 已移动: include/interfaces/interfaces_request_response.grpc.pb.h
  ✓ 已移动: source/interfaces/interfaces_request_response.pb.cc
  ✓ 已移动: source/interfaces/interfaces_request_response.grpc.pb.cc
  2. 添加导出符号到头文件...
    处理: include/interfaces/interfaces_request_response.pb.h
      ✓ 已添加导出符号(INTERFACES_API)到protobuf类
    处理: include/interfaces/interfaces_request_response.grpc.pb.h
      ✓ 已添加导出符号(INTERFACES_API)到gRPC类
  ✓ 处理完成: interfaces_request_response

=== 生成完成! ===
```

## 开发指南

### 添加新模块

1. **在 pb_export.h 中定义导出符号**:
```cpp
// 编辑 PB/include/pb_export.h 文件
// 在 Windows 平台部分添加：
#ifdef NEW_MODULE_EXPORTS
#define NEW_MODULE_API __declspec(dllexport)
#else
#define NEW_MODULE_API __declspec(dllimport)
#endif

// 在 Linux/Unix 平台部分添加：
#ifdef NEW_MODULE_EXPORTS
#define NEW_MODULE_API __attribute__((visibility("default")))
#else
#define NEW_MODULE_API __attribute__((visibility("default")))
#endif

// 在通用平台部分添加：
#define NEW_MODULE_API
```

**完整的 pb_export.h 更新示例**:
```cpp
// 在 Windows 部分（约第25行后）添加
#ifdef NEW_MODULE_EXPORTS
#define NEW_MODULE_API __declspec(dllexport)
#else
#define NEW_MODULE_API __declspec(dllimport)
#endif

// 在 Linux 部分（约第55行后）添加
#ifdef NEW_MODULE_EXPORTS
#define NEW_MODULE_API __attribute__((visibility("default")))
#else
#define NEW_MODULE_API __attribute__((visibility("default")))
#endif

// 在通用部分（约第68行后）添加
#define NEW_MODULE_API
```

2. **创建 proto 定义**:
```bash
mkdir -p proto/new_module
cat > proto/new_module/new_module.proto << 'EOF'
syntax = "proto3";
package new_module;

message NewMessage {
    string data = 1;
}

service NewService {
    rpc ProcessData(NewMessage) returns (NewMessage);
}
EOF
```

3. **生成代码**:
```bash
./generate_proto.sh proto/new_module NEW_MODULE_API
```

4. **验证生成**:
```bash
find include/new_module source/new_module -name "*.h" -o -name "*.cc"
grep "NEW_MODULE_API" include/new_module/new_module.pb.h
```

5. **CMake 会自动检测**新模块并创建对应的库目标 `libnew_modulePB`

### 添加新测试

1. **创建测试文件**:
```cpp
// tests/test_new_module.cpp
#include "new_module/new_module.pb.h"
#include "printUtil.h"

int main() {
    // 测试代码...
    return 0;
}
```

2. **CMake 会自动检测**新测试并创建可执行目标

### 最佳实践

#### 导出符号管理
- 新增模块前必须在 `pb_export.h` 中定义导出符号
- 导出符号命名规范: `<MODULE_NAME>_API`（全大写）
- 确保 Windows 和 Linux 平台都有对应定义
- 使用 `<MODULE_NAME>_EXPORTS` 宏控制导入/导出

#### Proto 设计
- 使用清晰的包名: `package module_name;`
- 为字段添加注释和文档
- 考虑向后兼容性
- 使用合理的字段编号

#### 测试编写
- 测试所有公共接口
- 包含正面和负面测试用例
- 使用 `printUtil` 进行调试输出
- 验证序列化/反序列化

#### 性能优化
- 重用 protobuf 对象
- 避免频繁的字符串拷贝
- 使用 `std::move` 进行大对象传递
- 考虑使用 protobuf arena

## 版本历史

### v1.1.0 (当前版本)
- ✅ 增加了 utils 工具库
- ✅ 完整的测试套件
- ✅ 修复了 CMake 构建问题
- ✅ 改进的错误处理和调试支持
- ✅ 完善的文档和示例

### v1.0.0 (基础版本)
- ✅ 基本的 proto 代码生成
- ✅ 多模块支持
- ✅ 导出符号注入

## 贡献指南

1. **Fork** 项目
2. **创建**特性分支 (`git checkout -b feature/new-feature`)
3. **提交**改动 (`git commit -am 'Add new feature'`)
4. **推送**到分支 (`git push origin feature/new-feature`)
5. **创建** Pull Request

### 代码规范
- 使用 C++17 标准
- 遵循 Google C++ Style Guide
- 添加适当的注释和文档
- 包含测试用例

## 许可证

Copyright (c) 2025 Humanoid Robot, Inc. All rights reserved.

---

**PB 模块** - 为人形机器人项目提供强大、可靠、高性能的 Protobuf/gRPC 支持 🤖✨

