#pragma once
typedef __int8 int8;
typedef __int16 int16;
typedef __int32 int32;
typedef __int64 int64;

typedef unsigned __int8 uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

enum class EPropertyFlags : uint64
{
	None = 0x0000000000000000,

	Edit = 0x0000000000000001,
	ConstParm = 0x0000000000000002,
	BlueprintVisible = 0x0000000000000004,
	ExportObject = 0x0000000000000008,
	BlueprintReadOnly = 0x0000000000000010,
	Net = 0x0000000000000020,
	EditFixedSize = 0x0000000000000040,
	Parm = 0x0000000000000080,
	OutParm = 0x0000000000000100,
	ZeroConstructor = 0x0000000000000200,
	ReturnParm = 0x0000000000000400,
	DisableEditOnTemplate = 0x0000000000000800,

	Transient = 0x0000000000002000,
	Config = 0x0000000000004000,

	DisableEditOnInstance = 0x0000000000010000,
	EditConst = 0x0000000000020000,
	GlobalConfig = 0x0000000000040000,
	InstancedReference = 0x0000000000080000,

	DuplicateTransient = 0x0000000000200000,
	SubobjectReference = 0x0000000000400000,

	SaveGame = 0x0000000001000000,
	NoClear = 0x0000000002000000,

	ReferenceParm = 0x0000000008000000,
	BlueprintAssignable = 0x0000000010000000,
	Deprecated = 0x0000000020000000,
	IsPlainOldData = 0x0000000040000000,
	RepSkip = 0x0000000080000000,
	RepNotify = 0x0000000100000000,
	Interp = 0x0000000200000000,
	NonTransactional = 0x0000000400000000,
	EditorOnly = 0x0000000800000000,
	NoDestructor = 0x0000001000000000,

	AutoWeak = 0x0000004000000000,
	ContainsInstancedReference = 0x0000008000000000,
	AssetRegistrySearchable = 0x0000010000000000,
	SimpleDisplay = 0x0000020000000000,
	AdvancedDisplay = 0x0000040000000000,
	Protected = 0x0000080000000000,
	BlueprintCallable = 0x0000100000000000,
	BlueprintAuthorityOnly = 0x0000200000000000,
	TextExportTransient = 0x0000400000000000,
	NonPIEDuplicateTransient = 0x0000800000000000,
	ExposeOnSpawn = 0x0001000000000000,
	PersistentInstance = 0x0002000000000000,
	UObjectWrapper = 0x0004000000000000,
	HasGetValueTypeHash = 0x0008000000000000,
	NativeAccessSpecifierPublic = 0x0010000000000000,
	NativeAccessSpecifierProtected = 0x0020000000000000,
	NativeAccessSpecifierPrivate = 0x0040000000000000,
	SkipSerialization = 0x0080000000000000,
};

enum class EFunctionFlags : uint32
{
	None = 0x00000000,

	Final = 0x00000001,
	RequiredAPI = 0x00000002,
	BlueprintAuthorityOnly = 0x00000004,
	BlueprintCosmetic = 0x00000008,
	Net = 0x00000040,
	NetReliable = 0x00000080,
	NetRequest = 0x00000100,
	Exec = 0x00000200,
	Native = 0x00000400,
	Event = 0x00000800,
	NetResponse = 0x00001000,
	Static = 0x00002000,
	NetMulticast = 0x00004000,
	UbergraphFunction = 0x00008000,
	MulticastDelegate = 0x00010000,
	Public = 0x00020000,
	Private = 0x00040000,
	Protected = 0x00080000,
	Delegate = 0x00100000,
	NetServer = 0x00200000,
	HasOutParms = 0x00400000,
	HasDefaults = 0x00800000,
	NetClient = 0x01000000,
	DLLImport = 0x02000000,
	BlueprintCallable = 0x04000000,
	BlueprintEvent = 0x08000000,
	BlueprintPure = 0x10000000,
	EditorOnly = 0x20000000,
	Const = 0x40000000,
	NetValidate = 0x80000000,

