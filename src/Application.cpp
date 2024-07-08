#include <Hephaestus/Application.hpp>
#include <Hephaestus/VectorMath.hpp>

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
bool g_isEditor = false;
bool g_sleepWhenWindowHasNoFocus = true;
bool g_windowHasFocus = false;
bool g_windowFramebufferResized = false;
bool g_sceneViewerResized = false;
bool g_cursorJustEntered = false;
bool g_cursorIsActive = true;
float g_cursorSensitivity = 0.0025f;

glm::dvec2 g_cursorPosition = {};
glm::dvec2 g_cursorFrameOffset = {};

glm::ivec2 g_windowFramebufferSize = {};
glm::ivec2 g_windowFramebufferScaledSize = {};
glm::ivec2 g_sceneViewerSize = {};
glm::ivec2 g_sceneViewerScaledSize = {};

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
        g_isEditor = !g_isEditor;
        g_windowFramebufferResized = !g_isEditor;
        g_sceneViewerResized = g_isEditor;
    }
}

auto OnWindowCursorEntered(
        [[maybe_unused]] GLFWwindow* window,
        int entered) -> void {

    if (entered) {
        g_cursorJustEntered = true;
    }

    g_windowHasFocus = entered == GLFW_TRUE;
}

auto OnWindowCursorPosition(
        [[maybe_unused]] GLFWwindow* window,
        double cursorPositionX,
        double cursorPositionY) -> void {

    if (g_cursorJustEntered)
    {
        g_cursorPosition = {cursorPositionX, cursorPositionY};
        g_cursorJustEntered = false;
    }

    g_cursorFrameOffset += glm::dvec2{
            cursorPositionX - g_cursorPosition.x,
            g_cursorPosition.y - cursorPositionY};
    g_cursorPosition = {cursorPositionX, cursorPositionY};
}

auto OnWindowFramebufferSizeChanged(
        [[maybe_unused]] GLFWwindow* window,
        const int width,
        const int height) -> void {

    g_windowFramebufferSize = glm::ivec2{width, height};
    g_windowFramebufferResized = true;
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

    const auto windowWidth = applicationSettings.ResolutionWidth;
    const auto windowHeight = applicationSettings.ResolutionHeight;

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
    glfwSetInputMode(g_window, GLFW_CURSOR, g_cursorIsActive ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);

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
                    std::function<void()>&& applicationUnload) -> void {

    if (!InternalApplicationInitialize(applicationSettings)) {
        return;
    }

    entt::registry registry;

    if (!applicationLoad(registry)) {
        return;
    }

    spdlog::info("Application: Initialized");

    uint64_t frameCounter = 0;
    auto currentTimeInSeconds = glfwGetTime();
    auto previousTimeInSeconds = glfwGetTime();
    auto accumulatedTimeInSeconds = 0.0;
    while (!glfwWindowShouldClose(g_window)) {

        auto deltaTimeInSeconds = currentTimeInSeconds - previousTimeInSeconds;
        accumulatedTimeInSeconds += deltaTimeInSeconds;
        previousTimeInSeconds = currentTimeInSeconds;
        currentTimeInSeconds = glfwGetTime();

        applicationUpdate(registry, static_cast<float>(deltaTimeInSeconds));
        applicationRender(registry, static_cast<float>(deltaTimeInSeconds));

        glfwPollEvents();
        glfwSwapBuffers(g_window);
    }

    applicationUnload();
    InternalApplicationUnload();

    spdlog::info("Application: Shutdown");
}