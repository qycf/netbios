#include "gui.h"
#include "Overlay.h"
c_gui gui;


void c_gui::render() {
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;

	/* ---------- 粉白配色方案 ---------- */
	ImVec4
		// 背景（保持原透明度）
		colWinBg = ImVec4(1.00f, 1.00f, 1.00f, 0.95f),   // 主窗口 纯白
		colChildBg = ImVec4(1.00f, 0.96f, 0.98f, 0.00f),   // Child 透明
		colPopupBg = ImVec4(1.00f, 0.94f, 0.97f, 0.98f),   // 弹出底

		// 文字
		colText = ImVec4(0.85f, 0.20f, 0.45f, 1.00f), // 主文字 玫粉
		colTextDis = ImVec4(0.75f, 0.55f, 0.65f, 1.00f), // 副文字 灰粉

		// 控件背景
		colFrameBg = ImVec4(1.00f, 0.92f, 0.96f, 1.00f), // 输入框 淡粉
		colFrameBgHov = ImVec4(1.00f, 0.88f, 0.94f, 1.00f),
		colFrameBgAct = ImVec4(1.00f, 0.82f, 0.90f, 1.00f),

		// 按钮
		colButton = ImVec4(1.00f, 0.52f, 0.82f, 1.00f), // #FF69B4 热粉
		colButtonHov = ImVec4(1.00f, 0.62f, 0.88f, 1.00f),
		colButtonAct = ImVec4(0.96f, 0.42f, 0.72f, 1.00f),

		// Header
		colHeader = ImVec4(1.00f, 0.70f, 0.90f, 0.60f),
		colHeaderHov = ImVec4(1.00f, 0.78f, 0.94f, 0.80f),
		colHeaderAct = ImVec4(1.00f, 0.58f, 0.82f, 1.00f),

		// Slider / Scrollbar
		colSliderGrab = ImVec4(1.00f, 0.58f, 0.88f, 1.00f),
		colSliderGrabA = ImVec4(1.00f, 0.68f, 0.92f, 1.00f),
		colScrollbarBg = ImVec4(1.00f, 0.94f, 0.98f, 1.00f),
		colScrollbarGr = ImVec4(0.96f, 0.68f, 0.84f, 1.00f),
		colScrollbarGrH = ImVec4(0.98f, 0.76f, 0.88f, 1.00f),
		colScrollbarGrA = ImVec4(1.00f, 0.82f, 0.92f, 1.00f),

		// CheckMark
		colCheckMark = ImVec4(1.00f, 0.58f, 0.88f, 1.00f),

		// Separator
		colSep = ImVec4(1.00f, 0.82f, 0.94f, 1.00f),
		colSepHov = ImVec4(1.00f, 0.58f, 0.88f, 1.00f),
		colSepAct = ImVec4(1.00f, 0.68f, 0.92f, 1.00f),

		// Border
		colBorder = ImVec4(1.00f, 0.58f, 0.88f, 0.50f);

	/* ---------- 应用到 ImGui 样式 ---------- */
	colors[ImGuiCol_WindowBg] = colWinBg;
	colors[ImGuiCol_ChildBg] = colChildBg;
	colors[ImGuiCol_PopupBg] = colPopupBg;
	colors[ImGuiCol_Text] = colText;
	colors[ImGuiCol_TextDisabled] = colTextDis;
	colors[ImGuiCol_FrameBg] = colFrameBg;
	colors[ImGuiCol_FrameBgHovered] = colFrameBgHov;
	colors[ImGuiCol_FrameBgActive] = colFrameBgAct;
	colors[ImGuiCol_Button] = colButton;
	colors[ImGuiCol_ButtonHovered] = colButtonHov;
	colors[ImGuiCol_ButtonActive] = colButtonAct;
	colors[ImGuiCol_Header] = colHeader;
	colors[ImGuiCol_HeaderHovered] = colHeaderHov;
	colors[ImGuiCol_HeaderActive] = colHeaderAct;
	colors[ImGuiCol_SliderGrab] = colSliderGrab;
	colors[ImGuiCol_SliderGrabActive] = colSliderGrabA;
	colors[ImGuiCol_ScrollbarBg] = colScrollbarBg;
	colors[ImGuiCol_ScrollbarGrab] = colScrollbarGr;
	colors[ImGuiCol_ScrollbarGrabHovered] = colScrollbarGrH;
	colors[ImGuiCol_ScrollbarGrabActive] = colScrollbarGrA;
	colors[ImGuiCol_CheckMark] = colCheckMark;
	colors[ImGuiCol_Separator] = colSep;
	colors[ImGuiCol_SeparatorHovered] = colSepHov;
	colors[ImGuiCol_SeparatorActive] = colSepAct;
	colors[ImGuiCol_Border] = colBorder;
	colors[ImGuiCol_BorderShadow] = ImVec4(0, 0, 0, 0);   // 去掉阴影

	ImGui::SetNextWindowSize(SCALE(ImVec2(1176, 700)));
	ImGui::Begin("siuhwhbbigo", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings);
	//绘制标题
	ImGui::SetCursorPos(SCALE(20, 8));
	ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled), u8"KY 内部  -v 1.6                         |                         Ins键 显/隐菜单");
	//主窗口内容区域
	ImGui::BeginChild("MiddleWindow", ImVec2(0, ImGui::GetContentRegionAvail().y - SCALE(60) - style.ItemSpacing.y), false, ImGuiWindowFlags_NoBackground);
	if (set->main_tab == 0) {
		this->Aim();
	}
	else if (set->main_tab == 1) {
		this->Pawn();
	}
	else if (set->main_tab == 2) {

		this->Item();
	}
	else if (set->main_tab == 3) {

		this->Map();
	}
	if (set->main_tab == 4) {
		this->Settting();
	}
	ImGui::EndChild();

	//底部选择栏 自瞄 视觉 物品 雷达 设置
	ImGui::BeginChild("BottomTabBar", SCALE(ImVec2(-1, 60)), false, ImGuiWindowFlags_NoBackground);
	widget::RenderTab(Overlay::IconFont, u8"A\0B\0C\0D\0E\0", set->main_tab);
	ImGui::EndChild();
	ImGui::End();

}

