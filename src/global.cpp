#include "global.hpp"

namespace global {

GLFWwindow* window = nullptr;
ProfilerManager profiler{60};

float dt = 0.f;
float time = 0.f;

bool guiFocused     = false;
bool wireframeMode  = false;
bool drawGlobalAxis = false;

}// global

