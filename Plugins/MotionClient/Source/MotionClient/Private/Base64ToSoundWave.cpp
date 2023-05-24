#include "Base64ToSoundWave.h"
#include "Sound/SoundWave.h"
#include "RuntimeAudioImporterLibrary.h"
#include "HAL/Event.h"


UBase64ToSoundWave* UBase64ToSoundWave::CreateSoundWaveFromBase64(UObject* WorldContextObject, const FString& Base64String)
{
    UBase64ToSoundWave* Converter = NewObject<UBase64ToSoundWave>();
    Converter->WorldContextObject = WorldContextObject;
    Converter->ConvertBase64ToSoundWave(Base64String);
    return Converter;
}

void UBase64ToSoundWave::ConvertBase64ToSoundWave(const FString& Base64String)
{
    TArray<uint8> AudioData;
    if (FBase64::Decode(Base64String, AudioData))
    {
        CreateSoundWaveFromResponse(AudioData);
    }
    else
    {
        FString ErrorMessage = "Failed to decode Base64 string.";
        OnCompleted.Broadcast(nullptr, ErrorMessage);
    }
}

void UBase64ToSoundWave::CreateSoundWaveFromResponse(const TArray<uint8>& AudioData)
{
    RuntimeAudioImporter = URuntimeAudioImporterLibrary::CreateRuntimeAudioImporter();

    if (!IsValid(RuntimeAudioImporter))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create audio importer"));
        return;
    }

    RuntimeAudioImporter->OnProgressNative.AddWeakLambda(this, [](int32 Percentage)
    {
        UE_LOG(LogTemp, Log, TEXT("Audio importing percentage: %d"), Percentage);
    });

    RuntimeAudioImporter->OnResultNative.AddWeakLambda(this, [this](URuntimeAudioImporterLibrary* Importer, UImportedSoundWave* ImportedSoundWave, ERuntimeImportStatus Status)
    {
        if (Status == ERuntimeImportStatus::SuccessfulImport)
        {
            UE_LOG(LogTemp, Warning, TEXT("Successfully imported audio with sound wave %s"), *ImportedSoundWave->GetName());
            OnCompleted.Broadcast(ImportedSoundWave, TEXT("SUCCESS"));
        }
        else
        {
            FString ErrorMessage = FString::Printf(TEXT("Failed to import audio: %d"), Status);
            UE_LOG(LogTemp, Error, TEXT("Failed to import audio"));
            OnCompleted.Broadcast(nullptr, ErrorMessage);
        }

        RuntimeAudioImporter = nullptr;
    });

    RuntimeAudioImporter->ImportAudioFromBuffer(AudioData, ERuntimeAudioFormat::Auto);
}