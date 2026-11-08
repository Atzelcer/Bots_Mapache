// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "Bots_Mapache/Bots_MapacheGameModeBase.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeBots_MapacheGameModeBase() {}
// Cross Module References
	BOTS_MAPACHE_API UClass* Z_Construct_UClass_ABots_MapacheGameModeBase_NoRegister();
	BOTS_MAPACHE_API UClass* Z_Construct_UClass_ABots_MapacheGameModeBase();
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	UPackage* Z_Construct_UPackage__Script_Bots_Mapache();
// End Cross Module References
	void ABots_MapacheGameModeBase::StaticRegisterNativesABots_MapacheGameModeBase()
	{
	}
	UClass* Z_Construct_UClass_ABots_MapacheGameModeBase_NoRegister()
	{
		return ABots_MapacheGameModeBase::StaticClass();
	}
	struct Z_Construct_UClass_ABots_MapacheGameModeBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ABots_MapacheGameModeBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_Bots_Mapache,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ABots_MapacheGameModeBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering Utilities|Transformation" },
		{ "IncludePath", "Bots_MapacheGameModeBase.h" },
		{ "ModuleRelativePath", "Bots_MapacheGameModeBase.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_ABots_MapacheGameModeBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ABots_MapacheGameModeBase>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_ABots_MapacheGameModeBase_Statics::ClassParams = {
		&ABots_MapacheGameModeBase::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009002ACu,
		METADATA_PARAMS(Z_Construct_UClass_ABots_MapacheGameModeBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ABots_MapacheGameModeBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ABots_MapacheGameModeBase()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_ABots_MapacheGameModeBase_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(ABots_MapacheGameModeBase, 1671433404);
	template<> BOTS_MAPACHE_API UClass* StaticClass<ABots_MapacheGameModeBase>()
	{
		return ABots_MapacheGameModeBase::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_ABots_MapacheGameModeBase(Z_Construct_UClass_ABots_MapacheGameModeBase, &ABots_MapacheGameModeBase::StaticClass, TEXT("/Script/Bots_Mapache"), TEXT("ABots_MapacheGameModeBase"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(ABots_MapacheGameModeBase);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
