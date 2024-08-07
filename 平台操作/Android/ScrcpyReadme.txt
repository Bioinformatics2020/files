常用命令：scrcpy.exe -S --no-audio --window-width=1200
--help

Options:

    --always-on-top
        Make scrcpy window always on top (above other windows).
		使scrcpy窗口始终位于顶部（高于其他窗口）

    --audio-bit-rate=value
        Encode the audio at the given bit-rate, expressed in bits/s. Unit suffixes are supported: 'K' (x1000) and 'M'
        (x1000000).
		以给定的比特率对音频进行编码，以比特/秒表示。支持单位后缀：'K'（x1000）和'M'（x1000000）。
        Default is 128K (128000).

    --audio-buffer=ms
        Configure the audio buffering delay (in milliseconds).
        Lower values decrease the latency, but increase the likelyhood of buffer underrun (causing audio glitches).
        配置音频缓冲延迟（以毫秒为单位）。
		较低的值会降低延迟，但会增加缓冲区不足（导致音频故障）的可能性。
		Default is 50.

    --audio-codec=name
        Select an audio codec (opus, aac or raw).
		选择音频编解码器（opus、aac或raw）。
        Default is opus.

    --audio-codec-options=key[:type]=value[,...]
        Set a list of comma-separated key:type=value options for the device audio encoder.
        The possible values for 'type' are 'int' (default), 'long', 'float' and 'string'.
        The list of possible codec options is available in the Android documentation:
        <https://d.android.com/reference/android/media/MediaFormat>

    --audio-encoder=name
        Use a specific MediaCodec audio encoder (depending on the codec provided by --audio-codec).
        The available encoders can be listed by --list-encoders.
		使用特定的MediaCodec音频编码器（取决于--audio codec提供的编解码器）。
		可用的编码器可以通过列表编码器列出。

    -b, --video-bit-rate=value
        Encode the video at the given bit-rate, expressed in bits/s. Unit suffixes are supported: 'K' (x1000) and 'M'
        (x1000000).
		以给定的比特率对视频进行编码，以比特/秒表示。支持单位后缀：'K'（x1000）和'M'（x1000000）。
        Default is 8M (8000000).

    --crop=width:height:x:y
        Crop the device screen on the server.
        The values are expressed in the device natural orientation (typically, portrait for a phone, landscape for a
        tablet). Any --max-size value is computed on the cropped size.

    -d, --select-usb
        Use USB device (if there is exactly one, like adb -d).
        Also see -e (--select-tcpip).

    --disable-screensaver
        Disable screensaver while scrcpy is running.

    --display=id
        Specify the device display id to mirror.
		指定屏幕
        The available display ids can be listed by:
		查看所有的displays：
            scrcpy --list-displays
        Default is 0.

    --display-buffer=ms
        Add a buffering delay (in milliseconds) before displaying. This increases latency to compensate for jitter.
        Default is 0 (no buffering).

    -e, --select-tcpip
        Use TCP/IP device (if there is exactly one, like adb -e).
        Also see -d (--select-usb).

    --force-adb-forward
        Do not attempt to use "adb reverse" to connect to the device.

    --forward-all-clicks
        By default, right-click triggers BACK (or POWER on) and middle-click triggers HOME. This option disables these
        shortcuts and forwards the clicks to the device instead.

    -f, --fullscreen
        Start in fullscreen.

    -K, --hid-keyboard
        Simulate a physical keyboard by using HID over AOAv2.
        It provides a better experience for IME users, and allows to generate non-ASCII characters, contrary to the
        default injection method.
        It may only work over USB.
        The keyboard layout must be configured (once and for all) on the device, via Settings -> System -> Languages and
        input -> Physical keyboard. This settings page can be started directly: `adb shell am start -a
        android.settings.HARD_KEYBOARD_SETTINGS`.
        However, the option is only available when the HID keyboard is enabled (or a physical keyboard is connected).
        Also see --hid-mouse.

    -h, --help
        Print this help.

    --legacy-paste
        Inject computer clipboard text as a sequence of key events on Ctrl+v (like MOD+Shift+v).
        This is a workaround for some devices not behaving as expected when setting the device clipboard
        programmatically.

    --list-displays
        List device displays.

    --list-encoders
        List video and audio encoders available on the device.

    --lock-video-orientation[=value]
        Lock video orientation to value.
        Possible values are "unlocked", "initial" (locked to the initial orientation), 0, 1, 2 and 3. Natural device
        orientation is 0, and each increment adds a 90 degrees rotation counterclockwise.
        Default is "unlocked".
        Passing the option without argument is equivalent to passing "initial".

    --max-fps=value
        Limit the frame rate of screen capture (officially supported since Android 10, but may work on earlier
        versions).

    -M, --hid-mouse
        Simulate a physical mouse by using HID over AOAv2.
        In this mode, the computer mouse is captured to control the device directly (relative mouse mode).
        LAlt, LSuper or RSuper toggle the capture mode, to give control of the mouse back to the computer.
        It may only work over USB.
        Also see --hid-keyboard.

    -m, --max-size=value
        Limit both the width and height of the video to value. The other dimension is computed so that the device
        aspect-ratio is preserved.
		将视频的宽度和高度限制为指定值。自动计算另一个维度，以便保留设备纵横比。
        Default is 0 (unlimited).

    --no-audio
        Disable audio forwarding.
		禁用音频转发。

    --no-cleanup
        By default, scrcpy removes the server binary from the device and restores the device state (show touches, stay
        awake and power mode) on exit.
        This option disables this cleanup.
		默认情况下，scrcpy从设备中删除服务器二进制文件并恢复设备状态（显示触摸，停留唤醒和电源模式）。
		此选项将禁用此清理。

    --no-clipboard-autosync
        By default, scrcpy automatically synchronizes the computer clipboard to the device clipboard before injecting
        Ctrl+v, and the device clipboard to the computer clipboard whenever it changes.
        This option disables this automatic synchronization.

    --no-downsize-on-error
        By default, on MediaCodec error, scrcpy automatically tries again with a lower definition.
        This option disables this behavior.

    -n, --no-control
        Disable device control (mirror the device in read-only).

    -N, --no-display
        Do not display device (only when screen recording or V4L2 sink is enabled).

    --no-key-repeat
        Do not forward repeated key events when a key is held down.

    --no-mipmaps
        If the renderer is OpenGL 3.0+ or OpenGL ES 2.0+, then mipmaps are automatically generated to improve
        downscaling quality. This option disables the generation of mipmaps.

    --no-power-on
        Do not power on the device on start.

    --otg
        Run in OTG mode: simulate physical keyboard and mouse, as if the computer keyboard and mouse were plugged
        directly to the device via an OTG cable.
        In this mode, adb (USB debugging) is not necessary, and mirroring is disabled.
        LAlt, LSuper or RSuper toggle the mouse capture mode, to give control of the mouse back to the computer.
        If any of --hid-keyboard or --hid-mouse is set, only enable keyboard or mouse respectively, otherwise enable
        both.
        It may only work over USB.
        See --hid-keyboard and --hid-mouse.

    -p, --port=port[:port]
        Set the TCP port (range) used by the client to listen.
        Default is 27183:27199.

    --power-off-on-close
        Turn the device screen off when closing scrcpy.

    --prefer-text
        Inject alpha characters and space as text events instead of key events.
        This avoids issues when combining multiple keys to enter a special character, but breaks the expected behavior
        of alpha keys in games (typically WASD).

    --print-fps
        Start FPS counter, to print framerate logs to the console. It can be started or stopped at any time with MOD+i.

    --push-target=path
        Set the target directory for pushing files to the device by drag & drop. It is passed as is to "adb push".
        Default is "/sdcard/Download/".

    --raw-key-events
        Inject key events for all input keys, and ignore text events.

    -r, --record=file.mp4
        Record screen to file.
        The format is determined by the --record-format option if set, or by the file extension (.mp4 or .mkv).

    --record-format=format
        Force recording format (either mp4 or mkv).

    --render-driver=name
        Request SDL to use the given render driver (this is just a hint).
        Supported names are currently "direct3d", "opengl", "opengles2", "opengles", "metal" and "software".
        <https://wiki.libsdl.org/SDL_HINT_RENDER_DRIVER>

    --require-audio
        By default, scrcpy mirrors only the video when audio capture fails on the device. This option makes scrcpy fail
        if audio is enabled but does not work.

    --rotation=value
        Set the initial display rotation.
        Possible values are 0, 1, 2 and 3. Each increment adds a 90 degrees rotation counterclockwise.

    -s, --serial=serial
        The device serial number. Mandatory only if several devices are connected to adb.

    --shortcut-mod=key[+...][,...]
        Specify the modifiers to use for scrcpy shortcuts.
        Possible keys are "lctrl", "rctrl", "lalt", "ralt", "lsuper" and "rsuper".
        A shortcut can consist in several keys, separated by '+'. Several shortcuts can be specified, separated by ','.
        For example, to use either LCtrl+LAlt or LSuper for scrcpy shortcuts, pass "lctrl+lalt,lsuper".
        Default is "lalt,lsuper" (left-Alt or left-Super).

    -S, --turn-screen-off
        Turn the device screen off immediately.

    -t, --show-touches
        Enable "show touches" on start, restore the initial value on exit.
        It only shows physical touches (not clicks from scrcpy).

    --tcpip[=ip[:port]]
        Configure and reconnect the device over TCP/IP.
        If a destination address is provided, then scrcpy connects to this address before starting. The device must
        listen on the given TCP port (default is 5555).
        If no destination address is provided, then scrcpy attempts to find the IP address of the current device
        (typically connected over USB), enables TCP/IP mode, then connects to this address before starting.

    --tunnel-host=ip
        Set the IP address of the adb tunnel to reach the scrcpy server. This option automatically enables
        --force-adb-forward.
        Default is localhost.

    --tunnel-port=port
        Set the TCP port of the adb tunnel to reach the scrcpy server. This option automatically enables
        --force-adb-forward.
        Default is 0 (not forced): the local port used for establishing the tunnel will be used.

    --v4l2-sink=/dev/videoN
        Output to v4l2loopback device.
        It requires to lock the video orientation (see --lock-video-orientation).
        This feature is only available on Linux.

    --v4l2-buffer=ms
        Add a buffering delay (in milliseconds) before pushing frames. This increases latency to compensate for jitter.
        This option is similar to --display-buffer, but specific to V4L2 sink.
        Default is 0 (no buffering).
        This option is only available on Linux.

    -V, --verbosity=value
        Set the log level (verbose, debug, info, warn or error).
        Default is info.

    -v, --version
        Print the version of scrcpy.

    --video-codec=name
        Select a video codec (h264, h265 or av1).
        Default is h264.

    --video-codec-options=key[:type]=value[,...]
        Set a list of comma-separated key:type=value options for the device video encoder.
        The possible values for 'type' are 'int' (default), 'long', 'float' and 'string'.
        The list of possible codec options is available in the Android documentation:
        <https://d.android.com/reference/android/media/MediaFormat>

    --video-encoder=name
        Use a specific MediaCodec video encoder (depending on the codec provided by --video-codec).
        The available encoders can be listed by --list-encoders.

    -w, --stay-awake
        Keep the device on while scrcpy is running, when the device is plugged in.
		当scrcpy运行时且设备插入电源时，保持设备处于打开状态。

    --window-borderless
        Disable window decorations (display borderless window).

    --window-title=text
        Set a custom window title.

    --window-x=value
        Set the initial window horizontal position.
        Default is "auto".

    --window-y=value
        Set the initial window vertical position.
        Default is "auto".

    --window-width=value
        Set the initial window width.
		设置初始化窗口宽度
        Default is 0 (automatic).

    --window-height=value
        Set the initial window height.
        Default is 0 (automatic).

