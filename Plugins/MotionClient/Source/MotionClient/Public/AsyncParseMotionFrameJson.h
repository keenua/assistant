#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "MotionFrame.h"
#include "MotionController.h"
#include "AsyncParseMotionFrameJson.generated.h"

UCLASS()
class MOTIONCLIENT_API UAsyncParseMotionFrameJson : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Async", meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "Parse Motion Frame Json"))
    static UAsyncParseMotionFrameJson* ParseMotionFrameJson(UObject* WorldContextObject, const FString& JsonString, AMotionController* MotionController);

private:
    void ParseJson(const FString& JsonString, AMotionController* MotionController);
    void CreateSoundWaveFromBase64(const FString& Base64String, TFunction<void(UImportedSoundWave*)> OnSoundWaveCreated);

    UPROPERTY()
    UObject* WorldContextObject;

    int32 PendingAudioImports;
    TArray<TSharedPtr<FMotionFrame>> ParsedFrames;
};