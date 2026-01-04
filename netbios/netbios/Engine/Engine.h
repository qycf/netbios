#pragma once
#include "pch.h"
#include "Class.h"
#include "imgui.h"
using namespace UE;
#define M_PI 3.14159265358979323846264338327950288419716939937510

enum BoneID
{
	Head,
	Neck,
	Hips,
	RightShoulder,
	RightArm,
	RightForeArm,
	RightHand,
	RightUpLeg,
	RightLeg,
	RightFoot,
	LeftShoulder,
	LeftArm,
	LeftForeArm,
	LeftHand,
	LeftUpLeg,
	LeftLeg,
	LeftFoot,
};

enum class PawnType {
	None,
	Robot,
	Elite,
	Boss,
	Animal,
};
struct BoneInfo
{
	FVector    pos;    // 世界坐标
	FVector2D  screen;   // 屏幕坐标
	bool       visible;        // 是否可见
	bool       valid;          // 是否有效
};

//定义全局变量
class UGlobal {
public:
	UEngine* GEngine = nullptr;
	UWorld* GWorld = nullptr;
	UCanvas* GameCanvas = nullptr;
	UGameViewportClient* GameViewport = nullptr;
	APlayerCameraManager* PlayerCameraManager = nullptr;
	APlayerController* PlayerController = nullptr;
	AGPCharacter* LocalGPCharacter = nullptr;
	UGPTeamComponent* LocalTeamComp = nullptr;
	int						SizeX = 0;
	int						SizeY = 0;
	FRotator				CameraRotation;
	FRotator				ControlRotation;
	FVector					CameraLocation;

	UBigMapHUDView* BigMapHUDController;


public:
	std::vector<ADFMCharacter*>		PawnList;
	std::vector<AInventoryPickup*>	PickupList;
public:
	struct FResourceItem
	{
		int32 Type = 0;
		UObject* Object = nullptr;
	};
	std::vector<FResourceItem>	OtherItemList;
public:
	struct
	{
		FName Head;
		FName Neck;
		FName Hips;
		FName RightShoulder;
		FName RightArm;
		FName RightForeArm;
		FName RightHand;
		FName RightUpLeg;
		FName RightLeg;
		FName RightFoot;
		FName LeftShoulder;
		FName LeftArm;
		FName LeftForeArm;
		FName LeftHand;
		FName LeftUpLeg;
		FName LeftLeg;
		FName LeftFoot;
		FName Skeleton[17];
	}Bone;
	void InitBoneName();
public:
	//Helper
	bool WorldLocationToScreenPoint(const FVector& Location, FVector2D* coords);
	bool WorldLocationToScreenRect(const FVector& Location, FVector2D* coords);
private:
	std::map<FText, std::string>Conv_TextToString_Cache;
	std::map<uint64_t, std::string>Conv_NameToString_Cache;
public:
	std::string Conv_TextToString(FText InText);
	std::string Conv_NameToString(FName InName);
private:
	std::map<uint64_t, std::string> PoolChosenName_Cache;
	std::map<int32_t, std::string>  PlayerNamePrivate_Cache;
	std::map<uint64_t, std::string> DeadBodyName_Cache;
	std::map<uint64_t, std::string> InventoryPickupName_Cache;
	std::map<uint64_t, std::string> ItemRowName_Cache;
public:
	void Cache_Clear();
public:
	std::string GetPoolChosenName(ADFMCharacter* InCharacter);
	std::string GetPlayerNamePrivate(APlayerState* PlayerState);
	std::string GetDeadBodyName(AInventoryPickup_DeadBody* DeadBody);

