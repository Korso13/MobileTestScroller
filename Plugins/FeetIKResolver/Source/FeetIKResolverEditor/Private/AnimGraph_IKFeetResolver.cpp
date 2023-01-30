// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimGraph_IKFeetResolver.h"

#define LOCTEXT_NAMESPACE "FeetIK"
FText UAnimGraph_IKFeetResolver::GetTooltipText() const
{
	//return FText::FromString(TEXT("Feet inverse kinematics resolver"));
	return LOCTEXT("FeetIK","Feet inverse kinematics resolver");
}

FText UAnimGraph_IKFeetResolver::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("FeetIK Resolver","FeetIK Resolver");
}
#undef LOCTEXT_NAMESPACE



FLinearColor UAnimGraph_IKFeetResolver::GetNodeTitleColor() const
{
	return FLinearColor::Yellow;
}

FString UAnimGraph_IKFeetResolver::GetNodeCategory() const
{
	return TEXT("IK");
}

const FAnimNode_SkeletalControlBase* UAnimGraph_IKFeetResolver::GetNode() const
{
	return &Node;
}