	AllFlags = 0xFFFFFFFF,
};

enum class EObjectFlags
{
	NoFlags = 0x00000000,

	Public = 0x00000001,
	Standalone = 0x00000002,
	MarkAsNative = 0x00000004,
	Transactional = 0x00000008,
	ClassDefaultObject = 0x00000010,
	ArchetypeObject = 0x00000020,
	Transient = 0x00000040,

	MarkAsRootSet = 0x00000080,
	TagGarbageTemp = 0x00000100,

	NeedInitialization = 0x00000200,
	NeedLoad = 0x00000400,
	KeepForCooker = 0x00000800,
	NeedPostLoad = 0x00001000,
	NeedPostLoadSubobjects = 0x00002000,
	NewerVersionExists = 0x00004000,
	BeginDestroyed = 0x00008000,
	FinishDestroyed = 0x00010000,

	BeingRegenerated = 0x00020000,
	DefaultSubObject = 0x00040000,
	WasLoaded = 0x00080000,
	TextExportTransient = 0x00100000,
	LoadCompleted = 0x00200000,
	InheritableComponentTemplate = 0x00400000,
	DuplicateTransient = 0x00800000,
	StrongRefOnFrame = 0x01000000,
	NonPIEDuplicateTransient = 0x02000000,
	Dynamic = 0x04000000,
	WillBeLoaded = 0x08000000,
};

enum class EFieldClassID : uint64
{
	Int8 = 1llu << 1,
	Byte = 1llu << 6,
	Int = 1llu << 7,
	Float = 1llu << 8,
	UInt64 = 1llu << 9,
	Class = 1llu << 10,
	UInt32 = 1llu << 11,
	Interface = 1llu << 12,
	Name = 1llu << 13,
	String = 1llu << 14,
	Object = 1llu << 16,
	Bool = 1llu << 17,
	UInt16 = 1llu << 18,
	Struct = 1llu << 20,
	Array = 1llu << 21,
	Int64 = 1llu << 22,
	Delegate = 1llu << 23,
	SoftObject = 1llu << 27,
	LazyObject = 1llu << 28,
	WeakObject = 1llu << 29,
	Text = 1llu << 30,
	Int16 = 1llu << 31,
	Double = 1llu << 32,
	SoftClass = 1llu << 33,
	Map = 1llu << 46,
	Set = 1llu << 47,
	Enum = 1llu << 48,
	MulticastInlineDelegate = 1llu << 50,
	MulticastSparseDelegate = 1llu << 51,
	ObjectPointer = 1llu << 53
};

enum class EClassCastFlags : uint64
{
	None = 0x0000000000000000,

