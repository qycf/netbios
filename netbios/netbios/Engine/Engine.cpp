#include "Engine.h"
#include "overlay/overlay.h"
#include "gui.h"
UGlobal UEGlobal;
AimAssistSystem AimBot;

void UGlobal::InitBoneName() {
	// 第一步：获取所有骨骼名称的值
	this->Bone.Head = UE::UKismetStringLibrary::Conv_StringToName(UE::FString(L"Head"));
	this->Bone.Neck = UE::UKismetStringLibrary::Conv_StringToName(UE::FString(L"Neck"));
	this->Bone.Hips = UE::UKismetStringLibrary::Conv_StringToName(UE::FString(L"Hips"));
	this->Bone.RightShoulder = UE::UKismetStringLibrary::Conv_StringToName(UE::FString(L"RightShoulder"));
	this->Bone.RightArm = UE::UKismetStringLibrary::Conv_StringToName(UE::FString(L"RightArm"));
	this->Bone.RightForeArm = UE::UKismetStringLibrary::Conv_StringToName(UE::FString(L"RightForeArm"));
	this->Bone.RightHand = UE::UKismetStringLibrary::Conv_StringToName(UE::FString(L"RightHand"));
	this->Bone.RightUpLeg = UE::UKismetStringLibrary::Conv_StringToName(UE::FString(L"RightUpLeg"));
	this->Bone.RightLeg = UE::UKismetStringLibrary::Conv_StringToName(UE::FString(L"RightLeg"));
	this->Bone.RightFoot = UE::UKismetStringLibrary::Conv_StringToName(UE::FString(L"RightFoot"));
	this->Bone.LeftShoulder = UE::UKismetStringLibrary::Conv_StringToName(UE::FString(L"LeftShoulder"));
	this->Bone.LeftArm = UE::UKismetStringLibrary::Conv_StringToName(UE::FString(L"LeftArm"));
	this->Bone.LeftForeArm = UE::UKismetStringLibrary::Conv_StringToName(UE::FString(L"LeftForeArm"));
	this->Bone.LeftHand = UE::UKismetStringLibrary::Conv_StringToName(UE::FString(L"LeftHand"));
	this->Bone.LeftUpLeg = UE::UKismetStringLibrary::Conv_StringToName(UE::FString(L"LeftUpLeg"));
	this->Bone.LeftLeg = UE::UKismetStringLibrary::Conv_StringToName(UE::FString(L"LeftLeg"));
	this->Bone.LeftFoot = UE::UKismetStringLibrary::Conv_StringToName(UE::FString(L"LeftFoot"));
	// 第二步：统一将值赋值给数组
	this->Bone.Skeleton[BoneID::Head] = this->Bone.Head;
	this->Bone.Skeleton[BoneID::Neck] = this->Bone.Neck;
	this->Bone.Skeleton[BoneID::Hips] = this->Bone.Hips;
	this->Bone.Skeleton[BoneID::RightShoulder] = this->Bone.RightShoulder;
	this->Bone.Skeleton[BoneID::RightArm] = this->Bone.RightArm;
	this->Bone.Skeleton[BoneID::RightForeArm] = this->Bone.RightForeArm;
	this->Bone.Skeleton[BoneID::RightHand] = this->Bone.RightHand;
	this->Bone.Skeleton[BoneID::RightUpLeg] = this->Bone.RightUpLeg;
	this->Bone.Skeleton[BoneID::RightLeg] = this->Bone.RightLeg;
	this->Bone.Skeleton[BoneID::RightFoot] = this->Bone.RightFoot;
	this->Bone.Skeleton[BoneID::LeftShoulder] = this->Bone.LeftShoulder;
	this->Bone.Skeleton[BoneID::LeftArm] = this->Bone.LeftArm;
	this->Bone.Skeleton[BoneID::LeftForeArm] = this->Bone.LeftForeArm;
	this->Bone.Skeleton[BoneID::LeftHand] = this->Bone.LeftHand;
	this->Bone.Skeleton[BoneID::LeftUpLeg] = this->Bone.LeftUpLeg;
	this->Bone.Skeleton[BoneID::LeftLeg] = this->Bone.LeftLeg;
	this->Bone.Skeleton[BoneID::LeftFoot] = this->Bone.LeftFoot;
}
bool UGlobal::WorldLocationToScreenPoint(const FVector& Location, FVector2D* coords) {
	return PlayerController->ProjectWorldLocationToScreen(Location, coords[0]);
}
bool UGlobal::WorldLocationToScreenRect(const FVector& Location, FVector2D* coords) {
	if (PlayerController->ProjectWorldLocationToScreen(Location + FVector(0, 0, 100.0f), coords[0]) && PlayerController->ProjectWorldLocationToScreen(Location + FVector(0, 0, -100.0f), coords[1])) {
		float width = (coords[1].Y - coords[0].Y) / 2.0f;
		coords[0].X -= width / 2.0f;
		coords[1].X = coords[0].X + width;
		return true;
	}
	return false;
}
//
std::string UGlobal::Conv_TextToString(FText InText) {
	auto TextData = InText.get_TextData();
	if (TextData == nullptr)return"";
	FString fstring = TextData->get_TextSource();
	if (fstring.CStr() == nullptr)return"";
	return us2_to_utf8(fstring.CStr());
}
//FName转字符串
std::string UGlobal::Conv_NameToString(FName InName) {
	uint64_t NameId = (uint64_t&)InName;
	if (NameId == 0) {
		return "";
	}
	auto Item = Conv_NameToString_Cache.find(NameId);
	if (Item != Conv_NameToString_Cache.end()) {
		return Item->second;
	}
	std::string NameStr = us2_to_utf8(InName.c_wstr());
	Conv_NameToString_Cache.insert({ NameId,NameStr });
	return NameStr;
}
//
std::string UGlobal::GetPoolChosenName(ADFMCharacter* InCharacter) {
	auto PoolName = InCharacter->get_PoolName();
	uint64_t PoolNameIndex = *(uint64_t*)&PoolName;
	std::string PoolChosenNameStr;
	if (PoolNameIndex == 0) {
		if (InCharacter->IsA(ADFMRangeTargetCharacter::StaticClass())) {
			return u8"靶场假人";
		}
		return "None";
	}
	auto it = PoolChosenName_Cache.find(PoolNameIndex);
	if (it != PoolChosenName_Cache.end()) {
		return it->second;
	}
	FString PoolChosenName = InCharacter->GetPoolChosenName();
	if (PoolChosenName.CStr() == nullptr) {
		PoolChosenNameStr = "None";
	}
	else
	{
		PoolChosenNameStr = us2_to_utf8(PoolChosenName.CStr());
	}
	PoolChosenName_Cache.insert({ PoolNameIndex,PoolChosenNameStr });
	return PoolChosenNameStr;
}
std::string UGlobal::GetPlayerNamePrivate(APlayerState* PlayerState) {
	//定义静态映射表
	auto PlayerId = PlayerState->get_PlayerId();
	auto it = PlayerNamePrivate_Cache.find(PlayerId);
	if (it != PlayerNamePrivate_Cache.end()) {
		return it->second;
	}
	std::string PlayerNamePrivateStr;
	FString PlayerNamePrivate = PlayerState->get_PlayerNamePrivate();
	if (PlayerNamePrivate.CStr() == nullptr) {
		PlayerNamePrivateStr = u8"编码9527";
	}
	else
	{
		PlayerNamePrivateStr = us2_to_utf8(PlayerNamePrivate.CStr());
	}
	PlayerNamePrivate_Cache.insert({ PlayerId,PlayerNamePrivateStr });
	return PlayerNamePrivateStr;
}
std::string UGlobal::GetDeadBodyName(AInventoryPickup_DeadBody* DeadBody) {
	auto Name = DeadBody->get_Name();
	uint64_t NameId = (uint64_t&)Name;
	if (NameId == 0) {
		return std::string();
	}
	auto it = DeadBodyName_Cache.find(NameId);
	if (it != DeadBodyName_Cache.end()) {
		return it->second;
	}
	std::string PlayerName;
	FString PlayerName_Buffer = DeadBody->GetPlayerName();
	if (PlayerName_Buffer.CStr() == nullptr) {
		PlayerName = "";
	}
	else
	{
		PlayerName = us2_to_utf8(PlayerName_Buffer.CStr());
	}
	DeadBodyName_Cache.insert({ NameId,PlayerName });
	return PlayerName;
}

