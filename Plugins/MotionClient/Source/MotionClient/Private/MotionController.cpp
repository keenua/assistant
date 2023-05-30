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

void AMotionController::ProcessAudio()
{
	// For each frame in TArray FrameBuffer
	for (int i = 0; i < FrameBuffer.Num(); i++)
	{
		TSharedPtr<FMotionFrame> Frame = FrameBuffer[i];

		if (LastProcessedAudioFrame >= Frame->Index)
		{
			continue;
		}

		// If frame has audio
		if (Frame->Audio.Num() > 0)
		{
			// Append audio to sound wave
			SoundWave->AppendAudioDataFromEncoded(Frame->Audio, ERuntimeAudioFormat::Auto);
		}

		LastProcessedAudioFrame = Frame->Index;
	}
}

void AMotionController::ProcessAnimation()
{
	int ExpectedFrameIndex = (int)(FrameTime / (1 / FPS));
	bool bFrameAvailable = false;
	FString Viseme = "";
	FString Emotion = "";

	if (FrameBuffer.Num() == 0)
	{
		return;
	}

	TSharedPtr<FMotionFrame> Frame;
	while (FrameBuffer.Num() > 0 && FrameBuffer[0]-> Index <= ExpectedFrameIndex)
	{
		Frame = FrameBuffer[0];
		FrameBuffer.RemoveAt(0);

		if (Frame->Emotion != "")
		{
			Emotion = Frame->Emotion;
		}

		if (Frame->Viseme != "")
		{
			Viseme = Frame->Viseme;
		}
	}

	if (Frame != nullptr)
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

	if (Emotion != "")
	{
		EEmotion EmotionValue = GetEmotionValueFromString(Emotion);
		OnEmotionReady.Broadcast(EmotionValue);
	}

	if (Viseme != "")
	{
		EViseme VisemeValue = GetVisemeValueFromString(Viseme);
		OnVisemeReady.Broadcast(VisemeValue);
	}
}

// Called every frame
void AMotionController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SoundWave == nullptr)
	{
		return;
	}
	
	FrameTime = SoundWave->GetPlaybackTime();

	if (FrameTime < 0.001f && FrameBuffer.Num() == 0)
	{
		return;
	}

	if (!bPlaying)
	{
		OnFirstFramePlayed.Broadcast();
	}

	bPlaying = true;
	ProcessAudio();
	ProcessAnimation();
}

void AMotionController::AddFrames(TArray<TSharedPtr<FMotionFrame>> Frames)
{
	UE_LOG(LogTemp, Warning, TEXT("Adding %d frames"), Frames.Num());
	FrameBuffer.Append(Frames);
}
