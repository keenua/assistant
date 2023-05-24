#include "MotionController.h"

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

	if (FrameTime == 0.0f && !FrameBuffer.Peek(Frame))
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

	TArray<TSharedPtr<FMotionFrame>> TempFrames;

	bool bGapFound = false;
	bool bAudioStarted = false;

	int Bitrate = 16000;

	while (FrameBuffer.Peek(Frame))
	{
		FrameBuffer.Dequeue(Frame);
		TempFrames.Add(Frame);

		if (Frame->Audio.Num() > 0 && !bGapFound)
		{
			if (Frame->Index > LastAddedAudioFrame)
			{
				if (Frame->Index - LastAddedAudioFrame > 5 && bAudioStarted)
				{
					UE_LOG(LogTemp, Warning, TEXT("Gap of %d found at %d"), Frame->Index - LastAddedAudioFrame, Frame->Index);
					bGapFound = true;
				}
				else
				{
					if (!bAudioStarted && Frame->Index > ExpectedFrameIndex) 
					{
						continue;
					}

					bAudioStarted = true;
					UE_LOG(LogTemp, Warning, TEXT("Adding audio frame %d"), Frame->Index);
					ImportedSoundWave.Append(Frame->Audio);
					int AudioLength = Frame->Audio.Num() / Bitrate * FPS;
					LastAddedAudioFrame = Frame->Index + AudioLength;
				}
			}
		}
	}

	// Add frames back to buffer
	for (int i = 0; i < TempFrames.Num(); i++)
	{
		FrameBuffer.Enqueue(TempFrames[i]);
	}

	TempFrames.Empty();

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
		OnEmotionReady.Broadcast(Emotion);
	}

	if (Viseme != "")
	{
		UE_LOG(LogTemp, Warning, TEXT("OnVisemeReady.Broadcast(Viseme)"));
		OnVisemeReady.Broadcast(Viseme);
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
