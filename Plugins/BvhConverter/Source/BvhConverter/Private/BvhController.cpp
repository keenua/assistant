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
	BoneMapping.Add(EMyPresetJointTag::R_HAND_THUMB1, FMyBoneMapConfig{ "RightHandThumb1", "RightHandThumb1" });
	BoneMapping.Add(EMyPresetJointTag::R_HAND_THUMB2, FMyBoneMapConfig{ "RightHandThumb2", "RightHandThumb2" });
	BoneMapping.Add(EMyPresetJointTag::R_HAND_THUMB3, FMyBoneMapConfig{ "RightHandThumb3", "RightHandThumb3" });
	BoneMapping.Add(EMyPresetJointTag::R_HAND_THUMB4, FMyBoneMapConfig{ "RightHandThumb4", "RightHandThumb4" });
	BoneMapping.Add(EMyPresetJointTag::R_HAND_INDEX1, FMyBoneMapConfig{ "RightHandIndex1", "RightHandIndex1" });
	BoneMapping.Add(EMyPresetJointTag::R_HAND_INDEX2, FMyBoneMapConfig{ "RightHandIndex2", "RightHandIndex2" });
	BoneMapping.Add(EMyPresetJointTag::R_HAND_INDEX3, FMyBoneMapConfig{ "RightHandIndex3", "RightHandIndex3" });
	BoneMapping.Add(EMyPresetJointTag::R_HAND_INDEX4, FMyBoneMapConfig{ "RightHandIndex4", "RightHandIndex4" });
	BoneMapping.Add(EMyPresetJointTag::R_HAND_MIDDLE1, FMyBoneMapConfig{ "RightHandMiddle1", "RightHandMiddle1" });
	BoneMapping.Add(EMyPresetJointTag::R_HAND_MIDDLE2, FMyBoneMapConfig{ "RightHandMiddle2", "RightHandMiddle2" });
	BoneMapping.Add(EMyPresetJointTag::R_HAND_MIDDLE3, FMyBoneMapConfig{ "RightHandMiddle3", "RightHandMiddle3" });
	BoneMapping.Add(EMyPresetJointTag::R_HAND_MIDDLE4, FMyBoneMapConfig{ "RightHandMiddle4", "RightHandMiddle4" });
	BoneMapping.Add(EMyPresetJointTag::R_HAND_RING1, FMyBoneMapConfig{ "RightHandRing1", "RightHandRing1" });
	BoneMapping.Add(EMyPresetJointTag::R_HAND_RING2, FMyBoneMapConfig{ "RightHandRing2", "RightHandRing2" });
	BoneMapping.Add(EMyPresetJointTag::R_HAND_RING3, FMyBoneMapConfig{ "RightHandRing3", "RightHandRing3" });
	BoneMapping.Add(EMyPresetJointTag::R_HAND_RING4, FMyBoneMapConfig{ "RightHandRing4", "RightHandRing4" });
	BoneMapping.Add(EMyPresetJointTag::R_HAND_PINKY1, FMyBoneMapConfig{ "RightHandPinky1", "RightHandPinky1" });
	BoneMapping.Add(EMyPresetJointTag::R_HAND_PINKY2, FMyBoneMapConfig{ "RightHandPinky2", "RightHandPinky2" });
	BoneMapping.Add(EMyPresetJointTag::R_HAND_PINKY3, FMyBoneMapConfig{ "RightHandPinky3", "RightHandPinky3" });
	BoneMapping.Add(EMyPresetJointTag::R_HAND_PINKY4, FMyBoneMapConfig{ "RightHandPinky4", "RightHandPinky4" });
	BoneMapping.Add(EMyPresetJointTag::L_HAND_THUMB1, FMyBoneMapConfig{ "LeftHandThumb1", "LeftHandThumb1" });
	BoneMapping.Add(EMyPresetJointTag::L_HAND_THUMB2, FMyBoneMapConfig{ "LeftHandThumb2", "LeftHandThumb2" });
	BoneMapping.Add(EMyPresetJointTag::L_HAND_THUMB3, FMyBoneMapConfig{ "LeftHandThumb3", "LeftHandThumb3" });
	BoneMapping.Add(EMyPresetJointTag::L_HAND_THUMB4, FMyBoneMapConfig{ "LeftHandThumb4", "LeftHandThumb4" });
	BoneMapping.Add(EMyPresetJointTag::L_HAND_INDEX1, FMyBoneMapConfig{ "LeftHandIndex1", "LeftHandIndex1" });
	BoneMapping.Add(EMyPresetJointTag::L_HAND_INDEX2, FMyBoneMapConfig{ "LeftHandIndex2", "LeftHandIndex2" });
	BoneMapping.Add(EMyPresetJointTag::L_HAND_INDEX3, FMyBoneMapConfig{ "LeftHandIndex3", "LeftHandIndex3" });
	BoneMapping.Add(EMyPresetJointTag::L_HAND_INDEX4, FMyBoneMapConfig{ "LeftHandIndex4", "LeftHandIndex4" });
	BoneMapping.Add(EMyPresetJointTag::L_HAND_MIDDLE1, FMyBoneMapConfig{ "LeftHandMiddle1", "LeftHandMiddle1" });
	BoneMapping.Add(EMyPresetJointTag::L_HAND_MIDDLE2, FMyBoneMapConfig{ "LeftHandMiddle2", "LeftHandMiddle2" });
	BoneMapping.Add(EMyPresetJointTag::L_HAND_MIDDLE3, FMyBoneMapConfig{ "LeftHandMiddle3", "LeftHandMiddle3" });
	BoneMapping.Add(EMyPresetJointTag::L_HAND_MIDDLE4, FMyBoneMapConfig{ "LeftHandMiddle4", "LeftHandMiddle4" });
	BoneMapping.Add(EMyPresetJointTag::L_HAND_RING1, FMyBoneMapConfig{ "LeftHandRing1", "LeftHandRing1" });
	BoneMapping.Add(EMyPresetJointTag::L_HAND_RING2, FMyBoneMapConfig{ "LeftHandRing2", "LeftHandRing2" });
	BoneMapping.Add(EMyPresetJointTag::L_HAND_RING3, FMyBoneMapConfig{ "LeftHandRing3", "LeftHandRing3" });
	BoneMapping.Add(EMyPresetJointTag::L_HAND_RING4, FMyBoneMapConfig{ "LeftHandRing4", "LeftHandRing4" });
	BoneMapping.Add(EMyPresetJointTag::L_HAND_PINKY1, FMyBoneMapConfig{ "LeftHandPinky1", "LeftHandPinky1" });
	BoneMapping.Add(EMyPresetJointTag::L_HAND_PINKY2, FMyBoneMapConfig{ "LeftHandPinky2", "LeftHandPinky2" });
	BoneMapping.Add(EMyPresetJointTag::L_HAND_PINKY3, FMyBoneMapConfig{ "LeftHandPinky3", "LeftHandPinky3" });
	BoneMapping.Add(EMyPresetJointTag::L_HAND_PINKY4, FMyBoneMapConfig{ "LeftHandPinky4", "LeftHandPinky4" });
	BoneMapping.Add(EMyPresetJointTag::R_FOREARM_END, FMyBoneMapConfig{ "RightForeArmEnd", "RightForeArmEnd" });
	BoneMapping.Add(EMyPresetJointTag::R_ARM_END, FMyBoneMapConfig{ "RightArmEnd", "RightArmEnd" });
	BoneMapping.Add(EMyPresetJointTag::L_FOREARM_END, FMyBoneMapConfig{ "LeftForeArmEnd", "LeftForeArmEnd" });
	BoneMapping.Add(EMyPresetJointTag::L_ARM_END, FMyBoneMapConfig{ "LeftArmEnd", "LeftArmEnd" });
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
		FrameOffset = Motion.size();

		UE_LOG(LogTemp, Warning, TEXT("Pose fully loaded!"));
		return true;
	}

	return false;
}

void ABvhController::AddMotion(const FString motion) 
{
 	Reader.AddBvhMotion(motion, &Motion);
}

TMap<FName, FTransform> ABvhController::GetCurrentFrame(USkeletalMeshComponent* mesh)
{
	TMap<FName, FTransform> CurframePose = TMap<FName, FTransform>(); 

	if (bPlay)
	{
		ml::Posture posture = Motion.GetPostureAtTime_ms(InternalTimeAccumulator);
		Poser.Retarget(posture, CurframePose);
		const int frame_id = (int)(Motion.fps() * InternalTimeAccumulator) - FrameOffset;

		if (frame_id >= 0)
		{
			OnFramePlayed.Broadcast(frame_id);
		}
		return CurframePose;
	}

	return CurframePose;
}


bool ABvhController::Pause(bool bPause)
{
	bPlay = !bPause;
	return true;
}
