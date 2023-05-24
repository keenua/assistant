#pragma once

#include "Engine.h"
#include "RuntimeAudioImporterLibrary.h"
#include "MotionFrame.generated.h"

USTRUCT(BlueprintType)
struct FMotionFrame
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    int Index = 0;

    UPROPERTY(BlueprintReadWrite)
    FString Motion = TEXT("");

    UPROPERTY(BlueprintReadWrite)
    TArray<uint8> Audio;

    UPROPERTY(BlueprintReadWrite)
    FString AudioString = TEXT("");

    UPROPERTY(BlueprintReadWrite)
    FString Emotion = TEXT("");

    UPROPERTY(BlueprintReadWrite)
    FString Text = TEXT("");

    UPROPERTY(BlueprintReadWrite)
    FString Viseme = TEXT("");
};