	Field = 0x0000000000000001,
	Int8Property = 0x0000000000000002,
	Enum = 0x0000000000000004,
	Struct = 0x0000000000000008,
	ScriptStruct = 0x0000000000000010,
	Class = 0x0000000000000020,
	ByteProperty = 0x0000000000000040,
	IntProperty = 0x0000000000000080,
	FloatProperty = 0x0000000000000100,
	UInt64Property = 0x0000000000000200,
	ClassProperty = 0x0000000000000400,
	UInt32Property = 0x0000000000000800,
	InterfaceProperty = 0x0000000000001000,
	NameProperty = 0x0000000000002000,
	StrProperty = 0x0000000000004000,
	Property = 0x0000000000008000,
	ObjectProperty = 0x0000000000010000,
	BoolProperty = 0x0000000000020000,
	UInt16Property = 0x0000000000040000,
	Function = 0x0000000000080000,
	StructProperty = 0x0000000000100000,
	ArrayProperty = 0x0000000000200000,
	Int64Property = 0x0000000000400000,
	DelegateProperty = 0x0000000000800000,
	NumericProperty = 0x0000000001000000,
	MulticastDelegateProperty = 0x0000000002000000,
	ObjectPropertyBase = 0x0000000004000000,
	WeakObjectProperty = 0x0000000008000000,
	LazyObjectProperty = 0x0000000010000000,
	SoftObjectProperty = 0x0000000020000000,
	TextProperty = 0x0000000040000000,
	Int16Property = 0x0000000080000000,
	DoubleProperty = 0x0000000100000000,
	SoftClassProperty = 0x0000000200000000,
	Package = 0x0000000400000000,
	Level = 0x0000000800000000,
	Actor = 0x0000001000000000,
	PlayerController = 0x0000002000000000,
	Pawn = 0x0000004000000000,
	SceneComponent = 0x0000008000000000,
	PrimitiveComponent = 0x0000010000000000,
	SkinnedMeshComponent = 0x0000020000000000,
	SkeletalMeshComponent = 0x0000040000000000,
	Blueprint = 0x0000080000000000,
	DelegateFunction = 0x0000100000000000,
	StaticMeshComponent = 0x0000200000000000,
	MapProperty = 0x0000400000000000,
	SetProperty = 0x0000800000000000,
	EnumProperty = 0x0001000000000000,
	SparseDelegateFunction = 0x0002000000000000,
	MulticastInlineDelegateProperty = 0x0004000000000000,
	MulticastSparseDelegateProperty = 0x0008000000000000,
	FieldPathProperty = 0x0010000000000000,
	// Removed							= 0x0200000000000000,
	// Removed							= 0x0400000000000000,
	LargeWorldCoordinatesRealProperty = 0x0080000000000000,
	OptionalProperty = 0x0100000000000000,
	VValueProperty = 0x0200000000000000,
	VerseVMClass = 0x0400000000000000,
	VRestValueProperty = 0x0800000000000000,
	Utf8StrProperty = 0x1000000000000000,
	AnsiStrProperty = 0x2000000000000000,
	VCellProperty = 0x4000000000000000,
};

enum class EClassFlags:uint32
{
	None = 0x00000000u,
	Abstract = 0x00000001u,
	DefaultConfig = 0x00000002u,
	Config = 0x00000004u,
	Transient = 0x00000008u,
	Parsed = 0x00000010u,
	MatchedSerializers = 0x00000020u,
	ProjectUserConfig = 0x00000040u,
	Native = 0x00000080u,
	NoExport = 0x00000100u,
	NotPlaceable = 0x00000200u,
	PerObjectConfig = 0x00000400u,
	ReplicationDataIsSetUp = 0x00000800u,
	EditInlineNew = 0x00001000u,
	CollapseCategories = 0x00002000u,
	Interface = 0x00004000u,
	CustomConstructor = 0x00008000u,
	Const = 0x00010000u,
	LayoutChanging = 0x00020000u,
	CompiledFromBlueprint = 0x00040000u,
	MinimalAPI = 0x00080000u,
	RequiredAPI = 0x00100000u,
	DefaultToInstanced = 0x00200000u,
	TokenStreamAssembled = 0x00400000u,
	HasInstancedReference = 0x00800000u,
	Hidden = 0x01000000u,
	Deprecated = 0x02000000u,
	HideDropDown = 0x04000000u,
	GlobalUserConfig = 0x08000000u,
	Intrinsic = 0x10000000u,
	Constructed = 0x20000000u,
	ConfigDoNotCheckDefaults = 0x40000000u,
	NewerVersionExists = 0x80000000u,
};

enum class EMappingsTypeFlags : uint8
{
	ByteProperty,
	BoolProperty,
	IntProperty,
	FloatProperty,
	ObjectProperty,
	NameProperty,
	DelegateProperty,
	DoubleProperty,
	ArrayProperty,
	StructProperty,
	StrProperty,
	TextProperty,
	InterfaceProperty,
	MulticastDelegateProperty,
	WeakObjectProperty, //
	LazyObjectProperty, // When deserialized, these 3 properties will be SoftObjects
	AssetObjectProperty, //
	SoftObjectProperty,
	UInt64Property,
	UInt32Property,
	UInt16Property,
	Int64Property,
	Int16Property,
	Int8Property,
	MapProperty,
	SetProperty,
	EnumProperty,
	FieldPathProperty,
	OptionalProperty,

