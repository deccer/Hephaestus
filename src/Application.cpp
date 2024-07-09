#include <Hephaestus/Application.hpp>
#include <Hephaestus/VectorMath.hpp>

#include <thread>

#include <spdlog/spdlog.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <debugbreak.h>

#include <entt/entt.hpp>

// Scene

// Application

constexpr ImVec2 g_imvec2UnitX = ImVec2(1, 0);
constexpr ImVec2 g_imvec2UnitY = ImVec2(0, 1);

const glm::vec3 g_unitX = glm::vec3{1.0f, 0.0f, 0.0f};
const glm::vec3 g_unitY = glm::vec3{0.0f, 1.0f, 0.0f};
const glm::vec3 g_unitZ = glm::vec3{0.0f, 0.0f, 1.0f};

ImGuiContext* g_imguiContext = nullptr;
GLFWwindow* g_window = nullptr;

auto OnWindowKey(
        GLFWwindow* window,
        const int32_t key,
        [[maybe_unused]] int32_t scancode,
        const int32_t action,
        [[maybe_unused]] int32_t mods) -> void {

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
        g_applicationContext.IsEditor = !g_applicationContext.IsEditor;
        g_applicationContext.WindowFramebufferResized = !g_applicationContext.IsEditor;
        g_applicationContext.SceneViewerResized = g_applicationContext.IsEditor;
    }
}

auto OnWindowCursorEntered(
        [[maybe_unused]] GLFWwindow* window,
        int entered) -> void {

    if (entered) {
        g_applicationContext.CursorJustEntered = true;
    }

    g_applicationContext.WindowHasFocus = entered == GLFW_TRUE;
}

auto OnWindowCursorPosition(
        [[maybe_unused]] GLFWwindow* window,
        double cursorPositionX,
        double cursorPositionY) -> void {

    if (g_applicationContext.CursorJustEntered)
    {
        g_applicationContext.CursorPosition = {cursorPositionX, cursorPositionY};
        g_applicationContext.CursorJustEntered = false;
    }

    g_applicationContext.CursorFrameOffset += glm::dvec2{
            cursorPositionX - g_applicationContext.CursorPosition.x,
            g_applicationContext.CursorPosition.y - cursorPositionY};
    g_applicationContext.CursorPosition = {cursorPositionX, cursorPositionY};
}

auto OnWindowFramebufferSizeChanged(
        [[maybe_unused]] GLFWwindow* window,
        const int width,
        const int height) -> void {

    g_applicationContext.WindowFramebufferSize = glm::ivec2{width, height};
    g_applicationContext.WindowFramebufferResized = true;
}

