#!/bin/bash

# 通用 Protobuf 代码生成脚本
# 用法: ./generate_proto.sh <proto_directory_path> <export_symbol>
# 参数:
#   proto_directory_path: proto文件所在目录路径
#   export_symbol: 导入导出符号名称（如 IMAGEPROCESSING_PB_API）

set -e

# 获取脚本所在目录（PB目录）
PB_ROOT="$(dirname "$(readlink -f "$0")")"
VCPKG_ROOT="/home/ubuntu/vcpkg"

echo "=== 通用 Protobuf 代码生成脚本 ==="
echo "PB 根目录: $PB_ROOT"

# 检查参数数量
if [ $# -ne 2 ]; then
    echo "错误: 需要提供两个参数"
    echo "用法: $0 <proto_directory_path> <export_symbol>"
    echo ""
    echo "参数说明:"
    echo "  proto_directory_path  proto文件所在目录路径"
    echo "  export_symbol         导入导出符号名称"
    echo ""
    echo "示例:"
    echo "  $0 proto/perception IMAGEPROCESSING_PB_API"
    echo "  $0 proto/detection DETECTION_PB_API"
    echo "  $0 /absolute/path/to/proto CUSTOM_API"
    exit 1
fi

# 解析参数
PROTO_DIR_ARG="$1"
EXPORT_SYMBOL="$2"

echo "Proto目录参数: $PROTO_DIR_ARG"
echo "导出符号: $EXPORT_SYMBOL"

# 处理proto目录路径
if [[ "$PROTO_DIR_ARG" = /* ]]; then
    # 绝对路径
    PROTO_SEARCH_DIR="$PROTO_DIR_ARG"
else
    # 相对路径，相对于PB根目录
    PROTO_SEARCH_DIR="$PB_ROOT/$PROTO_DIR_ARG"
fi

if [ ! -d "$PROTO_SEARCH_DIR" ]; then
    echo "错误: Proto 搜索目录不存在: $PROTO_SEARCH_DIR"
    exit 1
fi

echo "搜索 Proto 文件目录: $PROTO_SEARCH_DIR"

# 查找build目录（在Humanoid-Robot项目中）
BUILD_DIR=""
if [ -d "$PB_ROOT/../../build" ]; then
    BUILD_DIR="$PB_ROOT/../../build"
elif [ -d "$PB_ROOT/../build" ]; then
    BUILD_DIR="$PB_ROOT/../build"
else
    echo "错误: 找不到构建目录，请确保Humanoid-Robot项目已经配置过"
    exit 1
fi


echo "使用构建目录: $BUILD_DIR"

# 检查protobuf工具
PROTOC_PATH="$BUILD_DIR/vcpkg_installed/x64-linux/tools/protobuf/protoc"
GRPC_PLUGIN_PATH="$BUILD_DIR/vcpkg_installed/x64-linux/tools/grpc/grpc_cpp_plugin"

if [ ! -f "$PROTOC_PATH" ]; then
    echo "错误: protoc工具不存在: $PROTOC_PATH"
    echo "请先在Humanoid-Robot根目录运行cmake配置以安装vcpkg依赖"
    exit 1
fi

if [ ! -f "$GRPC_PLUGIN_PATH" ]; then
    echo "错误: grpc_cpp_plugin工具不存在: $GRPC_PLUGIN_PATH"
    echo "请先在Humanoid-Robot根目录运行cmake配置以安装vcpkg依赖"
    exit 1
fi

# 查找所有.proto文件
PROTO_FILES=($(find "$PROTO_SEARCH_DIR" -name "*.proto" -type f))

if [ ${#PROTO_FILES[@]} -eq 0 ]; then
    echo "警告: 在 $PROTO_SEARCH_DIR 中没有找到 .proto 文件"
    exit 0
fi

echo "找到 ${#PROTO_FILES[@]} 个 proto 文件:"
for proto_file in "${PROTO_FILES[@]}"; do
    echo "  - $(basename "$proto_file")"
done

# 为每个proto文件生成代码
for PROTO_FILE in "${PROTO_FILES[@]}"; do
    echo ""
    echo "=== 处理: $PROTO_FILE ==="
    
    # 获取相对于proto根目录的路径
    REL_PROTO_PATH=$(realpath --relative-to="$PB_ROOT/proto" "$PROTO_FILE")
    PROTO_DIR=$(dirname "$REL_PROTO_PATH")
    PROTO_NAME=$(basename "$PROTO_FILE" .proto)
    
    echo "  相对路径: $REL_PROTO_PATH"
    echo "  目录: $PROTO_DIR"
    echo "  文件名: $PROTO_NAME"
    
    # 创建输出目录
    mkdir -p "$PB_ROOT/include/$PROTO_DIR"
    mkdir -p "$PB_ROOT/source/$PROTO_DIR"
    
    # 生成临时输出目录
    TEMP_OUTPUT="$PB_ROOT/temp_proto_output"
    mkdir -p "$TEMP_OUTPUT"
    
    echo "  1. 生成protobuf代码..."
    
    # 生成protobuf代码到临时目录
    cd "$PB_ROOT"
    "$PROTOC_PATH" \
        --proto_path=proto \
        --cpp_out="$TEMP_OUTPUT" \
        --grpc_out="$TEMP_OUTPUT" \
        --plugin=protoc-gen-grpc="$GRPC_PLUGIN_PATH" \
        "$REL_PROTO_PATH"
    
    if [ $? -ne 0 ]; then
        echo "  错误: protobuf代码生成失败"
        rm -rf "$TEMP_OUTPUT"
        exit 1
    fi
    
    # 移动生成的文件到正确位置
    TEMP_PROTO_H="$TEMP_OUTPUT/$PROTO_DIR/$PROTO_NAME.pb.h"
    TEMP_PROTO_CC="$TEMP_OUTPUT/$PROTO_DIR/$PROTO_NAME.pb.cc"
    TEMP_GRPC_H="$TEMP_OUTPUT/$PROTO_DIR/$PROTO_NAME.grpc.pb.h"
    TEMP_GRPC_CC="$TEMP_OUTPUT/$PROTO_DIR/$PROTO_NAME.grpc.pb.cc"
    
    # 移动头文件
    if [ -f "$TEMP_PROTO_H" ]; then
        mv "$TEMP_PROTO_H" "$PB_ROOT/include/$PROTO_DIR/"
        echo "  ✓ 已移动: include/$PROTO_DIR/$PROTO_NAME.pb.h"
    fi
    
    if [ -f "$TEMP_GRPC_H" ]; then
        mv "$TEMP_GRPC_H" "$PB_ROOT/include/$PROTO_DIR/"
        echo "  ✓ 已移动: include/$PROTO_DIR/$PROTO_NAME.grpc.pb.h"
    fi
    
    # 移动源文件
    if [ -f "$TEMP_PROTO_CC" ]; then
        mv "$TEMP_PROTO_CC" "$PB_ROOT/source/$PROTO_DIR/"
        echo "  ✓ 已移动: source/$PROTO_DIR/$PROTO_NAME.pb.cc"
    fi
    
    if [ -f "$TEMP_GRPC_CC" ]; then
        mv "$TEMP_GRPC_CC" "$PB_ROOT/source/$PROTO_DIR/"
        echo "  ✓ 已移动: source/$PROTO_DIR/$PROTO_NAME.grpc.pb.cc"
    fi
    
    echo "  2. 添加导出符号到头文件..."
    
    # 处理protobuf头文件
    PROTO_HEADER="$PB_ROOT/include/$PROTO_DIR/$PROTO_NAME.pb.h"
    if [ -f "$PROTO_HEADER" ]; then
        echo "    处理: include/$PROTO_DIR/$PROTO_NAME.pb.h"
        
        # 添加pb_export.h包含 - 需要根据目录深度调整路径
        DEPTH=$(echo "$PROTO_DIR" | tr '/' '\n' | wc -l)
        INCLUDE_PATH=""
        for ((i=0; i<DEPTH; i++)); do
            INCLUDE_PATH="../$INCLUDE_PATH"
        done
        INCLUDE_PATH="${INCLUDE_PATH}pb_export.h"
        
        # 添加pb_export.h包含
        if ! grep -q '#include.*pb_export.h' "$PROTO_HEADER"; then
            sed -i "1i#include \"$INCLUDE_PATH\"" "$PROTO_HEADER"
        fi
        
        # 为protobuf类添加导出符号
        sed -i "s/class \\([A-Za-z0-9_]\\+\\) final/class $EXPORT_SYMBOL \\1 final/g" "$PROTO_HEADER"
        
        echo "      ✓ 已添加导出符号($EXPORT_SYMBOL)到protobuf类"
    fi
    
    # 处理gRPC头文件
    GRPC_HEADER="$PB_ROOT/include/$PROTO_DIR/$PROTO_NAME.grpc.pb.h"
    if [ -f "$GRPC_HEADER" ]; then
        echo "    处理: include/$PROTO_DIR/$PROTO_NAME.grpc.pb.h"
        
        # 添加pb_export.h包含
        if ! grep -q '#include.*pb_export.h' "$GRPC_HEADER"; then
            sed -i "1i#include \"$INCLUDE_PATH\"" "$GRPC_HEADER"
        fi
        
        # 为gRPC服务类添加导出符号
        sed -i "s/class \\([A-Za-z0-9_]\\+\\) final/class $EXPORT_SYMBOL \\1 final/g" "$GRPC_HEADER"
        
        # 为嵌套类添加导出符号
        sed -i "s/class \\(StubInterface\\|Stub\\|Service\\)\\b/class $EXPORT_SYMBOL \\1/g" "$GRPC_HEADER"
        
        echo "      ✓ 已添加导出符号($EXPORT_SYMBOL)到gRPC类"
    fi
    
    echo "  ✓ 处理完成: $PROTO_NAME"
done

# 清理临时目录
rm -rf "$TEMP_OUTPUT"

echo ""
echo "=== 生成完成! ==="
echo "生成的文件位于:"
echo "  头文件: $PB_ROOT/include/"
echo "  源文件: $PB_ROOT/source/"
echo ""
echo "目录结构:"
find "$PB_ROOT/include" "$PB_ROOT/source" -type f -name "*.h" -o -name "*.cc" | sort
