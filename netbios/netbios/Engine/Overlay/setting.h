#pragma once
#include "imgui.h"
#include <map>
#include <string>
#include <map>
class c_set {

public:
	int main_tab = 0;
	int aim_tab = 0;
	int pawn_tab = 0;
	int item_tab = 0;
	int map_tab = 0;

	struct c_Aimbot {
		bool Enable = true;
		int AimType = 0;


		int HotKey = 0;
		int HotKeyTwo = 0;


		int AimPos = 0;
		int AimPosTwo = 0;


		bool AimPlayer = true;
		bool AimBoss = true;
		bool AimElite = true;
		bool AimRobot = true;

		bool NotHealthZero = true;

		float maxDistance = 150.0f;

		bool showAimFov = true;
		bool showTarget = true;
		
		float AimFov = 100;

		ImColor AimFovColor = ImColor(255, 255, 255, 255);
		
		float SmoothFactorX = 0.2f;
		float SmoothFactorY = 0.2f;


		bool BulletSpread = true;

	}Aimbot;

	struct c_Pawn
	{
		bool Enable = true;
		bool showSkeleton = true;
		bool showBox = true;
		bool showName = true;
		bool showHero = true;
		bool showWeapon = true;
		bool showHealth = true;
		bool showHelmet = true;
		bool showDistance = true;
		bool showTeamId = true;

		ImColor SkeletonColor;
		ImColor BoxColor;
		ImColor NameColor;
		ImColor HeroColor;
		ImColor WeaponColor;
		ImColor DistanceColor;

		ImColor SkeletonInvisibleColor;
		ImColor BoxInvisibleColor;

		float fontSize = 16.0f;
		float BoxLine = 1.0f;
		float SkeletonLine = 1.0f;
		float maxDistance = 300.0f;

		int miniHelmetLevel = 4;
		int miniBreastPlateLevel = 4;
		int miniChestHangingLevel = 5;
		int miniBagLevel = 5;

	}Player, Boss, Elite, Robot;

	struct c_Item
	{
		//可拾取物
		struct c_LootingItem
		{
			bool Enable = true;
			bool showLv1 = true;
			bool showLv2 = true;
			bool showLv3 = true;
			bool showLv4 = true;
			bool showLv5 = true;
			bool showLv6 = true;

			bool  PriceFiltering = true;
			float minGuidePrice = 10000.0f;

			float fontSize = 16.0f;
			float maxDistance = 80.0f;


		}LootingItem;
		//容器
		struct c_OpenBox
		{
			bool Enable = true;

			bool showPickup = true;
			bool openFiltering = true;

			ImColor OpenBoxColor[3];
			bool	showOpenBox[3];

			float fontSize = 16.0f;
			float maxDistance = 50.0f;
		}OpenBox;
		//尸体盒子
		struct c_DeadBody
		{
			bool Enable = true;
			bool showPickup = true;
			bool openFiltering = true;
			bool showPlayerBox = true;
			bool showBossBox = true;
			bool showEliteBox = true;
			bool showRobotBox = true;

			ImColor PlayerBoxColor = ImColor(255, 255, 255, 255);
			ImColor BossBoxColor = ImColor(255, 255, 255, 255);
			ImColor EliteBoxColor = ImColor(239, 239, 239, 255);
			ImColor RobotBoxColor = ImColor(239, 239, 239, 255);


			float PlayerBoxDistance = 50.0f;
			float BossBoxDistance = 50.0f;
			float EliteBoxDistance = 50.0f;
			float RobotBoxDistance = 50.0f;


			float fontSize = 16.0f;

		}DeadBody;

		//核客电脑
		struct c_HackPC
		{
			bool Enable = true;

			ImColor color = 0xffffffff;
			float fontSize = 16.0f;
			float maxDistance = 50.0f;

			float BigMapfontSize = 13.0f;
			float BigMapMaxDistance = 800.0f;
		}HackPC;

		//密码门
		struct c_CodeLock
		{
			bool Enable = true;

			ImColor color = 0xffffffff;
			float fontSize = 16.0f;
			float maxDistance = 50.0f;

			float BigMapfontSize = 13.0f;
			float BigMapMaxDistance = 800.0f;
		}CodeLock;

		//拉闸点
		struct c_ExitTrigger
		{
			bool Enable = true;

			ImColor color = 0xffffffff;
			float fontSize = 16.0f;
			float maxDistance = 50.0f;

			float BigMapfontSize = 13.0f;
			float BigMapMaxDistance = 800.0f;
		}ExitTrigger;

		//撤离点
		struct c_PlayerExit
		{
			bool Enable = true;

			ImColor color = 0xffffffff;
			float fontSize = 16.0f;
			float maxDistance = 50.0f;

			float BigMapfontSize = 13.0f;
			float BigMapMaxDistance = 800.0f;
		}PlayerExit;

	}Item;

	struct c_BigMap
	{
		bool showPlayer = true;
		bool showBoss = true;
		bool showElite = true;
		bool showRobot = true;

		ImColor BossColor = 0xffffffff;
		ImColor EliteColor = 0xffffffff;
		ImColor RobotColor = 0xffffffff;

		float PawnMaxDistance = 800.0f;
		float PawnFontSize = 12.0f;


		bool showLootingItem[6] = { true ,true,true,true,true,true };
		bool  LootingItemPriceFiltering = true;
		float LootingItemMinGuidePrice = 10000.0f;
		float LootingMaxDistance = 800.0f;
		float LootingFontSize = 13.0f;

		bool openFilteringOpenBox = true;
		bool showOpenBox[3] = { true ,true,true };
		float OpenBoxMaxDistance = 800.0f;
		float OpenBoxFontSize = 13.0f;


		bool openFilteringDeadBody = true;
		bool showPlayerDeadBody = true;
		bool showBossDeadBody = true;
		bool showEliteDeadBody = true;
		bool showRobotDeadBody = true;
		ImColor PlayerDeadBodyColor = 0xffffffff;

		float DeadBodyMaxDistance = 800.0f;
		float DeadBodyFontSize = 13.0f;




	}BigMap;

	struct c_Setting
	{
		int game_mode = 1;
	}Setting;
public:
	std::map<std::string, std::pair<bool, int>> OpenBoxItems_Cache;
public:
	c_set();
public:
	void load();
	void save();
};


extern c_set* set;