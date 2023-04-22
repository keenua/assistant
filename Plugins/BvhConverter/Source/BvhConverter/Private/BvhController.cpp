// Fill out your copyright notice in the Description page of Project Settings.


#include "BvhController.h"

// Sets default values
ABvhController::ABvhController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BoneMapping = TMap<EMyPresetJointTag, FMyBoneMapConfig>();
	BoneMapping.Add(EMyPresetJointTag::PELVIS, FMyBoneMapConfig{ "Hips", "Hips" });
	BoneMapping.Add(EMyPresetJointTag::SPINE, FMyBoneMapConfig{ "Spine", "Spine" });
	BoneMapping.Add(EMyPresetJointTag::SPINE1, FMyBoneMapConfig{ "Spine1", "Spine1" });
	BoneMapping.Add(EMyPresetJointTag::CHEST, FMyBoneMapConfig{ "Spine3", "Spine3" });
	BoneMapping.Add(EMyPresetJointTag::SPINE2, FMyBoneMapConfig{ "Spine2", "Spine2" });
	BoneMapping.Add(EMyPresetJointTag::NECK, FMyBoneMapConfig{ "Neck", "Neck" });
	BoneMapping.Add(EMyPresetJointTag::HEAD, FMyBoneMapConfig{ "Head", "Head" });
	BoneMapping.Add(EMyPresetJointTag::HEAD_END, FMyBoneMapConfig{ "HeadEnd", "HeadEnd" });
	BoneMapping.Add(EMyPresetJointTag::R_CLAVICLE, FMyBoneMapConfig{ "RightShoulder", "RightShoulder" });
	BoneMapping.Add(EMyPresetJointTag::R_SHOULDER, FMyBoneMapConfig{ "RightArm", "RightArm" });
	BoneMapping.Add(EMyPresetJointTag::R_ELBOW, FMyBoneMapConfig{ "RightForeArm", "RightForeArm" });
	BoneMapping.Add(EMyPresetJointTag::R_WRIST, FMyBoneMapConfig{ "RightHand", "RightHand" });
	BoneMapping.Add(EMyPresetJointTag::R_HIP, FMyBoneMapConfig{ "RightUpLeg", "RightUpLeg" });
	BoneMapping.Add(EMyPresetJointTag::R_KNEE, FMyBoneMapConfig{ "RightLeg", "RightLeg" });
	BoneMapping.Add(EMyPresetJointTag::R_ANKLE, FMyBoneMapConfig{ "RightFoot", "RightFoot" });
	BoneMapping.Add(EMyPresetJointTag::R_TOE, FMyBoneMapConfig{ "RightToeBase", "RightToeBase" });
	BoneMapping.Add(EMyPresetJointTag::R_TOE_END, FMyBoneMapConfig{ "RightToeBaseEnd", "RightToeBaseEnd" });
	BoneMapping.Add(EMyPresetJointTag::L_CLAVICLE, FMyBoneMapConfig{ "LeftShoulder", "LeftShoulder" });
	BoneMapping.Add(EMyPresetJointTag::L_SHOULDER, FMyBoneMapConfig{ "LeftArm", "LeftArm" });
	BoneMapping.Add(EMyPresetJointTag::L_ELBOW, FMyBoneMapConfig{ "LeftForeArm", "LeftForeArm" });
	BoneMapping.Add(EMyPresetJointTag::L_WRIST, FMyBoneMapConfig{ "LeftHand", "LeftHand" });
	BoneMapping.Add(EMyPresetJointTag::L_HIP, FMyBoneMapConfig{ "LeftUpLeg", "LeftUpLeg" });
	BoneMapping.Add(EMyPresetJointTag::L_KNEE, FMyBoneMapConfig{ "LeftLeg", "LeftLeg" });
	BoneMapping.Add(EMyPresetJointTag::L_ANKLE, FMyBoneMapConfig{ "LeftFoot", "LeftFoot" });
	BoneMapping.Add(EMyPresetJointTag::L_TOE, FMyBoneMapConfig{ "LeftToeBase", "LeftToeBase" });
	BoneMapping.Add(EMyPresetJointTag::L_TOE_END, FMyBoneMapConfig{ "LeftToeBaseEnd", "LeftToeBaseEnd" });
}

// Called when the game starts or when spawned
void ABvhController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABvhController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bPlay)
	{
		InternalTimeAccumulator += DeltaTime;
		if (InternalTimeAccumulator > GetTotalLength())
		{
			if (bLoopAnimation)
			{
				InternalTimeAccumulator = 0.0f;
			}
			else
			{
				InternalTimeAccumulator = GetTotalLength();
			}
		}
	}
}

float ABvhController::GetTotalLength()
{
	return Motion.size() * (1.0 / Motion.fps());
}

void ABvhController::SetSkeleton(USkeletalMeshComponent* mesh) 
{
	Poser = ml::UE4Poser();
	Poser.SetUPoseableMeshComponent(mesh);

	for (auto Com : BoneMapping)
	{
		ml::JointTag _Tag = ml::JointTag((uint8)Com.Key - 1);
		Poser.SetJointTag(TCHAR_TO_UTF8(*Com.Value.UE_Bone), _Tag);
	}

	Poser.BuildSkeleton();
}

bool ABvhController::Play(FString TPoseFile, FString BVHFile, bool bLoop)
{
	if (Poser.GetJointNum() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Poser is empty inside"));
		return false;
	}

	if (TPoseFile.IsEmpty() || BVHFile.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Pose/Motion is empty inside"));
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("Playing this shit now"));

	try 
	{
		if (ml::LoadBVH_UE4(TPoseFile, BVHFile, Motion))
		{
			for (auto Com : BoneMapping)
			{
				ml::JointTag _Tag = ml::JointTag((uint8)Com.Key - 1);
				Motion.editable_body()->SetJointTag(TCHAR_TO_UTF8(*Com.Value.Bvh_Joint), _Tag);
			}


			bPlay = true;
			bLoopAnimation = bLoop;
			InternalTimeAccumulator = 0.0f;

			UE_LOG(LogTemp, Warning, TEXT("Pose fully loaded!"));
			return true;
		}
	} 
	catch (std::exception& e)
	{
		UE_LOG(LogTemp, Error, TEXT("Play Error: %s"), e.what());
	}

	return false;
}

TMap<FName, FTransform> ABvhController::GetCurrentFrame(USkeletalMeshComponent* mesh)
{
	TMap<FName, FTransform> CurframePose = TMap<FName, FTransform>(); 

	try 
	{
		if (bPlay)
		{
			ml::Posture posture = Motion.GetPostureAtTime_ms(InternalTimeAccumulator);
			Poser.Retarget(posture, CurframePose);
			return CurframePose;
		}

		// for (auto Com : BoneMapping)
		// {
		// 	CurframePose.Add(FName(Com.Value.UE_Bone), FTransform());
		// }
	}
	catch (std::exception& e)
	{
		UE_LOG(LogTemp, Error, TEXT("GetCurrentFrame Error: %s"), e.what());
	}

	return CurframePose;
}


bool ABvhController::Pause(bool bPause)
{
	bPlay = !bPause;
	return true;
}