	std::string GetPlayerHeroName(APlayerState* PlayerState);
	std::string GetPlayerWeapName(ADFMCharacter* Player);
	std::string GetInventoryPickupName(AInventoryPickup* InventoryPickup);
	std::string GetItemRowName(const FDFMCommonItemRow* ItemRow);
public:
	PawnType GetPawnType(ADFMCharacter* InCharacter, const std::string& PlayerName, bool IsAI);
	PawnType GetDeadBodyType(AInventoryPickup_DeadBody* InDeadBody, const std::string& DeadBodyName);
public:
	BoneInfo GetBonePostion(ADFMCharacter* InCharacter, USkeletalMeshComponent* SkinnedMeshComp, FName InBoneName);
	bool GetSkinnedMesh(ADFMCharacter* InCharacter, USkeletalMeshComponent* SkinnedMeshComp, BoneInfo* OutBoneInfo);
public:
	bool IsInScreen(const FVector2D WorldToScreen);
public:
	ImU32 get_clr(ImVec4 col) { if (col.w == 0.0f)col.w = 1.0f; return ImGui::ColorConvertFloat4ToU32(col); };
	ImU32 GetColorForNumber(int teamId);
	ImU32 GetItemLevelColor(int Level);
public:
	void DrawBigMapPawn(const FVector2D& ScreenPos, const FRotator& Rotation, const ImU32& color);
	void DrawLine(const FVector2D& from, const FVector2D& to, const ImVec4& col, float thicknes = 1.0f);
	void DrawRect(FVector2D* coords, const ImVec4& col, float thicknes = 1.0f);
	void QueueText(ImFont* Font, float FontSize, const std::string& text, ImVec2 pos, ImColor color, bool center = true);
	void RenderSkinnedMesh(const BoneInfo* InBoneInfo, const ImVec4& col, const ImVec4& InvisibleColor, float thicknes = 1.0f);
	void DrawScalingHealthBar(const ImVec2* coords, float currentHP, float maxHP, float barWidth = 8.0f, float minBarHeight = 4.0f, float spacing = 4.0f);
public:
	FVector2D GetDBMapScreenPos(Mapinfo Map, FVector Location, ImVec2 DisplaySize);
	FVector2D GetCGMapScreenPos(Mapinfo Map, FVector Location, ImVec2 DisplaySize);
	FVector2D GetCGMapScreenPos2(Mapinfo Map, FVector Location, ImVec2 DisplaySize);
	FVector2D GetMapScreenPos(Mapinfo Map, FVector Location);
};


class TextStackManager {
private:
	// 定义结构体存储文本信息
	struct TextInfo {
		int fontSize;
		std::string text;
		ImVec2 position;
		ImColor color;
		float distance; // 与参考点的距离
	};

	// 存储所有待显示的文本
	std::vector<TextInfo> textList;

	// 计算两点之间的屏幕距离（使用勾股定理）
	float CalculateScreenDistance(const ImVec2& a, const ImVec2& b) const;

public:
	// 构造函数和析构函数
	TextStackManager() = default;
	~TextStackManager() = default;

	// 禁止拷贝构造和赋值操作
	TextStackManager(const TextStackManager&) = delete;
	TextStackManager& operator=(const TextStackManager&) = delete;

	// 添加文本到列表
	void AddText(int fontSize, const std::string& text, const ImVec2& pos, const ImColor& color, float dist);

	// 合并并显示文本
	void DisplayStackedTexts(float maxScreenRange, float maxDistance);

	// 清空文本列表
	void Clear();
};

namespace Engine {
	extern void Intialize();
	extern HWND hUnrealWindow;
	extern HWND FindUnrealWindow();
	//HookPostRender
	extern void* fnPostRender;
	extern void HookPostRender();
	extern void(&DetourPostRender)();
	extern void IndirectPostRender(DWORD_PTR Rsp);
	extern void hkPostRender(UGameViewportClient* ViewportClient, UCanvas* Canvas);
	//mouse_move
	extern void mouse_move(int x, int y);
	//
	extern bool UpdataObjects();
	extern void CacheObjects();
	extern void CacheMapController();
	//
	extern void RenderPawn();
	extern void RenderPickup();
	extern void RenderInteractorProxy();
	//
	extern void RendBigMapPawn();
	extern void RenderBigMapPickup();
	extern void RenderBigMapInteractorProxy();
}

class AimAssistSystem {
private:
	float _MinScreenDistance = 999999999.0f;

	int			_LockBoneId = 0;
	BoneInfo	_mBoneInfo[17];
	AGPCharacter* _LockCharater = nullptr;
	float _LockScreenDistance = 999999999.0f;
public:
	DWORD64		dwSaveTime = 0;
	int			LockBoneId = 0;
	BoneInfo	mBoneInfo[17];
	AGPCharacter* LockCharater = nullptr;
	float LockScreenDistance = 999999999.0f;
public:
	void AddTarget(AGPCharacter* InCharacter, BoneInfo* mBoneInfo, PawnType pawnType = PawnType::None);
	void Clear();
	void Save();
	void Draw();
public:
	bool IsHotkeyDown();
	bool IsForceSelectHotkeyDown();

	int GetAimbotPos(int AimPos);

};