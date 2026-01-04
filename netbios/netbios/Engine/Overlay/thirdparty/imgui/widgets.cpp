#include "widgets.h"
#include <map>


int widget::rotation_start_index = 0;
void widget::ImRotateStart()
{
    rotation_start_index = ImGui::GetWindowDrawList()->VtxBuffer.Size;
}
ImVec2 widget::ImRotationCenter()
{
    ImVec2 l(FLT_MAX, FLT_MAX), u(-FLT_MAX, -FLT_MAX); // bounds

    const auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
    for (int i = rotation_start_index; i < buf.Size; i++)
        l = ImMin(l, buf[i].pos), u = ImMax(u, buf[i].pos);

    return ImVec2((l.x + u.x) / 2, (l.y + u.y) / 2); // or use _ClipRectStack?
}
void widget::ImRotateEnd(float rad, ImVec2 center)
{
    float s = sin(rad), c = cos(rad);
    center = ImRotate(center, s, c) - center;

    auto& buf = ImGui::GetWindowDrawList()->VtxBuffer;
    for (int i = rotation_start_index; i < buf.Size; i++)
        buf[i].pos = ImRotate(buf[i].pos, s, c) - center;
}
ImU32 widget::GetColorU32(ImGuiCol idx, float alpha_mul, float alpha)
{
    ImGuiStyle& style = GImGui->Style;
    ImVec4 c = style.Colors[idx];
    if (alpha == 0.0f) {
        c.w *= style.Alpha * alpha_mul;
    }
    else
    {
        c.w = alpha_mul;
    }
    return ImGui::ColorConvertFloat4ToU32(c);
}

void widget::begin_child(const char* name, ImVec2 size, bool is_draw_title) {
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = ImGui::GetCurrentWindow();

    ImVec2 p = window->DC.CursorPos;

    ImGui::BeginChild(std::string(name).append("main").c_str(), size, true, ImGuiWindowFlags_NoScrollbar);
    ImGui::SetCursorPos(SCALE(15, 15));
    if (is_draw_title) {
        ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled), name);
        ImGui::Separator();
        ImGui::BeginChild(name, { size.x, (size.y == 0 || size.y == -1) ? size.y : -1 }, false, ImGuiWindowFlags_NoBackground);
        ImGui::SetCursorPos(SCALE(15, 0));
        ImGui::BeginGroup();
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, SCALE({ 10, 12 }));
    }
    else
    {
        ImGui::BeginChild(name, { size.x, (size.y == 0 || size.y == -1) ? size.y : -1 }, false, ImGuiWindowFlags_NoBackground);
        ImGui::BeginGroup();
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, SCALE({ 0, 12 }));
    }

}
void widget::end_child() {
    ImGui::PopStyleVar();
    ImGui::EndGroup();
    ImGui::EndChild();
    ImGui::EndChild();
}