void c_gui::Aim() {
	ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
	ImGui::Columns(3, nullptr, false);
	ImGui::NextColumn();
	ImGui::NextColumn();
	ImGui::BeginChild("目标设置##11", ImVec2(-1, 0), false, ImGuiWindowFlags_NoBackground);
	{
		widget::begin_child(u8"Aiming", SCALE(0, 50), false);
		ImGui::Combo(u8"当前配置", &set->aim_tab, u8"AimCFG\0\0");
		widget::end_child();
	}
	ImGui::EndChild();
	ImGui::NextColumn();
	ImGui::Columns(1, nullptr, false);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - SCALE(8));
	ImGui::BeginChild(u8"Aim", ImVec2(-1, -1), false, ImGuiWindowFlags_NoBackground);
	{
		ImGui::Columns(3, nullptr, false);
		widget::begin_child(u8"基础设置", ImVec2(0, -1), true);
		{
			ImGui::Checkbox(u8"启用", &set->Aimbot.Enable);
			ImGui::Combo(u8"当前模式", &set->Aimbot.AimType, u8"磁吸\0\0");
			//MB1 1 MB2 2 MB4 4 MB5 5
			ImGui::Combo(u8"自瞄热键", &set->Aimbot.HotKey, u8"MB1\0MB2\0MB4\0MB5\0\0");
			ImGui::Combo(u8"自瞄", &set->Aimbot.AimPos, u8"随机\0头部\0胸部\0小头\0腿部\0\0");
			ImGui::Combo(u8"强锁部位热键", &set->Aimbot.HotKeyTwo, u8"关闭\0Shift\0Ctril\0\0");
			ImGui::Combo(u8"强选位置", &set->Aimbot.AimPosTwo, u8"随机\0头部\0胸部\0小头\0腿部\0\0");

			ImGui::Checkbox(u8"倒地不瞄", &set->Aimbot.NotHealthZero);
			ImGui::Checkbox(u8"玩家", &set->Aimbot.AimPlayer);
			ImGui::Checkbox(u8"Boss", &set->Aimbot.AimBoss);
			ImGui::Checkbox(u8"精英", &set->Aimbot.AimElite);
			ImGui::Checkbox(u8"人机", &set->Aimbot.AimRobot);

			ImGui::SliderFloat(u8"最大瞄准距离", &set->Aimbot.maxDistance, 1, 500, "%.fm");
		}
		widget::end_child();
		ImGui::NextColumn();
		widget::begin_child(u8"参数设置", ImVec2(0, -1), true);
		{
			ImGui::Checkbox(u8"显示目标", &set->Aimbot.showTarget);
			widget::CheckboxWithColor(u8"显示范围", &set->Aimbot.showAimFov, (float*)&set->Aimbot.AimFovColor);
			ImGui::SliderFloat(u8"瞄准范围", &set->Aimbot.AimFov, 1, 1000, "%.f");
			ImGui::SliderFloat(u8"X平滑速度", &set->Aimbot.SmoothFactorX, 0.01f, 1.0f, "%.2f");
			ImGui::SliderFloat(u8"Y平滑速度", &set->Aimbot.SmoothFactorY, 0.01f, 1.0f, "%.2f");
		}
		widget::end_child();
		ImGui::NextColumn();
		/*widget::begin_child(u8"功能设置", ImVec2(0, -1), true);
		{
			ImGui::Checkbox(u8"子弹散射", &set->Aimbot.BulletSpread);
		}
		widget::end_child();
		ImGui::NextColumn();*/
		ImGui::Columns(1, nullptr, false);
	}
	ImGui::EndChild();
	ImGui::PopStyleColor();
}
void c_gui::Pawn() {

	auto GetPawnSettingByIndex = [](int& Index)->c_set::c_Pawn* {
		switch (Index) {
		case 0:
			return &set->Player;
		case 1:
			return &set->Boss;
		case 2:
			return &set->Elite;
		case 3:
			return &set->Robot;
		default:
			Index = 0;
			return &set->Player;
		}
		return nullptr;
		};
	c_set::c_Pawn* Pawn = GetPawnSettingByIndex(set->pawn_tab);

	ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
	ImGui::Columns(3, nullptr, false);
	ImGui::NextColumn();
	ImGui::NextColumn();
	ImGui::BeginChild("目标设置##11", ImVec2(-1, 0), false, ImGuiWindowFlags_NoBackground);
	{
		widget::begin_child(u8"目标设置", SCALE(0, 50), false);
		ImGui::Combo(u8"目标类型", &set->pawn_tab, u8"玩家\0Boss\0精英\0人机\0\0");
		widget::end_child();
	}
	ImGui::EndChild();
	ImGui::NextColumn();
	ImGui::Columns(1, nullptr, false);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - SCALE(8));
	ImGui::BeginChild(u8"任务", ImVec2(-1, -1), false, ImGuiWindowFlags_NoBackground);
	{
		ImGui::Columns(3, nullptr, false);

		widget::begin_child(u8"基础设置", ImVec2(0, -1), true);
		{
			ImGui::Checkbox(u8"启用##", &Pawn->Enable);
			ImGui::Checkbox(u8"骨骼##", &Pawn->showSkeleton);
			ImGui::Checkbox(u8"方框##", &Pawn->showBox);
			if (Pawn == &set->Player)
				ImGui::Checkbox(u8"团队##", &Pawn->showTeamId);
			ImGui::Checkbox(u8"名字##", &Pawn->showName);
			if (Pawn == &set->Player)
				ImGui::Checkbox(u8"英雄", &Pawn->showHero);
			ImGui::Checkbox(u8"头甲", &Pawn->showHelmet);
			ImGui::Checkbox(u8"武器", &Pawn->showWeapon);
			ImGui::Checkbox(u8"血量", &Pawn->showHealth);
			ImGui::Checkbox(u8"距离", &Pawn->showDistance);
			ImGui::SliderFloat(u8"透视距离", &Pawn->maxDistance, 1, 500, "%.fm");
		}
		widget::end_child();

		ImGui::NextColumn();
		widget::begin_child(u8"颜色设置", SCALE(0, 300), true);
		{
			ImGui::ColorEdit3(u8"骨骼", (float*)&Pawn->SkeletonColor.Value, ImGuiColorEditFlags_NoInputs);
			ImGui::ColorEdit3(u8"方框", (float*)&Pawn->BoxColor.Value, ImGuiColorEditFlags_NoInputs);
			ImGui::ColorEdit3(u8"名字", (float*)&Pawn->NameColor.Value, ImGuiColorEditFlags_NoInputs);
			if (Pawn == &set->Player)
				ImGui::ColorEdit3(u8"英雄", (float*)&Pawn->HeroColor.Value, ImGuiColorEditFlags_NoInputs);
			ImGui::ColorEdit3(u8"武器", (float*)&Pawn->WeaponColor.Value, ImGuiColorEditFlags_NoInputs);
			ImGui::ColorEdit3(u8"距离", (float*)&Pawn->DistanceColor.Value, ImGuiColorEditFlags_NoInputs);
		}
		widget::end_child();
		widget::begin_child(u8"不可见颜色设置", { 0,-1 }, true);
		{
			ImGui::ColorEdit3(u8"骨骼", (float*)&Pawn->SkeletonInvisibleColor.Value, ImGuiColorEditFlags_NoInputs);
			ImGui::ColorEdit3(u8"方框", (float*)&Pawn->BoxInvisibleColor.Value, ImGuiColorEditFlags_NoInputs);
		}
		widget::end_child();
		ImGui::NextColumn();
		widget::begin_child(u8"等级设置", { 0,SCALE(266) }, true);
		{
			ImGui::Combo(u8"头盔(H)最小等级", &Pawn->miniHelmetLevel, u8"关闭\0Lv1\0Lv2\0Lv3\0\Lv4\0Lv5\0Lv6\0\0");
			ImGui::Combo(u8"护甲(B)最小等级", &Pawn->miniBreastPlateLevel, u8"关闭\0Lv1\0Lv2\0Lv3\0\Lv4\0Lv5\0Lv6\0\0");
			ImGui::Combo(u8"胸挂(C)最小等级", &Pawn->miniChestHangingLevel, u8"关闭\0Lv1\0Lv2\0Lv3\0\Lv4\0Lv5\0Lv6\0\0");
			ImGui::Combo(u8"背包(G)最小等级", &Pawn->miniBagLevel, u8"关闭\0Lv1\0Lv2\0Lv3\0\Lv4\0Lv5\0Lv6\0\0");
		}
		widget::end_child();
		widget::begin_child(u8"参数设置", { 0,-1 }, true);
		{
			ImGui::SliderFloat(u8"方框粗细", &Pawn->BoxLine, 0.5f, 5.0f, "%.1f");
			ImGui::SliderFloat(u8"骨骼粗细", &Pawn->SkeletonLine, 0.5f, 5.0f, "%.1f");
			ImGui::SliderFloat(u8"字体大小", &Pawn->fontSize, 12.0f, 32.0f, "%.f");
		}

		widget::end_child();
		ImGui::NextColumn();
		ImGui::Columns(1, nullptr, false);
	}
	ImGui::EndChild();
	ImGui::PopStyleColor();



}

