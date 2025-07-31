# PB Common Variant 测试程序说明

## 概述

`test_common_variant.cpp` 是一个全面的测试程序，用于验证 `PB/proto/common/variant.proto` 的功能。该测试程序只使用生成的头文件 `PB/include/common/variant.pb.h` 和动态库 `libcommon.so`。

## 测试功能

### 1. VariantType 枚举测试
验证所有枚举值的正确性：
- `VARIANT_TYPE_UNKNOWN` (0)
- `VARIANT_TYPE_INT32` (1) 到 `VARIANT_TYPE_MAP` (11)

### 2. 基本类型测试
测试所有基本的 Typed 消息类型：
- `TypedInt32` - 32位有符号整数
- `TypedInt64` - 64位有符号整数  
- `TypedUInt32` - 32位无符号整数
- `TypedUInt64` - 64位无符号整数
- `TypedFloat` - 单精度浮点数
- `TypedDouble` - 双精度浮点数
- `TypedBool` - 布尔值
- `TypedString` - 字符串
- `TypedBinary` - 二进制数据

### 3. TypedVariant (oneof) 测试
验证 `oneof` 字段的功能：
- 测试不同类型的 variant 值设置
- 验证 `has_*()` 方法的正确性
- 确保同时只能设置一个类型

### 4. 容器类型测试

#### VariantArray
- 创建包含多种类型元素的数组
- 验证数组大小和元素访问
- 测试混合类型存储（int32, string, double）

#### VariantMap  
- 创建字符串键到 TypedVariant 值的映射
- 验证 map 大小和键值对访问
- 测试不同类型值的存储

### 5. TypedArray 和 TypedMap 测试
测试顶层容器类型：
- `TypedArray` - 带类型标识的数组容器
- `TypedMap` - 带类型标识的映射容器

### 6. 序列化/反序列化测试
验证 Protocol Buffers 的核心功能：
- 将消息序列化为字节流
- 从字节流反序列化消息
- 验证数据的完整性

## 构建和运行

### 构建
```bash
cd /home/ubuntu/zhaokai/vs_workspace/Humanoid-Robot/build
cmake ..
make -j$(nproc)
```

### 运行测试
```bash
cd build/bin/linux_x64/release
./test_common_variant
```

### 验证动态库链接
```bash
ldd test_common_variant | grep libcommon
# 应该显示: libcommon.so.1 => .../libcommon.so.1
```

## 测试结果

成功运行时，测试程序会输出：
- 枚举值验证：12 个测试通过
- 基本类型验证：18 个测试通过  
- TypedVariant 验证：6 个测试通过
- 容器类型验证：10 个测试通过
- 序列化验证：3 个测试通过

总计：49 个测试全部通过

## 特性验证

✅ **类型安全**: 每个 Typed 消息都包含类型标识符  
✅ **oneof 功能**: TypedVariant 正确实现联合类型  
✅ **嵌套结构**: 支持数组和映射的嵌套  
✅ **序列化**: 完整的 protobuf 序列化/反序列化  
✅ **动态库链接**: 正确使用 libcommon.so 动态库  
✅ **跨平台兼容**: 使用 PERCEPTION_API 导出符号

## 文件结构

```
PB/tests/
├── test_common_variant.cpp     # 测试源代码
├── CMakeLists.txt             # 构建配置（已更新支持 common 模块）
└── build 后生成:
    └── test_common_variant    # 可执行文件
```

使用的库文件：
```
build/lib/linux_x64/release/
└── libcommon.so              # 动态库
    ├── libcommon.so.1        # 版本链接
    └── libcommon.so.1.0      # 实际库文件
```

使用的头文件：
```
PB/include/
├── pb_export.h              # 导出符号定义
└── common/
    ├── variant.pb.h         # protobuf 生成的头文件
    └── variant.grpc.pb.h    # gRPC 生成的头文件（未在测试中使用）
```

## 注意事项

1. **编译依赖**: 需要 protobuf 和 gRPC 库
2. **导出符号**: 所有类使用 `PERCEPTION_API` 导出符号
3. **内存管理**: 使用 protobuf 的内置内存管理
4. **错误处理**: 包含基本的异常处理
5. **平台兼容**: 支持 Linux x64 平台
