#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "RuntimeAudioImporterLibrary.h"
#include "Base64ToSoundWave.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBase64ToSoundWaveCompleted, UImportedSoundWave*, SoundWave, const FString&, ErrorMessage);

UCLASS()
class UBase64ToSoundWave : public UBlueprintAsyncActionBase
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Audio", meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "Create Sound Wave from Base64"))
    static UBase64ToSoundWave* CreateSoundWaveFromBase64(UObject* WorldContextObject, const FString& Base64String);

    UPROPERTY(BlueprintAssignable)
    FOnBase64ToSoundWaveCompleted OnCompleted;

private:
    void ConvertBase64ToSoundWave(const FString& Base64String);
    void CreateSoundWaveFromResponse(const TArray<uint8>& AudioData);

    UPROPERTY()
    UObject* WorldContextObject;

    UPROPERTY()
    URuntimeAudioImporterLibrary* RuntimeAudioImporter;
};