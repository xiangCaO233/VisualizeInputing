# Linux 全局输入监听权限

本项目的 Linux 全局输入监听通过 `InputListener` 读取 `/dev/input/event*`。这不依赖 GLFW 窗口焦点，但进程必须能读取 event 设备。

当前常见权限形态：

```bash
crw-rw---- root input /dev/input/event0
```

推荐方案是把运行用户加入 `input` 组，重新登录后即可无需 `sudo` 运行程序：

```bash
sudo usermod -aG input "$USER"
```

如果不想改用户组，可以对构建出的可执行文件授予一次性文件 capability：

```bash
sudo setcap cap_dac_read_search=ep ./build/visualizeinput
getcap ./build/visualizeinput
```

`cap_dac_read_search` 可以绕过文件读权限和目录搜索权限，用于只读打开 `/dev/input/event*`。它的权限范围比加入 `input` 组更宽，且重新链接可执行文件后通常需要重新执行 `setcap`，因此只作为替代方案。
