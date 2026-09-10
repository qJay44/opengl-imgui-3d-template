#include "core/EngineContext.hpp"
#include "ecs/components/CameraComponent.hpp"
#include "ecs/components/MeshComponent.hpp"
#include "ecs/components/TransformComponent.hpp"
#include "ecs/components/VelocityComponent.hpp"
#include "ecs/systems/CameraSystem.hpp"
#include "ecs/systems/MovementSystem.hpp"
#include "ecs/systems/RenderSystem.hpp"
#include "ecs/systems/InputSystem.hpp"
#include "ecs/systems/RenderSystem.hpp"
#include "ecs/systems/TimeSystem.hpp"
#include "entt/entity/fwd.hpp"
#include "gfx/AssetManager.hpp"
#include "gfx/Render.hpp"
#include "gfx/Shader.hpp"

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
  dvec2 winCenter = winSize / 2;
  GLFWwindow* window = glfwCreateWindow(winSize.x, winSize.y, "MyProgram", NULL, NULL);

  if (!window)
    error("[main.cpp] Failed to create GLFW window");

  glfwMakeContextCurrent(window);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetCursorPos(window, winCenter.x, winCenter.y);

  // GLAD init
  if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
    error("[main.cpp] Failed to initialize GLAD");

  // ----- Pre-loop init ----------------------------------------------------------------------------------------------------------- //

  entt::registry registry;

  // Context objects should be always accessed from [registry]
  {
    core::EngineContext ctx;
    ctx.window = window;

    gfx::Renderer renderer;
    renderer.init(&ctx);

    gfx::Shader::setDirectoryLocation("res/shaders");

    gfx::AssetManager assetManager;
    assetManager.loadFromObj("res/obj/Cube.obj");
    assetManager.createShader("DefaultCube", {"PTNC.vert", "test.frag"});
    assetManager.createCamera("DefaultCamera", {});

    registry.ctx().emplace<core::EngineContext>(std::move(ctx));
    registry.ctx().emplace<gfx::Renderer>(std::move(renderer));
    registry.ctx().emplace<gfx::AssetManager>(std::move(assetManager));
  }

  auto& assetManager = registry.ctx().get<gfx::AssetManager>();

  ecs::InputSystem::init(registry);

  // ----- Entities ---------------------------------------------------------------------------------------------------------------- //

  ecs::component::CameraComponent mainCamComponent{
    .cam = assetManager.getCamera("DefaultCamera"),
    .isActive = true
  };

  entt::entity entCube = registry.create();
  {
    ecs::component::MeshComponent meshComponent{
      .mesh = assetManager.getMesh("Cube.obj"),
      .shader = assetManager.getShader("DefaultCube")
    };

    registry.emplace<ecs::component::MeshComponent>(entCube, meshComponent);
    registry.emplace<ecs::component::CameraComponent>(entCube, mainCamComponent);
    registry.emplace<ecs::component::TransformComponent>(entCube, ecs::component::TransformComponent{});
  }

  entt::entity entCamera = registry.create();
  {
    ecs::component::TransformComponent transComponent{
      .pos = {0.f, 0.f, 25.f}
    };

    registry.emplace<ecs::component::CameraComponent>(entCamera, mainCamComponent);
    registry.emplace<ecs::component::TransformComponent>(entCamera, transComponent);
    registry.emplace<ecs::component::VelocityComponent>(entCamera, ecs::component::VelocityComponent{});
  }

  while (!glfwWindowShouldClose(window)) {
    // ----- Updates ----------------------------------------------------------------------------------------------------------------- //

    ecs::TimeSystem::update(registry);
    ecs::InputSystem::update(registry);
    ecs::MovementSystem::update(registry);
    ecs::CameraSystem::update(registry);

    // ----- Draw -------------------------------------------------------------------------------------------------------------------- //

    ecs::RenderSystem::render(registry);
  }

  glfwTerminate();

  return EXIT_SUCCESS;
}

