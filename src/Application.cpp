#include <Hephaestus/Application.hpp>
#include <Hephaestus/Input/Keyboard.hpp>
#include <Hephaestus/Input/Mouse.hpp>

#include <Hephaestus/DefaultScene.hpp>
#include <Hephaestus/DefaultRenderer.hpp>

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <debugbreak.h>

constexpr auto GlfwKeyToKey(int32_t glfwKey) -> TKey {
    switch (glfwKey) {
        case GLFW_KEY_ESCAPE: return TKey::KeyEscape;
        case GLFW_KEY_F1: return TKey::KeyF1;
        case GLFW_KEY_F2: return TKey::KeyF2;
        case GLFW_KEY_F3: return TKey::KeyF3;
        case GLFW_KEY_F4: return TKey::KeyF4;
        case GLFW_KEY_F5: return TKey::KeyF5;
        case GLFW_KEY_F6: return TKey::KeyF6;
        case GLFW_KEY_F7: return TKey::KeyF7;
        case GLFW_KEY_F8: return TKey::KeyF8;
        case GLFW_KEY_F9: return TKey::KeyF9;
        case GLFW_KEY_F10: return TKey::KeyF10;
        case GLFW_KEY_F11: return TKey::KeyF11;
        case GLFW_KEY_F12: return TKey::KeyF12;
        case GLFW_KEY_PRINT_SCREEN: return TKey::KeyPrint;
        case GLFW_KEY_SCROLL_LOCK: return TKey::KeyScrollLock;
        case GLFW_KEY_PAUSE: return TKey::KeyPause;

        case GLFW_KEY_GRAVE_ACCENT: return TKey::KeyGraveAccent;
        case GLFW_KEY_1: return TKey::Key1;
        case GLFW_KEY_2: return TKey::Key2;
        case GLFW_KEY_3: return TKey::Key3;
        case GLFW_KEY_4: return TKey::Key4;
        case GLFW_KEY_5: return TKey::Key5;
        case GLFW_KEY_6: return TKey::Key6;
        case GLFW_KEY_7: return TKey::Key7;
        case GLFW_KEY_8: return TKey::Key8;
        case GLFW_KEY_9: return TKey::Key9;
        case GLFW_KEY_0: return TKey::Key0;
        case GLFW_KEY_MINUS: return TKey::KeyMinus;
        case GLFW_KEY_EQUAL: return TKey::KeyEqual;
        case GLFW_KEY_BACKSPACE: return TKey::KeyBackspace;

        case GLFW_KEY_TAB: return TKey::KeyTab;
        case GLFW_KEY_Q: return TKey::KeyQ;
        case GLFW_KEY_W: return TKey::KeyW;
        case GLFW_KEY_E: return TKey::KeyE;
        case GLFW_KEY_R: return TKey::KeyR;
        case GLFW_KEY_T: return TKey::KeyT;
        case GLFW_KEY_Y: return TKey::KeyY;
        case GLFW_KEY_U: return TKey::KeyU;
        case GLFW_KEY_I: return TKey::KeyI;
        case GLFW_KEY_O: return TKey::KeyO;
        case GLFW_KEY_P: return TKey::KeyP;
        case GLFW_KEY_LEFT_BRACKET: return TKey::KeyLeftBracket;
        case GLFW_KEY_RIGHT_BRACKET: return TKey::KeyRightBracket;

        case GLFW_KEY_CAPS_LOCK: return TKey::KeyCapsLock;
        case GLFW_KEY_A: return TKey::KeyA;
        case GLFW_KEY_S: return TKey::KeyS;
        case GLFW_KEY_D: return TKey::KeyD;
        case GLFW_KEY_F: return TKey::KeyF;
        case GLFW_KEY_G: return TKey::KeyG;
        case GLFW_KEY_H: return TKey::KeyH;
        case GLFW_KEY_J: return TKey::KeyJ;
        case GLFW_KEY_K: return TKey::KeyK;
        case GLFW_KEY_L: return TKey::KeyL;
        case GLFW_KEY_SEMICOLON: return TKey::KeySemicolon;
        case GLFW_KEY_APOSTROPHE: return TKey::KeyApostrophe;
        case GLFW_KEY_ENTER: return TKey::KeyEnter;

        case GLFW_KEY_LEFT_SHIFT: return TKey::KeyLeftShift;
        case GLFW_KEY_Z: return TKey::KeyL;
        case GLFW_KEY_X: return TKey::KeyL;
        case GLFW_KEY_C: return TKey::KeyL;
        case GLFW_KEY_V: return TKey::KeyL;
        case GLFW_KEY_B: return TKey::KeyL;
        case GLFW_KEY_N: return TKey::KeyL;
        case GLFW_KEY_M: return TKey::KeyL;
        case GLFW_KEY_COMMA: return TKey::KeyComma;
        case GLFW_KEY_PERIOD: return TKey::KeyPeriod;
        case GLFW_KEY_SLASH: return TKey::KeySlash;
        case GLFW_KEY_RIGHT_SHIFT: return TKey::KeyRightShift;

        case GLFW_KEY_LEFT_CONTROL: return TKey::KeyLeftControl;
        case GLFW_KEY_LEFT_SUPER: return TKey::KeyLeftMeta;
        case GLFW_KEY_LEFT_ALT: return TKey::KeyLeftAlt;
        case GLFW_KEY_SPACE: return TKey::KeySpace;
        case GLFW_KEY_RIGHT_ALT: return TKey::KeyRightAlt;
        case GLFW_KEY_RIGHT_SUPER: return TKey::KeyRightMeta;
        case GLFW_KEY_RIGHT_CONTROL: return TKey::KeyRightControl;

        default: return TKey::KeyNone;
    }
}