bool widget::CheckboxWithColor(const char* label, bool* v, float* color)
{
    ImGuiWindow* window = GetCurrentWindow();
    ImGuiContext& g = *GImGui;

    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const ImVec2 pos = window->DC.CursorPos;

    const float w = GetContentRegionAvail().x - SCALE(10);
    const float h = GetFrameHeight();

    const ImRect clickable_rect(pos, pos + ImVec2(w, h));
    const ImRect rect(pos + ImVec2(w - SCALE(40), SCALE(4)), pos + ImVec2(w, h));

    ItemSize(clickable_rect, 0.f);
    if (!ItemAdd(clickable_rect, id)) return false;

    const ImVec2 stored_pos = ImGui::GetCursorScreenPos();


    ImVec2 color_pos = rect.Min - ImVec2(h + SCALE(15), 0);
    ImGui::SetCursorScreenPos(color_pos);
    {
        ImGui::BeginChild(std::string(label).append("checkbox.main").c_str(), ImVec2(h, h), false);
        ImGui::SetCursorPos(ImVec2(h + SCALE(8), 0));;
        ImGui::ColorEdit3(std::string(label).append("checkbox.color_").append(std::to_string(id)).c_str(), color, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
        ImGui::EndChild();
    }
    ImGui::SetCursorScreenPos(stored_pos);



    bool hovered, held, pressed = ButtonBehavior(clickable_rect, id, &hovered, &held);

    float anim = ImTricks::Animations::FastFloatLerp(std::string(label).append("checkbox.anim"), *v, 0.f, 1.f, 0.04f);
    ImColor text_colored = ImTricks::Animations::FastColorLerp(GetColorU32(ImGuiCol_TextDisabled), GetColorU32(ImGuiCol_Text), anim);
    ImColor background = ImTricks::Animations::FastColorLerp(GetColorU32(ImGuiCol_CheckboxBg), GetColorU32(ImGuiCol_CheckboxBgActive), anim);
    ImColor circle = ImTricks::Animations::FastColorLerp(GetColorU32(ImGuiCol_CheckboxCircle), GetColorU32(ImGuiCol_CheckboxCircleActive), anim);
    float circle_offset = 12.0f + anim * (28.0f - 12.0f);

    if (pressed)
    {
        *v = !(*v);
        MarkItemEdited(id);
    }
    PushStyleColor(ImGuiCol_Text, text_colored.Value);
    RenderText({ clickable_rect.Min.x, clickable_rect.Max.y - label_size.y }, label);
    PopStyleColor();

    window->DrawList->AddRectFilled(rect.Min, rect.Max, background, ImDrawFlags_RoundCornersAll);
    window->DrawList->AddCircleFilled({ rect.Min.x + SCALE(circle_offset), rect.GetCenter().y }, SCALE(6.f), circle);


    return pressed;
}

bool widget::CheckboxWithCombo(const char* label, bool* v, int* current_item, const char* items_separated_by_zeros) {

    ImGuiWindow* window = GetCurrentWindow();
    ImGuiContext& g = *GImGui;

    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);

    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const ImVec2 pos = window->DC.CursorPos;

    const float w = GetContentRegionAvail().x - SCALE(10);
    const float h = GetFrameHeight();

    const ImRect clickable_rect(pos, pos + ImVec2(w, h));
    const ImRect rect(pos + ImVec2(w - SCALE(40), SCALE(4)), pos + ImVec2(w, h));

    ItemSize(clickable_rect, 0.f);
    if (!ItemAdd(clickable_rect, id)) return false;

    const ImVec2 stored_pos = ImGui::GetCursorScreenPos();


    ImVec2 color_pos = rect.Min - ImVec2(SCALE(75) + SCALE(15), 0);
    ImGui::SetCursorScreenPos(color_pos);
    {
        ImGui::BeginChild(std::string(label).append("checkbox.main").c_str(), ImVec2(SCALE(75), SCALE(50)), false);
        ImGui::SetCursorPos(ImVec2(h + SCALE(8), 0));
        ImGui::Combo(std::string("##").append("checkbox.color_").append(std::to_string(id)).c_str(), current_item, items_separated_by_zeros);
        ImGui::EndChild();
    }
    ImGui::SetCursorScreenPos(stored_pos);



    bool hovered, held, pressed = ButtonBehavior(clickable_rect, id, &hovered, &held);

    float anim = ImTricks::Animations::FastFloatLerp(std::string(label).append("checkbox.anim"), *v, 0.f, 1.f, 0.04f);
    ImColor text_colored = ImTricks::Animations::FastColorLerp(GetColorU32(ImGuiCol_TextDisabled), GetColorU32(ImGuiCol_Text), anim);
    ImColor background = ImTricks::Animations::FastColorLerp(GetColorU32(ImGuiCol_CheckboxBg), GetColorU32(ImGuiCol_CheckboxBgActive), anim);
    ImColor circle = ImTricks::Animations::FastColorLerp(GetColorU32(ImGuiCol_CheckboxCircle), GetColorU32(ImGuiCol_CheckboxCircleActive), anim);
    float circle_offset = 12.0f + anim * (28.0f - 12.0f);

    if (pressed)
    {
        *v = !(*v);
        MarkItemEdited(id);
    }
    PushStyleColor(ImGuiCol_Text, text_colored.Value);
    RenderText({ clickable_rect.Min.x, clickable_rect.Max.y - label_size.y }, label);
    PopStyleColor();

    window->DrawList->AddRectFilled(rect.Min, rect.Max, background, ImDrawFlags_RoundCornersAll);
    window->DrawList->AddCircleFilled({ rect.Min.x + SCALE(circle_offset), rect.GetCenter().y }, SCALE(6.f), circle);


    return pressed;


}

