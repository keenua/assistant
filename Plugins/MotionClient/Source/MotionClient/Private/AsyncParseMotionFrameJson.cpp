#include "AsyncParseMotionFrameJson.h"
#include "Sound/SoundWave.h"
#include "RuntimeAudioImporterLibrary.h"
#include "MotionController.h"
#include "JsonUtilities/Public/JsonObjectConverter.h"

UAsyncParseMotionFrameJson* UAsyncParseMotionFrameJson::ParseMotionFrameJson(UObject* WorldContextObject, const FString& JsonString, AMotionController* MotionController)
{
    UAsyncParseMotionFrameJson* Node = NewObject<UAsyncParseMotionFrameJson>();
    Node->WorldContextObject = WorldContextObject;
    Node->ParseJson(JsonString, MotionController);
    return Node;
}

void UAsyncParseMotionFrameJson::ParseJson(const FString& JsonString, AMotionController* MotionController)
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);

    if (!FJsonSerializer::Deserialize(JsonReader, JsonObject) || !JsonObject.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON string"));
        return;
    }

    TArray<TSharedPtr<FJsonValue>> FramesJsonArray = JsonObject->GetArrayField("frames");

    for (TSharedPtr<FJsonValue> FrameJsonValue : FramesJsonArray)
    {
        TSharedPtr<FJsonObject> FrameJsonObject = FrameJsonValue->AsObject();

        TSharedPtr<FMotionFrame> MotionFrame = MakeShared<FMotionFrame>();
        MotionFrame->Index = FrameJsonObject->GetIntegerField("index");
        MotionFrame->Motion = FrameJsonObject->GetStringField("motion");
        MotionFrame->AudioString = FrameJsonObject->GetStringField("audio");
        MotionFrame->Emotion = FrameJsonObject->GetStringField("emotion");
        MotionFrame->Text = FrameJsonObject->GetStringField("text");
        MotionFrame->Viseme = FrameJsonObject->GetStringField("viseme");

        ParsedFrames.Add(MotionFrame);

        if (!MotionFrame->AudioString.IsEmpty())
        {
            // PendingAudioImports++;
            FBase64::Decode(MotionFrame->AudioString, MotionFrame->Audio);
            
            // CreateSoundWaveFromBase64(MotionFrame->AudioString, [this, MotionFrame, MotionController](UImportedSoundWave* ImportedSoundWave)
            // {
            //     MotionFrame->Audio = ImportedSoundWave;
            //     UE_LOG(LogTemp, Log, TEXT("Frame with sound: %d, address %p"), MotionFrame->Index, MotionFrame->Audio);
            //     PendingAudioImports--;

            //     if (PendingAudioImports == 0)
            //     {
            //         UE_LOG(LogTemp, Log, TEXT("Parsed all frames with sound"))
            //         MotionController->AddFrames(ParsedFrames);
            //         UE_LOG(LogTemp, Log, TEXT("OnFramesReady.Broadcast(ParsedFrames);"))
            //     }
            // });
        }
    }

    // if (PendingAudioImports == 0)
    // {
    UE_LOG(LogTemp, Log, TEXT("Parsed all frames"))
    MotionController->AddFrames(ParsedFrames);
    UE_LOG(LogTemp, Log, TEXT("OnFramesReady.Broadcast(ParsedFrames);"))
    // }
}

void UAsyncParseMotionFrameJson::CreateSoundWaveFromBase64(const FString& Base64String, TFunction<void(UImportedSoundWave*)> OnSoundWaveCreated)
{
    TArray<uint8> AudioData;
    if (FBase64::Decode(Base64String, AudioData))
    {
        URuntimeAudioImporterLibrary* RuntimeAudioImporter = URuntimeAudioImporterLibrary::CreateRuntimeAudioImporter();

        if (!IsValid(RuntimeAudioImporter))
        {
            OnSoundWaveCreated(nullptr);
            return;
        }

        RuntimeAudioImporter->OnResultNative.AddWeakLambda(this, [OnSoundWaveCreated](URuntimeAudioImporterLibrary* Importer, UImportedSoundWave* ImportedSoundWave, ERuntimeImportStatus Status)
        {
            if (Status == ERuntimeImportStatus::SuccessfulImport)
            {
                OnSoundWaveCreated(ImportedSoundWave);
            }
            else
            {
                OnSoundWaveCreated(nullptr);
            }
        });

        RuntimeAudioImporter->ImportAudioFromBuffer(AudioData, ERuntimeAudioFormat::Auto);
    }
    else
    {
        // Handle error
        OnSoundWaveCreated(nullptr);
    }
}