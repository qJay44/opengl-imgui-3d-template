#pragma once

#include "mesh/Mesh.hpp"
#include "Shader.hpp"
#include "../core/Camera.hpp"

namespace gfx {

class AssetManager {
public:
  struct ShaderMetadata {
    fspath vsPath{};
    fspath fsPath{};
    fspath tescPath{};
    fspath tesePath{};
    fspath gsPath{};
    fspath compPath{};
  };

  void loadFromObj(fspath filepath, bool printInfo = false);
  void createShader(const std::string& name, ShaderMetadata meta);
  void createCamera(const std::string& name, core::Camera&& camera);

  Mesh* getMesh(const std::string& name) const;
  Shader* getShader(const std::string& name) const;
  core::Camera* getCamera(const std::string& name) const;

private:
  std::unordered_map<std::string, std::unique_ptr<Mesh>> meshes;
  std::unordered_map<std::string, std::unique_ptr<Shader>> shaders;
  std::unordered_map<std::string, std::unique_ptr<core::Camera>> cameras;

private:
  static bool contains(const auto& map, const std::string& name);
};

};

