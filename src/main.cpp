#include "core/EngineContext.hpp"
#include "core/Light.hpp"
#include "ecs/components/CameraComponent.hpp"
#include "ecs/components/MeshComponent.hpp"
#include "ecs/components/TransformComponent.hpp"
#include "ecs/components/VelocityComponent.hpp"
#include "ecs/components/TextureComponent.hpp"
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
#include "gfx/texture/Texture2D.hpp"
#include "gui/gui.hpp"

int main() {
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

    core::Light globalLight{
      .color = vec3(1.f),
      .direction = glm::normalize(vec3(0.45f, 0.45f, 0.f)),
      .ambient = 0.1f,
      .specular = 0.2f,
    };

    gfx::Renderer renderer;
    renderer.init(&ctx);

    gfx::Shader::setDirectoryLocation("res/shaders");

    gfx::Texture2D debugTex0(
      gfx::image2D("res/tex/debug/uvChecker.jpg"),
      {
        .minFilter = GL_NEAREST,
        .magFilter = GL_NEAREST,
        .wrapS = GL_REPEAT,
        .wrapT = GL_REPEAT,
    });

    gfx::AssetManager assetManager;
    assetManager.loadFromObj("res/obj/Cube.obj");
    assetManager.addShader("DefaultCube", gfx::Shader("PTNC.vert", "test.frag"));
    assetManager.addTexture("DebugTexture0", std::move(debugTex0));
    assetManager.addCamera("DefaultCamera", {});
    assetManager.addLight("GlobalLight", std::move(globalLight));

    registry.ctx().emplace<core::EngineContext>(std::move(ctx));
    registry.ctx().emplace<gfx::Renderer>(std::move(renderer));
    registry.ctx().emplace<gfx::AssetManager>(std::move(assetManager));
  }

  auto& assetManager = registry.ctx().get<gfx::AssetManager>();

  gui::init(window);
  ecs::InputSystem::init(registry);

  // ----- Entities ---------------------------------------------------------------------------------------------------------------- //

  entt::entity entCube = registry.create();
  {
    ecs::component::MeshComponent meshComponent{
      .mesh = assetManager.getMesh("Cube.obj"),
      .shader = assetManager.getShader("DefaultCube")
    };

    ecs::component::TextureComponent textureComponent;
    textureComponent.textures.push_back(assetManager.getTexture("DebugTexture0"));

    registry.emplace<ecs::component::MeshComponent>(entCube, meshComponent);
    registry.emplace<ecs::component::TransformComponent>(entCube, ecs::component::TransformComponent{});
    registry.emplace<ecs::component::TextureComponent>(entCube, textureComponent);
  }

  entt::entity entCamera = registry.create();
  {
    ecs::component::TransformComponent transComponent{
      .pos = {0.f, 0.f, 25.f}
    };

    ecs::component::CameraComponent mainCamComponent{
      .cam = assetManager.getCamera("DefaultCamera"),
      .isActive = true
    };

    ecs::component::VelocityComponent velComponent{
      .scale = 10.f
    };

    registry.emplace<ecs::component::CameraComponent>(entCamera, mainCamComponent);
    registry.emplace<ecs::component::TransformComponent>(entCamera, transComponent);
    registry.emplace<ecs::component::VelocityComponent>(entCamera, velComponent);
  }

  while (!glfwWindowShouldClose(window)) {
    // ----- Updates ----------------------------------------------------------------------------------------------------------------- //

    assetManager.checkShaders();

    if (!ecs::TimeSystem::update(registry))
      continue;

    ecs::InputSystem::update(registry);
    ecs::MovementSystem::update(registry);
    ecs::CameraSystem::update(registry);

    // ----- Render ------------------------------------------------------------------------------------------------------------------ //

    ecs::RenderSystem::render(registry);
    gui::render(registry);

    // ----- Loop end ---------------------------------------------------------------------------------------------------------------- //

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  gui::shutdown();
  glfwTerminate();

  return EXIT_SUCCESS;
}

