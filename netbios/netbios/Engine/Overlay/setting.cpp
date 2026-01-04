#include "pch.h"
#include "setting.h"
#include "Base64Codec.h"
#include "MapSerializer.hpp"
c_set* set = new c_set();
//初始化配置
c_set::c_set() {

	{
		//玩家
		this->Player.Enable = true;
		this->Player.showSkeleton = true;
		this->Player.showBox = true;
		this->Player.showName = true;
		this->Player.showHero = true;
		this->Player.showWeapon = true;
		this->Player.showHealth = true;
		this->Player.showHelmet = true;
		this->Player.showDistance = true;
		this->Player.showTeamId = true;
		this->Player.SkeletonColor = ImVec4(0, 1, 1, 1);
		this->Player.BoxColor = ImVec4(0, 1, 1, 1);
		this->Player.NameColor = 0xFFFFFFFF;
		this->Player.HeroColor = 0xFFFFFFFF;
		this->Player.WeaponColor = 0xFFFFFFFF;
		this->Player.DistanceColor = 0xFFFFFFFF;
		this->Player.SkeletonInvisibleColor = ImVec4(1, 1, 0, 1);
		this->Player.BoxInvisibleColor = ImVec4(1, 1, 0, 1);
		this->Player.fontSize = 16.0f;
		this->Player.BoxLine = 1.0f;
		this->Player.SkeletonLine = 1.0f;
		this->Player.maxDistance = 300.0f;
		this->Player.miniHelmetLevel = 4;
		this->Player.miniBreastPlateLevel = 4;
		this->Player.miniChestHangingLevel = 5;
		this->Player.miniBagLevel = 5;

		//BOSS
		this->Boss.Enable = true;
		this->Boss.showSkeleton = true;
		this->Boss.showBox = true;
		this->Boss.showName = true;
		this->Boss.showHero = true;
		this->Boss.showWeapon = true;
		this->Boss.showHealth = true;
		this->Boss.showHelmet = true;
		this->Boss.showDistance = true;
		this->Boss.showTeamId = true;
		this->Boss.SkeletonColor = ImVec4(1, 0, 0, 1);
		this->Boss.BoxColor = ImVec4(1, 0, 0, 1);
		this->Boss.NameColor = 0xFF0000FF;
		this->Boss.HeroColor = 0xFFFFFFFF;
		this->Boss.WeaponColor = 0xFFFFFFFF;
		this->Boss.DistanceColor = 0xFFFFFFFF;
		this->Boss.SkeletonInvisibleColor = ImVec4(1, 1, 0, 1);
		this->Boss.BoxInvisibleColor = ImVec4(1, 1, 0, 1);
		this->Boss.fontSize = 16.0f;
		this->Boss.BoxLine = 1.0f;
		this->Boss.SkeletonLine = 1.0f;
		this->Boss.maxDistance = 300.0f;
		this->Boss.miniHelmetLevel = 4;
		this->Boss.miniBreastPlateLevel = 4;
		this->Boss.miniChestHangingLevel = 5;
		this->Boss.miniBagLevel = 5;

		//Elite
		this->Elite.Enable = true;
		this->Elite.showSkeleton = true;
		this->Elite.showBox = true;
		this->Elite.showName = true;
		this->Elite.showHero = true;
		this->Elite.showWeapon = true;
		this->Elite.showHealth = true;
		this->Elite.showHelmet = true;
		this->Elite.showDistance = true;
		this->Elite.showTeamId = true;
		this->Elite.SkeletonColor = ImVec4(1, 0, 0, 1);
		this->Elite.BoxColor = ImVec4(1, 0, 0, 1);
		this->Elite.NameColor = 0xFFFFFFFF;
		this->Elite.HeroColor = 0xFFFFFFFF;
		this->Elite.WeaponColor = 0xFFFFFFFF;
		this->Elite.DistanceColor = 0xFFFFFFFF;
		this->Elite.SkeletonInvisibleColor = ImVec4(1, 1, 0, 1);
		this->Elite.BoxInvisibleColor = ImVec4(1, 1, 0, 1);
		this->Elite.fontSize = 16.0f;
		this->Elite.BoxLine = 1.0f;
		this->Elite.SkeletonLine = 1.0f;
		this->Elite.maxDistance = 300.0f;
		this->Elite.miniHelmetLevel = 4;
		this->Elite.miniBreastPlateLevel = 4;
		this->Elite.miniChestHangingLevel = 5;
		this->Elite.miniBagLevel = 5;

		//BOSS
		this->Robot.Enable = true;
		this->Robot.showSkeleton = false;
		this->Robot.showBox = false;
		this->Robot.showName = true;
		this->Robot.showHero = true;
		this->Robot.showWeapon = true;
		this->Robot.showHealth = false;
		this->Robot.showHelmet = true;
		this->Robot.showDistance = true;
		this->Robot.showTeamId = true;
		this->Robot.SkeletonColor = ImVec4(1, 0, 0, 1);
		this->Robot.BoxColor = ImVec4(1, 0, 0, 1);
		this->Robot.NameColor = 0xFFF0F0F0;
		this->Robot.HeroColor = 0xFFF0F0F0;
		this->Robot.WeaponColor = 0xFFF0F0F0;
		this->Robot.DistanceColor = 0xFFF0F0F0;
		this->Robot.SkeletonInvisibleColor = ImVec4(1, 1, 0, 1);
		this->Robot.BoxInvisibleColor = ImVec4(1, 1, 0, 1);
		this->Robot.fontSize = 16.0f;
		this->Robot.BoxLine = 1.0f;
		this->Robot.SkeletonLine = 1.0f;
		this->Robot.maxDistance = 300.0f;
		this->Robot.miniHelmetLevel = 4;
		this->Robot.miniBreastPlateLevel = 4;
		this->Robot.miniChestHangingLevel = 5;
		this->Robot.miniBagLevel = 5;

		//OpenBoxItems_Cache = new std::remove_pointer_t<decltype(OpenBoxItems_Cache)>();


		//OpenBoxItems_Cache.insert({ u8"鸟窝", std::make_pair(true, 2) });
		//OpenBoxItems_Cache.insert({ u8"保险柜", std::make_pair(true, 2) });
		//OpenBoxItems_Cache.insert({ u8"保险箱", std::make_pair(true, 2) });
		//OpenBoxItems_Cache.insert({ u8"医疗物资堆", std::make_pair(true, 2) });
		//OpenBoxItems_Cache.insert({ u8"航空储物箱", std::make_pair(true, 2) });
		//OpenBoxItems_Cache.insert({ u8"高级储物箱", std::make_pair(true, 2) });


		//OpenBoxItems_Cache.insert({ u8"医疗包", std::make_pair(true, 1) });
		//OpenBoxItems_Cache.insert({ u8"高级旅行箱", std::make_pair(true, 1) });
		//OpenBoxItems_Cache.insert({ u8"大武器箱", std::make_pair(true, 1) });
		//OpenBoxItems_Cache.insert({ u8"手提箱", std::make_pair(true, 1) });
		//OpenBoxItems_Cache.insert({ u8"野外战备箱", std::make_pair(true, 1) });


		this->Item.OpenBox.showOpenBox[0] = true;
		this->Item.OpenBox.showOpenBox[1] = true;
		this->Item.OpenBox.showOpenBox[2] = true;

		this->Item.OpenBox.OpenBoxColor[0] = 0xFFEAEAEA;
		this->Item.OpenBox.OpenBoxColor[1] = 0xFF4EC9B0;
		this->Item.OpenBox.OpenBoxColor[2] = 0xFFEFB839;

	}
	load();
}

