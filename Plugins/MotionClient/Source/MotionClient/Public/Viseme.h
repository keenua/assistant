#pragma once
#include "CoreMinimal.h"

/**
* Enum representing a single viseme to be used in 
* setting of CurrentViseme state and for selecting
* the correct pose in the Anim Graph through a Blend Enum function 
*/
UENUM(BlueprintType)
enum class EViseme : uint8
{
    Sil UMETA(DisplayName = "Sil"),
    P UMETA(DisplayName = "P"),
    LowerT UMETA(DisplayName = "LowerT"),
    S UMETA(DisplayName = "S"),
    T UMETA(DisplayName = "T"),
    F UMETA(DisplayName = "F"),
    K UMETA(DisplayName = "K"),
    I UMETA(DisplayName = "I"),
    R UMETA(DisplayName = "R"),
    LowerS UMETA(DisplayName = "LowerS"),
    U UMETA(DisplayName = "U"),
    At UMETA(DisplayName = "@"),
    A UMETA(DisplayName = "A"),
    E UMETA(DisplayName = "E"),
    LowerE UMETA(DisplayName = "LowerE"),
    O UMETA(DisplayName = "O"),
    LowerO UMETA(DisplayName = "LowerO")
};

/**
 * Returns the enum represetnation of the viseme given 
 * the string representation of the viseme. Note that the
 * string representation is that returned by Amazon Polly,
 * not the DisplayName corresponding to the EViseme.
 * 
 * @param String the string representation of the viseme 
 * @return EViseme the enum represetnation of the viseme
 */
EViseme GetVisemeValueFromString(const FString& String);