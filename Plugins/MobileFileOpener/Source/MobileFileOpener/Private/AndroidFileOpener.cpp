// Fill out your copyright notice in the Description page of Project Settings.


#include "AndroidFileOpener.h"

#if PLATFORM_ANDROID

#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"

#define DECLARE_JAVA_METHOD(name) \
static jmethodID name = NULL;

//static jmethodID AndroidThunkJava_OpenMediaFile;
DECLARE_JAVA_METHOD(AndroidThunkJava_OpenMediaFile)

#define INIT_JAVA_METHOD(name, signature) \
if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true)) { \
name = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, #name, signature, false); \
check(name != NULL); \
} else { \
check(0); \
}

extern "C"
{
	JNIEXPORT void Java_com_epicgames_ue4_GameActivity_onFileSelected(JNIEnv* jni, jclass clazz, jbyteArray mediaBytes)
	{
		AsyncTask(ENamedThreads::GameThread, [=]()
		 {
		  if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
		  {
			jbyte* dataPtr = Env->GetByteArrayElements(mediaBytes, NULL);
			jsize len = Env->GetArrayLength(mediaBytes);
			TArray<uint8> result((uint8*)dataPtr, (int32)len);
		  	GEngine->AddOnScreenDebugMessage(4, 3,FColor::Green, FString::Printf(TEXT("Redirecting byte array via delegate")));
			OnFileSelectedProxy.ExecuteIfBound(result);
		  }
		 }
		); //AsyncTask end
	}
}

#endif

AndroidFileOpener::AndroidFileOpener()
{
#if PLATFORM_ANDROID
	OnFileSelectedProxy = OnFileSelected; //first one is delegate from interface, second - from xml file
	INIT_JAVA_METHOD(AndroidThunkJava_OpenMediaFile, "()V")
#endif
}

void AndroidFileOpener::OpenMedia()
{
#if PLATFORM_ANDROID
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv(true))
	{
		GEngine->AddOnScreenDebugMessage(5, 3,FColor::Green, FString::Printf(TEXT("Calling Java method to open file")));
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, AndroidThunkJava_OpenMediaFile);
	}
#endif
}
