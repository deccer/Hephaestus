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

constexpr auto GlfwKeyToKey(int32_t glfwKey) -> EKey {
    switch (glfwKey) {
        case GLFW_KEY_ESCAPE: return EKey::KeyEscape;
        case GLFW_KEY_F1: return EKey::KeyF1;
        case GLFW_KEY_F2: return EKey::KeyF2;
        case GLFW_KEY_F3: return EKey::KeyF3;
        case GLFW_KEY_F4: return EKey::KeyF4;
        case GLFW_KEY_F5: return EKey::KeyF5;
        case GLFW_KEY_F6: return EKey::KeyF6;
        case GLFW_KEY_F7: return EKey::KeyF7;
        case GLFW_KEY_F8: return EKey::KeyF8;
        case GLFW_KEY_F9: return EKey::KeyF9;
        case GLFW_KEY_F10: return EKey::KeyF10;
        case GLFW_KEY_F11: return EKey::KeyF11;
        case GLFW_KEY_F12: return EKey::KeyF12;
        case GLFW_KEY_PRINT_SCREEN: return EKey::KeyPrint;
        case GLFW_KEY_SCROLL_LOCK: return EKey::KeyScrollLock;
        case GLFW_KEY_PAUSE: return EKey::KeyPause;

        case GLFW_KEY_GRAVE_ACCENT: return EKey::KeyGraveAccent;
        case GLFW_KEY_1: return EKey::Key1;
        case GLFW_KEY_2: return EKey::Key2;
        case GLFW_KEY_3: return EKey::Key3;
        case GLFW_KEY_4: return EKey::Key4;
        case GLFW_KEY_5: return EKey::Key5;
        case GLFW_KEY_6: return EKey::Key6;
        case GLFW_KEY_7: return EKey::Key7;
        case GLFW_KEY_8: return EKey::Key8;
        case GLFW_KEY_9: return EKey::Key9;
        case GLFW_KEY_0: return EKey::Key0;
        case GLFW_KEY_MINUS: return EKey::KeyMinus;
        case GLFW_KEY_EQUAL: return EKey::KeyEqual;
        case GLFW_KEY_BACKSPACE: return EKey::KeyBackspace;

        case GLFW_KEY_TAB: return EKey::KeyTab;
        case GLFW_KEY_Q: return EKey::KeyQ;
        case GLFW_KEY_W: return EKey::KeyW;
        case GLFW_KEY_E: return EKey::KeyE;
        case GLFW_KEY_R: return EKey::KeyR;
        case GLFW_KEY_T: return EKey::KeyT;
        case GLFW_KEY_Y: return EKey::KeyY;
        case GLFW_KEY_U: return EKey::KeyU;
        case GLFW_KEY_I: return EKey::KeyI;
        case GLFW_KEY_O: return EKey::KeyO;
        case GLFW_KEY_P: return EKey::KeyP;
        case GLFW_KEY_LEFT_BRACKET: return EKey::KeyLeftBracket;
        case GLFW_KEY_RIGHT_BRACKET: return EKey::KeyRightBracket;

        case GLFW_KEY_CAPS_LOCK: return EKey::KeyCapsLock;
        case GLFW_KEY_A: return EKey::KeyA;
        case GLFW_KEY_S: return EKey::KeyS;
        case GLFW_KEY_D: return EKey::KeyD;
        case GLFW_KEY_F: return EKey::KeyF;
        case GLFW_KEY_G: return EKey::KeyG;
        case GLFW_KEY_H: return EKey::KeyH;
        case GLFW_KEY_J: return EKey::KeyJ;
        case GLFW_KEY_K: return EKey::KeyK;
        case GLFW_KEY_L: return EKey::KeyL;
        case GLFW_KEY_SEMICOLON: return EKey::KeySemicolon;
        case GLFW_KEY_APOSTROPHE: return EKey::KeyApostrophe;
        case GLFW_KEY_ENTER: return EKey::KeyEnter;

        case GLFW_KEY_LEFT_SHIFT: return EKey::KeyLeftShift;
        case GLFW_KEY_Z: return EKey::KeyL;
        case GLFW_KEY_X: return EKey::KeyL;
        case GLFW_KEY_C: return EKey::KeyL;
        case GLFW_KEY_V: return EKey::KeyL;
        case GLFW_KEY_B: return EKey::KeyL;
        case GLFW_KEY_N: return EKey::KeyL;
        case GLFW_KEY_M: return EKey::KeyL;
        case GLFW_KEY_COMMA: return EKey::KeyComma;
        case GLFW_KEY_PERIOD: return EKey::KeyPeriod;
        case GLFW_KEY_SLASH: return EKey::KeySlash;
        case GLFW_KEY_RIGHT_SHIFT: return EKey::KeyRightShift;

        case GLFW_KEY_LEFT_CONTROL: return EKey::KeyLeftControl;
        case GLFW_KEY_LEFT_SUPER: return EKey::KeyLeftMeta;
        case GLFW_KEY_LEFT_ALT: return EKey::KeyLeftAlt;
        case GLFW_KEY_SPACE: return EKey::KeySpace;
        case GLFW_KEY_RIGHT_ALT: return EKey::KeyRightAlt;
        case GLFW_KEY_RIGHT_SUPER: return EKey::KeyRightMeta;
        case GLFW_KEY_RIGHT_CONTROL: return EKey::KeyRightControl;

        default: return EKey::KeyNone;
    }
}

