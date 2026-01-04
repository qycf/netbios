#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_tricks.hpp"

#define SCALE(...) scale_impl(__VA_ARGS__, ImGui::GetStyle()._MainScale)
inline ImVec2 scale_impl(const ImVec2& vec, float dpi) {
    return ImVec2(roundf(vec.x * dpi), roundf(vec.y * dpi));
}
inline ImVec2 scale_impl(float x, float y, float dpi) {
    return ImVec2(roundf(x * dpi), roundf(y * dpi));
}
inline float scale_impl(float var, float dpi) {
    return roundf(var * dpi);
}

namespace widget {
    using namespace ImGui;

    extern int rotation_start_index;

    extern void ImRotateStart();
    extern ImVec2 ImRotationCenter();
    extern void ImRotateEnd(float rad, ImVec2 center = ImRotationCenter());
    extern ImU32 GetColorU32(ImGuiCol idx, float alpha_mul = 1.0f, float alpha = 0.0f);
    extern void begin_child(const char* name, ImVec2 size, bool is_draw_title = true);
    extern void end_child();
    extern bool RenderTab(ImFont* Font, const char* label, int& index);

    extern bool CheckboxWithColor(const char* label, bool* v, float* color);
    extern bool CheckboxWithCombo(const char* label, bool* v, int* current_item, const char* items_separated_by_zeros);
}
