#!/bin/bash

# 定义要构建的 Node.js 版本
VERSIONS=("14.0.0" "16.0.0" "18.0.0" "20.0.0")

for version in "${VERSIONS[@]}"; do
  echo "Building for Node.js ${version}"

  # x64 构建
  arch -x86_64 prebuildify --napi \
    --strip \
    --arch x64 \
    --target node@$version \
    --tag-uv \
    --tag-libc \
    --tag-armv \
    --platform

  # arm64 构建
  arch -arm64 prebuildify --napi \
    --strip \
    --arch arm64 \
    --target node@$version \
    --name $version
done