void c_set::load() {

	{
		std::string confStr = GetPrivateProfileStringA("C:\\YJ.conf", "Setting", "AimBot");
		if (confStr.empty() == false) {
			bytes confData;
			if (Base64Codec::base64Decode(confStr, confData)) {
				if (confData.size() == sizeof(Aimbot)) {
					memcpy(&Aimbot, confData.data(), confData.size());
				}
			}
		}
	}

	{
		std::string confStr = GetPrivateProfileStringA("C:\\YJ.conf", "Setting", "Player");
		if (confStr.empty() == false) {
			bytes confData;
			if (Base64Codec::base64Decode(confStr, confData)) {
				if (confData.size() == sizeof(Player)) {
					memcpy(&Player, confData.data(), confData.size());
				}
			}
		}
	}
	

	{
		std::string confStr = GetPrivateProfileStringA("C:\\YJ.conf", "Setting", "Boss");
		if (confStr.empty() == false) {
			bytes confData;
			if (Base64Codec::base64Decode(confStr, confData)) {
				if (confData.size() == sizeof(Boss)) {
					memcpy(&Boss, confData.data(), confData.size());
				}
			}
		}
	}

	{
		std::string confStr = GetPrivateProfileStringA("C:\\YJ.conf", "Setting", "Elite");
		if (confStr.empty() == false) {
			bytes confData;
			if (Base64Codec::base64Decode(confStr, confData)) {
				if (confData.size() == sizeof(Elite)) {
					memcpy(&Elite, confData.data(), confData.size());
				}
			}
		}
	}

	{
		std::string confStr = GetPrivateProfileStringA("C:\\YJ.conf", "Setting", "Robot");
		if (confStr.empty() == false) {
			bytes confData;
			if (Base64Codec::base64Decode(confStr, confData)) {
				if (confData.size() == sizeof(Robot)) {
					memcpy(&Robot, confData.data(), confData.size());
				}
			}
		}
	}

	{
		std::string confStr = GetPrivateProfileStringA("C:\\YJ.conf", "Setting", "Item");
		if (confStr.empty() == false) {
			bytes confData;
			if (Base64Codec::base64Decode(confStr, confData)) {
				if (confData.size() == sizeof(Item)) {
					memcpy(&Item, confData.data(), confData.size());
				}
			}
		}
	}

	{
		std::string confStr = GetPrivateProfileStringA("C:\\YJ.conf", "Setting", "BigMap");
		if (confStr.empty() == false) {
			bytes confData;
			if (Base64Codec::base64Decode(confStr, confData)) {
				if (confData.size() == sizeof(BigMap)) {
					memcpy(&BigMap, confData.data(), confData.size());
				}
			}
		}
	}

	
	{
		std::string confStr = GetPrivateProfileStringA("C:\\YJ.conf", "Setting", "Setting");
		if (confStr.empty() == false) {
			bytes confData;
			if (Base64Codec::base64Decode(confStr, confData)) {
				if (confData.size() == sizeof(Setting)) {
					memcpy(&Setting, confData.data(), confData.size());
				}
			}
		}
	}

	{
		std::string confStr = GetPrivateProfileStringA("C:\\YJ.conf", "Setting", "ItemsCache");
		if (confStr.empty() == false) {
			bytes confData;
			if (Base64Codec::base64Decode(confStr, confData)) {
				if (MapSerializer::deserialize(confData, OpenBoxItems_Cache) == false) {
					OpenBoxItems_Cache.clear();
				}
			}
		}
	}

}
void c_set::save() {
	WritePrivateProfileStringA("Setting", "AimBot", Base64Codec::base64Encode(std::to_bytes(&Aimbot, sizeof(Aimbot))).c_str(), "C:\\YJ.conf");
	WritePrivateProfileStringA("Setting", "Player", Base64Codec::base64Encode(std::to_bytes(&Player, sizeof(Player))).c_str(), "C:\\YJ.conf");
	WritePrivateProfileStringA("Setting", "Boss", Base64Codec::base64Encode(std::to_bytes(&Boss, sizeof(Boss))).c_str(), "C:\\YJ.conf");
	WritePrivateProfileStringA("Setting", "Robot", Base64Codec::base64Encode(std::to_bytes(&Robot, sizeof(Robot))).c_str(), "C:\\YJ.conf");
	WritePrivateProfileStringA("Setting", "Item", Base64Codec::base64Encode(std::to_bytes(&Item, sizeof(Item))).c_str(), "C:\\YJ.conf");
	WritePrivateProfileStringA("Setting", "BigMap", Base64Codec::base64Encode(std::to_bytes(&BigMap, sizeof(BigMap))).c_str(), "C:\\YJ.conf");
	WritePrivateProfileStringA("Setting", "Setting", Base64Codec::base64Encode(std::to_bytes(&Setting, sizeof(Setting))).c_str(), "C:\\YJ.conf");

	bytes serializedData;
	if (MapSerializer::serialize(set->OpenBoxItems_Cache, serializedData)) {
		WritePrivateProfileStringA("Setting", "ItemsCache", Base64Codec::base64Encode(serializedData).c_str(), "C:\\YJ.conf");
	}
}