constexpr auto GlfwMouseButtonToMouseButton(int32_t button) -> TMouseButton {
    switch (button) {
        case GLFW_MOUSE_BUTTON_1: return TMouseButton::Left;
        case GLFW_MOUSE_BUTTON_2: return TMouseButton::Right;
        case GLFW_MOUSE_BUTTON_3: return TMouseButton::Middle;
        default: return TMouseButton::None;
    }
}

class TApplicationAccess {
public:
    static auto OnWindowKey(
        [[maybe_unused]] GLFWwindow* window,
        const int32_t key,
        [[maybe_unused]] int32_t scancode,
        const int32_t action,
        [[maybe_unused]] int32_t mods) -> void {

        if (action == GLFW_PRESS || action == GLFW_RELEASE) {
            TKeyboard::SetKey(GlfwKeyToKey(key), action != GLFW_RELEASE);
        }
        /*
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
        if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
            g_applicationContext.IsEditor = !g_applicationContext.IsEditor;
            g_applicationContext.WindowFramebufferResized = !g_applicationContext.IsEditor;
            g_applicationContext.SceneViewerResized = g_applicationContext.IsEditor;
        }
        */
    }

    static auto OnWindowMouseButton(
        [[maybe_unused]] GLFWwindow* window,
        int32_t button,
        int32_t action,
        [[maybe_unused]] int32_t modifier) -> void {

        TMouse::SetButton(GlfwMouseButtonToMouseButton(button), action == GLFW_PRESS || action == GLFW_REPEAT);
    }

    static auto OnWindowCursorEntered(
        [[maybe_unused]] GLFWwindow* window,
        int32_t entered) -> void {

        auto application = static_cast<TApplication*>(glfwGetWindowUserPointer(window));
        if (entered) {
            application->OnWindowFocusGained();
        } else {
            application->OnWindowFocusLost();
        }
    }

    static auto OnWindowCursorPosition(
        [[maybe_unused]] GLFWwindow* window,
        double cursorPositionX,
        double cursorPositionY) -> void {

        static double lastCursorPositionX = cursorPositionX;
        static double lastCursorPositionY = cursorPositionY;

        if (_cursorJustEntered)
        {
            TMouse::SetDeltaCursorPosition(
                static_cast<float>(cursorPositionX),
                static_cast<float>(cursorPositionY));
            _cursorJustEntered = false;
        }

        TMouse::SetDeltaCursorPosition(
            static_cast<float>(cursorPositionX - lastCursorPositionX),
            static_cast<float>(lastCursorPositionY - cursorPositionY));

        lastCursorPositionX = cursorPositionX;
        lastCursorPositionY = cursorPositionY;
    }

    static auto OnWindowFramebufferSizeChanged(
        GLFWwindow* window,
        const int32_t width,
        const int32_t height) -> void {

        auto application = static_cast<TApplication*>(glfwGetWindowUserPointer(window));
        application->OnWindowFramebufferSizeChanged(width, height);
    }
private:
    static bool _cursorJustEntered;
};

bool TApplicationAccess::_cursorJustEntered = true;

