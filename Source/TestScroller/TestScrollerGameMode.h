// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TestScrollerGameMode.generated.h"

class IFileOpener;

UCLASS(minimalapi)
class ATestScrollerGameMode : public AGameModeBase
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	
	IFileOpener* MediaFileOpener;
	
public:
	ATestScrollerGameMode();

	UFUNCTION(BlueprintCallable)
	void OpenMediaFile();

	void OnMediaSelected(TArray<uint8> InMediaBytes);

	UTexture2D* LoadMediaFromBytes(const TArray<uint8>& InMediaBytes);

	//needs changing
	UFUNCTION(BlueprintImplementableEvent) 
	void BP_OnMediaSelected(class UTexture2D* SelectedPhoto);
	
};




