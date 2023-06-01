#include "SpeechToTextRequest.h"
#include "HttpModule.h"
#include "Audio.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Serialization/JsonSerializer.h"

USpeechToTextRequest* USpeechToTextRequest::SpeechToText(UObject* WorldContextObject, TArray<uint8> Sound, int SampleRate, const FString& API_Key)
{
    USpeechToTextRequest* Request = NewObject<USpeechToTextRequest>();
    Request->WorldContextObject = WorldContextObject;
    Request->RequestSpeechToText(Sound, SampleRate, API_Key);
    return Request;
}

void USpeechToTextRequest::RequestSpeechToText(TArray<uint8> Sound, int SampleRate, const FString& API_Key)
{
    FString Url = FString::Printf(TEXT("https://speech.googleapis.com/v1p1beta1/speech:recognize?key=%s"), *API_Key);
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetURL(Url);
    HttpRequest->SetVerb("POST");
    HttpRequest->SetHeader("Content-Type", "application/json");

    TSharedPtr<FJsonObject> RequestJson = MakeShareable(new FJsonObject());
    
    TSharedPtr<FJsonObject> ConfigJson = MakeShareable(new FJsonObject());
    ConfigJson->SetNumberField("sampleRateHertz", SampleRate);
    ConfigJson->SetStringField("languageCode", "en-US");
    ConfigJson->SetStringField("encoding", "LINEAR16");
    ConfigJson->SetNumberField("audioChannelCount", 2);

    TSharedPtr<FJsonObject> AudioJson = MakeShareable(new FJsonObject());
    UE_LOG(LogTemp, Error, TEXT("Sound size: %d"), Sound.Num());

    FString SoundBase64 = FBase64::Encode(Sound);
    AudioJson->SetStringField("content", SoundBase64);

    RequestJson->SetObjectField("config", ConfigJson);
    RequestJson->SetObjectField("audio", AudioJson);

    FString RequestBody;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
    FJsonSerializer::Serialize(RequestJson.ToSharedRef(), Writer);

    HttpRequest->SetContentAsString(RequestBody);
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &USpeechToTextRequest::OnRequestCompleted);
    HttpRequest->ProcessRequest();
}

void USpeechToTextRequest::OnRequestCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    FString ResponseString = Response->GetContentAsString();
    UE_LOG(LogTemp, Error, TEXT("Response: %s"), *ResponseString);

    if (bWasSuccessful && Response->GetResponseCode() == 200)
    {
        
        TSharedPtr<FJsonObject> ResponseJson;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);
        if (FJsonSerializer::Deserialize(Reader, ResponseJson))
        {
            // results
            TArray<TSharedPtr<FJsonValue>> Results = ResponseJson->GetArrayField("results");
            if (Results.Num() == 0)
            {
                FString ErrorMessage = "No results.";
                OnFailed.Broadcast(ErrorMessage);
                return;
            }

            // alternatives
            TArray<TSharedPtr<FJsonValue>> Alternatives = Results[0]->AsObject()->GetArrayField("alternatives");
            if (Alternatives.Num() == 0)
            {
                FString ErrorMessage = "No alternatives.";
                OnFailed.Broadcast(ErrorMessage);
                return;
            }

            // transcript
            FString Text = Alternatives[0]->AsObject()->GetStringField("transcript");
            OnCompleted.Broadcast(Text);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to parse response: %s"), *ResponseString);

            FString ErrorMessage = "Failed to parse response.";
            OnFailed.Broadcast(ErrorMessage);
        }
    }
    else
    {
        FString ErrorMessage = FString::Printf(TEXT("Request failed. Response code: %d"), Response->GetResponseCode());
        UE_LOG(LogTemp, Error, TEXT("Failed request: %s"), *ResponseString);
        OnFailed.Broadcast(ErrorMessage);
    }
}