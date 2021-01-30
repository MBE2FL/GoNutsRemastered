// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class AEggEffect;
class UPrimitiveComponent;
class AActor;
struct FHitResult;
#ifdef GONUTSREMASTERED_Egg_generated_h
#error "Egg.generated.h already included, missing '#pragma once' in Egg.h"
#endif
#define GONUTSREMASTERED_Egg_generated_h

#define GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_Egg_h_15_DELEGATE \
struct _Script_GoNutsRemastered_eventEggEffectDelegate_Parms \
{ \
	AEggEffect* eggEffect; \
}; \
static inline void FEggEffectDelegate_DelegateWrapper(const FMulticastScriptDelegate& EggEffectDelegate, AEggEffect* eggEffect) \
{ \
	_Script_GoNutsRemastered_eventEggEffectDelegate_Parms Parms; \
	Parms.eggEffect=eggEffect; \
	EggEffectDelegate.ProcessMulticastDelegate<UObject>(&Parms); \
}


#define GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_Egg_h_21_SPARSE_DATA
#define GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_Egg_h_21_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execOnOverlapEnd); \
	DECLARE_FUNCTION(execOnOverlapBegin);


#define GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_Egg_h_21_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execOnOverlapEnd); \
	DECLARE_FUNCTION(execOnOverlapBegin);


#define GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_Egg_h_21_EVENT_PARMS \
	struct Egg_eventonEggImpact_Parms \
	{ \
		TSubclassOf<AEggEffect>  eggEffectType; \
		AEggEffect* eggEffectActor; \
	};


#define GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_Egg_h_21_CALLBACK_WRAPPERS
#define GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_Egg_h_21_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAEgg(); \
	friend struct Z_Construct_UClass_AEgg_Statics; \
public: \
	DECLARE_CLASS(AEgg, AActor, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/GoNutsRemastered"), NO_API) \
	DECLARE_SERIALIZER(AEgg)


#define GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_Egg_h_21_INCLASS \
private: \
	static void StaticRegisterNativesAEgg(); \
	friend struct Z_Construct_UClass_AEgg_Statics; \
public: \
	DECLARE_CLASS(AEgg, AActor, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/GoNutsRemastered"), NO_API) \
	DECLARE_SERIALIZER(AEgg)


#define GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_Egg_h_21_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API AEgg(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(AEgg) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AEgg); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AEgg); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AEgg(AEgg&&); \
	NO_API AEgg(const AEgg&); \
public:


#define GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_Egg_h_21_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AEgg(AEgg&&); \
	NO_API AEgg(const AEgg&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AEgg); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AEgg); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(AEgg)


#define GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_Egg_h_21_PRIVATE_PROPERTY_OFFSET \
	FORCEINLINE static uint32 __PPO___debugColour() { return STRUCT_OFFSET(AEgg, _debugColour); } \
	FORCEINLINE static uint32 __PPO___eggEffectDelegate() { return STRUCT_OFFSET(AEgg, _eggEffectDelegate); } \
	FORCEINLINE static uint32 __PPO___eggMeshComp() { return STRUCT_OFFSET(AEgg, _eggMeshComp); } \
	FORCEINLINE static uint32 __PPO___impactParticleComponent() { return STRUCT_OFFSET(AEgg, _impactParticleComponent); } \
	FORCEINLINE static uint32 __PPO___eggEffectType() { return STRUCT_OFFSET(AEgg, _eggEffectType); }


#define GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_Egg_h_18_PROLOG \
	GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_Egg_h_21_EVENT_PARMS


#define GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_Egg_h_21_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_Egg_h_21_PRIVATE_PROPERTY_OFFSET \
	GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_Egg_h_21_SPARSE_DATA \
	GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_Egg_h_21_RPC_WRAPPERS \
	GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_Egg_h_21_CALLBACK_WRAPPERS \
	GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_Egg_h_21_INCLASS \
	GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_Egg_h_21_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_Egg_h_21_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_Egg_h_21_PRIVATE_PROPERTY_OFFSET \
	GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_Egg_h_21_SPARSE_DATA \
	GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_Egg_h_21_RPC_WRAPPERS_NO_PURE_DECLS \
	GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_Egg_h_21_CALLBACK_WRAPPERS \
	GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_Egg_h_21_INCLASS_NO_PURE_DECLS \
	GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_Egg_h_21_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> GONUTSREMASTERED_API UClass* StaticClass<class AEgg>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID GoNutsRemastered_GoNutsRemastered_4_25_Source_GoNutsRemastered_Egg_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
