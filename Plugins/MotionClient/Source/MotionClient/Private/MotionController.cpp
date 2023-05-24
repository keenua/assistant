#include "MotionController.h"
#include "Viseme.h"

AMotionController::AMotionController()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMotionController::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMotionController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Peek first frame
	TSharedPtr<FMotionFrame> Frame;

	if (FrameTime < 0.001f && !FrameBuffer.Peek(Frame))
	{
		return;
	}

	bPlaying = true;
	FrameTime += DeltaTime;
	int ExpectedFrameIndex = (int)(FrameTime / (1 / FPS));
	bool bFrameAvailable = false;
	TArray<uint8> ImportedSoundWave;
	FString Viseme = "";
	FString Emotion = "";

	int Bitrate = 16000;

	if (FrameBuffer.Peek(Frame) && Frame->Index <= ExpectedFrameIndex)
	{
		// Dequeue frame
		FrameBuffer.Dequeue(Frame);

		if (Frame->Emotion != "")
		{
			Emotion = Frame->Emotion;
		}

		if (Frame->Viseme != "")
		{
			Viseme = Frame->Viseme;
		}

		if (Frame->Audio.Num() > 0)
		{
			ImportedSoundWave.Append(Frame->Audio);
		}

		bFrameAvailable = true;
	}

	if (bFrameAvailable)
	{
		FString Motion = Frame->Motion;

		TArray<FString> MotionArray;
		Motion.ParseIntoArray(MotionArray, TEXT(" "), true);

		// Convert motion array to float array
		for (int i = 0; i < MotionArray.Num(); i++)
		{
			if (i < 3) 
			{
				RootPos[i] = FCString::Atof(*MotionArray[i]);
			}
			else 
			{
				MotionLine[i-3] = FCString::Atof(*MotionArray[i]);
			}
		}
	}

	if (ImportedSoundWave.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnSoundReady.Broadcast(ImportedSoundWave)"));
		OnSoundReady.Broadcast(ImportedSoundWave);
	}

	if (Emotion != "")
	{
		UE_LOG(LogTemp, Warning, TEXT("OnEmotionReady.Broadcast(Emotion)"));
		EEmotion EmotionValue = GetEmotionValueFromString(Emotion);
		OnEmotionReady.Broadcast(EmotionValue);
	}

	if (Viseme != "")
	{
		UE_LOG(LogTemp, Warning, TEXT("OnVisemeReady.Broadcast(Viseme)"));
		EViseme VisemeValue = GetVisemeValueFromString(Viseme);
		OnVisemeReady.Broadcast(VisemeValue);
	}
}

void AMotionController::AddFrames(TArray<TSharedPtr<FMotionFrame>> Frames)
{
	UE_LOG(LogTemp, Warning, TEXT("Adding %d frames"), Frames.Num());
	for (int i = 0; i < Frames.Num(); i++)
	{
		FrameBuffer.Enqueue(Frames[i]);
	}
}
