// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FileOpener.h"


static FOnFileSelected OnFileSelectedProxy;
#if PLATFORM_ANDROID
static struct _JNIEnv *ENV;
#endif
/**
 * 
 */
class MOBILEFILEOPENER_API AndroidFileOpener : public IFileOpener
{
public:
	AndroidFileOpener();

	virtual void OpenMedia() override;
	
};
