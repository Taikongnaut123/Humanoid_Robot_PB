# PB/generate_proto.sh 使用说明

## 概述

`generate_proto.sh` 是一个通用的 Protocol Buffers 和 gRPC 代码生成脚本，用于从 `.proto` 文件自动生成 C++ 代码，并自动添加适当的导出符号以支持跨平台动态库构建。

## 脚本功能

1. **自动代码生成**: 从 `.proto` 文件生成 C++ 头文件和源文件
2. **导出符号注入**: 自动为生成的类添加指定的导出符号（如 `DETECTION_API`、`PERCEPTION_API`）
3. **目录结构管理**: 自动创建和维护 `include/` 和 `source/` 目录结构
4. **依赖包含**: 自动在生成的头文件中添加 `pb_export.h` 的包含

## 使用前提条件

### 1. 构建环境要求

- **CMake 3.8+**: 项目必须已经通过 CMake 配置过
- **vcpkg**: 必须已安装并配置 protobuf 和 gRPC 依赖包
- **Linux 环境**: 脚本目前针对 Linux 系统优化

### 2. 目录结构要求

```
Humanoid-Robot/
├── build/                          # CMake 构建目录
│   └── vcpkg_installed/
│       └── x64-linux/
│           └── tools/
│               ├── protobuf/
│               │   └── protoc       # protoc 编译器
│               └── grpc/
│                   └── grpc_cpp_plugin  # gRPC C++ 插件
└── PB/
    ├── proto/                       # .proto 文件目录
    │   ├── detection/
    │   ├── perception/
    │   └── common/
    ├── include/                     # 生成的头文件目录
    │   └── pb_export.h             # 导出符号定义文件
    ├── source/                      # 生成的源文件目录
    └── generate_proto.sh           # 生成脚本
```

### 3. 依赖工具检查

在运行脚本前，确保以下工具可用：

```bash
# 检查 protoc 工具
ls build/vcpkg_installed/x64-linux/tools/protobuf/protoc

# 检查 gRPC 插件
ls build/vcpkg_installed/x64-linux/tools/grpc/grpc_cpp_plugin
```

### 4. pb_export.h 文件要求

必须存在 `PB/include/pb_export.h` 文件，包含所需的导出符号定义。例如：

```cpp
#ifndef PB_EXPORT_H
#define PB_EXPORT_H

// 检测检测模块的导出符号
#ifdef DETECTION_EXPORTS
#define DETECTION_API __attribute__((visibility("default")))
#else
#define DETECTION_API __attribute__((visibility("default")))
#endif

// 感知模块的导出符号
#ifdef PERCEPTION_EXPORTS
#define PERCEPTION_API __attribute__((visibility("default")))
#else
#define PERCEPTION_API __attribute__((visibility("default")))
#endif

// 通用模块的导出符号
#ifdef COMMON_EXPORTS
#define COMMON_API __attribute__((visibility("default")))
#else
#define COMMON_API __attribute__((visibility("default")))
#endif

#endif // PB_EXPORT_H
```

## 使用方法

### 基本语法

```bash
./generate_proto.sh <proto_directory_path> <export_symbol>
```

### 参数说明

- **proto_directory_path**: .proto 文件所在的目录路径（相对于 PB 根目录或绝对路径）
- **export_symbol**: 要添加到生成类的导出符号名称

### 使用示例

#### 1. 生成检测模块代码

```bash
cd PB
./generate_proto.sh proto/detection DETECTION_API
```

#### 2. 生成感知模块代码

```bash
cd PB
./generate_proto.sh proto/perception PERCEPTION_API
```

#### 3. 生成通用模块代码

```bash
cd PB
./generate_proto.sh proto/common COMMON_API
```

#### 4. 使用绝对路径

```bash
cd PB
./generate_proto.sh /home/ubuntu/project/custom_proto CUSTOM_API
```

## 脚本执行流程

1. **参数验证**: 检查参数数量和有效性
2. **环境检查**: 验证构建目录和工具存在性
3. **文件发现**: 扫描指定目录下的所有 .proto 文件
4. **代码生成**: 为每个 .proto 文件生成 C++ 代码
5. **文件移动**: 将生成的文件移动到正确的目录结构
6. **符号注入**: 在头文件中添加导出符号和必要的包含文件

## 生成的文件结构

执行脚本后，会在以下位置生成文件：

```
PB/
├── include/
│   └── <module_name>/
│       ├── <proto_name>.pb.h      # protobuf 头文件
│       └── <proto_name>.grpc.pb.h # gRPC 头文件
└── source/
    └── <module_name>/
        ├── <proto_name>.pb.cc     # protobuf 源文件
        └── <proto_name>.grpc.pb.cc # gRPC 源文件
```

## 常见问题与解决方案

### 1. 错误：找不到构建目录

**问题**: `错误: 找不到构建目录，请确保grpc_project已经配置过`

**解决方案**:
```bash
# 在项目根目录运行 CMake 配置
cd build
cmake ..
```

### 2. 错误：protoc 工具不存在

**问题**: protoc 或 grpc_cpp_plugin 工具不存在

**解决方案**:
```bash
# 确保 vcpkg 依赖已正确安装
cd build
cmake .. # 这会触发 vcpkg 安装 protobuf 和 gRPC
```

### 3. 错误：Proto 搜索目录不存在

**问题**: 指定的 proto 目录不存在

**解决方案**:
- 检查目录路径是否正确
- 确保 .proto 文件存在于指定目录

### 4. 权限问题

**问题**: 脚本没有执行权限

**解决方案**:
```bash
chmod +x generate_proto.sh
```

## 最佳实践

### 1. 模块化管理

为不同功能模块使用不同的 proto 目录和导出符号：

```bash
./generate_proto.sh proto/detection DETECTION_API
./generate_proto.sh proto/perception PERCEPTION_API
./generate_proto.sh proto/common COMMON_API
```

### 2. 版本控制

- 将 .proto 文件纳入版本控制
- 考虑将生成的代码排除在版本控制之外（添加到 .gitignore）
- 或者确保在构建过程中重新生成代码

### 3. 构建集成

在 CMake 中集成代码生成：

```cmake
# 添加自定义目标来重新生成代码
add_custom_target(regenerate_proto
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/generate_proto.sh proto/detection DETECTION_API
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/generate_proto.sh proto/perception PERCEPTION_API
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/generate_proto.sh proto/common COMMON_API
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Regenerating protobuf code"
)
```

### 4. 测试验证

生成代码后，运行构建验证：

```bash
cd build
make clean
cmake ..
make -j$(nproc)
```

## 脚本维护

### 添加新的导出符号

要添加新的模块支持，需要：

1. 在 `pb_export.h` 中添加新的导出符号定义
2. 使用对应的符号名称运行脚本

### 自定义符号处理

脚本使用 sed 正则表达式来添加导出符号，如果需要自定义处理逻辑，可以修改脚本中的以下部分：

```bash
# 为protobuf类添加导出符号
sed -i "s/class \\([A-Za-z0-9_]\\+\\) final/class $EXPORT_SYMBOL \\1 final/g" "$PROTO_HEADER"

# 为gRPC服务类添加导出符号
sed -i "s/class \\(StubInterface\\|Stub\\|Service\\)\\b/class $EXPORT_SYMBOL \\1/g" "$GRPC_HEADER"
```

## 相关文档

- [Protocol Buffers C++ 文档](https://protobuf.dev/programming-guides/proto3/)
- [gRPC C++ 文档](https://grpc.io/docs/languages/cpp/)
- [CMake 文档](https://cmake.org/documentation/)
- [vcpkg 文档](https://vcpkg.io/en/docs/README.html)
