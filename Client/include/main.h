#pragma once
#ifndef MAIN_H
#define MAIN_H

#include "Client.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ClientGame.h"
#include "Audio/include/AudioEngine.h"

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>

#endif // !MAIN_H