static auto OnOpenGLDebugMessage(
    [[maybe_unused]] uint32_t source,
    uint32_t type,
    [[maybe_unused]] uint32_t id,
    [[maybe_unused]] uint32_t severity,
    [[maybe_unused]] int32_t length,
    const char* message,
    [[maybe_unused]] const void* userParam) -> void {

    if (type == GL_DEBUG_TYPE_ERROR) {
        spdlog::error(message);
        debug_break();
    }
}

TApplication::TApplication(const TApplicationCreateInfo& applicationCreateInfo)
    : _applicationSettings(applicationCreateInfo.Settings), _applicationContext({}), _window(nullptr), _guiContext(nullptr) {

    _applicationContext.WindowFramebufferSize = {_applicationSettings.ResolutionWidth, _applicationSettings.ResolutionHeight};
    _applicationContext.WindowFramebufferScaledSize = {static_cast<int32_t>(_applicationSettings.ResolutionWidth * _applicationSettings.ResolutionScale),
                                                       static_cast<int32_t>(_applicationSettings.ResolutionHeight * _applicationSettings.ResolutionScale)};
    _applicationContext.SceneViewerSize = {_applicationSettings.ResolutionWidth, _applicationSettings.ResolutionHeight};

    if (applicationCreateInfo.Scene == nullptr) {
        _scene = std::make_unique<TDefaultScene>();
    } else {
        _scene.reset(applicationCreateInfo.Scene);
    }

    if (applicationCreateInfo.Renderer == nullptr) {
        _renderer = std::make_unique<TDefaultRenderer>(_applicationSettings, _applicationContext);
    } else {
        _renderer.reset(applicationCreateInfo.Renderer);
    }
}

auto TApplication::Run() -> void {

    if (!Initialize()) {
        return;
    }

    if (!Load()) {
        return;
    }

    TRenderContext renderContext = {
        .IsSrgbDisabled = true,
        .FrameCounter = 0,
    };

    auto currentTimeInSeconds = glfwGetTime();
    auto previousTimeInSeconds = glfwGetTime();
    auto accumulatedTimeInSeconds = 0.0;
    while (!glfwWindowShouldClose(_window)) {

        auto deltaTimeInSeconds = currentTimeInSeconds - previousTimeInSeconds;
        accumulatedTimeInSeconds += deltaTimeInSeconds;
        previousTimeInSeconds = currentTimeInSeconds;
        currentTimeInSeconds = glfwGetTime();

        renderContext.DeltaTime = static_cast<float>(deltaTimeInSeconds);
        renderContext.FrameCounter++;

        if (renderContext.IsSrgbDisabled) {
            glEnable(GL_FRAMEBUFFER_SRGB);
            renderContext.IsSrgbDisabled = false;
        }

        _renderer->Render(renderContext, *_scene);

        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
        }

        _renderer->RenderUserInterface(renderContext, *_scene);

        {
            ImGui::Render();
            auto* imGuiDrawData = ImGui::GetDrawData();
            if (imGuiDrawData != nullptr) {
                //PushDebugGroup("UI");
                glDisable(GL_FRAMEBUFFER_SRGB);
                renderContext.IsSrgbDisabled = true;
                glViewport(0, 0, _applicationContext.WindowFramebufferSize.x, _applicationContext.WindowFramebufferSize.y);
                ImGui_ImplOpenGL3_RenderDrawData(imGuiDrawData);
                //PopDebugGroup();
            }
        }

        glfwSwapBuffers(_window);

        glfwPollEvents();
    }

    Unload();
}