void c_gui::Item() {
	auto FormatWithThousandsSeparator = [](float value) ->std::string {
		// 转换为整数（忽略小数，因为你的场景是整数金额）
		long long num = static_cast<long long>(value);
		std::string str = std::to_string(num);
		std::string result;

		// 从后往前每3位插入一个逗号
		int count = 0;
		for (auto it = str.rbegin(); it != str.rend(); ++it) {
			if (count > 0 && count % 3 == 0) {
				result += ',';
			}
			result += *it;
			count++;
		}

		// 反转字符串恢复正确顺序，并添加美元符号
		std::reverse(result.begin(), result.end());
		return "$" + result;
		};

	ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
	ImGui::Columns(3, nullptr, false);
	ImGui::NextColumn();
	ImGui::NextColumn();
	ImGui::BeginChild("资源类型##11", ImVec2(-1, 0), false, ImGuiWindowFlags_NoBackground);
	{
		widget::begin_child(u8"资源类型", SCALE(0, 50), false);
		ImGui::Combo(u8"资源类型", &set->item_tab, u8"物资\0容器\0盒子\0其它\0\0");
		widget::end_child();
	}
	ImGui::EndChild();
	ImGui::NextColumn();
	ImGui::Columns(1, nullptr, false);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - SCALE(8));
	ImGui::BeginChild(u8"资源", ImVec2(-1, -1), false, ImGuiWindowFlags_NoBackground);
	{
		//物资
		if (set->item_tab == 0) {
			ImGui::Columns(3, nullptr, false);
			widget::begin_child(u8"基础设置", ImVec2(0, -1), true); {
				ImGui::Checkbox(u8"启用", &set->Item.LootingItem.Enable);
				ImGui::Checkbox(u8"白色物品", &set->Item.LootingItem.showLv1);
				ImGui::Checkbox(u8"绿色物品", &set->Item.LootingItem.showLv2);
				ImGui::Checkbox(u8"蓝色物品", &set->Item.LootingItem.showLv3);
				ImGui::Checkbox(u8"紫色物品", &set->Item.LootingItem.showLv4);
				ImGui::Checkbox(u8"金色物品", &set->Item.LootingItem.showLv5);
				ImGui::Checkbox(u8"红色物品", &set->Item.LootingItem.showLv6);
				ImGui::SliderFloat(u8"透视距离", &set->Item.LootingItem.maxDistance, 1, 300, "%.fm");
			}
			widget::end_child();
			ImGui::NextColumn();
			widget::begin_child(u8"过滤设置", ImVec2(0, -1), true); {
				ImGui::Checkbox(u8"价值过滤", &set->Item.LootingItem.PriceFiltering);
				ImGui::SliderFloat(u8"最低价值", &set->Item.LootingItem.minGuidePrice, 1000, 500000, FormatWithThousandsSeparator(set->Item.LootingItem.minGuidePrice).c_str());
			}
			widget::end_child();
			ImGui::NextColumn();
			widget::begin_child(u8"参数设置", ImVec2(0, -1), true); {
				ImGui::SliderFloat(u8"字体大小", &set->Item.LootingItem.fontSize, 12, 32, "%.f");
			}
			widget::end_child();
			ImGui::NextColumn();
			ImGui::Columns(1, nullptr, false);
		}

		//容器
		if (set->item_tab == 1) {
			ImGui::Columns(4, nullptr, false);
			widget::begin_child(u8"基础设置", ImVec2(0, -1), true); {
				ImGui::Checkbox(u8"启用", &set->Item.OpenBox.Enable);
				ImGui::Checkbox(u8"容器物资", &set->Item.OpenBox.showPickup);
				ImGui::Checkbox(u8"过滤打开", &set->Item.OpenBox.openFiltering);
				widget::CheckboxWithColor(u8"普通容器", &set->Item.OpenBox.showOpenBox[0], (float*)&set->Item.OpenBox.OpenBoxColor[0]);
				widget::CheckboxWithColor(u8"中级容器", &set->Item.OpenBox.showOpenBox[1], (float*)&set->Item.OpenBox.OpenBoxColor[1]);
				widget::CheckboxWithColor(u8"高级容器", &set->Item.OpenBox.showOpenBox[2], (float*)&set->Item.OpenBox.OpenBoxColor[2]);
				ImGui::SliderFloat(u8"透视距离", &set->Item.OpenBox.maxDistance, 1, 300, "%.fm");
				ImGui::SliderFloat(u8"字体大小", &set->Item.OpenBox.fontSize, 12, 32, "%.f");
			}
			widget::end_child();
			ImGui::NextColumn();
			widget::begin_child(u8"高级容器", ImVec2(0, -1), true);
			{
				for (auto& Item : set->OpenBoxItems_Cache)
				{
					if (Item.second.second == 2) {
						widget::CheckboxWithCombo(Item.first.c_str(), &Item.second.first, &Item.second.second, u8"普通容器\0中级容器\0高级容器\0\0");
					}
				}
			}
			widget::end_child();
			ImGui::NextColumn();
			widget::begin_child(u8"中级容器", ImVec2(0, -1), true);
			{
				for (auto& Item : set->OpenBoxItems_Cache)
				{
					if (Item.second.second == 1) {
						widget::CheckboxWithCombo(Item.first.c_str(), &Item.second.first, &Item.second.second, u8"普通容器\0中级容器\0高级容器\0\0");
					}
				}
			}
			widget::end_child();
			ImGui::NextColumn();
			widget::begin_child(u8"普通容器", ImVec2(0, -1), true);
			{
				for (auto& Item : set->OpenBoxItems_Cache)
				{
					if (Item.second.second == 0) {
						widget::CheckboxWithCombo(Item.first.c_str(), &Item.second.first, &Item.second.second, u8"普通容器\0中级容器\0高级容器\0\0");
					}
				}
			}
			widget::end_child();
			ImGui::NextColumn();
			ImGui::Columns(1, nullptr, false);
		}

		//尸体
		if (set->item_tab == 2) {
			ImGui::Columns(3, nullptr, false);
			widget::begin_child(u8"基础设置", ImVec2(0, -1), true); {
				ImGui::Checkbox(u8"启用", &set->Item.DeadBody.Enable);
				ImGui::Checkbox(u8"盒子物资", &set->Item.DeadBody.showPickup);
				ImGui::Checkbox(u8"过滤打开", &set->Item.DeadBody.openFiltering);
				widget::CheckboxWithColor(u8"玩家盒子", &set->Item.DeadBody.showPlayerBox, (float*)&set->Item.DeadBody.PlayerBoxColor);
				widget::CheckboxWithColor(u8"首领盒子", &set->Item.DeadBody.showBossBox, (float*)&set->Item.DeadBody.BossBoxColor);
				widget::CheckboxWithColor(u8"精英盒子", &set->Item.DeadBody.showEliteBox, (float*)&set->Item.DeadBody.EliteBoxColor);
				widget::CheckboxWithColor(u8"人机盒子", &set->Item.DeadBody.showRobotBox, (float*)&set->Item.DeadBody.RobotBoxColor);
			}
			widget::end_child();
			ImGui::NextColumn();
			widget::begin_child(u8"距离设置", ImVec2(0, -1), true); {
				ImGui::SliderFloat(u8"玩家盒子距离", &set->Item.DeadBody.PlayerBoxDistance, 1, 300, "%.f");
				ImGui::SliderFloat(u8"首领盒子距离", &set->Item.DeadBody.BossBoxDistance, 1, 300, "%.f");
				ImGui::SliderFloat(u8"精英盒子距离", &set->Item.DeadBody.EliteBoxDistance, 1, 300, "%.f");
				ImGui::SliderFloat(u8"人机盒子距离", &set->Item.DeadBody.RobotBoxDistance, 1, 300, "%.f");
			}
			widget::end_child();
			ImGui::NextColumn();
			widget::begin_child(u8"参数设置", ImVec2(0, -1), true); {
				ImGui::SliderFloat(u8"字体大小", &set->Item.DeadBody.fontSize, 12, 32, "%.f");
			}
			widget::end_child();
			ImGui::NextColumn();
			ImGui::Columns(1, nullptr, false);
		}


		//其它
		if (set->item_tab == 3) {
			ImGui::Columns(4, nullptr, false);
			widget::begin_child(u8"核客电脑", ImVec2(0, SCALE(300)), true); {
				widget::CheckboxWithColor(u8"启用##04", &set->Item.HackPC.Enable, (float*)&set->Item.HackPC.color);
				ImGui::SliderFloat(u8"字体大小", &set->Item.HackPC.fontSize, 12.0f, 32.0f, "%.f", ImGuiSliderFlags_NoInput);
				ImGui::SliderFloat(u8"透视距离", &set->Item.HackPC.maxDistance, 1.0f, 500.0f, "%.f", ImGuiSliderFlags_NoInput);
			}
			widget::end_child();
			widget::begin_child(u8"大地图配置##电脑", ImVec2(0, -1), true); {
				ImGui::SliderFloat(u8"字体大小", &set->Item.HackPC.BigMapfontSize, 12.0f, 32.0f, "%.f", ImGuiSliderFlags_NoInput);
				ImGui::SliderFloat(u8"透视距离", &set->Item.HackPC.BigMapMaxDistance, 1.0f, 800.0f, "%.f", ImGuiSliderFlags_NoInput);
			}
			widget::end_child();

			ImGui::NextColumn();
			widget::begin_child(u8"密码门", ImVec2(0, SCALE(300)), true); {
				widget::CheckboxWithColor(u8"启用##01", &set->Item.CodeLock.Enable, (float*)&set->Item.CodeLock.color);
				ImGui::SliderFloat(u8"字体大小", &set->Item.CodeLock.fontSize, 12.0f, 32.0f, "%.f", ImGuiSliderFlags_NoInput);
				ImGui::SliderFloat(u8"透视距离", &set->Item.CodeLock.maxDistance, 1.0f, 500.0f, "%.f", ImGuiSliderFlags_NoInput);

			}
			widget::end_child();
			widget::begin_child(u8"大地图配置##密码门", ImVec2(0, -1), true); {
				ImGui::SliderFloat(u8"字体大小", &set->Item.CodeLock.BigMapfontSize, 12.0f, 32.0f, "%.f", ImGuiSliderFlags_NoInput);
				ImGui::SliderFloat(u8"透视距离", &set->Item.CodeLock.BigMapMaxDistance, 1.0f, 800.0f, "%.f", ImGuiSliderFlags_NoInput);
			}
			widget::end_child();

			ImGui::NextColumn();
			widget::begin_child(u8"拉闸点", ImVec2(0, SCALE(300)), true); {
				widget::CheckboxWithColor(u8"启用##02", &set->Item.ExitTrigger.Enable, (float*)&set->Item.ExitTrigger.color);
				ImGui::SliderFloat(u8"字体大小", &set->Item.ExitTrigger.fontSize, 12.0f, 32.0f, "%.f", ImGuiSliderFlags_NoInput);
				ImGui::SliderFloat(u8"透视距离", &set->Item.ExitTrigger.maxDistance, 1.0f, 500.0f, "%.f", ImGuiSliderFlags_NoInput);
			}
			widget::end_child();
			widget::begin_child(u8"大地图配置##拉闸点", ImVec2(0, -1), true); {
				ImGui::SliderFloat(u8"字体大小", &set->Item.ExitTrigger.BigMapfontSize, 12.0f, 32.0f, "%.f", ImGuiSliderFlags_NoInput);
				ImGui::SliderFloat(u8"透视距离", &set->Item.ExitTrigger.BigMapMaxDistance, 1.0f, 800.0f, "%.f", ImGuiSliderFlags_NoInput);
			}
			widget::end_child();

			ImGui::NextColumn();
			widget::begin_child(u8"撤离点", ImVec2(0, SCALE(300)), true); {
				widget::CheckboxWithColor(u8"启用##03", &set->Item.PlayerExit.Enable, (float*)&set->Item.PlayerExit.color);
				ImGui::SliderFloat(u8"字体大小", &set->Item.PlayerExit.fontSize, 12.0f, 32.0f, "%.f", ImGuiSliderFlags_NoInput);
				ImGui::SliderFloat(u8"透视距离", &set->Item.PlayerExit.maxDistance, 1.0f, 500.0f, "%.f", ImGuiSliderFlags_NoInput);

			}
			widget::end_child();
			widget::begin_child(u8"大地图配置##撤离点", ImVec2(0, -1), true); {
				ImGui::SliderFloat(u8"字体大小", &set->Item.PlayerExit.BigMapfontSize, 12.0f, 32.0f, "%.f", ImGuiSliderFlags_NoInput);
				ImGui::SliderFloat(u8"透视距离", &set->Item.PlayerExit.BigMapMaxDistance, 1.0f, 800.0f, "%.f", ImGuiSliderFlags_NoInput);
			}
			widget::end_child();
			ImGui::NextColumn();
			ImGui::Columns(1, nullptr, false);
		}

	}
	ImGui::EndChild();
	ImGui::PopStyleColor();

}

