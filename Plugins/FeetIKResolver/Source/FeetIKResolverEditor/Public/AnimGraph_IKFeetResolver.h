// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimGraphNode_SkeletalControlBase.h"
#include "FeetIKResolver/Public/FeetIKNode.h"
#include "AnimGraph_IKFeetResolver.generated.h"

UCLASS()
class FEETIKRESOLVEREDITOR_API UAnimGraph_IKFeetResolver : public UAnimGraphNode_SkeletalControlBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Settings")
	FFeetIKNode Node;

	virtual FText GetTooltipText() const override;

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

	virtual FLinearColor GetNodeTitleColor() const override;

	virtual FString GetNodeCategory() const override;

	virtual const FAnimNode_SkeletalControlBase* GetNode() const override; 

};