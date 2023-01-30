#pragma once
#include "CoreMinimal.h"
#include "FeetIKBoneStructs.h"
#include "BoneControllers/AnimNode_SkeletalControlBase.h"
#include "FeetIKNode.generated.h"

USTRUCT(BlueprintInternalUseOnly)
struct FEETIKRESOLVER_API FFeetIKNode : public FAnimNode_SkeletalControlBase
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, Category = "SkeletalControl")
	FIK_Pelvis PelvisBone;

	UPROPERTY(EditAnywhere, Category = "SkeletalControl")
	FIK_Foot LeftFoot;

	UPROPERTY(EditAnywhere, Category = "SkeletalControl")
	FIK_Foot RightFoot;

	UPROPERTY(EditAnywhere, Category = Calculations, meta = (ClampMin = "0.0", DisplayName = "Feet IK Apply distance"))
	float IKApplyDistance;
	
	UPROPERTY(EditAnywhere, Category = Calculations, meta = (ClampMin = "0.0", ClampMax = "1.0", DisplayName = "Feet offset interpolation speed (0-1)"))
	float OffsetInterpSpeed;
	
	UPROPERTY(EditAnywhere, Category = Calculations, meta = (ClampMin = "0.0", ClampMax = "1.0", DisplayName = "Feet rotation interpolation speed (0-1)"))
	float RotationInterpSpeed;

	UPROPERTY(EditAnywhere, Category = "SkeletalControl")
	TArray<AActor*> IgnoredActors;
	
	float PelvisOffset_R;
	float PelvisOffset_L;

private:

	UPROPERTY()
	class AActor* Actor{nullptr};

	UPROPERTY()
	class USkeletalMeshComponent* Mesh{nullptr};

	UPROPERTY()
	class UCapsuleComponent* CapsuleComponent{nullptr};

	UPROPERTY()
	class UCharacterMovementComponent* CharacterMovement{nullptr};

	FCompactPoseBoneIndex CachedLowerLimbIndex_L{INDEX_NONE};
	FCompactPoseBoneIndex CachedLowerLimbIndex_R{INDEX_NONE};

	FCompactPoseBoneIndex CachedUpperLimbIndex_L{INDEX_NONE};
	FCompactPoseBoneIndex CachedUpperLimbIndex_R{INDEX_NONE};
	
public:
	
	virtual void Initialize_AnyThread(const FAnimationInitializeContext & Context) override;

	virtual void EvaluateSkeletalControl_AnyThread( FComponentSpacePoseContext & Output, TArray< FBoneTransform > & OutBoneTransforms) override;

	virtual bool IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones) override;

	virtual void InitializeBoneReferences(const FBoneContainer& RequiredBones) override;

	//firing trace for passed foot from pelvis
	void IKLineTrace(FVector_NetQuantizeNormal& HitNormal, float& PelvisOffset, const FVector& ActorLocation, const FVector& FootLocation, float
	                 CapsuleHalfHeight);

	//IK functions:

	void PelvisSkeletalControl(FComponentSpacePoseContext& Output,
		TArray<FBoneTransform>& OutBoneTransforms,
		const FBoneReference& BoneReference,
		const FBoneContainer& BoneContainer,
		const FVector& Translation);
	
	void FootSkeletalControl(FComponentSpacePoseContext& Output,
		TArray<FBoneTransform>& OutBoneTransforms,
		const FBoneReference& BoneReference,
		const FBoneContainer& BoneContainer,
		const FCompactPoseBoneIndex& CachedUpperLimbIndex,
		const FCompactPoseBoneIndex& CachedLowerLimbIndex,
		const FBoneSocketTarget& EffectorTarget,
		const FVector& EffectorLocation,
		const FBoneSocketTarget& JointTarget,
		const FVector& JointLocation,
		const FRotator& Rotation);
};