void c_gui::Map() {
	auto FormatWithThousandsSeparator = [](float value) ->std::string {
		// 转换为整数（忽略小数，因为你的场景是整数金额）
		long long num = static_cast<long long>(value);
		std::string str = std::to_string(num);
		std::string result;

		// 从后往前每3位插入一个逗号
		int count = 0;
		for (auto it = str.rbegin(); it != str.rend(); ++it) {
			if (count > 0 && count % 3 == 0) {
				result += ',';
			}
			result += *it;
			count++;
		}

		// 反转字符串恢复正确顺序，并添加美元符号
		std::reverse(result.begin(), result.end());
		return "$" + result;
		};


	ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
	ImGui::Columns(3, nullptr, false);
	ImGui::NextColumn();
	ImGui::NextColumn();
	ImGui::BeginChild("雷达类型##11", ImVec2(-1, 0), false, ImGuiWindowFlags_NoBackground);
	{
		widget::begin_child(u8"雷达类型", SCALE(0, 50), false);
		ImGui::Combo(u8"地图类型", &set->map_tab, u8"大地图\0\0");
		widget::end_child();
	}
	ImGui::EndChild();
	ImGui::NextColumn();
	ImGui::Columns(1, nullptr, false);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - SCALE(8));
	ImGui::BeginChild(u8"大地图", ImVec2(-1, -1), false, ImGuiWindowFlags_NoBackground);
	{
		ImGui::Columns(4, nullptr, false);
		widget::begin_child(u8"人物设置", ImVec2(0, -1), true);
		{
			ImGui::Checkbox(u8"显示玩家", &set->BigMap.showPlayer);
			widget::CheckboxWithColor(u8"显示首领", &set->BigMap.showBoss, (float*)&set->BigMap.BossColor);
			widget::CheckboxWithColor(u8"显示精英", &set->BigMap.showElite, (float*)&set->BigMap.EliteColor);
			widget::CheckboxWithColor(u8"显示人机", &set->BigMap.showRobot, (float*)&set->BigMap.RobotColor);
			ImGui::SliderFloat(u8"透视距离", &set->BigMap.PawnMaxDistance, 1, 800.0f, "%.fm");
			ImGui::SliderFloat(u8"字体大小", &set->BigMap.PawnFontSize, 12, 32, "%.f");
		}
		widget::end_child();
		ImGui::NextColumn();
		widget::begin_child(u8"物资设置", ImVec2(0, -1), true);
		{
			ImGui::Checkbox(u8"白色物资", &set->BigMap.showLootingItem[0]);
			ImGui::Checkbox(u8"绿色物资", &set->BigMap.showLootingItem[1]);
			ImGui::Checkbox(u8"蓝色物资", &set->BigMap.showLootingItem[2]);
			ImGui::Checkbox(u8"紫色物资", &set->BigMap.showLootingItem[3]);
			ImGui::Checkbox(u8"金色物资", &set->BigMap.showLootingItem[4]);
			ImGui::Checkbox(u8"红色物资", &set->BigMap.showLootingItem[5]);
			ImGui::Checkbox(u8"启用价值过滤", &set->BigMap.LootingItemPriceFiltering);
			ImGui::SliderFloat(u8"最低价值", &set->BigMap.LootingItemMinGuidePrice, 1000, 500000, FormatWithThousandsSeparator(set->BigMap.LootingItemMinGuidePrice).c_str());
			ImGui::SliderFloat(u8"透视距离", &set->BigMap.LootingMaxDistance, 1, 800.0f, "%.fm");
			ImGui::SliderFloat(u8"字体大小", &set->BigMap.LootingFontSize, 12, 32, "%.f");
		}
		widget::end_child();
		ImGui::NextColumn();
		widget::begin_child(u8"盒子设置", ImVec2(0, -1), true);
		{
			ImGui::Checkbox(u8"过滤开启##01", &set->BigMap.openFilteringDeadBody);
			widget::CheckboxWithColor(u8"玩家盒子", &set->BigMap.showPlayerDeadBody, (float*)&set->BigMap.PlayerDeadBodyColor);
			widget::CheckboxWithColor(u8"首领盒子", &set->BigMap.showBossDeadBody, (float*)&set->Item.DeadBody.BossBoxColor);
			widget::CheckboxWithColor(u8"精英盒子", &set->BigMap.showEliteDeadBody, (float*)&set->Item.DeadBody.EliteBoxColor);
			widget::CheckboxWithColor(u8"人机盒子", &set->BigMap.showRobotDeadBody, (float*)&set->Item.DeadBody.RobotBoxColor);
			ImGui::SliderFloat(u8"透视距离", &set->BigMap.DeadBodyMaxDistance, 1, 800.0f, "%.fm");
			ImGui::SliderFloat(u8"字体大小", &set->BigMap.DeadBodyFontSize, 12, 32, "%.f");
		}
		widget::end_child();
		ImGui::NextColumn();
		widget::begin_child(u8"容器设置", ImVec2(0, -1), true);
		{
			ImGui::Checkbox(u8"过滤开启##02", &set->BigMap.openFilteringOpenBox);
			ImGui::Checkbox(u8"初级容器", &set->BigMap.showOpenBox[0]);
			ImGui::Checkbox(u8"中级容器", &set->BigMap.showOpenBox[1]);
			ImGui::Checkbox(u8"高级容器", &set->BigMap.showOpenBox[2]);
			ImGui::SliderFloat(u8"透视距离", &set->BigMap.OpenBoxMaxDistance, 1, 800.0f, "%.fm");
			ImGui::SliderFloat(u8"字体大小", &set->BigMap.OpenBoxFontSize, 12, 32, "%.f");
		}
		widget::end_child();


		ImGui::NextColumn();
		ImGui::Columns(1, nullptr, false);




	}
	ImGui::EndChild();
	ImGui::PopStyleColor();

}

