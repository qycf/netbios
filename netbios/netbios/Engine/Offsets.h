#pragma once
typedef int int32;

namespace Off {
	namespace UObject
	{
		inline int32 Vft = 0x00;
		inline int32 Class = 0x08;
		inline int32 Outer = 0x10;
		inline int32 Flags = 0x18;
		inline int32 Name = 0x1C;
		inline int32 Index = 0x24;

		inline int32 UEFree = 0xA8411D0;
		inline int32 StaticFindObject = 0xACD4570;
	}

	namespace FField
	{
		inline int32 Next = 0x18;
		inline int32 Class = 0x20;
		inline int32 Name = 0x28;
		inline int32 Flags = 0x30;
	}
	namespace Property
	{
		inline int32 ArrayDim = 0x3C;
		inline int32 ElementSize = 0x40;
		inline int32 PropertyFlags = 0x48;
		inline int32 Offset_Internal = 0x54;
	}

	namespace UField
	{
		inline int32 Next = 0x28;
	}

	namespace UStruct
	{
		inline int32 Size = 0x40;
		inline int32 SuperStruct = 0x48;
		inline int32 MinAlignment = 0x50;
		inline int32 Children = 0x58;
		inline int32 ChildProperties = 0x70;
	}

	namespace UFunction
	{
		inline int32 FunctionFlags = 0xC0;
		inline int32 ExecFunction = 0xE0;
	}

	namespace UClass
	{
		inline int32 CastFlags = 0xE0;
		inline int32 ClassDefaultObject = 0x140;
	}

	namespace Name {
		inline int32 AppendNameToString = 0xA967EB0;
	}

	namespace ProcessEvent {
		inline int32 PEIndex = 0x44;
	}

	namespace Text {
		inline int32 TextSize = 0x18;
		inline int32 TextDatOffset = 0x00;
		inline int32 InTextDataStringOffset = 0x00;
	}

	namespace UCanvas {
		inline int32 PostRender = 0x60;
	}
}