bool widget::RenderTab(ImFont* Font, const char* label, int& index) {
    bool changed = false;
    const char* currentLabel = label;
    std::vector<const char*> labels;

    // 解析所有标签
    while (*currentLabel) {
        labels.push_back(currentLabel);
        while (*currentLabel) currentLabel++;
        currentLabel++;
    }

    int buttonCount = labels.size();
    if (buttonCount == 0) return false;

    // 确保索引有效
    if (index < 0 || index >= buttonCount) index = 0;

    // 获取当前窗口信息
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    if (!window) return false;

    const ImGuiStyle& style = ImGui::GetStyle();
    ImDrawList* drawList = ImGui::GetWindowDrawList();

    // 基础按钮尺寸（将通过SCALE宏进行缩放）
    const float BASE_BUTTON_WIDTH = 60.0f;    // 基础按钮宽度
    const float BASE_BUTTON_HEIGHT = 50.0f;   // 基础按钮高度
    const float BASE_SPACING = 10.0f;         // 基础按钮间距
    const float BASE_ROUNDING = 6.0f;         // 基础圆角半径
    const float BASE_BORDER_WIDTH = 1.2f;     // 基础边框宽度
    //const ImVec2 BASE_SHADOW_OFFSET = ImVec2(1.0f, 2.0f); // 基础阴影偏移

    // 应用DPI缩放
    const float BUTTON_WIDTH = SCALE(BASE_BUTTON_WIDTH);
    const float BUTTON_HEIGHT = SCALE(BASE_BUTTON_HEIGHT);
    const float SPACING = SCALE(BASE_SPACING);
    const float ROUNDING = SCALE(BASE_ROUNDING);
    const float BORDER_WIDTH = SCALE(BASE_BORDER_WIDTH);
    //const ImVec2 SHADOW_OFFSET = SCALE(BASE_SHADOW_OFFSET);

    // 定义配色方案
    const ImVec4 COLOR_NORMAL_BG = ImVec4(0.85f, 0.85f, 0.85f, 0.9f);   // 银白色（正常状态背景）
    const ImVec4 COLOR_HOVER_BG = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);      // 亮银白色（悬停状态背景）
    const ImVec4 COLOR_SELECTED_BG = ImVec4(0.00f, 0.47f, 1.00f, 1.0f); // 指定蓝色（选中状态背景）


    // 文字颜色方案
    const ImVec4 COLOR_TEXT_SELECTED = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  // 白色（选中状态文字）
    //const ImVec4 COLOR_TEXT_NORMAL = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);    // 深灰色（正常状态文字）
    const ImVec4 COLOR_TEXT_NORMAL = ImVec4(0.1f, 0.1f, 0.1f, 0.5f);    // 深灰色（正常状态文字）
    const ImVec4 COLOR_TEXT_HOVER = ImVec4(0.1f, 0.1f, 0.1f, 0.8f);     // 近黑色（悬停状态文字）

    const ImVec4 COLOR_SHADOW = ImVec4(0.0f, 0.0f, 0.0f, 0.2f);         // 阴影颜色

    // 计算总宽度并居中对齐（已缩放）
    float totalWidth = (BUTTON_WIDTH * buttonCount) + (SPACING * (buttonCount - 1));
    float startX = (window->ContentRegionRect.Max.x - window->ContentRegionRect.Min.x - totalWidth) * 0.5f
        + window->ContentRegionRect.Min.x;

    // 底部位置计算（考虑内边距）
    float bottomY = window->ContentRegionRect.Max.y - BUTTON_HEIGHT - style.WindowPadding.y;

    // 保存光标位置
    ImVec2 cursorPos = ImGui::GetCursorPos();

    // 绘制每个自定义按钮
    for (int i = 0; i < buttonCount; i++) {
        // 计算按钮位置（已缩放）
        float x = startX + i * (BUTTON_WIDTH + SPACING);
        float y = bottomY;
        ImRect buttonRect(ImVec2(x, y), ImVec2(x + BUTTON_WIDTH, y + BUTTON_HEIGHT));

        // 检查鼠标交互
        bool isHovered = ImGui::IsMouseHoveringRect(buttonRect.Min, buttonRect.Max);
        bool isClicked = isHovered && ImGui::IsMouseClicked(ImGuiMouseButton_Left);

        // 处理点击事件
        if (isClicked) {
            if (index != i) {
                index = i;
                changed = true;
            }
            ImGui::MarkItemEdited(window->ID); // 捕获点击，防止穿透
        }

        // 根据状态选择颜色
        ImVec4 bgColor, textColor;
        if (index == i) {
            // 选中状态
            bgColor = COLOR_SELECTED_BG;
            textColor = COLOR_TEXT_SELECTED; // 选中状态用白色文字
        }
        else if (isHovered) {
            // 悬停状态
            bgColor = COLOR_HOVER_BG;
            textColor = COLOR_TEXT_HOVER; // 悬停状态用近黑色文字
        }
        else {
            // 正常状态
            bgColor = COLOR_NORMAL_BG;
            textColor = COLOR_TEXT_NORMAL; // 正常状态用深灰色文字
        }

        // 绘制按钮背景
        drawList->AddRectFilled(buttonRect.Min, buttonRect.Max, ImGui::GetColorU32(bgColor), ROUNDING);

        // 绘制按钮文本（居中）
        ImVec2 textSize = Font->CalcTextSizeA(Font->FontSize * style._MainScale, FLT_MAX, 0, labels[i]);

        ImVec2 textPos;

        textPos.x = buttonRect.Min.x + (BUTTON_WIDTH - textSize.x) * 0.5f;
        textPos.y = buttonRect.Min.y + (BUTTON_HEIGHT - textSize.y) * 0.5f;

        drawList->AddText(Font, Font->FontSize * style._MainScale, textPos, ImGui::GetColorU32(textColor), labels[i]);
    }

    // 恢复光标位置
    ImGui::SetCursorPos(cursorPos);

    return changed;
}