void c_gui::Settting() {

	ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
	ImGui::Columns(3, nullptr, false);
	ImGui::NextColumn();
	ImGui::NextColumn();
	ImGui::BeginChild(u8"模式设置##11", ImVec2(-1, 0), false, ImGuiWindowFlags_NoBackground);
	{
		widget::begin_child(u8"游戏模式", SCALE(0, 50), false);
		ImGui::Combo(u8"当前模式", &set->Setting.game_mode, u8"全面战场\0烽火地带\0\0");
		widget::end_child();
	}
	ImGui::EndChild();
	ImGui::NextColumn();
	ImGui::Columns(1, nullptr, false);
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - SCALE(8));
	ImGui::BeginChild(u8"设置", ImVec2(-1, -1), false, ImGuiWindowFlags_NoBackground);
	{
		ImGui::Columns(3, nullptr, false);
		widget::begin_child(u8"配置", ImVec2(0, 0), true); {
			ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_Scheme));
			if (ImGui::Button(u8"加载配置")) {
				set->load();
			}
			if (ImGui::Button(u8"保存配置")) {
				set->save();
			}
			ImGui::Dummy(ImVec2(0, 5));
			ImGui::PopStyleColor();

		}
		widget::end_child();
		ImGui::NextColumn();
		ImGui::Columns(3, nullptr, false);
	}
	ImGui::EndChild();
	ImGui::PopStyleColor();
}