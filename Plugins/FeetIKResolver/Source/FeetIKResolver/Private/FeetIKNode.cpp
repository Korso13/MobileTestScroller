#include "FeetIKNode.h"

#include "AnimationCoreLibrary.h"
#include "TwoBoneIK.h"
#include "Animation/AnimInstanceProxy.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

void FFeetIKNode::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{
	Super::Initialize_AnyThread(Context);

	RightFoot.EffectorTarget.Initialize(Context.AnimInstanceProxy);
	RightFoot.JointTarget.Initialize(Context.AnimInstanceProxy);

	LeftFoot.EffectorTarget.Initialize(Context.AnimInstanceProxy);
	LeftFoot.JointTarget.Initialize(Context.AnimInstanceProxy);

	Mesh = Context.AnimInstanceProxy->GetSkelMeshComponent();
	Actor = Mesh->GetOwner();

	CapsuleComponent = Actor->FindComponentByClass<UCapsuleComponent>();
	CharacterMovement = Actor->FindComponentByClass<UCharacterMovementComponent>();
	
}


void FFeetIKNode::EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output,
	TArray<FBoneTransform>& OutBoneTransforms)
{
	//this function is called roughly every tick
	
	Super::EvaluateSkeletalControl_AnyThread(Output, OutBoneTransforms);

	//getting data needed for further steps:
	const FBoneContainer& BoneContainer = Output.Pose.GetPose().GetBoneContainer();

	const float CapsuleHalfHeight = (CapsuleComponent ? CapsuleComponent->GetUnscaledCapsuleHalfHeight() : 0.f);

	TEnumAsByte<enum EMovementMode> MovementMode = (CharacterMovement ? CharacterMovement->MovementMode : EMovementMode::MOVE_None);

	FVector ActorLocation = Actor->GetActorLocation();

	LeftFoot.BoneLocation = Mesh->GetSocketLocation(LeftFoot.SocketName);
	RightFoot.BoneLocation = Mesh->GetSocketLocation(RightFoot.SocketName);

	//checking if the character is moving
	/*if(MovementMode != EMovementMode::MOVE_Walking || MovementMode != EMovementMode::MOVE_Custom)
	{
		return;
	}*/

	//running trace on underlying surface
	IKLineTrace(LeftFoot.HitNormal, PelvisOffset_L, ActorLocation, LeftFoot.BoneLocation, CapsuleHalfHeight);
	IKLineTrace(RightFoot.HitNormal, PelvisOffset_R, ActorLocation, RightFoot.BoneLocation, CapsuleHalfHeight);

	//setting up pelvis offset - thus lowering body mesh if needed
	float PelvisOffsetNew = PelvisOffset_L < PelvisOffset_R ? PelvisOffset_L : PelvisOffset_R;
	PelvisBone.BoneOffset = PelvisBone.BoneOffset + OffsetInterpSpeed * (PelvisOffsetNew - PelvisBone.BoneOffset);
	PelvisBone.BoneTranslation = FVector{0.f, 0.f, PelvisBone.BoneOffset};

	//adjusting feet height
	if(PelvisOffset_L > PelvisOffset_R)
	{
		LeftFoot.BoneOffset += OffsetInterpSpeed * (PelvisOffset_L - PelvisOffset_R - LeftFoot.BoneOffset);
		RightFoot.BoneOffset += OffsetInterpSpeed * (0.f - RightFoot.BoneOffset);
	}
	else
	{
		LeftFoot.BoneOffset += OffsetInterpSpeed * (0.f - LeftFoot.BoneOffset);
		RightFoot.BoneOffset += OffsetInterpSpeed * (PelvisOffset_L - PelvisOffset_R - RightFoot.BoneOffset);
	}

	//creating effector target
	RightFoot.EffectorLocation = FVector{RightFoot.BoneOffset, 0.f, 0.f};
	LeftFoot.EffectorLocation = FVector{LeftFoot.BoneOffset, 0.f, 0.f};

	//getting feet rotation to apply in IK
	float LeftFootPitch{-180.f / PI * FGenericPlatformMath::Atan2(LeftFoot.HitNormal.X, LeftFoot.HitNormal.Z)}; //up-down hills roll of the foot 
	float LeftFootYaw{0.f} ; //we don't turn foot along "leg bone axis" 
	float LeftFootRoll{180.f / PI * FGenericPlatformMath::Atan2(LeftFoot.HitNormal.Y, LeftFoot.HitNormal.Z)};
	LeftFoot.BoneRotation += RotationInterpSpeed * (FRotator{LeftFootPitch, LeftFootYaw, LeftFootRoll} - LeftFoot.BoneRotation); //rotation of foot for slipping to the side (around "finger axis")

	float RightFootPitch{-180.f / PI * FGenericPlatformMath::Atan2(RightFoot.HitNormal.X, RightFoot.HitNormal.Z)}; //up-down hills roll of the foot 
	float RightFootYaw{0.f} ; //we don't turn foot along "leg bone axis" 
	float RightFootRoll{180.f / PI * FGenericPlatformMath::Atan2(RightFoot.HitNormal.Y, RightFoot.HitNormal.Z)};
	RightFoot.BoneRotation += RotationInterpSpeed * (FRotator{RightFootPitch, RightFootYaw, RightFootRoll} - RightFoot.BoneRotation); //rotation of foot for slipping to the side (around "finger axis")

	//applying IK
	//pelvis:
	PelvisSkeletalControl(Output, OutBoneTransforms, PelvisBone.BoneReference, BoneContainer, PelvisBone.BoneTranslation);
	
	//left leg:
	FootSkeletalControl(
		Output,
		OutBoneTransforms,
		LeftFoot.EffectorTarget.BoneReference,
		BoneContainer,
		CachedUpperLimbIndex_L,
		CachedLowerLimbIndex_L,
		LeftFoot.EffectorTarget,
		LeftFoot.EffectorLocation,
		LeftFoot.JointTarget,
		LeftFoot.JointTargetLocation,
		LeftFoot.BoneRotation);

	//right leg:
	FootSkeletalControl(
		Output,
		OutBoneTransforms,
		RightFoot.EffectorTarget.BoneReference,
		BoneContainer,
		CachedUpperLimbIndex_R,
		CachedLowerLimbIndex_R,
		RightFoot.EffectorTarget,
		RightFoot.EffectorLocation,
		RightFoot.JointTarget,
		RightFoot.JointTargetLocation,
		RightFoot.BoneRotation);
}