constexpr auto GlfwMouseButtonToMouseButton(int32_t button) -> EMouseButton {
    switch (button) {
        case GLFW_MOUSE_BUTTON_1: return EMouseButton::Left;
        case GLFW_MOUSE_BUTTON_2: return EMouseButton::Right;
        case GLFW_MOUSE_BUTTON_3: return EMouseButton::Middle;
        default: return EMouseButton::None;
    }
}

class ApplicationAccess {
public:
    static auto OnWindowKey(
        [[maybe_unused]] GLFWwindow* window,
        const int32_t key,
        [[maybe_unused]] int32_t scancode,
        const int32_t action,
        [[maybe_unused]] int32_t mods) -> void {

        if (action == GLFW_PRESS || action == GLFW_RELEASE) {
            Keyboard::SetKey(GlfwKeyToKey(key), action != GLFW_RELEASE);
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

        Mouse::SetButton(GlfwMouseButtonToMouseButton(button), action == GLFW_PRESS || action == GLFW_REPEAT);
    }

    static auto OnWindowCursorEntered(
        [[maybe_unused]] GLFWwindow* window,
        int32_t entered) -> void {

        auto application = static_cast<Application*>(glfwGetWindowUserPointer(window));
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
            Mouse::SetDeltaCursorPosition(
                static_cast<float>(cursorPositionX),
                static_cast<float>(cursorPositionY));
            _cursorJustEntered = false;
        }

        Mouse::SetDeltaCursorPosition(
            static_cast<float>(cursorPositionX - lastCursorPositionX),
            static_cast<float>(lastCursorPositionY - cursorPositionY));

        lastCursorPositionX = cursorPositionX;
        lastCursorPositionY = cursorPositionY;
    }

    static auto OnWindowFramebufferSizeChanged(
        GLFWwindow* window,
        const int32_t width,
        const int32_t height) -> void {

        auto application = static_cast<Application*>(glfwGetWindowUserPointer(window));
        application->OnWindowFramebufferSizeChanged(width, height);
    }
private:
    static bool _cursorJustEntered;
};

bool ApplicationAccess::_cursorJustEntered = true;

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

Application::Application(const SApplicationCreateInfo& applicationCreateInfo)
    : _applicationSettings(applicationCreateInfo.Settings), _applicationContext({}), _window(nullptr), _guiContext(nullptr) {

    _applicationContext.WindowFramebufferSize = {_applicationSettings.ResolutionWidth, _applicationSettings.ResolutionHeight};
    _applicationContext.WindowFramebufferScaledSize = {static_cast<int32_t>(_applicationSettings.ResolutionWidth * _applicationSettings.ResolutionScale),
                                                       static_cast<int32_t>(_applicationSettings.ResolutionHeight * _applicationSettings.ResolutionScale)};
    _applicationContext.SceneViewerSize = {_applicationSettings.ResolutionWidth, _applicationSettings.ResolutionHeight};

    if (applicationCreateInfo.Scene == nullptr) {
        _scene = std::make_unique<DefaultScene>();
    } else {
        _scene.reset(applicationCreateInfo.Scene);
    }

    if (applicationCreateInfo.Renderer == nullptr) {
        _renderer = std::make_unique<DefaultRenderer>(_applicationSettings, _applicationContext);
    } else {
        _renderer.reset(applicationCreateInfo.Renderer);
    }
}

