#include "core/EngineContext.hpp"
#include "ecs/Registry.hpp"
#include "ecs/components/CameraComponent.hpp"
#include "ecs/components/MeshComponent.hpp"
#include "ecs/components/TransformComponent.hpp"
#include "ecs/systems/RenderSystem.hpp"
#include "ecs/systems/InputSystem.hpp"
#include "ecs/systems/RenderSystem.hpp"
#include "ecs/systems/TimeSystem.hpp"
#include "gfx/AssetManager.hpp"
#include "gfx/Render.hpp"

int main() {
  // Assuming the executable is launching from its own directory
  CHDIR("../../..");

  // GLFW init
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

  // Window init
  ivec2 winSize{1600, 900};
  dvec2 winCenter = dvec2(winSize) / 2.;
  GLFWwindow* window = glfwCreateWindow(winSize.x, winSize.y, "MyProgram", NULL, NULL);

  if (!window)
    error("[main.cpp] Failed to create GFLW window");

  glfwMakeContextCurrent(window);
  glfwSetCursorPos(window, winCenter.x, winCenter.y);

  // GLAD init
  if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
    error("[main.cpp] Failed to initialize GLAD");

  // ----- Pre-loop init ----------------------------------------------------------------------------------------------------------- //

  core::EngineContext ctx;
  ctx.window = window;

  gfx::Renderer renderer;
  renderer.init(&ctx);

  gfx::AssetManager assetManager;

  ecs::Registry registry;
  registry.setEngineContext(&ctx);
  registry.setRenderer(&renderer);
  registry.setAssetManager(&assetManager);

  ecs::InputSystem::init(registry);

  assetManager.loadFromObj("res/obj/Cube.obj");
  assetManager.createShader("DefaultCube", {"PTNC.vert", "test.frag"});
  assetManager.createCamera("DefaultCamera", {});

  // ----- Entities ---------------------------------------------------------------------------------------------------------------- //

  ecs::Entity entCube = registry.createEntity();
  {

    MeshComponent meshComponent{
      .mesh = assetManager.getMesh("Cube.obj"),
      .shader = assetManager.getShader("DefaultCube")
    };

    registry.emplace(entCube, meshComponent);
  }

  ecs::Entity entCamera = registry.createEntity();
  {
    CameraComponent camComponent{
      .cam = assetManager.getCamera("DefaultCamera")
    };

    TransformComponent transComponent{
      .pos = {10.f, 10.f, 10.f},
      .rotation = 0.f,
      .scale = 1.f
    };

    registry.emplace(entCamera, camComponent);
    registry.emplace(entCamera, transComponent);
  }

  while (!glfwWindowShouldClose(window)) {
    // ----- Updates ----------------------------------------------------------------------------------------------------------------- //

    ecs::TimeSystem::update(registry);
    ecs::InputSystem::update(registry);

    // ----- Draw -------------------------------------------------------------------------------------------------------------------- //

    ecs::RenderSystem::render(registry);
  }

  glfwTerminate();

  return EXIT_SUCCESS;
}