std::string UGlobal::GetPlayerHeroName(APlayerState* PlayerState) {
	auto HeroID = PlayerState->Cast<ADFMPlayerState>()->get_HeroID();
	switch ((unsigned char)HeroID)
	{
	case 0x19:return u8"威龙";
	case 0x1A:return u8"骇爪";
	case 0x1B:return u8"蜂医";
	case 0x1C:return u8"露娜";
	case 0x1D:return u8"牧羊人";
	case 0x1E:return u8"红狼";
	case 0x23:return u8"乌鲁鲁";
	case 0x24:return u8"蛊";
	case 0x25:return u8"深蓝";
	case 0x26:return u8"无名";
	case 0x27:return u8"疾风";
	case 0x28:return u8"银翼";
	}
	return std::to_string((unsigned char)HeroID);
}
std::string UGlobal::GetPlayerWeapName(ADFMCharacter* Player) {
	AWeaponBase* CacheCurWeapon = Player->get_CacheCurWeapon();
	uint64_t WeaponID = CacheCurWeapon->get_WeaponID();
	//没武器
	if (WeaponID == 0)return std::string();
	//武器没刷新出来
	if (WeaponID == 18110000000)return std::string();
	// 一、冲锋枪（SMG）
	else if (WeaponID == 18020000001) return u8"MP5";
	else if (WeaponID == 18020000002) return u8"P90";
	else if (WeaponID == 18020000003) return u8"Vector";
	else if (WeaponID == 18020000004) return u8"UZI";
	else if (WeaponID == 18020000005) return u8"野牛";
	else if (WeaponID == 18020000006) return u8"SMG-45";
	else if (WeaponID == 18020000008) return u8"SR-3M";
	else if (WeaponID == 18020000009) return u8"勇士";
	else if (WeaponID == 18020000010) return u8"MP7";
	else if (WeaponID == 18020000011) return u8"QCQ171";

	// 二、手枪
	else if (WeaponID == 18070000002) return u8"QSZ92G";
	else if (WeaponID == 18070000003) return u8".357左轮";
	else if (WeaponID == 18070000004) return u8"沙漠之鹰";
	else if (WeaponID == 18070000005) return u8"G18";
	else if (WeaponID == 18070000006) return u8"93R";
	else if (WeaponID == 18070000010) return u8"G17";
	else if (WeaponID == 18070000033) return u8"M1911";
	else if (WeaponID == 18070000030) return u8"峰医激素枪";

	// 三、步枪
	else if (WeaponID == 18010000001) return u8"M4A1";
	else if (WeaponID == 18010000006) return u8"AKM";
	else if (WeaponID == 18010000008) return u8"QBZ95-1";
	else if (WeaponID == 18010000010) return u8"AKS-74U";
	else if (WeaponID == 18010000012) return u8"ASh-12";
	else if (WeaponID == 18010000013) return u8"K416";
	else if (WeaponID == 18010000014) return u8"M16A4";
	else if (WeaponID == 18010000015) return u8"AUG";
	else if (WeaponID == 18010000016) return u8"M7";
	else if (WeaponID == 18010000017) return u8"SG552";
	else if (WeaponID == 18010000018) return u8"AK-12";
	else if (WeaponID == 18010000021) return u8"SCAR-H";
	else if (WeaponID == 18010000023) return u8"G3";
	else if (WeaponID == 18010000024) return u8"PTR-32";
	else if (WeaponID == 18010000031) return u8"CAR-15";
	else if (WeaponID == 18010000037) return u8"AS VAL";
	else if (WeaponID == 18010000038) return u8"191式";
	else if (WeaponID == 18010000040) return u8"K437";
	else if (WeaponID == 18010000042) return u8"KC17";

	// 四、狙击枪/精确射手步枪
	else if (WeaponID == 18050000002) return u8"Mini-14";
	else if (WeaponID == 18050000003) return u8"VSS";
	else if (WeaponID == 18050000004) return u8"SVD";
	else if (WeaponID == 18050000005) return u8"M14";
	else if (WeaponID == 18050000006) return u8"SKS";
	else if (WeaponID == 18050000007) return u8"SR-25";
	else if (WeaponID == 18050000008) return u8"SR9";
	else if (WeaponID == 18050000031) return u8"PSG-1";
	else if (WeaponID == 18060000007) return u8"SV-98";
	else if (WeaponID == 18060000008) return u8"R93";
	else if (WeaponID == 18060000009) return u8"M700";
	else if (WeaponID == 18060000011) return u8"AWM";

	// 五、机枪
	else if (WeaponID == 18040000001) return u8"PKM";
	else if (WeaponID == 18040000002) return u8"M249";
	else if (WeaponID == 18040000003) return u8"M250";
	else if (WeaponID == 18040000004) return u8"QJB201";
	else if (WeaponID == 18040000008) return u8"机枪";

	// 六、霰弹枪
	else if (WeaponID == 18030000001) return u8"M1014";
	else if (WeaponID == 18030000002) return u8"S12K";
	else if (WeaponID == 18030000004) return u8"M870";
	else if (WeaponID == 18030000005) return u8"727";

	// 七、特殊武器（含弓类）
	else if (WeaponID == 18130000001) return u8"电击箭矢";
	else if (WeaponID == 18130000002) return u8"侦察箭矢";
	else if (WeaponID == 18150000001) return u8"复合弓";
	else if (WeaponID == 18080000009) return u8"巡飞弹";

	// 八、发射器（榴弹/火箭筒等）
	else if (WeaponID == 18080000003) return u8"AT-4发射器";
	else if (WeaponID == 18080000005) return u8"毒刺发射器";
	else if (WeaponID == 18080000007) return u8"火箭筒";
	else if (WeaponID == 18080000010) return u8"三联装手炮";
	else if (WeaponID == 18080000011) return u8"虎蹲炮";
	else if (WeaponID == 18080000012) return u8"反人员榴弹";
	else if (WeaponID == 18080000013) return u8"EMP榴弹";
	else if (WeaponID == 18080000014) return u8"标地发射器";
	else if (WeaponID == 18080000019) return u8"高爆榴弹发射器";
	else if (WeaponID == 18080000020) return u8"烟雾榴弹发射器";

	// 九、近战武器
	else if (WeaponID >= 18100000000 && WeaponID <= 18100000020) return u8"刀/X";  // 刀类范围

	// 十、投掷物/装备
	else if (WeaponID == 21010000001) return u8"增强破片手雷";
	else if (WeaponID == 21010000003) return u8"数据飞刀";
	else if (WeaponID == 21010000005) return u8"速凝掩体";
	else if (WeaponID == 21010000006) return u8"燃烧弹";
	else if (WeaponID == 21010000012) return u8"磁吸炸弹";
	else if (WeaponID == 21010000017) return u8"流萤集群系统";
	else if (WeaponID == 21010000018) return u8"致盲瓦斯弹";
	else if (WeaponID == 21010000022) return u8"强化破片手雷";
	else if (WeaponID == 21020300006) return u8"声波陷阱";
	else if (WeaponID == 21020300007) return u8"烟幕弹";
	else if (WeaponID == 21020300009) return u8"弹药箱";
	else if (WeaponID == 21020300010) return u8"重生信标";
	else if (WeaponID == 21020300011) return u8"工程箱";
	else if (WeaponID == 21020300012) return u8"医药箱";
	else if (WeaponID == 21020300013) return u8"ADS近防系统";
	else if (WeaponID == 21020300017) return u8"反坦克地雷";
	else if (WeaponID == 21020300018) return u8"阔剑地雷";

	// 十一、设施/装置
	else if (WeaponID == 18080000008) return u8"镭射指示器";
	else if (WeaponID == 18990000003) return u8"机枪塔";
	else if (WeaponID == 18990000007) return u8"防空炮";
	else if (WeaponID == 18990000008) return u8"岸防炮";

	// 未知武器
	return u8"未知武器 " + std::to_string(WeaponID);
}
std::string UGlobal::GetInventoryPickupName(AInventoryPickup* InventoryPickup) {
	auto ItemIdName = InventoryPickup->get_InventoryIdName();
	uint64_t ItemIdNameIndex = *(uint64_t*)&ItemIdName;
	if (ItemIdNameIndex == 0) {
		return std::string();
	}
	auto it = InventoryPickupName_Cache.find(ItemIdNameIndex);
	if (it != InventoryPickupName_Cache.end()) {
		return it->second;
	}
	std::string ItemName;
	FString DisplayString = UKismetTextLibrary::Conv_TextToString(InventoryPickup->GetItemName());
	if (DisplayString.IsValid()) {
		ItemName = us2_to_utf8(DisplayString.CStr());
	}
	InventoryPickupName_Cache.insert({ ItemIdNameIndex,ItemName });
	return ItemName;
}
std::string UGlobal::GetItemRowName(const FDFMCommonItemRow* ItemRow) {
	auto ItemIdName = ItemRow->ItemID;
	uint64_t ItemIdNameIndex = *(uint64_t*)&ItemIdName;
	if (ItemIdNameIndex == 0) {
		return std::string();
	}
	auto it = ItemRowName_Cache.find(ItemIdNameIndex);
	if (it != ItemRowName_Cache.end()) {
		return it->second;
	}
	std::string ItemName;
	FString DisplayString = UKismetTextLibrary::Conv_TextToString(ItemRow->ShortName);
	if (DisplayString.IsValid()) {
		ItemName = us2_to_utf8(DisplayString.CStr());
	}
	ItemRowName_Cache.insert({ ItemIdNameIndex,ItemName });
	return ItemName;


}
//清空缓存
void UGlobal::Cache_Clear() {
	if (Conv_TextToString_Cache.size())Conv_TextToString_Cache.clear();
	if (Conv_NameToString_Cache.size())Conv_NameToString_Cache.clear();
	if (PoolChosenName_Cache.size())PoolChosenName_Cache.clear();
	if (PlayerNamePrivate_Cache.size())PlayerNamePrivate_Cache.clear();
	if (InventoryPickupName_Cache.size())InventoryPickupName_Cache.clear();
	if (ItemRowName_Cache.size())ItemRowName_Cache.clear();
}
//
PawnType UGlobal::GetPawnType(ADFMCharacter* InCharacter, const std::string& PlayerName, bool IsAI) {
	if (IsAI == false) return PawnType::None;
	//创建数据缓存表
	thread_local std::map<uint64_t, PawnType>* PlayerCharacterTypeList = nullptr;
	if (PlayerCharacterTypeList == nullptr) {
		PlayerCharacterTypeList = new std::map<uint64_t, PawnType>();
	}
	//从静态数据中查找
	auto Name = InCharacter->get_Name();
	uint64_t NameIndex = *(uint64_t*)&Name;
	auto it = PlayerCharacterTypeList->find(NameIndex);
	if (it != PlayerCharacterTypeList->end()) {
		return it->second;
	}
	//进行判断
	PawnType characterType = PawnType::Robot;
	if (InCharacter->IsA(ADFMRangeTargetCharacter::StaticClass())) {
		characterType = PawnType::Robot;
	}
	else if (InCharacter->IsA(ADFMAIAnimalCharacter::StaticClass())) {
		characterType = PawnType::Animal;
	}
	else if (PlayerName[0] == u8'[' && PlayerName.find(']') != std::string::npos) {
		characterType = PawnType::Boss;
	}
	else if (PlayerName.find(u8"士兵") == std::string::npos) {
		characterType = PawnType::Elite;
	}
	//进行缓存
	PlayerCharacterTypeList->insert({ NameIndex,characterType });
	return characterType;
}
PawnType UGlobal::GetDeadBodyType(AInventoryPickup_DeadBody* InDeadBody, const std::string& DeadBodyName) {
	if (InDeadBody->get_LootingCharacterOwner() && InDeadBody->get_OwnerPlayerState()) return PawnType::None;
	//创建数据缓存表
	thread_local std::map<uint64_t, PawnType>* PlayerCharacterTypeList = nullptr;
	if (PlayerCharacterTypeList == nullptr) {
		PlayerCharacterTypeList = new std::map<uint64_t, PawnType>();
	}
	//从静态数据中查找
	auto Name = InDeadBody->get_Name();
	uint64_t NameIndex = *(uint64_t*)&Name;
	auto it = PlayerCharacterTypeList->find(NameIndex);
	if (it != PlayerCharacterTypeList->end()) {
		return it->second;
	}
	//进行判断
	PawnType characterType = PawnType::Robot;
	if (DeadBodyName[0] == u8'[' && DeadBodyName.find(']') != std::string::npos) {
		characterType = PawnType::Boss;
	}
	else if (DeadBodyName.find(u8"士兵") == std::string::npos) {
		characterType = PawnType::Elite;
	}
	//进行缓存
	PlayerCharacterTypeList->insert({ NameIndex,characterType });
	return characterType;
}
//
BoneInfo UGlobal::GetBonePostion(ADFMCharacter* InCharacter, USkeletalMeshComponent* SkinnedMeshComp, FName InBoneName) {
	BoneInfo _BoneInfo;
	_BoneInfo.valid = false;
	_BoneInfo.pos = SkinnedMeshComp->GetBonePositionByName(InBoneName);
	if (PlayerController->ProjectWorldLocationToScreen(_BoneInfo.pos, _BoneInfo.screen)) {
		_BoneInfo.valid = true;
		_BoneInfo.visible = !UKismetSystemLibrary::LineTraceSingle(GWorld, LocalGPCharacter, InCharacter, CameraLocation, _BoneInfo.pos);
	}
	return _BoneInfo;
}
bool UGlobal::GetSkinnedMesh(ADFMCharacter* InCharacter, USkeletalMeshComponent* SkinnedMeshComp, BoneInfo* OutBoneInfo) {
	bool visible = false;
	for (size_t i = 0; i < sizeof(Bone.Skeleton) / sizeof(Bone.Skeleton[0]); i++)
	{
		OutBoneInfo[i] = GetBonePostion(InCharacter, SkinnedMeshComp, Bone.Skeleton[i]);
		if (OutBoneInfo[i].visible)visible = true;
	}
	return visible;
}
//
bool UGlobal::IsInScreen(const FVector2D WorldToScreen) {
	const float ScreenMargin = SCALE(100.0f);

	const float MinX = -ScreenMargin;
	const float MinY = -ScreenMargin;
	const float MaxX = this->SizeX + ScreenMargin;
	const float MaxY = this->SizeY + ScreenMargin;


	return (WorldToScreen.X >= MinX) &&
		(WorldToScreen.X <= MaxX) &&
		(WorldToScreen.Y >= MinY) &&
		(WorldToScreen.Y <= MaxY);

}
//
ImU32 UGlobal::GetColorForNumber(int number) {
	// 精选12种高端质感颜色，色调更优雅，避免基础色
	switch (number % 12) {
	case 0:  return IM_COL32(236, 94, 135, 230);   // 柔和桃红色（高端粉调）
	case 1:  return IM_COL32(249, 136, 126, 230);  // 珊瑚橙色（温暖高级感）
	case 2:  return IM_COL32(181, 71, 135, 230);   // 莓红色（深沉优雅）
	case 3:  return IM_COL32(102, 187, 106, 230);  // 嫩芽绿（低饱和高级绿）
	case 4:  return IM_COL32(79, 193, 233, 230);   // 浅青蓝（清新质感）
	case 5:  return IM_COL32(103, 128, 159, 230);  // 石板蓝（高级灰蓝）
	case 6:  return IM_COL32(171, 130, 255, 230);  // 淡紫蓝（雅致紫色系）
	case 7:  return IM_COL32(232, 121, 249, 230);  // 粉紫色（柔和高贵）
	case 8:  return IM_COL32(131, 96, 195, 230);   // 深紫罗兰（沉稳高级）
	case 9:  return IM_COL32(220, 148, 111, 230);  // 焦糖橙（温暖质感）
	case 10: return IM_COL32(102, 153, 153, 230);  // 青瓷色（雅致冷调）
	case 11: return IM_COL32(143, 143, 159, 230);  // 高级灰（中性质感色）
	default: return IM_COL32(143, 143, 159, 230);  // 默认高级灰
	}
}
ImU32 UGlobal::GetItemLevelColor(int Level) {
	switch (Level) {
	case 1:  // 普通
		return IM_COL32(245, 245, 245, 255);       // 亮灰色，提高亮度
	case 2:  // 优秀
		return IM_COL32(76, 217, 100, 255);        // 明亮青绿色，更高饱和度
	case 3:  // 稀有
		return IM_COL32(85, 172, 238, 255);        // 亮天蓝色，增强亮度
	case 4:  // 史诗
		return IM_COL32(214, 137, 255, 255);       // 亮紫色，显著提高亮度
	case 5:  // 传说
		return IM_COL32(255, 214, 70, 255);        // 亮金色，更耀眼
	case 6:  // 神器
		return IM_COL32(255, 105, 97, 255);        // 亮红色，更明亮
	default: // 默认
		return IM_COL32(220, 220, 220, 230);       // 浅灰色，提高亮度和透明度
	}
}
//
void UGlobal::DrawBigMapPawn(const FVector2D& ScreenPos, const FRotator& Rotation, const ImU32& color) {
	// --- 你已有的绘制圆形的代码 (保持不变) ---
	const float CircleOuterRadius = SCALE(10.0f);
	const float CircleBorderWidth = SCALE(2.0f);
	const float CircleFillRadius = CircleOuterRadius - (CircleBorderWidth / 2.0f);

	ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(ScreenPos.X, ScreenPos.Y), CircleFillRadius, color);
	// --- 开始绘制外部指向三角形 (修改后的代码) ---

	// 1. 定义可调节的参数
	const float DistanceFromCircle = SCALE(1.0f);      // 三角形底边与圆边框之间的距离
	const float TriangleBaseWidth = SCALE(12.0f);      // NEW: 三角形底边的总宽度 (越大越胖)
	const float TriangleHeight = SCALE(6.0f);          // NEW: 从底边到尖端的高度 (越小越矮)


	float YawRadians = Rotation.Yaw * M_PI / 180.0f;

	// 计算旋转后的方向向量
	FVector2D DirVector(cos(YawRadians), sin(YawRadians));

	auto MapX = UEGlobal.BigMapHUDController->get_Mapinfo().MapX;
	if (MapX == 330062 || MapX == 400062) {
		DirVector = FVector2D(-sin(YawRadians), cos(YawRadians));
	}

	// 3. 计算垂直于方向的向量 (保持不变)
	FVector2D PerpendicularVector(-DirVector.Y, DirVector.X);

	// 4. 计算三角形底边的中心点 (BaseCenter) (保持不变)
	ImVec2 BaseCenter = ImVec2(
		ScreenPos.X + DirVector.X * (CircleOuterRadius + DistanceFromCircle),
		ScreenPos.Y + DirVector.Y * (CircleOuterRadius + DistanceFromCircle)
	);

	// 5. 从底边中心点计算三角形的另外两个顶点 (P2, P3)
	// NEW: 使用 TriangleBaseWidth 来控制宽度，这会让 P2 和 P3 更往外
	ImVec2 P2 = ImVec2(
		BaseCenter.x - PerpendicularVector.X * (TriangleBaseWidth / 2.0f),
		BaseCenter.y - PerpendicularVector.Y * (TriangleBaseWidth / 2.0f)
	);
	ImVec2 P3 = ImVec2(
		BaseCenter.x + PerpendicularVector.X * (TriangleBaseWidth / 2.0f),
		BaseCenter.y + PerpendicularVector.Y * (TriangleBaseWidth / 2.0f)
	);

	// 6. 计算三角形尖端的位置 (P1)
	// NEW: 从 BaseCenter 向前（沿方向向量）移动 TriangleHeight 的距离，而不是一个固定的大值。
	// 这让你可以精确控制 P1 的“短”的程度。
	ImVec2 P1 = ImVec2(
		BaseCenter.x + DirVector.X * TriangleHeight,
		BaseCenter.y + DirVector.Y * TriangleHeight
	);

	// 7. 绘制实心三角形
	ImGui::GetBackgroundDrawList()->AddTriangleFilled(P1, P2, P3, color);
}

