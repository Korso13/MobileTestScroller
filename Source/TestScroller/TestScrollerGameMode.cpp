// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestScrollerGameMode.h"
#include "TestScrollerCharacter.h"

#if PLATFORM_ANDROID || PLATFORM_WINDOWS
#include "ThirdPartyLibraries/PugiXMLLib/src/pugixml.hpp"
#include "ThirdPartyLibraries/json11/json11.hpp"
#endif

#include "MobileFileOpener/Public/FileOpener.h"
#if PLATFORM_ANDROID
#include "MobileFileOpener/Public/AndroidFileOpener.h"
#endif
#include "UObject/ConstructorHelpers.h"

void ATestScrollerGameMode::BeginPlay()
{
	Super::BeginPlay();

#if PLATFORM_ANDROID || PLATFORM_WINDOWS
	
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("tree.xml");
	if(result.status == pugi::status_file_not_found)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1,FColor::Green, FString::Printf(TEXT("pugiXML module output: No such file found")));
	}

	json11::Json my_json = json11::Json::object {
	    { "key1", "value1" },
		{ "key2", false },
		{ "key3", json11::Json::array { 1, 2, 3 } },
	};
	std::string json_str = my_json.dump();
	FString OutString = UTF8_TO_TCHAR(json_str.c_str());
	 
	GEngine->AddOnScreenDebugMessage(-1, 1,FColor::Green, FString::Printf(TEXT("json11 module output: %s"), ToCStr(OutString)));
#endif

#if PLATFORM_ANDROID
	//MediaFileOpener = new AndroidFileOpener;
#endif
}

ATestScrollerGameMode::ATestScrollerGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/SideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ATestScrollerGameMode::OpenMediaFile()
{
	GEngine->AddOnScreenDebugMessage(3, 3,FColor::Green, FString::Printf(TEXT("OpenMediaFile in GameMode called")));

#if PLATFORM_ANDROID
	if(MediaFileOpener)
	{
		GEngine->AddOnScreenDebugMessage(4, 3,FColor::Green, FString::Printf(TEXT("MediaFileOpener is valid")));
	
		MediaFileOpener->OnFileSelected.BindUObject(this, &ATestScrollerGameMode::OnMediaSelected); //or onFileSelected ?!
		MediaFileOpener->OpenMedia();
	}
#endif
}

void ATestScrollerGameMode::OnMediaSelected(TArray<uint8> InMediaBytes)
{
	//this function is called via Delegate carrying media's byte array
	//for now it just notifies it got smth

	const int32 len = InMediaBytes.Num();
	GEngine->AddOnScreenDebugMessage(1, 3,FColor::Green, FString::Printf(TEXT("OnMediaSelected called via OnFileSelectedProxy. Incoming byte array length: %d"), len));
	AsyncTask(ENamedThreads::GameThread, [=]()
	 {
		UTexture2D* ImportedTexture = LoadMediaFromBytes(InMediaBytes);
		if(ImportedTexture)
		{
			BP_OnMediaSelected(ImportedTexture); // BlueprintImplementableEvent
		}
	 }
	);
}

UTexture2D* ATestScrollerGameMode::LoadMediaFromBytes(const TArray<uint8>& InMediaBytes)
{
	GEngine->AddOnScreenDebugMessage(2, 3,FColor::Green, FString::Printf(TEXT("LoadMediaFromBytes called")));
	return nullptr;
}
