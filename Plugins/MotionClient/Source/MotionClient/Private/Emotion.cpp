#include "Emotion.h"
#include "CaseSensitiveKeyFunc.h"

EEmotion GetEmotionValueFromString(const FString& String)
{
    int32 Int = FCString::Atoi(*String);
    EEmotion Emotion = static_cast<EEmotion>(Int - 1);
    return Emotion;
};