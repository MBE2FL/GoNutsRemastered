// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef OURPROJECT_OurProjectCharacter_generated_h
#error "OurProjectCharacter.generated.h already included, missing '#pragma once' in OurProjectCharacter.h"
#endif
#define OURPROJECT_OurProjectCharacter_generated_h

#define OurProject_Source_OurProject_OurProjectCharacter_h_12_SPARSE_DATA
#define OurProject_Source_OurProject_OurProjectCharacter_h_12_RPC_WRAPPERS
#define OurProject_Source_OurProject_OurProjectCharacter_h_12_RPC_WRAPPERS_NO_PURE_DECLS
#define OurProject_Source_OurProject_OurProjectCharacter_h_12_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAOurProjectCharacter(); \
	friend struct Z_Construct_UClass_AOurProjectCharacter_Statics; \
public: \
	DECLARE_CLASS(AOurProjectCharacter, ACharacter, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/OurProject"), NO_API) \
	DECLARE_SERIALIZER(AOurProjectCharacter)


#define OurProject_Source_OurProject_OurProjectCharacter_h_12_INCLASS \
private: \
	static void StaticRegisterNativesAOurProjectCharacter(); \
	friend struct Z_Construct_UClass_AOurProjectCharacter_Statics; \
public: \
	DECLARE_CLASS(AOurProjectCharacter, ACharacter, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/OurProject"), NO_API) \
	DECLARE_SERIALIZER(AOurProjectCharacter)


#define OurProject_Source_OurProject_OurProjectCharacter_h_12_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API AOurProjectCharacter(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(AOurProjectCharacter) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AOurProjectCharacter); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AOurProjectCharacter); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AOurProjectCharacter(AOurProjectCharacter&&); \
	NO_API AOurProjectCharacter(const AOurProjectCharacter&); \
public:


#define OurProject_Source_OurProject_OurProjectCharacter_h_12_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AOurProjectCharacter(AOurProjectCharacter&&); \
	NO_API AOurProjectCharacter(const AOurProjectCharacter&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AOurProjectCharacter); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AOurProjectCharacter); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(AOurProjectCharacter)


#define OurProject_Source_OurProject_OurProjectCharacter_h_12_PRIVATE_PROPERTY_OFFSET \
	FORCEINLINE static uint32 __PPO__CameraBoom() { return STRUCT_OFFSET(AOurProjectCharacter, CameraBoom); } \
	FORCEINLINE static uint32 __PPO__FollowCamera() { return STRUCT_OFFSET(AOurProjectCharacter, FollowCamera); }


#define OurProject_Source_OurProject_OurProjectCharacter_h_9_PROLOG
#define OurProject_Source_OurProject_OurProjectCharacter_h_12_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	OurProject_Source_OurProject_OurProjectCharacter_h_12_PRIVATE_PROPERTY_OFFSET \
	OurProject_Source_OurProject_OurProjectCharacter_h_12_SPARSE_DATA \
	OurProject_Source_OurProject_OurProjectCharacter_h_12_RPC_WRAPPERS \
	OurProject_Source_OurProject_OurProjectCharacter_h_12_INCLASS \
	OurProject_Source_OurProject_OurProjectCharacter_h_12_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define OurProject_Source_OurProject_OurProjectCharacter_h_12_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	OurProject_Source_OurProject_OurProjectCharacter_h_12_PRIVATE_PROPERTY_OFFSET \
	OurProject_Source_OurProject_OurProjectCharacter_h_12_SPARSE_DATA \
	OurProject_Source_OurProject_OurProjectCharacter_h_12_RPC_WRAPPERS_NO_PURE_DECLS \
	OurProject_Source_OurProject_OurProjectCharacter_h_12_INCLASS_NO_PURE_DECLS \
	OurProject_Source_OurProject_OurProjectCharacter_h_12_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> OURPROJECT_API UClass* StaticClass<class AOurProjectCharacter>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID OurProject_Source_OurProject_OurProjectCharacter_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