	Unknown = 0xFF
};

enum class EUsmapCompressionMethod : uint8
{
	None,
	Oodle,
	Brotli,
	ZStandard,
	Unknown = 0xFF
};

enum class EMainFlowState : uint8
{
	Default = 0,
	Lobby = 1,
	Loading = 2,
	SafeHouse = 3,
	InGame = 4,
	LobbyBHD = 5,
	EMainFlowState_MAX = 6,
};

enum class EExitState : uint8_t {
	Normal = 1,
	WaitingToEscape = 2,
	Escaped = 3,
	ReadyToStartEscape = 4,
	InEscaping = 5,
	EExitState_MAX = 6
};


enum class EEquipmentType : uint8
{
	None = 0,
	Helmet = 1,
	Headset = 2,
	FaceMask = 3,
	Armband = 4,
	BreastPlate = 5,
	Glasses = 6,
	ChestHanging = 7,
	Bag = 8,
	SafeBox = 9,
	Tool = 10,
	Shoes = 11,
	KeyChain = 12,
	Max = 13,
};


enum class ETraceTypeQuery : uint8
{
	TraceTypeQuery1 = 0,
	TraceTypeQuery2 = 1,
	TraceTypeQuery3 = 2,
	TraceTypeQuery4 = 3,
	TraceTypeQuery5 = 4,
	TraceTypeQuery6 = 5,
	TraceTypeQuery7 = 6,
	TraceTypeQuery8 = 7,
	TraceTypeQuery9 = 8,
	TraceTypeQuery10 = 9,
	TraceTypeQuery11 = 10,
	TraceTypeQuery12 = 11,
	TraceTypeQuery13 = 12,
	TraceTypeQuery14 = 13,
	TraceTypeQuery15 = 14,
	TraceTypeQuery16 = 15,
	TraceTypeQuery17 = 16,
	TraceTypeQuery18 = 17,
	TraceTypeQuery19 = 18,
	TraceTypeQuery20 = 19,
	TraceTypeQuery21 = 20,
	TraceTypeQuery22 = 21,
	TraceTypeQuery23 = 22,
	TraceTypeQuery24 = 23,
	TraceTypeQuery25 = 24,
	TraceTypeQuery26 = 25,
	TraceTypeQuery27 = 26,
	TraceTypeQuery28 = 27,
	TraceTypeQuery29 = 28,
	TraceTypeQuery30 = 29,
	TraceTypeQuery31 = 30,
	TraceTypeQuery32 = 31,
	TraceTypeQuery_MAX = 32,
	ETraceTypeQuery_MAX = 33,
};

enum class EDrawDebugTrace : uint8
{
	None = 0,
	ForOneFrame = 1,
	ForDuration = 2,
	Persistent = 3,
	EDrawDebugTrace_MAX = 4,
};


// Enum  GPGameplay.EMarkingItemType
enum class EMarkingItemType : uint8_t
{
	None = 0,
	CommonItem = 1,
	LootingItem = 2,
	Container = 3,
	DyingTeammate = 4,
	DeadBody = 5,
	Weapon = 6,
	EngineerSonicTrap = 7,
	Vehicle = 8,
	TacticalEquip = 9,
	PendingDeathCharacter = 10,
	SectorAnchor = 11,
	MandelBrickInteractor = 12,
	MandelBrickDecipherDevice = 13,
	COUNT = 14,
	EMarkingItemType_MAX = 15
};

enum class EFastArraySerializerDeltaFlags : uint8
{
	None = 0,
	HasBeenSerialized = 1,
	HasDeltaBeenRequested = 2,
	IsUsingDeltaSerialization = 4,
	EFastArraySerializerDeltaFlags_MAX = 5,
};