auto Application::Run() -> void {

    if (!Initialize()) {
        return;
    }

    if (!Load()) {
        return;
    }

    SRenderContext renderContext = {
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

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        //if (!_applicationContext.IsEditor) {
            ImGui::SetNextWindowPos({32, 32});
            ImGui::SetNextWindowSize({168, 176});
            auto windowBackgroundColor = ImGui::GetStyleColorVec4(ImGuiCol_WindowBg);
            windowBackgroundColor.w = 0.4f;
            ImGui::PushStyleColor(ImGuiCol_WindowBg, windowBackgroundColor);
            if (ImGui::Begin("#InGameStatistics", nullptr, ImGuiWindowFlags_Modal | ImGuiWindowFlags_NoDecoration)) {

                ImGui::TextColored(ImVec4{0.9f, 0.7f, 0.0f, 1.0f}, "F1 to toggle editor");
                ImGui::SeparatorText("Frame Statistics");

                auto framesPerSecond = 1.0f / renderContext.DeltaTime;
                ImGui::Text("afps: %.0f rad/s", glm::two_pi<float>() * framesPerSecond);
                ImGui::Text("dfps: %.0f °/s", glm::degrees(glm::two_pi<float>() * framesPerSecond));
                ImGui::Text("rfps: %.0f", framesPerSecond);
                ImGui::Text("rpms: %.0f", framesPerSecond * 60.0f);
                ImGui::Text("  ft: %.2f ms", renderContext.DeltaTime * 1000.0f);
                ImGui::Text("   f: %lu", renderContext.FrameCounter);
            }
            ImGui::End();
            ImGui::PopStyleColor();
        //}


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

        glfwWaitEventsTimeout(0.007);
    }

    Unload();
}

auto Application::Initialize() -> bool {

    if (glfwInit() == GLFW_FALSE) {
        return false;
    }

    const auto isWindowWindowed = _applicationSettings.WindowStyle == EWindowStyle::Windowed;
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

    const auto windowWidth = _applicationSettings.ResolutionStartup == EResolutionStartup::NinetyPercentOfScreenSize
                             ? static_cast<int>(static_cast<float>(screenWidth) * 0.9f)
                             : _applicationSettings.ResolutionWidth;
    const auto windowHeight = _applicationSettings.ResolutionStartup == EResolutionStartup::NinetyPercentOfScreenSize
                              ? static_cast<int>(static_cast<float>(screenHeight) * 0.9f)
                              : _applicationSettings.ResolutionHeight;

    auto monitor = _applicationSettings.WindowStyle == EWindowStyle::FullscreenExclusive
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

    glfwSetKeyCallback(_window, ApplicationAccess::OnWindowKey);
    glfwSetMouseButtonCallback(_window, ApplicationAccess::OnWindowMouseButton);
    glfwSetCursorPosCallback(_window, ApplicationAccess::OnWindowCursorPosition);
    glfwSetCursorEnterCallback(_window, ApplicationAccess::OnWindowCursorEntered);
    glfwSetFramebufferSizeCallback(_window, ApplicationAccess::OnWindowFramebufferSizeChanged);

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

auto Application::Load() -> bool {

    if (!_renderer->Load()) {
        return false;
    }

    return true;
}

auto Application::Unload() -> void {

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(_guiContext);
    _guiContext = nullptr;

    _renderer->Unload();
    glfwDestroyWindow(_window);
    glfwTerminate();
}

auto Application::OnWindowFocusGained() -> void {

}

auto Application::OnWindowFocusLost() -> void {

}

auto Application::OnWindowFramebufferSizeChanged(
    int32_t framebufferWidth,
    int32_t framebufferHeight) -> void {

    //_renderer->ResizeForWindowFramebuffer(framebufferWidth, framebufferHeight);
}