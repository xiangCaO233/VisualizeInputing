#pragma once

#include <string>

namespace vi
{

/// @brief 应用启动配置。
struct ApplicationConfig {
    /// @brief 主窗口标题。
    std::string title{ "VisualizeInputing" };

    /// @brief 主窗口初始宽度。
    int width{ 1280 };

    /// @brief 主窗口初始高度。
    int height{ 800 };

    /// @brief 是否在启动时启用垂直同步。
    bool enableVsync{ true };
};

/// @brief 应用组合入口，负责装配窗口、ImGui 层、UI 和主循环。
class Application
{
public:
    /// @brief 构造应用入口。
    /// @param config 启动配置。
    explicit Application(ApplicationConfig config = {});
    ~Application() = default;

    Application(const Application&)            = delete;
    Application& operator=(const Application&) = delete;

    /// @brief 启动应用并阻塞到主窗口关闭。
    /// @return 进程退出码，0 表示正常退出。
    int run();

private:
    /// @brief 启动配置。
    ApplicationConfig m_config;
};

}  // namespace vi
