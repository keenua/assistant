#pragma once
#include "CoreMinimal.h"


UENUM(BlueprintType)
enum class EEmotion : uint8
{
    Sternness UMETA(DisplayName = "Sternness"),
    Indignation UMETA(DisplayName = "Indignation"),
    Anger UMETA(DisplayName = "Anger"),
    Rage UMETA(DisplayName = "Rage"),
    Outrage UMETA(DisplayName = "Outrage (Anger + Disgust)"),
    Anguish UMETA(DisplayName = "Anguish (Anger + Fear)"),
    Cruelty UMETA(DisplayName = "Cruelty (Anger + Joy)"),
    Betrayal UMETA(DisplayName = "Betrayal (Anger + Sadness)"),
    Appalled UMETA(DisplayName = "Appalled (Anger + Surprise)"),
    Horror UMETA(DisplayName = "Horror (Disgust + Fear)"),
    Ewww UMETA(DisplayName = "Ewww (Disgust + Joy)"),
    PainEmpathy UMETA(DisplayName = "Pain Empathy (Disgust + Sadness)"),
    Disbelief UMETA(DisplayName = "Disbelief (Disgust + Surprise)"),
    Desperation UMETA(DisplayName = "Desperation (Fear + Joy)"),
    Devastation UMETA(DisplayName = "Devastation (Fear + Sadness)"),
    Spooked UMETA(DisplayName = "Spooked (Fear + Surprise)"),
    FaintHope UMETA(DisplayName = "Faint Hope (Joy + Sadness)"),
    Amazement UMETA(DisplayName = "Amazement (Joy + Surprise)"),
    Disappointment UMETA(DisplayName = "Disappointment (Sadness + Surprise)"),
    Disdain UMETA(DisplayName = "Disdain"),
    Aversion UMETA(DisplayName = "Aversion"),
    Disgust UMETA(DisplayName = "Disgust"),
    Revulsion UMETA(DisplayName = "Revulsion"),
    Concern UMETA(DisplayName = "Concern"),
    Anxiety UMETA(DisplayName = "Anxiety"),
    Fear UMETA(DisplayName = "Fear"),
    Terror UMETA(DisplayName = "Terror"),
    Satisfaction UMETA(DisplayName = "Satisfaction"),
    Amusement UMETA(DisplayName = "Amusement"),
    Joy UMETA(DisplayName = "Joy"),
    Laughter UMETA(DisplayName = "Laughter"),
    Dejection UMETA(DisplayName = "Dejection"),
    Melancholy UMETA(DisplayName = "Melancholy"),
    Sadness UMETA(DisplayName = "Sadness"),
    Grief UMETA(DisplayName = "Grief"),
    Alertness UMETA(DisplayName = "Alertness"),
    Wonder UMETA(DisplayName = "Wonder"),
    Surprise UMETA(DisplayName = "Surprise"),
    Shock UMETA(DisplayName = "Shock")
};

EEmotion GetEmotionValueFromString(const FString& String);