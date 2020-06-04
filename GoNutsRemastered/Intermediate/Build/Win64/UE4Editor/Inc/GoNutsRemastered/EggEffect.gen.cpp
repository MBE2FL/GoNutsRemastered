// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GoNutsRemastered/EggEffect.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeEggEffect() {}
// Cross Module References
	GONUTSREMASTERED_API UFunction* Z_Construct_UDelegateFunction_GoNutsRemastered_EggEffectSignature__DelegateSignature();
	UPackage* Z_Construct_UPackage__Script_GoNutsRemastered();
	GONUTSREMASTERED_API UClass* Z_Construct_UClass_AEggEffect_NoRegister();
	GONUTSREMASTERED_API UClass* Z_Construct_UClass_AEggEffect();
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	ENGINE_API UClass* Z_Construct_UClass_APawn_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_UParticleSystemComponent_NoRegister();
// End Cross Module References
	struct Z_Construct_UDelegateFunction_GoNutsRemastered_EggEffectSignature__DelegateSignature_Statics
	{
		struct _Script_GoNutsRemastered_eventEggEffectSignature_Parms
		{
			float paramOne;
		};
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_paramOne;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UDelegateFunction_GoNutsRemastered_EggEffectSignature__DelegateSignature_Statics::NewProp_paramOne = { "paramOne", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(_Script_GoNutsRemastered_eventEggEffectSignature_Parms, paramOne), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_GoNutsRemastered_EggEffectSignature__DelegateSignature_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_GoNutsRemastered_EggEffectSignature__DelegateSignature_Statics::NewProp_paramOne,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UDelegateFunction_GoNutsRemastered_EggEffectSignature__DelegateSignature_Statics::Function_MetaDataParams[] = {
		{ "Comment", "//UDELEGATE(BlueprintAuthorityOnly)\n//DECLARE_DELEGATE(FEggDelegate)\n" },
		{ "ModuleRelativePath", "EggEffect.h" },
		{ "ToolTip", "UDELEGATE(BlueprintAuthorityOnly)\nDECLARE_DELEGATE(FEggDelegate)" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UDelegateFunction_GoNutsRemastered_EggEffectSignature__DelegateSignature_Statics::FuncParams = { (UObject*(*)())Z_Construct_UPackage__Script_GoNutsRemastered, nullptr, "EggEffectSignature__DelegateSignature", nullptr, nullptr, sizeof(_Script_GoNutsRemastered_eventEggEffectSignature_Parms), Z_Construct_UDelegateFunction_GoNutsRemastered_EggEffectSignature__DelegateSignature_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_GoNutsRemastered_EggEffectSignature__DelegateSignature_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04130000, 0, 0, METADATA_PARAMS(Z_Construct_UDelegateFunction_GoNutsRemastered_EggEffectSignature__DelegateSignature_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_GoNutsRemastered_EggEffectSignature__DelegateSignature_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UDelegateFunction_GoNutsRemastered_EggEffectSignature__DelegateSignature()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UDelegateFunction_GoNutsRemastered_EggEffectSignature__DelegateSignature_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	DEFINE_FUNCTION(AEggEffect::execisPlayerWithinRange)
	{
		P_GET_PROPERTY(FFloatProperty,Z_Param_range);
		P_GET_PROPERTY_REF(FFloatProperty,Z_Param_Out_dist);
		P_GET_UBOOL(Z_Param_calcRealDist);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(bool*)Z_Param__Result=P_THIS->isPlayerWithinRange(Z_Param_range,Z_Param_Out_dist,Z_Param_calcRealDist);
		P_NATIVE_END;
	}
	static FName NAME_AEggEffect_onEggEffectBegin = FName(TEXT("onEggEffectBegin"));
	void AEggEffect::onEggEffectBegin()
	{
		ProcessEvent(FindFunctionChecked(NAME_AEggEffect_onEggEffectBegin),NULL);
	}
	void AEggEffect::StaticRegisterNativesAEggEffect()
	{
		UClass* Class = AEggEffect::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "isPlayerWithinRange", &AEggEffect::execisPlayerWithinRange },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_AEggEffect_isPlayerWithinRange_Statics
	{
		struct EggEffect_eventisPlayerWithinRange_Parms
		{
			float range;
			float dist;
			bool calcRealDist;
			bool ReturnValue;
		};
		static void NewProp_ReturnValue_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
		static void NewProp_calcRealDist_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_calcRealDist;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_dist;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_range_MetaData[];
#endif
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_range;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	void Z_Construct_UFunction_AEggEffect_isPlayerWithinRange_Statics::NewProp_ReturnValue_SetBit(void* Obj)
	{
		((EggEffect_eventisPlayerWithinRange_Parms*)Obj)->ReturnValue = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_AEggEffect_isPlayerWithinRange_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(EggEffect_eventisPlayerWithinRange_Parms), &Z_Construct_UFunction_AEggEffect_isPlayerWithinRange_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_AEggEffect_isPlayerWithinRange_Statics::NewProp_calcRealDist_SetBit(void* Obj)
	{
		((EggEffect_eventisPlayerWithinRange_Parms*)Obj)->calcRealDist = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_AEggEffect_isPlayerWithinRange_Statics::NewProp_calcRealDist = { "calcRealDist", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(EggEffect_eventisPlayerWithinRange_Parms), &Z_Construct_UFunction_AEggEffect_isPlayerWithinRange_Statics::NewProp_calcRealDist_SetBit, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_AEggEffect_isPlayerWithinRange_Statics::NewProp_dist = { "dist", nullptr, (EPropertyFlags)0x0010000000000180, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(EggEffect_eventisPlayerWithinRange_Parms, dist), METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_AEggEffect_isPlayerWithinRange_Statics::NewProp_range_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_AEggEffect_isPlayerWithinRange_Statics::NewProp_range = { "range", nullptr, (EPropertyFlags)0x0010000000000082, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(EggEffect_eventisPlayerWithinRange_Parms, range), METADATA_PARAMS(Z_Construct_UFunction_AEggEffect_isPlayerWithinRange_Statics::NewProp_range_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_AEggEffect_isPlayerWithinRange_Statics::NewProp_range_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_AEggEffect_isPlayerWithinRange_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AEggEffect_isPlayerWithinRange_Statics::NewProp_ReturnValue,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AEggEffect_isPlayerWithinRange_Statics::NewProp_calcRealDist,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AEggEffect_isPlayerWithinRange_Statics::NewProp_dist,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_AEggEffect_isPlayerWithinRange_Statics::NewProp_range,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_AEggEffect_isPlayerWithinRange_Statics::Function_MetaDataParams[] = {
		{ "Category", "Egg Effect|Utility" },
		{ "CPP_Default_calcRealDist", "false" },
		{ "ModuleRelativePath", "EggEffect.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_AEggEffect_isPlayerWithinRange_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_AEggEffect, nullptr, "isPlayerWithinRange", nullptr, nullptr, sizeof(EggEffect_eventisPlayerWithinRange_Parms), Z_Construct_UFunction_AEggEffect_isPlayerWithinRange_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_AEggEffect_isPlayerWithinRange_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14480401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_AEggEffect_isPlayerWithinRange_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_AEggEffect_isPlayerWithinRange_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_AEggEffect_isPlayerWithinRange()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_AEggEffect_isPlayerWithinRange_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_AEggEffect_onEggEffectBegin_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_AEggEffect_onEggEffectBegin_Statics::Function_MetaDataParams[] = {
		{ "Category", "Egg Effect|Effect Settings" },
		{ "ModuleRelativePath", "EggEffect.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_AEggEffect_onEggEffectBegin_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_AEggEffect, nullptr, "onEggEffectBegin", nullptr, nullptr, 0, nullptr, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x08080800, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_AEggEffect_onEggEffectBegin_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_AEggEffect_onEggEffectBegin_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_AEggEffect_onEggEffectBegin()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_AEggEffect_onEggEffectBegin_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_AEggEffect_NoRegister()
	{
		return AEggEffect::StaticClass();
	}
	struct Z_Construct_UClass_AEggEffect_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp__playerPawn_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp__playerPawn;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp__effectParticleComp_MetaData[];
#endif
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp__effectParticleComp;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp__eggEffectSig_MetaData[];
#endif
		static const UE4CodeGen_Private::FMulticastDelegatePropertyParams NewProp__eggEffectSig;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AEggEffect_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_GoNutsRemastered,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_AEggEffect_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_AEggEffect_isPlayerWithinRange, "isPlayerWithinRange" }, // 3405918561
		{ &Z_Construct_UFunction_AEggEffect_onEggEffectBegin, "onEggEffectBegin" }, // 2052956862
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AEggEffect_Statics::Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "EggEffect.h" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "EggEffect.h" },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AEggEffect_Statics::NewProp__playerPawn_MetaData[] = {
		{ "Category", "Egg Effect|General Settings" },
		{ "ModuleRelativePath", "EggEffect.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AEggEffect_Statics::NewProp__playerPawn = { "_playerPawn", nullptr, (EPropertyFlags)0x0020080000000005, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(AEggEffect, _playerPawn), Z_Construct_UClass_APawn_NoRegister, METADATA_PARAMS(Z_Construct_UClass_AEggEffect_Statics::NewProp__playerPawn_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_AEggEffect_Statics::NewProp__playerPawn_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AEggEffect_Statics::NewProp__effectParticleComp_MetaData[] = {
		{ "Category", "Egg Effect|Effect Settings" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "EggEffect.h" },
	};
#endif
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AEggEffect_Statics::NewProp__effectParticleComp = { "_effectParticleComp", nullptr, (EPropertyFlags)0x002008000008000d, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(AEggEffect, _effectParticleComp), Z_Construct_UClass_UParticleSystemComponent_NoRegister, METADATA_PARAMS(Z_Construct_UClass_AEggEffect_Statics::NewProp__effectParticleComp_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_AEggEffect_Statics::NewProp__effectParticleComp_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AEggEffect_Statics::NewProp__eggEffectSig_MetaData[] = {
		{ "Category", "Egg Effect|Effect Settings" },
		{ "Comment", "//UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable, Category = \"Egg Effect|Effect Settings\")\n//FEggEffectDelegate _eggEffectDelegate;\n" },
		{ "ModuleRelativePath", "EggEffect.h" },
		{ "ToolTip", "UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintAssignable, Category = \"Egg Effect|Effect Settings\")\nFEggEffectDelegate _eggEffectDelegate;" },
	};
#endif
	const UE4CodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_AEggEffect_Statics::NewProp__eggEffectSig = { "_eggEffectSig", nullptr, (EPropertyFlags)0x0020080010080005, UE4CodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(AEggEffect, _eggEffectSig), Z_Construct_UDelegateFunction_GoNutsRemastered_EggEffectSignature__DelegateSignature, METADATA_PARAMS(Z_Construct_UClass_AEggEffect_Statics::NewProp__eggEffectSig_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_AEggEffect_Statics::NewProp__eggEffectSig_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_AEggEffect_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AEggEffect_Statics::NewProp__playerPawn,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AEggEffect_Statics::NewProp__effectParticleComp,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AEggEffect_Statics::NewProp__eggEffectSig,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_AEggEffect_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AEggEffect>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_AEggEffect_Statics::ClassParams = {
		&AEggEffect::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		Z_Construct_UClass_AEggEffect_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		UE_ARRAY_COUNT(Z_Construct_UClass_AEggEffect_Statics::PropPointers),
		0,
		0x009000A5u,
		METADATA_PARAMS(Z_Construct_UClass_AEggEffect_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AEggEffect_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AEggEffect()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_AEggEffect_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(AEggEffect, 3434236525);
	template<> GONUTSREMASTERED_API UClass* StaticClass<AEggEffect>()
	{
		return AEggEffect::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_AEggEffect(Z_Construct_UClass_AEggEffect, &AEggEffect::StaticClass, TEXT("/Script/GoNutsRemastered"), TEXT("AEggEffect"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(AEggEffect);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