auto OnOpenGLDebugMessage(
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

auto InternalApplicationInitialize(const SApplicationSettings& applicationSettings) -> bool {

    if (glfwInit() == GLFW_FALSE) {
        spdlog::error("Glfw: Unable to initialize");
        return false;
    }

    const auto isWindowWindowed = applicationSettings.WindowStyle == EWindowStyle::Windowed;
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

    const auto windowWidth = applicationSettings.ResolutionStartup == EResolutionStartup::NinetyPercentOfScreenSize
        ? static_cast<int>(static_cast<float>(screenWidth) * 0.9f)
        : applicationSettings.ResolutionWidth;
    const auto windowHeight = applicationSettings.ResolutionStartup == EResolutionStartup::NinetyPercentOfScreenSize
        ? static_cast<int>(static_cast<float>(screenHeight) * 0.9f)
        : applicationSettings.ResolutionHeight;

    auto monitor = applicationSettings.WindowStyle == EWindowStyle::FullscreenExclusive
        ? primaryMonitor
        : nullptr;

    g_window = glfwCreateWindow(windowWidth, windowHeight, "Hephaestus", monitor, nullptr);
    if (g_window == nullptr) {
        spdlog::error("Glfw: Unable to create a window");
        return false;
    }

    int32_t monitorLeft = 0;
    int32_t monitorTop = 0;
    glfwGetMonitorPos(primaryMonitor, &monitorLeft, &monitorTop);
    if (isWindowWindowed) {
        glfwSetWindowPos(g_window, screenWidth / 2 - windowWidth / 2 + monitorLeft, screenHeight / 2 - windowHeight / 2 + monitorTop);
    } else {
        glfwSetWindowPos(g_window, monitorLeft, monitorTop);
    }

    glfwSetKeyCallback(g_window, OnWindowKey);
    glfwSetCursorPosCallback(g_window, OnWindowCursorPosition);
    glfwSetCursorEnterCallback(g_window, OnWindowCursorEntered);
    glfwSetFramebufferSizeCallback(g_window, OnWindowFramebufferSizeChanged);
    glfwSetInputMode(g_window, GLFW_CURSOR, g_applicationContext.CursorIsActive ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);

    int32_t windowFramebufferWidth = 0;
    int32_t windowFramebufferHeight = 0;
    glfwGetFramebufferSize(g_window, &windowFramebufferWidth, &windowFramebufferHeight);
    OnWindowFramebufferSizeChanged(g_window, windowFramebufferWidth, windowFramebufferHeight);

    glfwMakeContextCurrent(g_window);

    if (gladLoadGL(glfwGetProcAddress) == GL_FALSE) {
        spdlog::error("Glad: Unable to initialize");
        return false;
    }

    if (applicationSettings.IsDebug) {
        glDebugMessageCallback(OnOpenGLDebugMessage, nullptr);
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    }

    g_imguiContext = ImGui::CreateContext();
    auto& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_IsSRGB; // this little shit doesn't do anything
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    if (!ImGui_ImplGlfw_InitForOpenGL(g_window, true)) {
        spdlog::error("ImGui: Unable to initialize the GLFW backend");
        return false;
    }
    spdlog::debug("ImGui: Initialized GLFW backend");

    if (!ImGui_ImplOpenGL3_Init("#version 460")) {
        spdlog::error("ImGui: Unable to initialize the OpenGL backend");
        return false;
    }
    spdlog::debug("ImGui: Initialized OpenGL backend");

    if (applicationSettings.IsVSyncEnabled) {
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

auto InternalApplicationUnload() -> void {

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(g_imguiContext);
}

auto RunApplication(const SApplicationSettings& applicationSettings,
                    std::function<bool(entt::registry&)>&& applicationLoad,
                    std::function<void(entt::registry&, float)>&& applicationUpdate,
                    std::function<void(entt::registry&, float)>&& applicationRender,
                    std::function<void(entt::registry&, float)>&& applicationRenderUi,
                    std::function<void()>&& applicationUnload) -> void {

    if (!InternalApplicationInitialize(applicationSettings)) {
        return;
    }

    entt::registry registry;

    if (!applicationLoad(registry)) {
        return;
    }

    auto isSrgbDisabled = false;
    auto isCullfaceDisabled = false;

    spdlog::info("Application: Initialized");

    glm::ivec2 scaledFramebufferSize = {};

    auto currentTimeInSeconds = glfwGetTime();
    auto previousTimeInSeconds = glfwGetTime();
    auto accumulatedTimeInSeconds = 0.0;
    while (!glfwWindowShouldClose(g_window)) {

        auto deltaTimeInSeconds = currentTimeInSeconds - previousTimeInSeconds;
        accumulatedTimeInSeconds += deltaTimeInSeconds;
        previousTimeInSeconds = currentTimeInSeconds;
        currentTimeInSeconds = glfwGetTime();

        auto deltaTime = static_cast<float>(deltaTimeInSeconds);
        applicationUpdate(registry, deltaTime);

        if (isSrgbDisabled) {
            glEnable(GL_FRAMEBUFFER_SRGB);
            isSrgbDisabled = false;
        }

        applicationRender(registry, deltaTime);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        applicationRenderUi(registry, deltaTime);

        {
            ImGui::Render();
            auto* imGuiDrawData = ImGui::GetDrawData();
            if (imGuiDrawData != nullptr) {
                glDisable(GL_FRAMEBUFFER_SRGB);
                //isSrgbDisabled = true;
                //PushDebugGroup("UI");
                glViewport(0, 0, g_applicationContext.WindowFramebufferSize.x, g_applicationContext.WindowFramebufferSize.y);
                ImGui_ImplOpenGL3_RenderDrawData(imGuiDrawData);
                //PopDebugGroup();
            }
        }
        glfwSwapBuffers(g_window);

        glfwPollEvents();
        g_applicationContext.FrameCounter++;
        if (!g_applicationContext.WindowHasFocus && g_applicationContext.SleepWhenWindowHasNoFocus) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }

    applicationUnload();
    InternalApplicationUnload();

    spdlog::info("Application: Shutdown");
}