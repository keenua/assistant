#pragma once
#include "CoreMinimal.h"
#include "Engine.h"
#include "MotionController.h"
#include "MotionFrame.h"
#include "MotionLibrary.generated.h"

UENUM(BlueprintType)
enum class EBonesEnum : uint8
{
	VE_Hips				UMETA(DisplayName = "Hips"),
	VE_Spine			UMETA(DisplayName = "Spine"),
	VE_Spine1			UMETA(DisplayName = "Spine1"),
	VE_Spine2			UMETA(DisplayName = "Spine2"),
	VE_Spine3			UMETA(DisplayName = "Spine3"),
	VE_Neck				UMETA(DisplayName = "Neck"),
	VE_Neck1			UMETA(DisplayName = "Neck1"),
	VE_Head				UMETA(DisplayName = "Head"),
	VE_RightShoulder	UMETA(DisplayName = "RightShoulder"),
	VE_RightArm			UMETA(DisplayName = "RightArm"),
	VE_RightForeArm		UMETA(DisplayName = "RightForeArm"),
	VE_RightHand		UMETA(DisplayName = "RightHand"),
	VE_RightHandThumb1	UMETA(DisplayName = "RightHandThumb1"),
	VE_RightHandThumb2	UMETA(DisplayName = "RightHandThumb2"),
	VE_RightHandThumb3	UMETA(DisplayName = "RightHandThumb3"),
	VE_RightHandIndex1	UMETA(DisplayName = "RightHandIndex1"),
	VE_RightHandIndex2	UMETA(DisplayName = "RightHandIndex2"),
	VE_RightHandIndex3	UMETA(DisplayName = "RightHandIndex3"),
	VE_RightHandMiddle1	UMETA(DisplayName = "RightHandMiddle1"),
	VE_RightHandMiddle2	UMETA(DisplayName = "RightHandMiddle2"),
	VE_RightHandMiddle3	UMETA(DisplayName = "RightHandMiddle3"),
	VE_RightHandRing1	UMETA(DisplayName = "RightHandRing1"),
	VE_RightHandRing2	UMETA(DisplayName = "RightHandRing2"),
	VE_RightHandRing3	UMETA(DisplayName = "RightHandRing3"),
	VE_RightHandPinky1	UMETA(DisplayName = "RightHandPinky1"),
	VE_RightHandPinky2	UMETA(DisplayName = "RightHandPinky2"),
	VE_RightHandPinky3	UMETA(DisplayName = "RightHandPinky3"),
	VE_LeftShoulder		UMETA(DisplayName = "LeftShoulder"),
	VE_LeftArm			UMETA(DisplayName = "LeftArm"),
	VE_LeftForeArm		UMETA(DisplayName = "LeftForeArm"),
	VE_LeftHand			UMETA(DisplayName = "LeftHand"),
	VE_LeftHandThumb1	UMETA(DisplayName = "LeftHandThumb1"),
	VE_LeftHandThumb2	UMETA(DisplayName = "LeftHandThumb2"),
	VE_LeftHandThumb3	UMETA(DisplayName = "LeftHandThumb3"),
	VE_LeftHandIndex1	UMETA(DisplayName = "LeftHandIndex1"),
	VE_LeftHandIndex2	UMETA(DisplayName = "LeftHandIndex2"),
	VE_LeftHandIndex3	UMETA(DisplayName = "LeftHandIndex3"),
	VE_LeftHandMiddle1	UMETA(DisplayName = "LeftHandMiddle1"),
	VE_LeftHandMiddle2	UMETA(DisplayName = "LeftHandMiddle2"),
	VE_LeftHandMiddle3	UMETA(DisplayName = "LeftHandMiddle3"),
	VE_LeftHandRing1	UMETA(DisplayName = "LeftHandRing1"),
	VE_LeftHandRing2	UMETA(DisplayName = "LeftHandRing2"),
	VE_LeftHandRing3	UMETA(DisplayName = "LeftHandRing3"),
	VE_LeftHandPinky1	UMETA(DisplayName = "LeftHandPinky1"),
	VE_LeftHandPinky2	UMETA(DisplayName = "LeftHandPinky2"),
	VE_LeftHandPinky3	UMETA(DisplayName = "LeftHandPinky3"),
	VE_RightUpLeg		UMETA(DisplayName = "RightUpLeg"),
	VE_RightLeg			UMETA(DisplayName = "RightLeg"),
	VE_RightFoot		UMETA(DisplayName = "RightFoot"),
	VE_RightToeBase		UMETA(DisplayName = "RightToeBase"),
	VE_LeftUpLeg		UMETA(DisplayName = "LeftUpLeg"),
	VE_LeftLeg			UMETA(DisplayName = "LeftLeg"),
	VE_LeftFoot			UMETA(DisplayName = "LeftFoot"),
	VE_LeftToeBase		UMETA(DisplayName = "LeftToeBase"),
};