Shortcuts:
	MOD键默认为(left) Alt 或 (left) Super
    In the following list, MOD is the shortcut modifier. By default, it's (left) Alt or (left) Super, but it can be
    configured by --shortcut-mod (see above).

    MOD+f
        Switch fullscreen mode

    MOD+Left
        Rotate display left

    MOD+Right
        Rotate display right

    MOD+g
        Resize window to 1:1 (pixel-perfect)

    MOD+w
    Double-click on black borders
        Resize window to remove black borders

    MOD+h
    Middle-click
        Click on HOME

    MOD+b
    MOD+Backspace
    Right-click (when screen is on)
        Click on BACK

    MOD+s
    4th-click
        Click on APP_SWITCH

    MOD+m
        Click on MENU

    MOD+Up
		模拟点击加音量键
        Click on VOLUME_UP

    MOD+Down
		模拟点击减音量键
        Click on VOLUME_DOWN

    MOD+p
        Click on POWER (turn screen on/off)

    Right-click (when screen is off)
        Power on

    MOD+o
        Turn device screen off (keep mirroring)

    MOD+Shift+o
        Turn device screen on

    MOD+r
        Rotate device screen

    MOD+n
    5th-click
        Expand notification panel

    MOD+Shift+n
        Collapse notification panel

    MOD+c
        Copy to clipboard (inject COPY keycode, Android >= 7 only)

    MOD+x
        Cut to clipboard (inject CUT keycode, Android >= 7 only)

    MOD+v
        Copy computer clipboard to device, then paste (inject PASTE keycode, Android >= 7 only)

    MOD+Shift+v
        Inject computer clipboard text as a sequence of key events

    MOD+i
        Enable/disable FPS counter (print frames/second in logs)

    Ctrl+click-and-move
        Pinch-to-zoom from the center of the screen

    Drag & drop APK file
        Install APK from computer

    Drag & drop non-APK file
        Push file to device (see --push-target)

Environment variables:

    ADB
        Path to adb executable

    ANDROID_SERIAL
        Device serial to use if no selector (-s, -d, -e or --tcpip=<addr>) is specified

    SCRCPY_ICON_PATH
        Path to the program icon

    SCRCPY_SERVER_PATH
        Path to the server binary

Exit status:

      0  Normal program termination
      1  Start failure
      2  Device disconnected while running