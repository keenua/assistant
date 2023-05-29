#include "MotionLibrary.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "RuntimeAudioImporterLibrary.h"

bool UMotionLibrary::InitFile(AMotionController *Controller, const FString BVHFileName)
{
    if (Controller == nullptr)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Controller is invalid. (Init File)")));
		}
		return false;
	}

	if (Controller->Skeleton.ParseBVHReferenceFile(BVHFileName) != true)
		return false;

	return true;
}

bool UMotionLibrary::AddFramesFromFile(AMotionController *Controller, const FString BVHFileName)
{
	if (Controller == nullptr)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Controller is invalid. (Add Frames From File)")));
		}
		return false;
	}

	// Load BVH file
	TArray<FString> Lines;
	if (FFileHelper::LoadANSITextFileToStrings(*(FPaths::ProjectContentDir() + BVHFileName), NULL, Lines) != true)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Could not load BVH file:%s"), *(FPaths::ProjectContentDir() + BVHFileName)));
		}
		return false;
	}
	
	bool bMotionStarted = false;
	int Index = 0;
	TArray<TSharedPtr<FMotionFrame>> Frames;

	for (FString &Line : Lines)
	{
		Line = Line.TrimStartAndEnd();
		if (Line.StartsWith(TEXT("Frame Time"), ESearchCase::IgnoreCase))
		{
			bMotionStarted = true;
			continue;
		}

		if (bMotionStarted)
		{
			TSharedPtr<FMotionFrame> Frame = MakeShared<FMotionFrame>();
			Frame->Motion = Line;
			Frame->Index = Index;
			Frames.Add(Frame);

			Index++;
		}
	}

	Controller->AddFrames(Frames);

	return true;
}

bool UMotionLibrary::ReadMotion(AMotionController *Controller, USkeletalMeshComponent *Mesh, FVector &Translation, FRotator &Rotation, FVector AdditionalTranslation, FRotator AdditionalRotation, EBonesEnum BVHBone, FName CustomBoneName, bool InverseForward)
{
    if (Controller == nullptr)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Controller is invalid. (Read Motion)")));
		}
		Rotation.Yaw = Rotation.Pitch = Rotation.Roll = 0;
		Translation.X = Translation.Y = Translation.Z = 0;
		return false;
	}
	else if (!Controller->bPlaying)
	{
		Rotation.Yaw = Rotation.Pitch = Rotation.Roll = 0;
		Translation.X = Translation.Y = Translation.Z = 0;
		return false;
	}

	int32 BVHBoneIndex = (int32)BVHBone;
	if (BVHBoneIndex >= Controller->Skeleton.BoneNr)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Boneindex %d exceeds maximum available bones %d."), BVHBoneIndex, Controller->Skeleton.BoneNr));
		}
		Rotation.Yaw = Rotation.Pitch = Rotation.Roll = 0;
		Translation.X = Translation.Y = Translation.Z = 0;
		return false;
	}

	int32 FloatsPerBone = 3;

	// Get the rotation of the reference pose bone
	FQuat RefQuat(FQuat::Identity);
	if (Mesh != nullptr && Mesh->SkeletalMesh != nullptr)
	{
		const FReferenceSkeleton& RefSkeleton(Mesh->SkeletalMesh->RefSkeleton);
		int32 RefBoneIndex = Mesh->GetBoneIndex(CustomBoneName);
		while (RefBoneIndex != INDEX_NONE)
		{
			RefQuat = RefSkeleton.GetRefBonePose()[RefBoneIndex].GetRotation() * RefQuat;
			RefBoneIndex = RefSkeleton.GetParentIndex(RefBoneIndex);
		}
	}

	//
	// Translation
	//

	if (BVHBoneIndex == 0)
	{
		float X = Controller->RootPos[Controller->Skeleton.Bones[BVHBoneIndex].XPos] - Controller->Skeleton.Bones[BVHBoneIndex].Offset[0];
		float Y = Controller->RootPos[Controller->Skeleton.Bones[BVHBoneIndex].YPos] - Controller->Skeleton.Bones[BVHBoneIndex].Offset[1];
		float Z = Controller->RootPos[Controller->Skeleton.Bones[BVHBoneIndex].ZPos] - Controller->Skeleton.Bones[BVHBoneIndex].Offset[2];

		// Map BVH translation to UE4 world coordinate system (Inverse if forward direction is -Y)
		if (InverseForward)
		{
			Translation = FVector(-X, -Z, Y);
		}			
		else
		{
			Translation = FVector(X, Z, Y);
		}			

		// Map UE4 world translation to bone space	
		Translation = RefQuat.Inverse().RotateVector(Translation);
	}
	else 
	{
		Translation.X = Translation.Y = Translation.Z = 0;
	}

	// Add additional translation
	Translation.X += AdditionalTranslation.X;
	Translation.Y += AdditionalTranslation.Y;
	Translation.Z += AdditionalTranslation.Z;


	//
	// Rotation 
	//

	// Read rotation values and map to pitch, yaw, roll (y, z, x)
	float XR = Controller->MotionLine[(BVHBoneIndex * FloatsPerBone) + Controller->Skeleton.Bones[BVHBoneIndex].XRot] * PI / 180.f;
	float YR = Controller->MotionLine[(BVHBoneIndex * FloatsPerBone) + Controller->Skeleton.Bones[BVHBoneIndex].YRot] * PI / 180.f;
	float ZR = Controller->MotionLine[(BVHBoneIndex * FloatsPerBone) + Controller->Skeleton.Bones[BVHBoneIndex].ZRot] * PI / 180.f;

	// Calculate Rotation Matrix and map to Quaternion
	FQuat BVHQuat = CalculateQuat(XR, YR, ZR, Controller->Skeleton.Bones[BVHBoneIndex].RotOrder);

	// Map BVH rotation to UE4 world coordinate system (Inverse if forward direction is -Y)
	float Y = BVHQuat.Y;
	float Z = BVHQuat.Z;
	if (InverseForward)
	{
		BVHQuat.X *= -1.0;
		BVHQuat.Y = -Z;
		BVHQuat.Z = Y;
	}
	else
	{
		BVHQuat.Y = Z;
		BVHQuat.Z = Y;
	}

	// Map UE4 world rotation to bone space
	FQuat Quat(RefQuat.Inverse() * BVHQuat * RefQuat);
	
	// Add additional rotation
	FQuat AddRot(AdditionalRotation);
	Quat *= AddRot;

	// Convert to Rotator
	Rotation = Quat.Rotator();
	Rotation.Normalize();

	return true;
}