USTRUCT(BlueprintType)
struct FBoneMap
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion Client")
	EBonesEnum BVHBone = EBonesEnum::VE_Hips;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Motion Client")
	FName CustomBoneName = FName(TEXT("None"));
};

UCLASS()
class MOTIONCLIENT_API UMotionLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

	public:
		// Init functions
		UFUNCTION(BlueprintCallable, Category = "Motion Client", meta = (DisplayName = "Init Motion File", ToolTip = "Init BVH reference skeleton with a BVH file."))
		static bool InitFile(AMotionController *Controller, const FString BVHFileName = FString(TEXT("init.bvh")));

		UFUNCTION(BlueprintCallable, Category = "Motion Client", meta = (DisplayName = "Read Motion", ToolTip = "Read incoming motion data for all bones."))
		static bool ReadMotion(AMotionController *Controller, USkeletalMeshComponent *Mesh, FVector& Translation, FRotator& Rotation, FVector AdditionalTranslation, FRotator AdditionalRotation, EBonesEnum BVHBone = EBonesEnum::VE_Hips, FName CustomBoneName = FName(TEXT("None")), bool InverseForward = false);
		UFUNCTION(BlueprintCallable, Category = "Motion Client", meta = (DisplayName = "Read Motion Array", ToolTip = "Read current frame's motion data for all bones."))
		static bool ReadMotionArray(AMotionController *Controller, USkeletalMeshComponent *Mesh, TArray<FVector> &Translation, TArray<FRotator> &Rotation, TArray<FVector> AdditionalTranslation, TArray<FRotator> AdditionalRotation, TArray<FBoneMap> BoneMap, bool InverseForward = false);

		UFUNCTION(BlueprintCallable, Category = "Motion Client", meta = (DisplayName = "Add Motion Frames", ToolTip = "Add motion frames to the motion controller."))
		static bool AddFrames(AMotionController *Controller, TArray<FMotionFrame> Frames);
		UFUNCTION(BlueprintCallable, Category = "Motion Client", meta = (DisplayName = "Correct Frame", ToolTip = "Correct frame offset for the motion controller."))
		static void CorrectFrame(AMotionController *Controller, int SoundPlayedFrameIndex, float Duration, float PlaybackTime, int AudioBytes);
		UFUNCTION(BlueprintCallable, Category = "Motion Client", meta = (DisplayName = "Add Motion Frames From File", ToolTip = "Add motion frames to the motion controller from BVH file."))
		static bool AddFramesFromFile(AMotionController *Controller, const FString BVHFileName);

		UFUNCTION(BlueprintPure, Category = "Motion Client", meta = (DisplayName = "Get BVH Bone Index", CompactNodeTitle = "Index", ToolTip = "Return BVH Bone Index from Enum.",  BlueprintThreadSafe))
		static int32 GetBVHBoneIndex(EBonesEnum BVHBone = EBonesEnum::VE_Hips);
};