bool FFeetIKNode::IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones)
{
	//pelvis check:
	if(!PelvisBone.BoneReference.IsValidToEvaluate(RequiredBones))
	{
		return false;
	}

	//left foot check:
	if((CachedLowerLimbIndex_L == INDEX_NONE) ||
		(CachedUpperLimbIndex_L == INDEX_NONE) ||
		(!LeftFoot.EffectorTarget.BoneReference.IsValidToEvaluate(RequiredBones)) ||
		(!LeftFoot.JointTarget.IsValidToEvaluate(RequiredBones)))
	{
		return false;
	}

	//right foot check:
	if((CachedLowerLimbIndex_R == INDEX_NONE) ||
	(CachedUpperLimbIndex_R == INDEX_NONE) ||
	(!RightFoot.EffectorTarget.BoneReference.IsValidToEvaluate(RequiredBones)) ||
	(!RightFoot.JointTarget.IsValidToEvaluate(RequiredBones)))
	{
		return false;
	}
	
	return true;
}

void FFeetIKNode::InitializeBoneReferences(const FBoneContainer& RequiredBones)
{
	Super::InitializeBoneReferences(RequiredBones);

	//Initialize bone references:
	PelvisBone.BoneReference.Initialize(RequiredBones);

	RightFoot.EffectorTarget.InitializeBoneReferences(RequiredBones);
	RightFoot.JointTarget.InitializeBoneReferences(RequiredBones);

	LeftFoot.EffectorTarget.InitializeBoneReferences(RequiredBones);
	LeftFoot.JointTarget.InitializeBoneReferences(RequiredBones);

	//getting indices of "upper" and "lower" bones
	
	//for left leg:
	FCompactPoseBoneIndex IKBoneCompactPoseIndex_L = LeftFoot.EffectorTarget.BoneReference.GetCompactPoseIndex(RequiredBones);
	//getting bone index of left Effector
	if(IKBoneCompactPoseIndex_L != INDEX_NONE)
	{
		//now we get index for bone up from the effector
		CachedLowerLimbIndex_L = RequiredBones.GetParentBoneIndex(IKBoneCompactPoseIndex_L);
		//and up from pelvis
		if(CachedLowerLimbIndex_L != INDEX_NONE)
		{
			CachedUpperLimbIndex_L = RequiredBones.GetParentBoneIndex(CachedLowerLimbIndex_L);
		}
	}

	//now the same for right leg:
	FCompactPoseBoneIndex IKBoneCompactPoseIndex_R = RightFoot.EffectorTarget.BoneReference.GetCompactPoseIndex(RequiredBones);
	//getting bone index of right Effector
	if(IKBoneCompactPoseIndex_R != INDEX_NONE)
	{
		//now we get index for bone up from the effector
		CachedLowerLimbIndex_R = RequiredBones.GetParentBoneIndex(IKBoneCompactPoseIndex_R);
		//and up from pelvis
		if(CachedLowerLimbIndex_R != INDEX_NONE)
		{
			CachedUpperLimbIndex_R = RequiredBones.GetParentBoneIndex(CachedLowerLimbIndex_R);
		}
	}
}

