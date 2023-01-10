// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "MobileFileOpener/Public/FileOpener.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeFileOpener() {}
// Cross Module References
	MOBILEFILEOPENER_API UClass* Z_Construct_UClass_UFileOpener_NoRegister();
	MOBILEFILEOPENER_API UClass* Z_Construct_UClass_UFileOpener();
	COREUOBJECT_API UClass* Z_Construct_UClass_UInterface();
	UPackage* Z_Construct_UPackage__Script_MobileFileOpener();
// End Cross Module References
	void UFileOpener::StaticRegisterNativesUFileOpener()
	{
	}
	UClass* Z_Construct_UClass_UFileOpener_NoRegister()
	{
		return UFileOpener::StaticClass();
	}
	struct Z_Construct_UClass_UFileOpener_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UFileOpener_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UInterface,
		(UObject* (*)())Z_Construct_UPackage__Script_MobileFileOpener,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UFileOpener_Statics::Class_MetaDataParams[] = {
		{ "ModuleRelativePath", "Public/FileOpener.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UFileOpener_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<IFileOpener>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UFileOpener_Statics::ClassParams = {
		&UFileOpener::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x000840A1u,
		METADATA_PARAMS(Z_Construct_UClass_UFileOpener_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UFileOpener_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UFileOpener()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UFileOpener_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UFileOpener, 1771336056);
	template<> MOBILEFILEOPENER_API UClass* StaticClass<UFileOpener>()
	{
		return UFileOpener::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UFileOpener(Z_Construct_UClass_UFileOpener, &UFileOpener::StaticClass, TEXT("/Script/MobileFileOpener"), TEXT("UFileOpener"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UFileOpener);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
