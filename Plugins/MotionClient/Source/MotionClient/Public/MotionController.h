#pragma once

#include "Emotion.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "MotionSkeleton.h"
#include "MotionFrame.h"
#include "RuntimeAudioImporterLibrary.h"
#include "RuntimeAudioImporterTypes.h"
#include "Sound/StreamingSoundWave.h"
#include "Viseme.h"
#include "MotionController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSoundReady, const TArray<uint8>&, SoundBytes, int, FrameIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVisemeReady, EViseme, Viseme);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEmotionReady, const EEmotion, Emotion);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFirstFramePlayed);

UCLASS()
class MOTIONCLIENT_API AMotionController : public AActor
{
    GENERATED_BODY()

public:
    AMotionController();

    // Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

    void AddFrames(TArray<TSharedPtr<FMotionFrame>> Frames);

	UPROPERTY(BlueprintAssignable)
	FOnVisemeReady OnVisemeReady;

	UPROPERTY(BlueprintAssignable)
	FOnEmotionReady OnEmotionReady;

	UPROPERTY(BlueprintAssignable)
	FOnFirstFramePlayed OnFirstFramePlayed;

public:
	MotionSkeleton Skeleton;
	UStreamingSoundWave* SoundWave;

	// Map bone coordinate system (Used for VE_Map)
	typedef struct BoneMap
	{
		int32 XYZ[3];		// Map XYZ => X'Y'Z'
		int32 Sign[3];		// Map direction 1 or -1
	} BONEMAP;

	BONEMAP Bonemap[MAXBONES];

#define MAXFLOATS	MAXBONES * 6	// 3 for x,y,z translation and 3 for x,y,z rotation	
    float RootPos[3];
	float MotionLine[MAXFLOATS];
    bool bPlaying = false;

private:
    TArray<TSharedPtr<FMotionFrame>> FrameBuffer;

    float FPS = 60.0f;
	float FrameTime = 0.0f;
	int LastProcessedAudioFrame = -1;
	float LastDelta = 0.0f;

	void ProcessAudio();
	void ProcessAnimation();
};