void FFeetIKNode::IKLineTrace(FVector_NetQuantizeNormal& HitNormal, float& PelvisOffset, const FVector& ActorLocation,
	const FVector& FootLocation, float CapsuleHalfHeight)
{
	FHitResult HitResult;

	if(Actor && !IgnoredActors.Contains(Actor))
	{
		IgnoredActors.Add(Actor);
	}
	
	FVector TraceStart{FVector{FootLocation.X, FootLocation.Y, ActorLocation.Z}};
	FVector TraceEnd{FVector{FootLocation.X, FootLocation.Y, (ActorLocation.Z - CapsuleHalfHeight - IKApplyDistance)}};

	UKismetSystemLibrary::LineTraceSingle(Mesh, TraceStart, TraceEnd, TraceTypeQuery1,
		false, IgnoredActors, EDrawDebugTrace::None, HitResult, true);

	HitNormal = HitResult.Normal;

	if(HitResult.bBlockingHit)
	{
		PelvisOffset = CapsuleHalfHeight - HitResult.Distance;
	}
	else
	{
		PelvisOffset = 0.f;
	}
}

void FFeetIKNode::PelvisSkeletalControl(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms,
										const FBoneReference& BoneReference, const FBoneContainer& BoneContainer,
										const FVector& Translation)
{
	const FCompactPoseBoneIndex CompactPoseBoneReference{BoneReference.GetCompactPoseIndex(BoneContainer)};
	FTransform NewBoneTransform{Output.Pose.GetComponentSpaceTransform(CompactPoseBoneReference)};
	const FTransform ComponentTransform{Output.AnimInstanceProxy->GetComponentTransform()};

	//converting NewBoneTransform into BoneSpace transform
	FAnimationRuntime::ConvertCSTransformToBoneSpace(
		ComponentTransform,
		Output.Pose,
		NewBoneTransform,
		CompactPoseBoneReference,
		BCS_ComponentSpace
		);

	//applying IK to the pelvis bone:
	NewBoneTransform.AddToTranslation(Translation);

	//converting back to CSTransform
	FAnimationRuntime::ConvertBoneSpaceTransformToCS(
	ComponentTransform,
	Output.Pose,
	NewBoneTransform,
	CompactPoseBoneReference,
	BCS_ComponentSpace
	);

	//applying changes to pose:
	OutBoneTransforms.Add(FBoneTransform{BoneReference.GetCompactPoseIndex(BoneContainer), NewBoneTransform});
}