bool UMotionLibrary::ReadMotionArray(AMotionController *Controller, USkeletalMeshComponent *Mesh, TArray<FVector> &Translation, TArray<FRotator> &Rotation, TArray<FVector> AdditionalTranslation, TArray<FRotator> AdditionalRotation, TArray<FBoneMap> BoneMap, bool InverseForward)
{
	if (Controller == nullptr)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Controller is invalid. (Read Motion Array)")));
		}
		return false;
	}

    for (int32 BoneIndex = 0; BoneIndex < BoneMap.Num(); BoneIndex++)
	{
		EBonesEnum BVHBoneEnum = BoneMap[BoneIndex].BVHBone;
		FName CustomBoneName = BoneMap[BoneIndex].CustomBoneName;
		FVector TempTranslation;
		FRotator TempRotation;
		if (!ReadMotion(Controller, Mesh, TempTranslation, TempRotation, AdditionalTranslation[BoneIndex], AdditionalRotation[BoneIndex], BVHBoneEnum, CustomBoneName, InverseForward))
			return false;
		Translation.Add(TempTranslation);
		Rotation.Add(TempRotation);
	}

	return true;
}

bool UMotionLibrary::AddFrames(AMotionController *Controller, TArray<FMotionFrame> Frames)
{
	if (Controller == nullptr)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Controller is invalid. (Add Frames)")));
		}
		return false;
	}

	// Add frames to controller. Convert each frame to TSharedPtr
	TArray<TSharedPtr<FMotionFrame>> TempFrames;

	for (int32 FrameIndex = 0; FrameIndex < Frames.Num(); FrameIndex++)
	{
		TempFrames.Add(MakeShareable(new FMotionFrame(Frames[FrameIndex])));
	}

	Controller->AddFrames(TempFrames);

    return true;
}

void UMotionLibrary::CorrectFrame(AMotionController *Controller, int SoundPlayedFrameIndex, float Duration, float PlaybackTime, int AudioBytes)
{
	if (Controller == nullptr)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Controller is invalid. (Correct Frame)")));
		}
		return;
	}

	Controller->CorrectFrame(SoundPlayedFrameIndex, Duration, PlaybackTime, AudioBytes);
}

int32 UMotionLibrary::GetBVHBoneIndex(EBonesEnum BVHBone)
{
	return (int32)BVHBone;
}

