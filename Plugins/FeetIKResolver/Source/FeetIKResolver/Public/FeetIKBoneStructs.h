#pragma once

#include "CoreMinimal.h"
#include "BoneControllers/AnimNode_SkeletalControlBase.h"
#include "FeetIKBoneStructs.generated.h"

USTRUCT()
struct FIK_Foot
{
	GENERATED_BODY()

	//Effectors:
	UPROPERTY(EditAnywhere, Category = "SkeletalControl")
	FBoneSocketTarget EffectorTarget;

	FVector EffectorLocation;
	
	//Joints
	UPROPERTY(EditAnywhere, Category = "SkeletalControl")
	FBoneSocketTarget JointTarget;

	UPROPERTY(EditAnywhere, Category = "SkeletalControl")
	FVector JointTargetLocation;

	//Line tracing:
	bool bIsHit;
	
	FVector_NetQuantizeNormal HitNormal;

	float HitNormalMax;
	
	//Other vars:
	UPROPERTY(EditAnywhere, Category = "SkeletalControl")
	FName SocketName;

	FVector BoneLocation;
	
	FRotator BoneRotation;

	float BoneOffset;
};

USTRUCT()
struct FIK_Pelvis
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "SkeletalControl")
	FBoneReference BoneReference;

	float BoneOffset;

	FVector BoneTranslation;
};