void UGlobal::DrawLine(const FVector2D& from, const FVector2D& to, const ImVec4& col, float thicknes) {
	ImGui::GetBackgroundDrawList()->AddLine((ImVec2&)from, (ImVec2&)to, get_clr(col), thicknes);
}
void UGlobal::DrawRect(FVector2D* coords, const ImVec4& col, float thickness) {
	ImDrawList* draw = ImGui::GetBackgroundDrawList();
	ImVec2        tl = *(ImVec2*)&coords[0];   // top-left
	ImVec2        br = *(ImVec2*)&coords[1];   // bottom-right
	ImU32       color = get_clr(col);

	float w = br.x - tl.x;
	float h = br.y - tl.y;
	float len = min(w, h) * 0.25f;       // 角长

	draw->AddLine({ tl.x, tl.y }, { tl.x + len, tl.y }, color, thickness);                 // 左上横
	draw->AddLine({ br.x - len, tl.y }, { br.x, tl.y }, color, thickness);                 // 右上横
	draw->AddLine({ tl.x, br.y }, { tl.x + len, br.y }, color, thickness);                 // 左下横
	draw->AddLine({ br.x - len, br.y }, { br.x, br.y }, color, thickness);                 // 右下横
	draw->AddLine({ tl.x, tl.y }, { tl.x, tl.y + len }, color, thickness);                 // 左上竖
	draw->AddLine({ br.x, tl.y }, { br.x, tl.y + len }, color, thickness);                 // 右上竖
	draw->AddLine({ tl.x, br.y - len }, { tl.x, br.y }, color, thickness);                 // 左下竖
	draw->AddLine({ br.x, br.y - len }, { br.x, br.y }, color, thickness);                 // 右下竖
}
void UGlobal::QueueText(ImFont* Font, float FontSize, const std::string& text, ImVec2 pos, ImColor color, bool center) {
	if (Font == nullptr) {
		Font = Overlay::NormalFont;
	}
	if (FontSize == 0.0f) {
		FontSize = SCALE(Font->FontSize);
	}
	float left = pos.x;
	if (center) {
		left -= Font->CalcTextSizeA(FontSize, FLT_MAX, 0.f, text.c_str()).x / 2.f;
	}
	ImGui::GetBackgroundDrawList()->AddText(Font, FontSize, ImVec2(left, pos.y), get_clr(color), text.c_str());
}
void UGlobal::RenderSkinnedMesh(const BoneInfo* InBoneInfo, const ImVec4& col, const ImVec4& InvisibleColor, float thicknes) {
	static UGlobal* _this = nullptr;
	auto DrawLine = [](const BoneInfo& from, const BoneInfo& to, const ImVec4& col, const ImVec4& InvisibleColor, float thicknes) ->void {
		_this->DrawLine(from.screen, to.screen, from.visible ? col : InvisibleColor, thicknes);
		};
	_this = this;
	// 躯干部分
	DrawLine(InBoneInfo[BoneID::Head], InBoneInfo[BoneID::Neck], col, InvisibleColor, thicknes);
	DrawLine(InBoneInfo[BoneID::Neck], InBoneInfo[BoneID::Hips], col, InvisibleColor, thicknes);

	// 右臂部分
	DrawLine(InBoneInfo[BoneID::Neck], InBoneInfo[BoneID::RightShoulder], col, InvisibleColor, thicknes);
	DrawLine(InBoneInfo[BoneID::RightShoulder], InBoneInfo[BoneID::RightArm], col, InvisibleColor, thicknes);
	DrawLine(InBoneInfo[BoneID::RightArm], InBoneInfo[BoneID::RightForeArm], col, InvisibleColor, thicknes);
	DrawLine(InBoneInfo[BoneID::RightForeArm], InBoneInfo[BoneID::RightHand], col, InvisibleColor, thicknes);

	// 右腿部分
	DrawLine(InBoneInfo[BoneID::Hips], InBoneInfo[BoneID::RightUpLeg], col, InvisibleColor, thicknes);
	DrawLine(InBoneInfo[BoneID::RightUpLeg], InBoneInfo[BoneID::RightLeg], col, InvisibleColor, thicknes);
	DrawLine(InBoneInfo[BoneID::RightLeg], InBoneInfo[BoneID::RightFoot], col, InvisibleColor, thicknes);

	// 左臂部分
	DrawLine(InBoneInfo[BoneID::Neck], InBoneInfo[BoneID::LeftShoulder], col, InvisibleColor, thicknes);
	DrawLine(InBoneInfo[BoneID::LeftShoulder], InBoneInfo[BoneID::LeftArm], col, InvisibleColor, thicknes);
	DrawLine(InBoneInfo[BoneID::LeftArm], InBoneInfo[BoneID::LeftForeArm], col, InvisibleColor, thicknes);
	DrawLine(InBoneInfo[BoneID::LeftForeArm], InBoneInfo[BoneID::LeftHand], col, InvisibleColor, thicknes);

	// 左腿部分
	DrawLine(InBoneInfo[BoneID::Hips], InBoneInfo[BoneID::LeftUpLeg], col, InvisibleColor, thicknes);
	DrawLine(InBoneInfo[BoneID::LeftUpLeg], InBoneInfo[BoneID::LeftLeg], col, InvisibleColor, thicknes);
	DrawLine(InBoneInfo[BoneID::LeftLeg], InBoneInfo[BoneID::LeftFoot], col, InvisibleColor, thicknes);
}
void UGlobal::DrawScalingHealthBar(const ImVec2* coords, float currentHP, float maxHP, float barWidth, float minBarHeight, float spacing) {
	ImDrawList* drawList = ImGui::GetBackgroundDrawList();
	if (!drawList) return;

	// 数值安全处理
	currentHP = max(0.0f, min(currentHP, maxHP));
	maxHP = max(maxHP, 0.01f);
	const float hpRatio = currentHP / maxHP;

	// 计算矩形尺寸
	const float rectHeight = coords[1].y - coords[0].y;
	const float rectCenterY = coords[0].y + rectHeight * 0.5f;
	const float barHeight = 2.0f;
	const float barX = coords[0].x - spacing - barWidth;
	const float barY = coords[1].y + 2;
	const float fullWidth = coords[1].x - coords[0].x;
	const float hpWidth = fullWidth * hpRatio;
	const ImVec2 hpTopLeft(coords[0].x, barY);
	const ImVec2 hpBottomRight(coords[0].x + hpWidth, barY + barHeight);

	uint8_t red = (uint8_t)(255 * (1.0f - hpRatio));
	uint8_t green = (uint8_t)(255 * hpRatio);
	red = max(red, 50);
	green = max(green, 50);

	drawList->AddRectFilled(hpTopLeft, hpBottomRight, IM_COL32(red, green, 0, 255));

	const ImVec2 bgBottomRight(coords[1].x, barY + barHeight);
	drawList->AddRectFilled(hpTopLeft, bgBottomRight, IM_COL32(0, 0, 0, 120));

}
//世界坐标转大地图坐标
FVector2D UGlobal::GetDBMapScreenPos(Mapinfo Map, FVector Location, ImVec2 DisplaySize) {
	ImVec2 MapPos = { float(Map.MapX) + Location.X - Map.X ,float(Map.MapY) + Location.Y - Map.Y };
	return { MapPos.x / Map.W * DisplaySize.x,MapPos.y / Map.H * DisplaySize.y };
}
FVector2D UGlobal::GetCGMapScreenPos(Mapinfo Map, FVector Location, ImVec2 DisplaySize) {
	ImVec2 MapPos = { float(Map.MapX) * 1.00675f - Location.Y - Map.X  ,float(Map.MapY) * 0.99805f + Location.X - Map.Y };
	return  { MapPos.x / Map.W * DisplaySize.x,MapPos.y / Map.H * DisplaySize.y };
}
FVector2D UGlobal::GetCGMapScreenPos2(Mapinfo Map, FVector Location, ImVec2 DisplaySize) {
	ImVec2 MapPos = { (float(Map.MapX) * 0.8305726f) - Map.X - Location.Y   ,(float(Map.MapY) * 0.8889654f) - Map.Y + Location.X };

	return  { MapPos.x / Map.W * DisplaySize.x,MapPos.y / Map.H * DisplaySize.y };
}
FVector2D UGlobal::GetMapScreenPos(Mapinfo Map, FVector Location) {
	FVector2D ScreenPos;
	if (Map.MapX == 330062)
		ScreenPos = UEGlobal.GetCGMapScreenPos(Map, Location, ImVec2(UEGlobal.SizeX, UEGlobal.SizeY));
	else if (Map.MapX == 400062)
		ScreenPos = UEGlobal.GetCGMapScreenPos2(Map, Location, ImVec2(UEGlobal.SizeX, UEGlobal.SizeY));
	else
		ScreenPos = UEGlobal.GetDBMapScreenPos(Map, Location, ImVec2(UEGlobal.SizeX, UEGlobal.SizeY));
	return ScreenPos;
}
//堆叠渲染
TextStackManager LootingTextManager, ContainerTextManager;
float TextStackManager::CalculateScreenDistance(const ImVec2& a, const ImVec2& b) const {
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	return sqrt(dx * dx + dy * dy); // 计算平方根得到实际距离
}
void TextStackManager::AddText(int fontSize, const std::string& text, const ImVec2& pos, const ImColor& color, float dist) {
	textList.push_back({ fontSize,text, pos, color, dist });
}
void TextStackManager::DisplayStackedTexts(float maxScreenRange, float maxDistance) {
	// 简单的聚类逻辑：将相近的文本合并
	std::vector<std::vector<TextInfo>> clusters;

	for (const auto& textInfo : textList) {
		bool added = false;

		// 检查是否可以加入已有聚类
		for (auto& cluster : clusters) {
			// 取聚类中第一个元素作为参考
			const auto& ref = cluster[0];

			// 检查是否在屏幕范围内且距离相近
			float screenDist = CalculateScreenDistance(textInfo.position, ref.position);

			if (screenDist < maxScreenRange &&
				fabs(textInfo.distance - ref.distance) < maxDistance) {

				cluster.push_back(textInfo);
				added = true;
				break;
			}
		}

		// 如果没加入任何聚类，则创建新聚类
		if (!added) {
			clusters.push_back({ textInfo });
		}
	}

	for (const auto& cluster : clusters) {
		if (cluster.empty()) continue;

		// 计算聚类的平均位置作为显示位置
		ImVec2 avgPos(0, 0);
		for (const auto& textInfo : cluster) {
			avgPos.x += textInfo.position.x;
			avgPos.y += textInfo.position.y;
		}
		avgPos.x /= cluster.size();
		avgPos.y /= cluster.size();

		// 显示合并后的文本
		const float fontSize = cluster[0].fontSize;
		// 堆叠显示聚类中的所有文本
		float verticalOffset = 0;
		for (const auto& textInfo : cluster) {
			// 假设UEGlobal是一个全局可用的对象
			UEGlobal.QueueText(Overlay::NormalFont, fontSize, textInfo.text.c_str(),
				ImVec2(avgPos.x, avgPos.y + verticalOffset),
				textInfo.color, false);
			verticalOffset += fontSize;
		}
	}

	// 清空列表，准备下一轮
	Clear();
}
void TextStackManager::Clear() {
	textList.clear();
}
//FUNCTION
void Engine::Intialize() {
	while (!(hUnrealWindow = Engine::FindUnrealWindow()))Sleep(500);
	Sleep(2000);
	while (!(UEGlobal.GEngine = UMTAPI_UEngine::GetEngine()))Sleep(500);
	while (!(UEGlobal.GameViewport = UEGlobal.GEngine->get_GameViewport()))Sleep(500);
	UEGlobal.InitBoneName();
	HookPostRender();
}
HWND Engine::hUnrealWindow = nullptr;
HWND Engine::FindUnrealWindow() {
	HWND window = nullptr;
	if (window == nullptr) {
		DWORD dwForegroundWindowProcessID = 0;
		DWORD dwProcessId = GetCurrentProcessId();
		do
		{
			window = ::FindWindowExA(nullptr, window, obf("UnrealWindow"), nullptr);
			if (window && GetWindowThreadProcessId(window, &dwForegroundWindowProcessID) && dwForegroundWindowProcessID == dwProcessId)break;
		} while (window);
	}
	return window;
}
//PostRender
DECLARE_NAMESPACE_DETOUR(Engine, PostRender)
void* Engine::fnPostRender = nullptr;
void Engine::HookPostRender() {

	if (fnPostRender == nullptr) {
		fnPostRender = UEGlobal.GameViewport->GetVTableFunc(Off::UCanvas::PostRender);
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&fnPostRender, &DetourPostRender);
		DetourTransactionCommit();
	}

}
void Engine::IndirectPostRender(DWORD_PTR Rsp) {
	if (Overlay::NormalFont == nullptr) {
		Overlay::CreateOverlay(hUnrealWindow, 18.0f);
		return;
	}
	if (Overlay::StartRender()) {
		UEGlobal.GameViewport = *(UGameViewportClient**)(Rsp - 4 * 8);
		UEGlobal.GameCanvas = *(UCanvas**)(Rsp - 5 * 8);
		hkPostRender(UEGlobal.GameViewport, UEGlobal.GameCanvas);
		{
			static bool IsRender = true;
			if (IsRender) {
				gui.render();
			}
			if (GetAsyncKeyState(VK_INSERT) & 1) {
				IsRender = !IsRender;
			}
		}
		Overlay::EndRender();
	}
}
void Engine::hkPostRender(UGameViewportClient* ViewportClient, UCanvas* Canvas) {
	if (UpdataObjects()) {
		CacheObjects();
		if (UEGlobal.BigMapHUDController && UEGlobal.BigMapHUDController->get_preShouldDraw())
		{
			//渲染自己
			Mapinfo MapInfo = UEGlobal.BigMapHUDController->get_Mapinfo();
			FVector LocationPosition = UEGlobal.LocalGPCharacter->K2_GetRootComponent()->K2_GetComponentLocation();
			FVector2D ScreenPos = UEGlobal.GetMapScreenPos(MapInfo, LocationPosition);
			FRotator Rotator = UEGlobal.LocalGPCharacter->K2_GetRootComponent()->K2_GetComponentRotation();
			UEGlobal.DrawBigMapPawn(ScreenPos, Rotator, 0xa00000ff);

			RendBigMapPawn();
			RenderBigMapPickup();
			RenderBigMapInteractorProxy();
		}
		else
		{
			RenderPawn();
			RenderPickup();
			RenderInteractorProxy();
		}
	}
}
//mouse_move
void Engine::mouse_move(int x, int y) {

	double distance = sqrt(x * x + y * y);
	if (distance > SCALE(10)) {
		// 总时长固定为100ms
		const float duration = 100.0f;
		// 计算每帧的时间（毫秒）
		const float frameTime = 1000.0f / ImGui::GetIO().Framerate;
		// 每帧时间占总时长的比例（用于动态调整缓动幅度）
		const float timeRatio = frameTime / duration;
		// 限制缓动系数范围（0.01~1）
		float smoothFactorX = set->Aimbot.SmoothFactorX;
		float smoothFactorY = set->Aimbot.SmoothFactorY;
		smoothFactorX = max(0.01f, min(1.0f, smoothFactorX));
		smoothFactorY = max(0.01f, min(1.0f, smoothFactorY));

		x = x * (smoothFactorX + (1.0f - smoothFactorX) * timeRatio);
		y = y * (smoothFactorY + (1.0f - smoothFactorY) * timeRatio);
	}
	if (x && y) {
		mouse_event(MOUSEEVENTF_MOVE, x, y, 0, 0);
	}

}
//遍历数据
bool Engine::UpdataObjects() {
	//判断游戏模式
	EMainFlowState MainFlowState = UGPScalabilityBlueprintTools::GetMainFlowState();
	if (MainFlowState != EMainFlowState::SafeHouse && MainFlowState != EMainFlowState::InGame) {
		UEGlobal.Cache_Clear();
		UEGlobal.BigMapHUDController = nullptr;
		return false;
	}
	//加载数据
	if (!(UEGlobal.GWorld = UEGlobal.GameViewport->get_World()))return false;
	if (!(UEGlobal.PlayerCameraManager = UGPWeaponBlueprintLibrary::GetPlayerCameraManager(UEGlobal.GWorld)))return false;
	if (!(UEGlobal.PlayerController = UGPWeaponBlueprintLibrary::GetPlayerController(UEGlobal.GWorld)))return false;
	if (!(UEGlobal.LocalGPCharacter = UGameplayBlueprintHelper::GetLocalGPCharacter(UEGlobal.GWorld)))return false;
	if (!(UEGlobal.LocalTeamComp = UEGlobal.LocalGPCharacter->GetTeamComp()))return false;
	UEGlobal.SizeX = UEGlobal.GameCanvas->get_SizeX();
	UEGlobal.SizeY = UEGlobal.GameCanvas->get_SizeY();
	UEGlobal.CameraRotation = UEGlobal.PlayerCameraManager->GetCameraRotation();
	UEGlobal.ControlRotation = UEGlobal.PlayerController->get_ControlRotation();
	UEGlobal.CameraLocation = UEGlobal.PlayerCameraManager->GetCameraLocation();
	if (MainFlowState == EMainFlowState::InGame) CacheMapController();
	return true;
}
//缓存地图组件
void Engine::CacheMapController() {
	if (UEGlobal.BigMapHUDController == nullptr && UEGlobal.PlayerController) {
		auto MyHUD = UEGlobal.PlayerController->GetHUD();
		if (MyHUD) {
			auto Controllers = MyHUD->Cast<ABaseHUD>()->get_Controllers();
			if (!Controllers.IsValid()) return;
			for (auto Controller : Controllers) {
				auto ChildControllers = Controller->get_ChildControllers();
				if (!ChildControllers.IsValid())continue;
				for (auto ChildController : ChildControllers) {
					if (!ChildController->IsA(USOLMapHUDController::StaticClass()))continue;
					auto View = ChildController->Cast<UBatchUIController>()->get_View();
					if (View && View->IsA(UBigMapHUDView::StaticClass())) {
						UEGlobal.BigMapHUDController = View->Cast<UBigMapHUDView>();
						break;
					}
				}
			}
		}
	}
}
//缓存数据
void Engine::CacheObjects() {
	//间隔1s遍历一次
	static DWORD64 LastTime = 0;
	auto curTime = GetTickCount64();
	if (LastTime + 500 > curTime)return;
	LastTime = curTime;
	/*遍历数据并分配*/
	TArray<AActor*> actors;
	UEGlobal.PawnList.clear();
	UEGlobal.PickupList.clear();
	UEGlobal.OtherItemList.clear();
	UGameplayStatics::GetAllActorsOfClass(UEGlobal.GWorld, AActor::StaticClass(), &actors);
	for (int i = 0; i < actors.Num(); i++)
	{
		if (!actors.IsValidIndex(i)) continue;
		auto actor = actors[i];
		//玩家
		if (actor->IsA(EClassCastFlags::Pawn) && actor->IsA(ADFMCharacter::StaticClass())) {
			UEGlobal.PawnList.push_back(reinterpret_cast<ADFMCharacter*>(actor));
		}
		else if (actor->IsA(EClassCastFlags::Actor) && actor->IsA(AInteractorBase::StaticClass())) {
			AInteractorBase* Interactor = reinterpret_cast<AInteractorBase*>(actor);
			EMarkingItemType ItemType = Interactor->get_MarkingItemType();
			if (ItemType == EMarkingItemType::LootingItem) {
				if (Interactor->IsA(AInventoryPickup::StaticClass())) {
					UEGlobal.PickupList.push_back(Interactor->Cast<AInventoryPickup>());
				}
			}
			else if (ItemType == EMarkingItemType::Container) {
				if (Interactor->IsA(AInventoryPickup_OpenBox::StaticClass())) {
					UEGlobal.PickupList.push_back(Interactor->Cast<AInventoryPickup>());
				}
			}
			else if (ItemType == EMarkingItemType::DeadBody) {
				if (Interactor->IsA(AInventoryPickup_DeadBody::StaticClass())) {
					UEGlobal.PickupList.push_back(Interactor->Cast<AInventoryPickup>());
				}
			}
			if (ItemType == EMarkingItemType::CommonItem) {
				if (Interactor->IsA(AInteractor_HackPC::StaticClass())) {
					UEGlobal.OtherItemList.push_back({ 0,actor });
				}
				else if (Interactor->IsA(AInteractor_CodedLock::StaticClass())) {
					UEGlobal.OtherItemList.push_back({ 1,actor });
				}
				else if (Interactor->IsA(AInteractor_ExitTrigger::StaticClass())) {
					UEGlobal.OtherItemList.push_back({ 2,actor });
				}
			}
		}
		else if (actor->IsA(APlayerExitBase::StaticClass())) {
			UEGlobal.OtherItemList.push_back({ 3,actor });
		}
	}
	UObject::Free((void*)actors.GetDataPtr());
}
//渲染玩家
void Engine::RenderPawn() {
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec2 HeroTextSize;
	BoneInfo m_BoneInfo[17];
	AimBot.Clear();//清理
	for (size_t i = 0; i < UEGlobal.PawnList.size(); i++)
	{
		ADFMCharacter* Character = UEGlobal.PawnList[i];
		if (Character == nullptr || Character == UEGlobal.LocalGPCharacter || Character->IsDead())continue;
		//保存指针
		UGPTeamComponent* TeamComp = nullptr;
		UGPHealthDataComponent* HealthComp = nullptr;
		USceneComponent* SceneComp = nullptr;
		USkeletalMeshComponent* SkinnedMeshComp = nullptr;
		APlayerState* PlayerState = nullptr;
		int dwCamp = 0, dwTeamId = 0;
		if (!(TeamComp = Character->GetTeamComp()))continue;
		if (!(HealthComp = Character->GetHealthComp()))continue;
		if (!(SceneComp = Character->K2_GetRootComponent()))continue;
		FVector Location = SceneComp->K2_GetComponentLocation();//坐标
		if (Location.IsZero())continue;
		dwCamp = TeamComp->GetCamp(); dwTeamId = TeamComp->GetTeamID();
		if (!(SkinnedMeshComp = UMTAPI_ACharacter::GetMesh(Character)))continue;
		//过滤
		{
			//全面战场过滤队友
			if (set->Setting.game_mode == 0 && dwCamp == UEGlobal.LocalTeamComp->GetCamp())continue;
			//烽火模式过滤队友
			if (set->Setting.game_mode == 1 && dwTeamId == UEGlobal.LocalTeamComp->GetTeamID())continue;
			PlayerState = Character->get_PlayerState();
			//过滤掉撤离玩家
			if (set->Setting.game_mode == 1 && PlayerState && PlayerState->Cast<ADFMPlayerState>()->get_ExitState() == EExitState::Escaped)continue;
		}
		//判断AI
		bool IsAI = !PlayerState || Character->IsAI();
		std::string PlayerName = IsAI ? UEGlobal.GetPoolChosenName(Character) : UEGlobal.GetPlayerNamePrivate(PlayerState);
		std::string HeroName = IsAI ? "" : UEGlobal.GetPlayerHeroName(PlayerState);
		PawnType pawnType = UEGlobal.GetPawnType(Character, PlayerName, IsAI);
		FRotator Rotation = SceneComp->K2_GetComponentRotation();//旋转
		float Distance = UEGlobal.CameraLocation.GetDistanceTo(Location) / 100.0f;
		//获取绘制参数
		c_set::c_Pawn* DrawPawn = nullptr;
		{
			if (pawnType == PawnType::None) DrawPawn = &set->Player;
			else if (pawnType == PawnType::Boss) DrawPawn = &set->Boss;
			else if (pawnType == PawnType::Elite) DrawPawn = &set->Elite;
			else DrawPawn = &set->Robot;
		}
		FVector2D coords[2]; bool isNotFullyOutside = false;
		if (PawnType::Animal != pawnType && UEGlobal.WorldLocationToScreenRect(Location, coords)) {
			if ((isNotFullyOutside = UEGlobal.IsInScreen(coords[0]) || UEGlobal.IsInScreen(coords[1]))) {
				//获取骨骼坐标
				bool visible = UEGlobal.GetSkinnedMesh(Character, SkinnedMeshComp, m_BoneInfo);
				if (visible) AimBot.AddTarget(Character, m_BoneInfo, pawnType);

				//字体
				ImFont* Font = Overlay::NormalFont; float FontSize = SCALE(DrawPawn->fontSize);
				//设置初始化字体绘制
				ImVec2 top_pos(coords[0].X + (coords[1].X - coords[0].X) / 2.0f, coords[0].Y);
				ImVec2 bottom_pos(coords[0].X + (coords[1].X - coords[0].X) / 2.0f, coords[1].Y);
				if (Distance <= DrawPawn->maxDistance && DrawPawn->Enable) {
					//绘制方框
					if (DrawPawn->showBox) {
						UEGlobal.DrawRect(coords, visible ? DrawPawn->BoxColor : DrawPawn->BoxInvisibleColor, SCALE(DrawPawn->BoxLine));
					}
					//绘制骨骼
					if (DrawPawn->showSkeleton) {
						UEGlobal.RenderSkinnedMesh(m_BoneInfo, DrawPawn->SkeletonColor, DrawPawn->BoxInvisibleColor, SCALE(DrawPawn->SkeletonLine));
					}
					//绘制血条
					if (DrawPawn->showHealth) {
						UEGlobal.DrawScalingHealthBar((ImVec2*)coords, HealthComp->GetHealth(), HealthComp->GetHealthMax(), SCALE(3.5f), SCALE(20.0f), SCALE(3.0f));
					}
					//玩家名字
					if (DrawPawn->showName) {
						top_pos.y -= FontSize;
						UEGlobal.QueueText(Font, FontSize, PlayerName, top_pos, DrawPawn->NameColor, true);
					}
					//绘制玩家职业
					if (PawnType::None == pawnType && DrawPawn->showHero) {
						top_pos.y -= FontSize;
						HeroTextSize = Font->CalcTextSizeA(FontSize, FLT_MAX, 0, HeroName.c_str());
						ImGui::GetBackgroundDrawList()->AddText(Font, FontSize, ImVec2(top_pos.x - HeroTextSize.x / 2.0f, top_pos.y), UEGlobal.get_clr(DrawPawn->HeroColor), HeroName.c_str());
					}
					//绘制玩家队标
					if (PawnType::None == pawnType && DrawPawn->showTeamId) {
						ImVec2 org_top_pos = top_pos;
						if (DrawPawn->showName && DrawPawn->showHero) {
							top_pos.y += FontSize;
							top_pos.x -= HeroTextSize.x / 2.0f + FontSize;
						}

						ImDrawList* drawList = ImGui::GetBackgroundDrawList();
						float circleSize = FontSize;
						std::string teamId = std::to_string(dwTeamId);
						ImVec2 text_size = Font->CalcTextSizeA(FontSize, FLT_MAX, 0, teamId.c_str());
						//绘制背景色
						drawList->AddCircleFilled({ top_pos.x,top_pos.y - circleSize / 2.0f }, circleSize / 2.0f, UEGlobal.GetColorForNumber(dwTeamId), 360);
						//绘制对标
						drawList->AddText(Font, FontSize, ImVec2(top_pos.x - text_size.x / 2, top_pos.y - circleSize / 2.0f - text_size.y / 2.0f), 0xa0ffffff, teamId.c_str());
						top_pos.x = org_top_pos.x;
						top_pos.y = org_top_pos.y - circleSize;

					}
					//绘制武器
					if (DrawPawn->showWeapon) {
						auto PlayerWeapName = UEGlobal.GetPlayerWeapName(Character);
						if (PlayerWeapName.empty() == false) {
							UEGlobal.QueueText(Font, FontSize, PlayerWeapName.c_str(), bottom_pos, DrawPawn->WeaponColor, true);
							bottom_pos.y += FontSize;
						}
					}
					//绘制距离
					if (DrawPawn->showDistance) {
						UEGlobal.QueueText(Font, FontSize, std::to_string((int)Distance) + "m", bottom_pos, DrawPawn->DistanceColor, true);
						bottom_pos.y += FontSize;
					}

					//绘制装备等级
					if (DrawPawn->showHelmet) {
						int HelmetLevel = 0, BreastPlateLevel = 0, ChestHangingLevel = 0, BagLevel = 0;
						UCharacterEquipComponent* EquipComp = UCharacterEquipComponent::Get(Character);
						if (EquipComp) {
							FEquipmentInfo HelmetInfo = EquipComp->GetEquipmentInfoByType(EEquipmentType::Helmet);
							FEquipmentInfo BreastPlateInfo = EquipComp->GetEquipmentInfoByType(EEquipmentType::BreastPlate);
							FEquipmentInfo ChestHangingInfo = EquipComp->GetEquipmentInfoByType(EEquipmentType::ChestHanging);
							FEquipmentInfo BagInfo = EquipComp->GetEquipmentInfoByType(EEquipmentType::Bag);
							if (HelmetInfo.ItemID)  HelmetLevel = (HelmetInfo.ItemID % 1000000) / 1000;
							if (BreastPlateInfo.ItemID)BreastPlateLevel = (BreastPlateInfo.ItemID % 1000000) / 1000;
							if (ChestHangingInfo.ItemID)  ChestHangingLevel = (ChestHangingInfo.ItemID % 1000000) / 1000;
							if (BagInfo.ItemID)BagLevel = (BagInfo.ItemID % 1000000) / 1000;
						}
						if (HelmetLevel || BreastPlateLevel || ChestHangingLevel || BagLevel) {
							ImVec2 pos = { coords[1].X + SCALE(5),coords[0].Y };
							char textBuffer[32]; // 复用缓冲区
							if (DrawPawn->miniHelmetLevel != 0 && HelmetLevel >= DrawPawn->miniHelmetLevel) {
								sprintf_s(textBuffer, u8"H%d ", HelmetLevel);
								UEGlobal.QueueText(Font, FontSize, textBuffer, pos, UEGlobal.GetItemLevelColor(HelmetLevel), false);
								pos.y += FontSize * 0.85f;
							}
							if (DrawPawn->miniBreastPlateLevel != 0 && BreastPlateLevel >= DrawPawn->miniBreastPlateLevel) {
								sprintf_s(textBuffer, u8"B%d ", BreastPlateLevel);
								UEGlobal.QueueText(Font, FontSize, textBuffer, pos, UEGlobal.GetItemLevelColor(BreastPlateLevel), false);
								pos.y += FontSize * 0.85f;
							}
							if (DrawPawn->miniChestHangingLevel != 0 && ChestHangingLevel >= DrawPawn->miniChestHangingLevel) {
								sprintf_s(textBuffer, u8"C%d ", ChestHangingLevel);
								UEGlobal.QueueText(Font, FontSize, textBuffer, pos, UEGlobal.GetItemLevelColor(ChestHangingLevel), false);
								pos.y += FontSize * 0.85f;
							}
							if (DrawPawn->miniBagLevel != 0 && BagLevel >= DrawPawn->miniBagLevel) {
								sprintf_s(textBuffer, u8"G%d ", BagLevel);
								UEGlobal.QueueText(Font, FontSize, textBuffer, pos, UEGlobal.GetItemLevelColor(BagLevel), false);
								pos.y += FontSize * 0.85f;
							}
						}
					}
				}
			}
		}
	}
	AimBot.Save();
	AimBot.Draw();
}
//渲染物品
void Engine::RenderPickup() {
	auto FormatWithUnits = [](float value) -> std::string {
		// 转换为整数处理
		long long num = static_cast<long long>(value);
		std::string prefix = "$";

		// 辅助函数：为数字字符串添加千位分隔符
		auto addThousandsSeparator = [](const std::string& numStr) -> std::string {
			std::string result;
			int count = 0;

			// 从后往前每3位插入一个逗号
			for (auto it = numStr.rbegin(); it != numStr.rend(); ++it) {
				if (count > 0 && count % 3 == 0) {
					result += ',';
				}
				result += *it;
				count++;
			}

			// 反转恢复正确顺序
			std::reverse(result.begin(), result.end());
			return result;
			};

		// 处理百万级 (1,000,000+)
		if (num >= 1000000) {
			long long millions = num / 1000000;
			int remainder = num % 1000000 / 100000; // 取小数点后第一位

			std::string millionStr = std::to_string(millions);
			millionStr = addThousandsSeparator(millionStr);

			if (remainder == 0) {
				return prefix + millionStr + "M"; // 没有小数部分
			}
			else {
				return prefix + millionStr + "." + std::to_string(remainder) + "M"; // 有小数部分
			}
		}
		// 处理千级 (1,000+)
		else if (num >= 1000) {
			long long thousands = num / 1000;
			int remainder = num % 1000;

			std::string thousandStr = std::to_string(thousands);
			thousandStr = addThousandsSeparator(thousandStr);

			if (thousands >= 100) {
				// 大于100k时保留一位小数
				int firstDecimal = remainder / 100;
				if (firstDecimal == 0) {
					return prefix + thousandStr + "k";
				}
				else {
					return prefix + thousandStr + "." + std::to_string(firstDecimal) + "k";
				}
			}
			else {
				// 1k-99.9k保留两位小数
				int firstDecimal = remainder / 100;
				int secondDecimal = (remainder % 100) / 10;

				if (firstDecimal == 0 && secondDecimal == 0) {
					return prefix + thousandStr + "k";
				}
				else if (secondDecimal == 0) {
					return prefix + thousandStr + "." + std::to_string(firstDecimal) + "k";
				}
				else {
					return prefix + thousandStr + "." + std::to_string(firstDecimal) + std::to_string(secondDecimal) + "k";
				}
			}
		}
		// 处理小数值，使用千位分隔符
		else {
			std::string str = std::to_string(num);
			return prefix + addThousandsSeparator(str);
		}
		};
	for (int i = 0; i < UEGlobal.PickupList.size(); i++)
	{
		USceneComponent* SceneComp = nullptr;
		AInventoryPickup* InventoryPickup = nullptr;
		EMarkingItemType ItemType = EMarkingItemType::None;
		InventoryPickup = reinterpret_cast<AInventoryPickup*>(UEGlobal.PickupList[i]);
		if (!(SceneComp = InventoryPickup->K2_GetRootComponent()))continue;
		ItemType = InventoryPickup->get_MarkingItemType();
		//是否拾取物
		if (ItemType == EMarkingItemType::LootingItem) {
			auto& LootingItem = set->Item.LootingItem;
			//判断是否启用
			if (LootingItem.Enable == false)continue;
			FVector Location = SceneComp->K2_GetComponentLocation();
			if (Location.IsZero())continue;
			float Distance = UEGlobal.CameraLocation.GetDistanceTo(Location) / 100.0f;
			//过滤距离
			if (LootingItem.maxDistance < Distance)continue;
			FInventoryItemInfo* pPickupItemInfo = InventoryPickup->get_PickupItemInfoAddress();
			auto ItemRow = pPickupItemInfo->ItemRow;
			if (ItemRow == nullptr)continue;
			//过滤价值
			if (LootingItem.PriceFiltering && set->Item.LootingItem.minGuidePrice > ItemRow->InitialGuidePrice)continue;
			//等级过滤
			if (ItemRow->Quality == 1 && !LootingItem.showLv1)continue;
			if (ItemRow->Quality == 2 && !LootingItem.showLv2)continue;
			if (ItemRow->Quality == 3 && !LootingItem.showLv3)continue;
			if (ItemRow->Quality == 4 && !LootingItem.showLv4)continue;
			if (ItemRow->Quality == 5 && !LootingItem.showLv5)continue;
			if (ItemRow->Quality == 6 && !LootingItem.showLv6)continue;
			//获取物品名称
			std::string ItemName = UEGlobal.GetInventoryPickupName(InventoryPickup);
			if (ItemName.empty())continue; FVector2D coord;
			if (UEGlobal.WorldLocationToScreenPoint(Location, &coord) && UEGlobal.IsInScreen(coord)) {
				ImColor color = UEGlobal.GetItemLevelColor(ItemRow->Quality);
				char ShowText[128];
				sprintf_s(ShowText, sizeof(ShowText), u8"%s(%s)%.fm",
					ItemName.c_str(),
					FormatWithUnits((int)ItemRow->InitialGuidePrice),
					Distance);
				LootingTextManager.AddText(SCALE(LootingItem.fontSize), ShowText, *(ImVec2*)&coord, color, Distance);
			}
		}
		//是否容器
		if (ItemType == EMarkingItemType::Container) {
			auto& OpenBoxItem = set->Item.OpenBox;
			//判断是否启用
			if (OpenBoxItem.Enable == false)continue;
			FVector Location = SceneComp->K2_GetComponentLocation();
			if (Location.IsZero())continue;
			float Distance = UEGlobal.CameraLocation.GetDistanceTo(Location) / 100.0f;
			//过滤距离
			if (OpenBoxItem.maxDistance < Distance)continue;
			//获取容器名称
			std::string ItemName = UEGlobal.GetInventoryPickupName(InventoryPickup);
			if (ItemName.empty())continue; FVector2D coord;
			//判断渲染容器类型和获取颜色
			bool IsItemShow = false; ImColor color;
			auto BoxItem = set->OpenBoxItems_Cache.find(ItemName);
			if (BoxItem != set->OpenBoxItems_Cache.end()) {
				int& BoxLevel = BoxItem->second.second;
				if (BoxLevel >= 0 && BoxLevel < sizeof(OpenBoxItem.showOpenBox) / sizeof(OpenBoxItem.showOpenBox[0])) {
					if (BoxItem->second.first && OpenBoxItem.showOpenBox[BoxLevel]) {
						IsItemShow = true;
						color = OpenBoxItem.OpenBoxColor[BoxLevel];
					}
				}
				else {
					BoxLevel = 0;
				}
			}
			else
			{
				set->OpenBoxItems_Cache.insert({ ItemName,std::make_pair(false,0) });

			}
			if (IsItemShow == false)continue;
			//矩阵转换
			if (UEGlobal.WorldLocationToScreenPoint(Location, &coord) && UEGlobal.IsInScreen(coord)) {
				AInventoryPickup_OpenBox* OpenBox = InventoryPickup->Cast<AInventoryPickup_OpenBox>();
				char ShowText[128];
				if (OpenBox->get_bHasOpened() == false)
				{
					sprintf_s(ShowText, sizeof(ShowText), u8"%s%.fm", ItemName.c_str(), Distance);
					ContainerTextManager.AddText(SCALE(OpenBoxItem.fontSize), ShowText, (ImVec2&)coord, color, Distance);
				}
				else
				{
					//这里显示打开后的容器
					ImVec2 pos = (ImVec2&)coord;
					int fontSize = SCALE(OpenBoxItem.fontSize);
					pos.x -= fontSize * 3.0f;
					//判断是否显示打开容器
					if (OpenBoxItem.openFiltering == false) {
						sprintf_s(ShowText, sizeof(ShowText), u8"%s|开启%.fm", ItemName.c_str(), Distance);
						UEGlobal.QueueText(Overlay::NormalFont, fontSize, ShowText, pos, color, false);
						pos.y -= fontSize;
					}
					if (OpenBoxItem.showPickup) {
						auto pRepItemArray = OpenBox->get_RepItemArrayAddress();
						if (!pRepItemArray->Items.IsValid())continue;
						for (size_t i = 0; i < pRepItemArray->Items.Num(); i++)
						{
							auto& ItemRow = pRepItemArray->Items[i].ItemRow;
							auto& LootingItem = set->Item.LootingItem;
							//过滤价值
							if (LootingItem.PriceFiltering && set->Item.LootingItem.minGuidePrice > ItemRow->InitialGuidePrice)continue;
							//等级过滤
							if (ItemRow->Quality == 1 && !LootingItem.showLv1)continue;
							if (ItemRow->Quality == 2 && !LootingItem.showLv2)continue;
							if (ItemRow->Quality == 3 && !LootingItem.showLv3)continue;
							if (ItemRow->Quality == 4 && !LootingItem.showLv4)continue;
							if (ItemRow->Quality == 5 && !LootingItem.showLv5)continue;
							if (ItemRow->Quality == 6 && !LootingItem.showLv6)continue;
							std::string ItemRowName = UEGlobal.GetItemRowName(ItemRow);
							if (ItemRowName.empty())continue;
							UEGlobal.QueueText(Overlay::NormalFont, fontSize, ItemRowName + "(" + FormatWithUnits(ItemRow->InitialGuidePrice) + ")", pos, UEGlobal.GetItemLevelColor(ItemRow->Quality), false);
							pos.y -= fontSize;
						}
					}
				}
			}
		}
		//是否盒子
		if (ItemType == EMarkingItemType::DeadBody) {
			auto DeadBody = InventoryPickup->Cast<AInventoryPickup_DeadBody>();
			auto& DeadBodyItem = set->Item.DeadBody;
			FVector Location = SceneComp->K2_GetComponentLocation();
			if (Location.IsZero())continue;
			float Distance = UEGlobal.CameraLocation.GetDistanceTo(Location) / 100.0f;
			std::string DeadBodyName = UEGlobal.GetDeadBodyName(DeadBody);
			if (DeadBodyName.empty())continue; FVector2D coord;
			if (UEGlobal.WorldLocationToScreenPoint(Location, &coord) && UEGlobal.IsInScreen(coord)) {
				PawnType pawnType = UEGlobal.GetDeadBodyType(DeadBody, DeadBodyName);
				float fontSize = SCALE(DeadBodyItem.fontSize); ImColor color;
				{
					if (pawnType == PawnType::None) {
						if (DeadBodyItem.showPlayerBox == false || DeadBodyItem.PlayerBoxDistance < Distance)continue;
						color = DeadBodyItem.PlayerBoxColor;
					}
					else if (pawnType == PawnType::Boss) {
						if (DeadBodyItem.showBossBox == false || DeadBodyItem.BossBoxDistance < Distance)continue;
						color = DeadBodyItem.BossBoxColor;
					}
					else if (pawnType == PawnType::Elite) {
						if (DeadBodyItem.showEliteBox == false || DeadBodyItem.EliteBoxDistance < Distance)continue;
						color = DeadBodyItem.EliteBoxColor;
					}
					else if (pawnType == PawnType::Robot) {
						if (DeadBodyItem.showRobotBox == false || DeadBodyItem.RobotBoxDistance < Distance)continue;
						color = DeadBodyItem.RobotBoxColor;
					}
					else continue;
				}
				char ShowText[128];
				//显示未打开盒子
				if (DeadBody->get_bLooted() == false) {
					sprintf_s(ShowText, sizeof(ShowText), u8"%s%.fm", DeadBodyName.c_str(), Distance);
					ContainerTextManager.AddText(fontSize, ShowText, (ImVec2&)coord, color, Distance);
				}
				else
				{
					//这里显示打开后的容器
					ImVec2 pos = (ImVec2&)coord;
					pos.x -= fontSize * 3.0f;
					if (DeadBodyItem.openFiltering == false) {
						sprintf_s(ShowText, sizeof(ShowText), u8"%s|开启%.fm", DeadBodyName.c_str(), Distance);
						UEGlobal.QueueText(Overlay::NormalFont, fontSize, ShowText, pos, color, false);
						pos.y -= fontSize;
					}
					if (DeadBodyItem.showPickup) {
						auto pRepItemArray = DeadBody->get_RepItemArrayAddress();
						if (!pRepItemArray->Items.IsValid())continue;
						for (int i = 0; i < pRepItemArray->Items.Num(); i++)
						{
							auto& ItemRow = pRepItemArray->Items[i].ItemRow;
							auto& LootingItem = set->Item.LootingItem;
							//过滤价值
							if (LootingItem.PriceFiltering && set->Item.LootingItem.minGuidePrice > ItemRow->InitialGuidePrice)continue;
							//等级过滤
							if (ItemRow->Quality == 1 && !LootingItem.showLv1)continue;
							if (ItemRow->Quality == 2 && !LootingItem.showLv2)continue;
							if (ItemRow->Quality == 3 && !LootingItem.showLv3)continue;
							if (ItemRow->Quality == 4 && !LootingItem.showLv4)continue;
							if (ItemRow->Quality == 5 && !LootingItem.showLv5)continue;
							if (ItemRow->Quality == 6 && !LootingItem.showLv6)continue;
							std::string ItemRowName = UEGlobal.GetItemRowName(ItemRow);
							if (ItemRowName.empty())continue;
							UEGlobal.QueueText(Overlay::NormalFont, fontSize, ItemRowName + "(" + FormatWithUnits((int)ItemRow->InitialGuidePrice) + ")", pos, UEGlobal.GetItemLevelColor(ItemRow->Quality), false);
							pos.y -= fontSize;
						}
					}


				}

			}
		}
	}
	LootingTextManager.DisplayStackedTexts(50.0f, SCALE(20.0f));
	ContainerTextManager.DisplayStackedTexts(50.0f, SCALE(20.0f));
}
//渲染其它物资
void Engine::RenderInteractorProxy() {
	char ShowText[128];
	for (int i = 0; i < UEGlobal.OtherItemList.size(); i++)
	{
		auto Item = UEGlobal.OtherItemList[i];
		if (Item.Type == 0 && set->Item.HackPC.Enable) {
			auto HackPC = Item.Object->Cast<AInteractor_HackPC>();
			auto RootComp = HackPC->K2_GetRootComponent();
			auto Location = RootComp->K2_GetComponentLocation();
			if (Location.IsZero())continue;
			float Distance = UEGlobal.CameraLocation.GetDistanceTo(Location) / 100.0f;
			if (set->Item.HackPC.maxDistance < Distance)continue; FVector2D coord;
			if (HackPC->IsMorseCodeSuccess())continue;
			if (UEGlobal.WorldLocationToScreenPoint(Location, &coord) && UEGlobal.IsInScreen(coord)) {
				sprintf_s(ShowText, u8"电脑[%04d]%.fm", HackPC->get_Password(), Distance);
				UEGlobal.QueueText(nullptr, SCALE(set->Item.HackPC.fontSize), ShowText, (ImVec2&)coord, set->Item.HackPC.color, true);
			}
		}
		if (Item.Type == 1 && set->Item.CodeLock.Enable) {
			auto CodedLock = Item.Object->Cast<AInteractor_CodedLock>();
			auto RootComp = CodedLock->K2_GetRootComponent();
			auto Location = RootComp->K2_GetComponentLocation();
			if (Location.IsZero())continue;
			float Distance = UEGlobal.CameraLocation.GetDistanceTo(Location) / 100.0f;
			if (set->Item.CodeLock.maxDistance < Distance)continue; FVector2D coord;
			if (CodedLock->IsPassWordCorrect())continue;
			if (UEGlobal.WorldLocationToScreenPoint(Location, &coord) && UEGlobal.IsInScreen(coord)) {
				std::string PassWorld;
				auto Pwd = CodedLock->get_Pwd();
				for (int i = 0; i < Pwd.Num(); i++)
				{
					PassWorld += std::to_string(Pwd[i]);
				}
				sprintf_s(ShowText, u8"门锁[%s]%.fm", PassWorld.c_str(), Distance);
				UEGlobal.QueueText(nullptr, SCALE(set->Item.CodeLock.fontSize), ShowText, (ImVec2&)coord, set->Item.CodeLock.color, true);
			}
		}
		if (Item.Type == 2 && set->Item.ExitTrigger.Enable) {
			auto ExitTrigger = Item.Object->Cast<AInteractor_ExitTrigger>();
			auto RootComp = ExitTrigger->K2_GetRootComponent();
			auto Location = RootComp->K2_GetComponentLocation();
			if (Location.IsZero())continue;
			float Distance = UEGlobal.CameraLocation.GetDistanceTo(Location) / 100.0f;
			if (set->Item.ExitTrigger.maxDistance < Distance)continue; FVector2D coord;
			if (UEGlobal.WorldLocationToScreenPoint(Location, &coord) && UEGlobal.IsInScreen(coord)) {
				sprintf_s(ShowText, u8"拉闸点%.fm", Distance);
				UEGlobal.QueueText(nullptr, SCALE(set->Item.ExitTrigger.fontSize), ShowText, (ImVec2&)coord, set->Item.ExitTrigger.color, true);
			}
		}
		if (Item.Type == 3 && set->Item.PlayerExit.Enable) {
			auto PlayerExit = Item.Object->Cast<APlayerExitBase>();
			if (PlayerExit->get_bExitActive() == false)continue;
			auto RootComp = PlayerExit->K2_GetRootComponent();
			auto Location = RootComp->K2_GetComponentLocation();
			if (Location.IsZero())continue;
			float Distance = UEGlobal.CameraLocation.GetDistanceTo(Location) / 100.0f;
			if (set->Item.PlayerExit.maxDistance < Distance)continue; FVector2D coord;
			if (UEGlobal.WorldLocationToScreenPoint(Location, &coord) && UEGlobal.IsInScreen(coord)) {
				sprintf_s(ShowText, u8"撤离点%.fm", Distance);
				UEGlobal.QueueText(nullptr, SCALE(set->Item.PlayerExit.fontSize), ShowText, (ImVec2&)coord, set->Item.PlayerExit.color, true);
			}
		}
	}

}
//大地图绘制
void Engine::RendBigMapPawn() {

	ImGuiStyle& style = ImGui::GetStyle();
	Mapinfo Map = UEGlobal.BigMapHUDController->get_Mapinfo();

	ImVec2 HeroTextSize;
	for (int i = 0; i < UEGlobal.PawnList.size(); i++)
	{
		ADFMCharacter* Character = UEGlobal.PawnList[i];
		if (Character == nullptr || Character == UEGlobal.LocalGPCharacter || Character->IsDead())continue;
		//保存指针
		UGPTeamComponent* TeamComp = nullptr;
		UGPHealthDataComponent* HealthComp = nullptr;
		USceneComponent* SceneComp = nullptr;
		USkeletalMeshComponent* SkinnedMeshComp = nullptr;
		APlayerState* PlayerState = nullptr;
		int dwCamp = 0, dwTeamId = 0;
		if (!(TeamComp = Character->GetTeamComp()))continue;
		if (!(HealthComp = Character->GetHealthComp()))continue;
		if (!(SceneComp = Character->K2_GetRootComponent()))continue;
		FVector Location = SceneComp->K2_GetComponentLocation();//坐标
		if (Location.IsZero())continue;
		dwCamp = TeamComp->GetCamp(); dwTeamId = TeamComp->GetTeamID();
		if (!(SkinnedMeshComp = UMTAPI_ACharacter::GetMesh(Character)))continue;
		//过滤
		{
			//全面战场过滤队友
			if (set->Setting.game_mode == 0 && dwCamp == UEGlobal.LocalTeamComp->GetCamp())continue;
			//烽火模式过滤队友
			if (set->Setting.game_mode == 1 && dwTeamId == UEGlobal.LocalTeamComp->GetTeamID())continue;
			PlayerState = Character->get_PlayerState();
			//过滤掉撤离玩家
			if (set->Setting.game_mode == 1 && PlayerState && PlayerState->Cast<ADFMPlayerState>()->get_ExitState() == EExitState::Escaped)continue;
		}
		//判断AI
		bool IsAI = !PlayerState || Character->IsAI();
		std::string PlayerName = IsAI ? UEGlobal.GetPoolChosenName(Character) : UEGlobal.GetPlayerNamePrivate(PlayerState);
		std::string HeroName = IsAI ? "" : UEGlobal.GetPlayerHeroName(PlayerState);
		PawnType pawnType = UEGlobal.GetPawnType(Character, PlayerName, IsAI);
		FRotator Rotation = SceneComp->K2_GetComponentRotation();//旋转
		float Distance = UEGlobal.CameraLocation.GetDistanceTo(Location) / 100.0f;
		//过滤超过距离的玩家
		if (set->BigMap.PawnMaxDistance < Distance)continue; ImColor color;
		//玩家
		if (pawnType == PawnType::None) {
			if (set->BigMap.showPlayer == false)continue;
			color = UEGlobal.GetColorForNumber(TeamComp->GetTeamID());
		}
		//Boss
		else if (pawnType == PawnType::Boss) {
			if (set->BigMap.showBoss == false)continue;
			color = set->BigMap.BossColor;
		}
		//Elit
		else if (pawnType == PawnType::Elite) {
			if (set->BigMap.showElite == false)continue;
			color = set->BigMap.EliteColor;
		}
		//Elit
		else if (pawnType == PawnType::Animal) {
			continue;
		}
		//Robot
		else {
			if (set->BigMap.showRobot == false)continue;
			color = set->BigMap.RobotColor;
		}

		//获取绘制参数
		FVector2D ScreenPos = UEGlobal.GetMapScreenPos(Map, Location);

		UEGlobal.DrawBigMapPawn(ScreenPos, Rotation, color);
		//绘制队标
		char showText[128];
		if (pawnType == PawnType::None) {
			sprintf_s(showText, "%d", TeamComp->GetTeamID());
			ImVec2 text_size = Overlay::NormalFont->CalcTextSizeA(SCALE(16.0f), FLT_MAX, 0.0f, showText);
			ImGui::GetBackgroundDrawList()->AddText(Overlay::NormalFont, SCALE(16.0f), (ImVec2&)ScreenPos - text_size / 2.0f, 0xffffffff, showText);
		}

		int HelmetLevel = 0, BreastPlateLevel = 0, ChestHangingLevel = 0, BagLevel = 0;
		UCharacterEquipComponent* EquipComp = UCharacterEquipComponent::Get(Character);
		if (EquipComp) {
			FEquipmentInfo HelmetInfo = EquipComp->GetEquipmentInfoByType(EEquipmentType::Helmet);
			FEquipmentInfo BreastPlateInfo = EquipComp->GetEquipmentInfoByType(EEquipmentType::BreastPlate);
			FEquipmentInfo ChestHangingInfo = EquipComp->GetEquipmentInfoByType(EEquipmentType::ChestHanging);
			FEquipmentInfo BagInfo = EquipComp->GetEquipmentInfoByType(EEquipmentType::Bag);
			if (HelmetInfo.ItemID)  HelmetLevel = (HelmetInfo.ItemID % 1000000) / 1000;
			if (BreastPlateInfo.ItemID)BreastPlateLevel = (BreastPlateInfo.ItemID % 1000000) / 1000;
			if (ChestHangingInfo.ItemID)  ChestHangingLevel = (ChestHangingInfo.ItemID % 1000000) / 1000;
			if (BagInfo.ItemID)BagLevel = (BagInfo.ItemID % 1000000) / 1000;
		}
		//绘制
		float fontSize = SCALE(set->BigMap.PawnFontSize);
		UEGlobal.QueueText(Overlay::NormalFont, fontSize, (std::to_string((int)HealthComp->GetHealth())).c_str(), (ImVec2&)ScreenPos - ImVec2(0, SCALE(12) + fontSize * 2.f), UEGlobal.GetItemLevelColor(HelmetLevel));
		if (pawnType == PawnType::None) {
			UEGlobal.QueueText(Overlay::NormalFont, fontSize, HeroName.c_str(), (ImVec2&)ScreenPos - ImVec2(0, SCALE(12) + fontSize), UEGlobal.GetItemLevelColor(BreastPlateLevel));
		}
		else if (pawnType == PawnType::Boss) {
			UEGlobal.QueueText(Overlay::NormalFont, fontSize, PlayerName.c_str(), (ImVec2&)ScreenPos - ImVec2(0, SCALE(12) + fontSize), UEGlobal.GetItemLevelColor(BreastPlateLevel));
		}
		else if (pawnType == PawnType::Elite) {
			UEGlobal.QueueText(Overlay::NormalFont, fontSize, PlayerName.c_str(), (ImVec2&)ScreenPos - ImVec2(0, SCALE(12) + fontSize), UEGlobal.GetItemLevelColor(BreastPlateLevel));
		}
		else if (pawnType == PawnType::Robot)
		{
			UEGlobal.QueueText(Overlay::NormalFont, fontSize, u8"士兵", (ImVec2&)ScreenPos - ImVec2(0, SCALE(12) + fontSize), UEGlobal.GetItemLevelColor(BreastPlateLevel));
		}
	}
}
void Engine::RenderBigMapPickup() {
	auto FormatWithUnits = [](float value) -> std::string {
		// 转换为整数处理
		long long num = static_cast<long long>(value);
		std::string prefix = "$";

		// 辅助函数：为数字字符串添加千位分隔符
		auto addThousandsSeparator = [](const std::string& numStr) -> std::string {
			std::string result;
			int count = 0;

			// 从后往前每3位插入一个逗号
			for (auto it = numStr.rbegin(); it != numStr.rend(); ++it) {
				if (count > 0 && count % 3 == 0) {
					result += ',';
				}
				result += *it;
				count++;
			}

			// 反转恢复正确顺序
			std::reverse(result.begin(), result.end());
			return result;
			};

		// 处理百万级 (1,000,000+)
		if (num >= 1000000) {
			long long millions = num / 1000000;
			int remainder = num % 1000000 / 100000; // 取小数点后第一位

			std::string millionStr = std::to_string(millions);
			millionStr = addThousandsSeparator(millionStr);

			if (remainder == 0) {
				return prefix + millionStr + "M"; // 没有小数部分
			}
			else {
				return prefix + millionStr + "." + std::to_string(remainder) + "M"; // 有小数部分
			}
		}
		// 处理千级 (1,000+)
		else if (num >= 1000) {
			long long thousands = num / 1000;
			int remainder = num % 1000;

			std::string thousandStr = std::to_string(thousands);
			thousandStr = addThousandsSeparator(thousandStr);

			if (thousands >= 100) {
				// 大于100k时保留一位小数
				int firstDecimal = remainder / 100;
				if (firstDecimal == 0) {
					return prefix + thousandStr + "k";
				}
				else {
					return prefix + thousandStr + "." + std::to_string(firstDecimal) + "k";
				}
			}
			else {
				// 1k-99.9k保留两位小数
				int firstDecimal = remainder / 100;
				int secondDecimal = (remainder % 100) / 10;

				if (firstDecimal == 0 && secondDecimal == 0) {
					return prefix + thousandStr + "k";
				}
				else if (secondDecimal == 0) {
					return prefix + thousandStr + "." + std::to_string(firstDecimal) + "k";
				}
				else {
					return prefix + thousandStr + "." + std::to_string(firstDecimal) + std::to_string(secondDecimal) + "k";
				}
			}
		}
		// 处理小数值，使用千位分隔符
		else {
			std::string str = std::to_string(num);
			return prefix + addThousandsSeparator(str);
		}
		};
	Mapinfo Map = UEGlobal.BigMapHUDController->get_Mapinfo();
	for (int i = 0; i < UEGlobal.PickupList.size(); i++) {
		AInventoryPickup* InventoryPickup = reinterpret_cast<AInventoryPickup*>(UEGlobal.PickupList[i]);
		USceneComponent* SceneComp = InventoryPickup->K2_GetRootComponent();
		if (SceneComp == nullptr)continue;
		EMarkingItemType ItemType = InventoryPickup->get_MarkingItemType();
		//物资
		if (ItemType == EMarkingItemType::LootingItem) {
			FVector Location = SceneComp->K2_GetComponentLocation();
			if (Location.IsZero())continue;
			float Distance = UEGlobal.CameraLocation.GetDistanceTo(Location) / 100.0f;
			//过滤距离
			if (set->BigMap.LootingMaxDistance < Distance)continue;
			FInventoryItemInfo* pPickupItemInfo = InventoryPickup->get_PickupItemInfoAddress();
			auto ItemRow = pPickupItemInfo->ItemRow;
			if (ItemRow == nullptr)continue;
			//过滤价值
			if (set->BigMap.LootingItemPriceFiltering && set->BigMap.LootingItemMinGuidePrice > ItemRow->InitialGuidePrice)continue;
			//等级过滤
			if (ItemRow->Quality == 1 && !set->BigMap.showLootingItem[0])continue;
			if (ItemRow->Quality == 2 && !set->BigMap.showLootingItem[1])continue;
			if (ItemRow->Quality == 3 && !set->BigMap.showLootingItem[2])continue;
			if (ItemRow->Quality == 4 && !set->BigMap.showLootingItem[3])continue;
			if (ItemRow->Quality == 5 && !set->BigMap.showLootingItem[4])continue;
			if (ItemRow->Quality == 6 && !set->BigMap.showLootingItem[5])continue;
			//获取物品名称
			std::string ItemName = UEGlobal.GetInventoryPickupName(InventoryPickup);
			if (ItemName.empty())continue;
			//获取绘制参数
			FVector2D ScreenPos = UEGlobal.GetMapScreenPos(Map, Location);
			ImColor color = UEGlobal.GetItemLevelColor(ItemRow->Quality);
			float fontSize = SCALE(set->BigMap.LootingFontSize);
			UEGlobal.QueueText(Overlay::NormalFont, fontSize, (ItemName + FormatWithUnits((int)ItemRow->InitialGuidePrice)).c_str(), (ImVec2&)ScreenPos - ImVec2(0, SCALE(3.0f) + fontSize), color);
			ImGui::GetBackgroundDrawList()->AddCircleFilled((ImVec2&)ScreenPos, SCALE(3.0f), color);
		}
		//容器
		if (ItemType == EMarkingItemType::Container) {
			AInventoryPickup_OpenBox* OpenBox = InventoryPickup->Cast<AInventoryPickup_OpenBox>();
			//打开过滤
			if (set->BigMap.openFilteringOpenBox && OpenBox->get_bHasOpened())continue;
			FVector Location = SceneComp->K2_GetComponentLocation();
			if (Location.IsZero())continue;
			float Distance = UEGlobal.CameraLocation.GetDistanceTo(Location) / 100.0f;
			//过滤距离
			if (set->BigMap.OpenBoxMaxDistance < Distance)continue;
			//获取容器名称
			std::string ItemName = UEGlobal.GetInventoryPickupName(InventoryPickup);
			if (ItemName.empty())continue;
			auto BoxItem = set->OpenBoxItems_Cache.find(ItemName);
			if (BoxItem == set->OpenBoxItems_Cache.end()) continue;
			bool isShowBox = BoxItem->second.first;
			int BoxLevel = BoxItem->second.second;
			if (isShowBox == false)continue;
			if (BoxLevel < 0 || BoxLevel >= sizeof(set->BigMap.showOpenBox) / sizeof(set->BigMap.showOpenBox[0]))continue;
			if (set->BigMap.showOpenBox[BoxLevel] == false)continue;
			ImColor BoxColor = set->Item.OpenBox.OpenBoxColor[BoxLevel];

			//获取绘制参数
			FVector2D ScreenPos = UEGlobal.GetMapScreenPos(Map, Location);

			float fontSize = SCALE(set->BigMap.OpenBoxFontSize);
			UEGlobal.QueueText(Overlay::NormalFont, fontSize, (ItemName + (OpenBox->get_bHasOpened() ? u8"|开启" : "")).c_str(), (ImVec2&)ScreenPos - ImVec2(0, SCALE(3.0f) + fontSize), BoxColor);
			ImGui::GetBackgroundDrawList()->AddCircleFilled((ImVec2&)ScreenPos, SCALE(3.0f), BoxColor);
		}
		//盒子
		if (ItemType == EMarkingItemType::DeadBody) {
			AInventoryPickup_DeadBody* DeadBody = InventoryPickup->Cast<AInventoryPickup_DeadBody>();
			//打开过滤
			if (set->BigMap.openFilteringDeadBody && DeadBody->get_bLooted())continue;
			FVector Location = SceneComp->K2_GetComponentLocation();
			if (Location.IsZero())continue;
			float Distance = UEGlobal.CameraLocation.GetDistanceTo(Location) / 100.0f;
			//过滤距离
			if (set->BigMap.DeadBodyMaxDistance < Distance)continue;
			//获取容器名称
			std::string DeadBodyName = UEGlobal.GetDeadBodyName(DeadBody);
			if (DeadBodyName.empty())continue;
			PawnType pawnType = UEGlobal.GetDeadBodyType(DeadBody, DeadBodyName);
			ImColor DeadBodyColor;
			if (pawnType == PawnType::None) {
				if (set->BigMap.showPlayerDeadBody == false)continue;
				DeadBodyColor = set->BigMap.PlayerDeadBodyColor;
			}
			else if (pawnType == PawnType::Boss) {
				if (set->BigMap.showBossDeadBody == false)continue;
				DeadBodyColor = set->Item.DeadBody.BossBoxColor;
			}
			else if (pawnType == PawnType::Elite) {
				if (set->BigMap.showEliteDeadBody == false)continue;
				DeadBodyColor = set->Item.DeadBody.EliteBoxColor;
			}
			else if (pawnType == PawnType::Robot) {
				if (set->BigMap.showRobotDeadBody == false)continue;
				DeadBodyColor = set->Item.DeadBody.RobotBoxColor;
			}
			else continue;

			//获取绘制参数
			FVector2D ScreenPos = UEGlobal.GetMapScreenPos(Map, Location);

			float fontSize = SCALE(set->BigMap.DeadBodyFontSize);
			UEGlobal.QueueText(Overlay::NormalFont, fontSize, (DeadBodyName + (DeadBody->get_bLooted() ? u8"|开启" : "")).c_str(), (ImVec2&)ScreenPos - ImVec2(0, SCALE(3.0f) + fontSize), DeadBodyColor);
			ImGui::GetBackgroundDrawList()->AddCircleFilled((ImVec2&)ScreenPos, SCALE(3.0f), DeadBodyColor);
		}
	}
}
void Engine::RenderBigMapInteractorProxy() {
	char ShowText[128];
	Mapinfo Map = UEGlobal.BigMapHUDController->get_Mapinfo();
	for (size_t i = 0; i < UEGlobal.OtherItemList.size(); i++)
	{
		auto Item = UEGlobal.OtherItemList[i];
		if (Item.Type == 0 && set->Item.HackPC.Enable) {
			auto HackPC = Item.Object->Cast<AInteractor_HackPC>();
			auto RootComp = HackPC->K2_GetRootComponent();
			auto Location = RootComp->K2_GetComponentLocation();
			if (Location.IsZero())continue;
			float Distance = UEGlobal.CameraLocation.GetDistanceTo(Location) / 100.0f;
			if (set->Item.HackPC.BigMapMaxDistance < Distance)continue; FVector2D coord;
			if (HackPC->IsMorseCodeSuccess())continue;

			//获取绘制参数
			FVector2D ScreenPos = UEGlobal.GetMapScreenPos(Map, Location);

			float fontSize = SCALE(set->Item.HackPC.BigMapfontSize);
			ImColor ItemColor = set->Item.HackPC.color;

			sprintf_s(ShowText, u8"电脑[%04d]", HackPC->get_Password());

			UEGlobal.QueueText(Overlay::NormalFont, fontSize, ShowText, (ImVec2&)ScreenPos - ImVec2(0, SCALE(3.0f) + fontSize), ItemColor);
			ImGui::GetBackgroundDrawList()->AddCircleFilled((ImVec2&)ScreenPos, SCALE(3.0f), ItemColor);
		}
		if (Item.Type == 1 && set->Item.CodeLock.Enable) {
			auto CodedLock = Item.Object->Cast<AInteractor_CodedLock>();
			auto RootComp = CodedLock->K2_GetRootComponent();
			auto Location = RootComp->K2_GetComponentLocation();
			if (Location.IsZero())continue;
			float Distance = UEGlobal.CameraLocation.GetDistanceTo(Location) / 100.0f;
			if (set->Item.CodeLock.BigMapMaxDistance < Distance)continue; FVector2D coord;
			if (CodedLock->IsPassWordCorrect())continue;

			std::string PassWorld;
			auto Pwd = CodedLock->get_Pwd();
			for (int i = 0; i < Pwd.Num(); i++)
			{
				PassWorld += std::to_string(Pwd[i]);
			}

			//获取绘制参数
			FVector2D ScreenPos = UEGlobal.GetMapScreenPos(Map, Location);

			float fontSize = SCALE(set->Item.CodeLock.BigMapfontSize);
			ImColor ItemColor = set->Item.CodeLock.color;

			sprintf_s(ShowText, u8"门锁[%s]", PassWorld.c_str());

			UEGlobal.QueueText(Overlay::NormalFont, fontSize, ShowText, (ImVec2&)ScreenPos - ImVec2(0, SCALE(3.0f) + fontSize), ItemColor);
			ImGui::GetBackgroundDrawList()->AddCircleFilled((ImVec2&)ScreenPos, SCALE(3.0f), ItemColor);

		}
		if (Item.Type == 2 && set->Item.ExitTrigger.Enable) {
			auto ExitTrigger = Item.Object->Cast<AInteractor_ExitTrigger>();
			auto RootComp = ExitTrigger->K2_GetRootComponent();
			auto Location = RootComp->K2_GetComponentLocation();
			if (Location.IsZero())continue;
			float Distance = UEGlobal.CameraLocation.GetDistanceTo(Location) / 100.0f;
			if (set->Item.ExitTrigger.BigMapMaxDistance < Distance)continue; FVector2D coord;

			//获取绘制参数
			FVector2D ScreenPos = UEGlobal.GetMapScreenPos(Map, Location);

			float fontSize = SCALE(set->Item.ExitTrigger.BigMapfontSize);
			ImColor ItemColor = set->Item.ExitTrigger.color;


			UEGlobal.QueueText(Overlay::NormalFont, fontSize, u8"拉闸点", (ImVec2&)ScreenPos - ImVec2(0, SCALE(3.0f) + fontSize), ItemColor);
			ImGui::GetBackgroundDrawList()->AddCircleFilled((ImVec2&)ScreenPos, SCALE(3.0f), ItemColor);
		}
		if (Item.Type == 3 && set->Item.PlayerExit.Enable) {
			auto PlayerExit = Item.Object->Cast<APlayerExitBase>();
			if (PlayerExit->get_bExitActive() == false)continue;
			auto RootComp = PlayerExit->K2_GetRootComponent();
			auto Location = RootComp->K2_GetComponentLocation();
			if (Location.IsZero())continue;
			float Distance = UEGlobal.CameraLocation.GetDistanceTo(Location) / 100.0f;
			if (set->Item.PlayerExit.BigMapMaxDistance < Distance)continue; FVector2D coord;

			//获取绘制参数
			FVector2D ScreenPos = UEGlobal.GetMapScreenPos(Map, Location);

			float fontSize = SCALE(set->Item.PlayerExit.BigMapfontSize);
			ImColor ItemColor = set->Item.PlayerExit.color;


			UEGlobal.QueueText(Overlay::NormalFont, fontSize, u8"撤离点", (ImVec2&)ScreenPos - ImVec2(0, SCALE(3.0f) + fontSize), ItemColor);
			ImGui::GetBackgroundDrawList()->AddCircleFilled((ImVec2&)ScreenPos, SCALE(3.0f), ItemColor);
		}
	}
}
//寻找梓喵对象
void AimAssistSystem::AddTarget(AGPCharacter* InCharacter, BoneInfo* mBoneInfo, PawnType pawnType) {
	//过滤人物
	if (!set->Aimbot.AimPlayer && PawnType::None == pawnType)return;
	if (!set->Aimbot.AimBoss && PawnType::Boss == pawnType)return;
	if (!set->Aimbot.AimElite && PawnType::Elite == pawnType)return;
	if (!set->Aimbot.AimRobot && PawnType::Robot == pawnType)return;
	//过滤倒地
	if (set->Aimbot.NotHealthZero && InCharacter != this->LockCharater && InCharacter->GetHealthComp()->GetHealth() <= 0.0f)return;
	FVector2D ScreenCenter(UEGlobal.SizeX / 2.0f, UEGlobal.SizeY / 2.0f);
	float ScreenDistance = SCALE(set->Aimbot.AimFov);
	float maxAimBotDistance = set->Aimbot.maxDistance * 100.f;
	bool bLockTarget = false;
	int  dwLockBoneId = 0;
	float MinScreenDistance = 99999999.0f;
	for (size_t i = 0; i < 17; i++)
	{
		if (mBoneInfo[i].valid && mBoneInfo[i].visible) {
			if (UEGlobal.CameraLocation.GetDistanceTo(mBoneInfo[i].pos) < maxAimBotDistance) {
				float curScreenDistance = ScreenCenter.GetDistanceTo(mBoneInfo[i].screen);
				if (curScreenDistance <= ScreenDistance && curScreenDistance < MinScreenDistance) {
					dwLockBoneId = i;
					bLockTarget = true;
					MinScreenDistance = curScreenDistance;
				}
			}
		}
	}
	if (bLockTarget) {
		//保存之前的锁定目标
		if (this->LockCharater && this->LockCharater == InCharacter) {
			this->dwSaveTime = GetTickCount64();
			this->LockBoneId = LockBoneId;
			this->LockScreenDistance = MinScreenDistance;
			memcpy(this->mBoneInfo, this->_mBoneInfo, sizeof(this->mBoneInfo));
		}

		if (MinScreenDistance < this->_MinScreenDistance) {
			this->_MinScreenDistance = MinScreenDistance;
			this->_LockCharater = InCharacter;
			this->_LockBoneId = dwLockBoneId;
			this->_LockScreenDistance = MinScreenDistance;
			memcpy(this->_mBoneInfo, mBoneInfo, sizeof(this->_mBoneInfo));
		}
	}







}
void AimAssistSystem::Clear() {
	this->_MinScreenDistance = 999999999.0f;
	this->_LockCharater = nullptr;
}
void AimAssistSystem::Save() {
	if (this->_LockCharater) {
		//没有直接更新
		if (this->LockCharater == nullptr || this->LockCharater == this->_LockCharater) {
			this->dwSaveTime = GetTickCount64();
			this->LockCharater = this->_LockCharater;
			this->LockBoneId = this->_LockBoneId;
			memcpy(this->mBoneInfo, this->_mBoneInfo, sizeof(this->mBoneInfo));
		}
		else if (abs(this->LockScreenDistance - this->_LockScreenDistance) > SCALE(5))
		{
			this->dwSaveTime = GetTickCount64();
			this->LockCharater = this->_LockCharater;
			this->LockBoneId = this->_LockBoneId;
			memcpy(this->mBoneInfo, this->_mBoneInfo, sizeof(this->mBoneInfo));
		}
		//死亡或者脱框100ms后切换目标
		else if (GetTickCount64() - this->dwSaveTime > 300)
		{
			this->dwSaveTime = GetTickCount64();
			this->LockCharater = this->_LockCharater;
			this->LockBoneId = this->_LockBoneId;
			memcpy(this->mBoneInfo, this->_mBoneInfo, sizeof(this->mBoneInfo));
		}
	}
}
void AimAssistSystem::Draw() {

	//渲染Fov
	if (set->Aimbot.showAimFov) {
		ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(UEGlobal.SizeX / 2.0f, UEGlobal.SizeY / 2.0f), SCALE(set->Aimbot.AimFov), set->Aimbot.AimFovColor);
	}

	FVector2D ScreenPos;
	if (this->LockCharater && GetTickCount64() - this->dwSaveTime < 200) {
		if (UEGlobal.WorldLocationToScreenPoint(this->mBoneInfo[0].pos, &ScreenPos)) {
			if (set->Aimbot.showTarget) {
				ImGui::GetBackgroundDrawList()->AddCircleFilled((ImVec2&)ScreenPos, SCALE(3), 0xa00000ff);
			}
			//判断本人是否死亡
			if (UEGlobal.LocalGPCharacter == nullptr || UEGlobal.LocalGPCharacter->IsDead() || UEGlobal.LocalGPCharacter->GetHealthComp()->GetHealth() <= 0.0f) {
				return;
			}
			auto CacheCurWeapon = UEGlobal.LocalGPCharacter->get_CacheCurWeapon();
			//没有武器或者支援武器直接返回
			if (CacheCurWeapon == nullptr || CacheCurWeapon->get_bIsSupportWeapon())return;
			auto WeaponID = CacheCurWeapon->get_WeaponID();

			//这是梓喵位置
			if (set->Aimbot.AimType == 0) {
				int BoneId = -1;
				bool IsAimBot = IsHotkeyDown();
				if (IsAimBot) {
					if (IsForceSelectHotkeyDown()) {
						BoneId = GetAimbotPos(set->Aimbot.AimPosTwo);
					}
					else
					{
						BoneId = GetAimbotPos(set->Aimbot.AimPos);
					}
				}
				if (BoneId == -1)return;

				if (UEGlobal.WorldLocationToScreenPoint(this->mBoneInfo[BoneId].pos, &ScreenPos)) {


					Engine::mouse_move(ScreenPos.X - UEGlobal.SizeX / 2.0f, ScreenPos.Y - UEGlobal.SizeY / 2.0f);
				}
			}

		}
	}

}
//判断自瞄热键
bool AimAssistSystem::IsHotkeyDown() {
	static bool isKeyDown = false;
	static DWORD64 dwLastTime = 0;
	//测试热键50ms是按下的
	if (isKeyDown) {
		if (GetTickCount64() - dwLastTime < 50)return isKeyDown;
		else isKeyDown = false;
	}
	int vKey = 0;
	switch (set->Aimbot.HotKey) {
	case 0:
		vKey = 1;
		break;
	case 1:
		vKey = 2;
		break;
	case 2:
		vKey = 4;
		break;
	case 3:
		vKey = 5;
		break;
	default:
		return false;
	}
	if ((isKeyDown = GetAsyncKeyState(vKey) != 0)) {
		dwLastTime = GetTickCount64();
	}
	return isKeyDown;
}
bool AimAssistSystem::IsForceSelectHotkeyDown() {
	static bool isKeyDown = false;
	static DWORD64 dwLastTime = 0;
	//测试热键50ms是按下的
	if (isKeyDown) {
		if (GetTickCount64() - dwLastTime < 50)return isKeyDown;
		else isKeyDown = false;
	}
	int vKey = 0;
	switch (set->Aimbot.HotKeyTwo) {
	case 1:
		vKey = VK_SHIFT;
		break;
	case 2:
		vKey = VK_CONTROL;
		break;
	default:
		return false;

	}
	if ((isKeyDown = GetAsyncKeyState(vKey) != 0)) {
		dwLastTime = GetTickCount64();
	}
	return isKeyDown;
}
//获取自瞄位置
int AimAssistSystem::GetAimbotPos(int AimPos) {
	if (AimPos == 0) {
		return this->LockBoneId;
	}
	if (AimPos == 1) {
		const BoneID BoneIds[] = { Head,Neck,RightShoulder,LeftShoulder,RightArm,LeftArm,RightForeArm,LeftForeArm,RightHand,LeftHand,Hips,RightUpLeg,LeftUpLeg,RightLeg,LeftLeg,RightFoot,LeftFoot };
		for (size_t i = 0; i < sizeof(BoneIds) / sizeof(BoneIds[0]); i++)
		{
			int BoneId = BoneIds[i];
			auto& BoneInfo = this->mBoneInfo[BoneId];
			if (BoneInfo.valid && BoneInfo.visible) {
				return BoneId;
			}
		}
		return -1;
	}

	if (AimPos == 2) {
		const BoneID BoneIds[] = { Neck,RightShoulder,LeftShoulder,Head,RightArm,LeftArm,RightForeArm,LeftForeArm,RightHand,LeftHand,Hips,RightUpLeg,LeftUpLeg,RightLeg,LeftLeg,RightFoot,LeftFoot };
		for (size_t i = 0; i < sizeof(BoneIds) / sizeof(BoneIds[0]); i++)
		{
			int BoneId = BoneIds[i];
			auto& BoneInfo = this->mBoneInfo[BoneId];
			if (BoneInfo.valid && BoneInfo.visible) {
				return BoneId;
			}
		}
		return -1;
	}

	if (AimPos == 3) {
		const BoneID BoneIds[] = { Hips,RightArm,LeftArm,RightShoulder,LeftShoulder,RightForeArm,LeftForeArm,RightHand,LeftHand,Neck,Head,RightUpLeg,LeftUpLeg,RightLeg,LeftLeg,RightFoot,LeftFoot };
		for (size_t i = 0; i < sizeof(BoneIds) / sizeof(BoneIds[0]); i++)
		{
			int BoneId = BoneIds[i];
			auto& BoneInfo = this->mBoneInfo[BoneId];
			if (BoneInfo.valid && BoneInfo.visible) {
				return BoneId;
			}
		}
		return -1;
	}

	if (AimPos == 4) {
		const BoneID BoneIds[] = { RightLeg, LeftLeg, RightFoot, LeftFoot,RightUpLeg, LeftUpLeg,Hips,RightArm,LeftArm,RightShoulder,LeftShoulder,RightForeArm,LeftForeArm,RightHand,LeftHand,Neck,Head };
		for (size_t i = 0; i < sizeof(BoneIds) / sizeof(BoneIds[0]); i++)
		{
			int BoneId = BoneIds[i];
			auto& BoneInfo = this->mBoneInfo[BoneId];
			if (BoneInfo.valid && BoneInfo.visible) {
				return BoneId;
			}
		}
		return -1;
	}

	return -1;
}