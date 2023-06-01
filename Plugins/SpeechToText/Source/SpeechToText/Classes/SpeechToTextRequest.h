#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "SpeechToTextRequest.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionCompleted, const FString&, Text);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActionFailed, const FString&, ErrorMessage);

UCLASS()
class SPEECHTOTEXT_API USpeechToTextRequest : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "SpeechToText", meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject", DisplayName = "Speech To Text"))
	static USpeechToTextRequest* SpeechToText(UObject* WorldContextObject, TArray<uint8> Sound, int SampleRate, const FString& API_Key);

	UPROPERTY(BlueprintAssignable)
	FOnActionCompleted OnCompleted;

	UPROPERTY(BlueprintAssignable)
	FOnActionFailed OnFailed;

private:
	void RequestSpeechToText(TArray<uint8> Sound, int SampleRate, const FString& API_Key);
	void OnRequestCompleted(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UPROPERTY()
	UObject* WorldContextObject;
};