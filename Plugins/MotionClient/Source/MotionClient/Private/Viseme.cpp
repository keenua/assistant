#include "Viseme.h"
#include "CaseSensitiveKeyFunc.h"

/*
* Maps the FString representation of a viseme to its corresponding EViseme enum 
* NOTE: A custom case-sensitive key-func is implemented to compare FStrings, as Visemes 
* have both capital and lowercase versions, and the default FString comparison 
* (uses ==) is case-insensitive 
* @param String - an FString of the viseme 
* @return - the EViseme enum corresponding to the FString 
*/
EViseme GetVisemeValueFromString(const FString& String)
{
    static TMap<FString, EViseme, FDefaultSetAllocator, CaseSensitiveKeyFunc<EViseme>> VisemeMap = {
        {TEXT("p"),EViseme::P}, 
        {TEXT("t"),EViseme::LowerT},
        {TEXT("S"),EViseme::S},
        {TEXT("T"),EViseme::T}, 
        {TEXT("f"),EViseme::F},
        {TEXT("k"),EViseme::K},
        {TEXT("i"),EViseme::I},
        {TEXT("r"),EViseme::R},
        {TEXT("s"),EViseme::LowerS},
        {TEXT("u"),EViseme::U},
        {TEXT("@"),EViseme::At},
        {TEXT("a"),EViseme::A}, 
        {TEXT("e"),EViseme::LowerE}, 
        {TEXT("E"),EViseme::E},
        {TEXT("o"),EViseme::LowerO},
        {TEXT("O"),EViseme::O},
        {TEXT("sil"),EViseme::Sil} 
    };
    auto Viseme = VisemeMap.Find(String);
    if (!Viseme)
    {
        return EViseme::Sil;
    }
    return *Viseme;
};