auto TApplication::Initialize() -> bool {

    if (glfwInit() == GLFW_FALSE) {
        return false;
    }

    const auto isWindowWindowed = _applicationSettings.WindowStyle == TWindowStyle::Windowed;
    glfwWindowHint(GLFW_DECORATED, isWindowWindowed ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_RESIZABLE, isWindowWindowed ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    const auto primaryMonitor = glfwGetPrimaryMonitor();
    if (primaryMonitor == nullptr) {
        spdlog::error("Glfw: Unable to get the primary monitor");
        return false;
    }

    const auto primaryMonitorVideoMode = glfwGetVideoMode(primaryMonitor);
    const auto screenWidth = primaryMonitorVideoMode->width;
    const auto screenHeight = primaryMonitorVideoMode->height;

    const auto windowWidth = _applicationSettings.ResolutionStartup == TResolutionStartup::NinetyPercentOfScreenSize
                             ? static_cast<int>(static_cast<float>(screenWidth) * 0.9f)
                             : _applicationSettings.ResolutionWidth;
    const auto windowHeight = _applicationSettings.ResolutionStartup == TResolutionStartup::NinetyPercentOfScreenSize
                              ? static_cast<int>(static_cast<float>(screenHeight) * 0.9f)
                              : _applicationSettings.ResolutionHeight;

    auto monitor = _applicationSettings.WindowStyle == TWindowStyle::FullscreenExclusive
                   ? primaryMonitor
                   : nullptr;

    _window = glfwCreateWindow(windowWidth,
                               windowHeight,
                               _applicationSettings.Title.empty() ? "OpenSpace" : _applicationSettings.Title.data(),
                               monitor,
                               nullptr);
    if (_window == nullptr) {
        spdlog::error("Glfw: Unable to create a window");
        return false;
    }

    int32_t monitorLeft = 0;
    int32_t monitorTop = 0;
    glfwGetMonitorPos(primaryMonitor, &monitorLeft, &monitorTop);
    if (isWindowWindowed) {
        glfwSetWindowPos(_window, screenWidth / 2 - windowWidth / 2 + monitorLeft, screenHeight / 2 - windowHeight / 2 + monitorTop);
    } else {
        glfwSetWindowPos(_window, monitorLeft, monitorTop);
    }

    glfwSetKeyCallback(_window, TApplicationAccess::OnWindowKey);
    glfwSetMouseButtonCallback(_window, TApplicationAccess::OnWindowMouseButton);
    glfwSetCursorPosCallback(_window, TApplicationAccess::OnWindowCursorPosition);
    glfwSetCursorEnterCallback(_window, TApplicationAccess::OnWindowCursorEntered);
    glfwSetFramebufferSizeCallback(_window, TApplicationAccess::OnWindowFramebufferSizeChanged);

    int32_t windowFramebufferWidth = 0;
    int32_t windowFramebufferHeight = 0;
    glfwGetFramebufferSize(_window, &windowFramebufferWidth, &windowFramebufferHeight);
    OnWindowFramebufferSizeChanged(windowFramebufferWidth, windowFramebufferHeight);

    glfwSetInputMode(_window, GLFW_CURSOR, _applicationContext.CursorIsActive ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    glfwMakeContextCurrent(_window);

    if (gladLoadGL(glfwGetProcAddress) == GL_FALSE) {
        spdlog::error("Glad: Unable to initialize");
        return false;
    }

    if (_applicationSettings.IsDebug) {
        glDebugMessageCallback(OnOpenGLDebugMessage, nullptr);
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    }

    _guiContext = ImGui::CreateContext();
    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_IsSRGB; // this little shit doesn't do anything
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    io.Fonts->ClearFonts();
    io.Fonts->AddFontFromFileTTF("data/Fonts/Ubuntu/UbuntuMono-R.ttf", 16.0f);
    if (!io.Fonts->Build()) {
        spdlog::error("ImGui: Unable to Build Font Atlas");
    }

    if (!ImGui_ImplGlfw_InitForOpenGL(_window, true)) {
        spdlog::error("ImGui: Unable to initialize the GLFW backend");
        return false;
    }
    spdlog::debug("ImGui: Initialized GLFW backend");

    if (!ImGui_ImplOpenGL3_Init("#version 460")) {
        spdlog::error("ImGui: Unable to initialize the OpenGL backend");
        return false;
    }
    spdlog::debug("ImGui: Initialized OpenGL backend");

    if (_applicationSettings.IsVSyncEnabled) {
        glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }

    glEnable(GL_FRAMEBUFFER_SRGB);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.03f, 0.05f, 0.07f, 1.0f);

    return true;
}

auto TApplication::Load() -> bool {

    if (!_renderer->Load()) {
        return false;
    }

    if (!_scene->Load()) {
        return false;
    }

    return true;
}

auto TApplication::Unload() -> void {

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(_guiContext);
    _guiContext = nullptr;

    _renderer->Unload();
    glfwDestroyWindow(_window);
    glfwTerminate();
}

auto TApplication::OnWindowFocusGained() -> void {

}

auto TApplication::OnWindowFocusLost() -> void {

}

auto TApplication::OnWindowFramebufferSizeChanged(
    int32_t framebufferWidth,
    int32_t framebufferHeight) -> void {

    //_renderer->ResizeForWindowFramebuffer(framebufferWidth, framebufferHeight);
}