// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef GONUTSREMASTERED_EggEffect_generated_h
#error "EggEffect.generated.h already included, missing '#pragma once' in EggEffect.h"
#endif
#define GONUTSREMASTERED_EggEffect_generated_h

#define GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_EggEffect_h_18_DELEGATE \
struct _Script_GoNutsRemastered_eventEggEffectSignature_Parms \
{ \
	float paramOne; \
}; \
static inline void FEggEffectSignature_DelegateWrapper(const FMulticastScriptDelegate& EggEffectSignature, float paramOne) \
{ \
	_Script_GoNutsRemastered_eventEggEffectSignature_Parms Parms; \
	Parms.paramOne=paramOne; \
	EggEffectSignature.ProcessMulticastDelegate<UObject>(&Parms); \
}


#define GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_EggEffect_h_24_SPARSE_DATA
#define GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_EggEffect_h_24_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execisPlayerWithinRange);


#define GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_EggEffect_h_24_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execisPlayerWithinRange);


#define GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_EggEffect_h_24_EVENT_PARMS
#define GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_EggEffect_h_24_CALLBACK_WRAPPERS
#define GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_EggEffect_h_24_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAEggEffect(); \
	friend struct Z_Construct_UClass_AEggEffect_Statics; \
public: \
	DECLARE_CLASS(AEggEffect, AActor, COMPILED_IN_FLAGS(CLASS_Abstract | CLASS_Config), CASTCLASS_None, TEXT("/Script/GoNutsRemastered"), NO_API) \
	DECLARE_SERIALIZER(AEggEffect)


#define GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_EggEffect_h_24_INCLASS \
private: \
	static void StaticRegisterNativesAEggEffect(); \
	friend struct Z_Construct_UClass_AEggEffect_Statics; \
public: \
	DECLARE_CLASS(AEggEffect, AActor, COMPILED_IN_FLAGS(CLASS_Abstract | CLASS_Config), CASTCLASS_None, TEXT("/Script/GoNutsRemastered"), NO_API) \
	DECLARE_SERIALIZER(AEggEffect)


#define GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_EggEffect_h_24_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API AEggEffect(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(AEggEffect) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AEggEffect); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AEggEffect); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AEggEffect(AEggEffect&&); \
	NO_API AEggEffect(const AEggEffect&); \
public:


#define GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_EggEffect_h_24_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AEggEffect(AEggEffect&&); \
	NO_API AEggEffect(const AEggEffect&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AEggEffect); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AEggEffect); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(AEggEffect)


#define GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_EggEffect_h_24_PRIVATE_PROPERTY_OFFSET \
	FORCEINLINE static uint32 __PPO___eggEffectSig() { return STRUCT_OFFSET(AEggEffect, _eggEffectSig); } \
	FORCEINLINE static uint32 __PPO___effectParticleComp() { return STRUCT_OFFSET(AEggEffect, _effectParticleComp); } \
	FORCEINLINE static uint32 __PPO___playerPawn() { return STRUCT_OFFSET(AEggEffect, _playerPawn); }


#define GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_EggEffect_h_21_PROLOG \
	GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_EggEffect_h_24_EVENT_PARMS


#define GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_EggEffect_h_24_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_EggEffect_h_24_PRIVATE_PROPERTY_OFFSET \
	GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_EggEffect_h_24_SPARSE_DATA \
	GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_EggEffect_h_24_RPC_WRAPPERS \
	GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_EggEffect_h_24_CALLBACK_WRAPPERS \
	GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_EggEffect_h_24_INCLASS \
	GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_EggEffect_h_24_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_EggEffect_h_24_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_EggEffect_h_24_PRIVATE_PROPERTY_OFFSET \
	GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_EggEffect_h_24_SPARSE_DATA \
	GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_EggEffect_h_24_RPC_WRAPPERS_NO_PURE_DECLS \
	GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_EggEffect_h_24_CALLBACK_WRAPPERS \
	GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_EggEffect_h_24_INCLASS_NO_PURE_DECLS \
	GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_EggEffect_h_24_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> GONUTSREMASTERED_API UClass* StaticClass<class AEggEffect>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_EggEffect_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
