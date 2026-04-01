# Destiny Rendering Demo

Destiny 是一款基于 C++ 和 Direct3D 11 开发的 3D 渲染 Demo。主要用于展示现代图形渲染技术在 D3D11 下的实现，包含一个灵活的高级图形管线和多种内置渲染特性。

## 核心特性

- **现代渲染管线**:
  - **延迟渲染 (Deferred Shading)**: 支持高效的多光源渲染与 G-Buffer 管理。
  - **基于物理的渲染 (PBR)**: 实现标准金属/粗糙度工作流。
  - **屏幕空间反射 (SSR)**: 提供动态场景反射效果。
  - **阴影映射**: 支持方向光级联阴影 (CSM 风格) 及点光源阴影。
  - **Shader 自动化**: 基于 **D3DReflect** 实现了着色器变量与 Constant Buffer 的自动反射与绑定。

- **高效资源系统**:
  - **多线程加载**: 集成自定义线程池 (`ThreadPool`)，支持模型、纹理等资产的异步并发加载。
  - **Blob 抽象层**: 统一的二进制数据管理，优化 IO 效率。

- **架构与扩展**:
  - **场景树 (Scene Graph)**: 采用 `Node` 和 `Component` 模式，灵活组织空间实体。
  - **运行时反射**: 集成 **RTTR** 库，支持完整的对象序列化与反序列化系统。
  - **物理集成**: 深度集成 **Bullet3** 物理引擎，支持刚体动力学模拟。

- **内置渲染组件**:
  - **特效管线**: 支持 GPU 加速的粒子系统（火、雨、水）。
  - **编辑器辅助**: 集成 **ImGui** 提供实时的场景参数调整与状态监控。

## 运行效果

[1](screenshots/screenshot1.png)

[2](screenshots/screenshot2.png)

[3](screenshots/screenshot3.png)

[4](screenshots/screenshot4.png)

## 项目结构

- `src/Engine`: 核心逻辑（资源管理、序列化、基础对象）。
- `src/Graphics`: 渲染管线、着色器管理、GUI 与图形 API 封装。
- `src/Scene`: 场景管理器、内置组件及运行效果。
- `src/Math`: 数学工具库。
- `src/Physics`: 物理系统集成。
- `3rdparty`: 第三方库依赖。
- `builtin`: 内置资源（着色器、字体、模型、纹理）。

## 第三方依赖

- **spdlog**: 高性能日志库。
- **rapidjson**: JSON 解析。
- **assimp**: 模型加载。
- **rttr**: 运行时类型反射。
- **freetype**: 字体渲染。
- **bullet3**: 物理模拟。
- **curl**: 网络通信。
- **ImGui**: 编辑器界面。

## 构建说明

### 环境要求
- Windows 操作系统
- Visual Studio 2019 或更高版本 (支持 C++17)
- CMake 3.14 或更高版本

### 构建步骤
1. 克隆项目到本地。
2. 在项目根目录下，使用 CMake 生成解决方案：
   ```bash
   mkdir build
   cd build
   cmake ..
   ```
3. 打开生成的 `Destiny.sln` 文件并进行编译。
4. 编译完成后，`builtin` 资源会自动拷贝到运行目录。

## 许可证

本项目采用 [MIT License](LICENSE)。