void FFeetIKNode::FootSkeletalControl(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms,
                                      const FBoneReference& BoneReference, const FBoneContainer& BoneContainer,
                                      const FCompactPoseBoneIndex& CachedUpperLimbIndex, const FCompactPoseBoneIndex& CachedLowerLimbIndex,
                                      const FBoneSocketTarget& EffectorTarget, const FVector& EffectorLocation,
                                      const FBoneSocketTarget& JointTarget, const FVector& JointLocation,
                                      const FRotator& Rotation)
{
	//index for edited bone (BoneReference):
	FCompactPoseBoneIndex BoneReferenceIndex{BoneReference.GetCompactPoseIndex(BoneContainer)};

	//get component space transforms for related bones:
	FTransform UpperLimbCSTransform {Output.Pose.GetComponentSpaceTransform(CachedUpperLimbIndex)};
	FTransform LowerLimbCSTransform{Output.Pose.GetComponentSpaceTransform(CachedLowerLimbIndex)};
	FTransform EndBoneCSTransform {Output.Pose.GetComponentSpaceTransform(BoneReferenceIndex)};

	//creating vectors for target positions of the effector and the Joint
	FVector EffectorTargetPosition {EffectorTarget.GetTargetTransform(
		EffectorLocation,
		Output.Pose,
		Output.AnimInstanceProxy->GetComponentTransform()).GetTranslation() + PelvisBone.BoneTranslation};
	FVector JointTargetPosition {JointTarget.GetTargetTransform(
		JointLocation,
		Output.Pose,
		Output.AnimInstanceProxy->GetComponentTransform()).GetTranslation()};

	//correcting bones' position to correspond to pelvis bone's translation (lowering)
	UpperLimbCSTransform.SetLocation(UpperLimbCSTransform.GetTranslation() + PelvisBone.BoneTranslation);
	LowerLimbCSTransform.SetLocation(LowerLimbCSTransform.GetTranslation() + PelvisBone.BoneTranslation);
	EndBoneCSTransform.SetLocation(EndBoneCSTransform.GetTranslation() + PelvisBone.BoneTranslation);

	//calling embedded IK function:
	AnimationCore::SolveTwoBoneIK(
		UpperLimbCSTransform,
		LowerLimbCSTransform,
		EndBoneCSTransform,
		JointTargetPosition,
		EffectorTargetPosition,
		false,
		1.f,
		1.2f);

	//applying rotation:
	const FTransform ComponentTransform{Output.AnimInstanceProxy->GetComponentTransform()};
	
	FAnimationRuntime::ConvertCSTransformToBoneSpace(
		ComponentTransform,
		Output.Pose,
		EndBoneCSTransform,
		BoneReferenceIndex,
		BCS_WorldSpace
		);

	//setting quaternion rotation for foot
	const FQuat BoneQuat{Rotation};
	EndBoneCSTransform.SetRotation(BoneQuat * EndBoneCSTransform.GetRotation());

	FAnimationRuntime::ConvertBoneSpaceTransformToCS(
	ComponentTransform,
	Output.Pose,
	EndBoneCSTransform,
	BoneReferenceIndex,
	BCS_WorldSpace
	);

	//applying transforms. PRESERVE ORDER!
	OutBoneTransforms.Add(FBoneTransform{CachedUpperLimbIndex, UpperLimbCSTransform});
	OutBoneTransforms.Add(FBoneTransform{CachedLowerLimbIndex, LowerLimbCSTransform});
	OutBoneTransforms.Add(FBoneTransform{BoneReferenceIndex, EndBoneCSTransform});
}