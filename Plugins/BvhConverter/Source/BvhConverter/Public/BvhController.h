// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Motion/ml.h"
#include "BvhController.generated.h"


UENUM(BlueprintType)
enum class EMyPresetJointTag : uint8
{
	UNKNOWN = 0,
	PELVIS = 1, SPINE, SPINE1, SPINE2, CHEST, NECK, HEAD, HEAD_END,
	L_HIP, L_KNEE, L_ANKLE, L_FOOT, L_TOE, L_TOE_END,
	R_HIP, R_KNEE, R_ANKLE, R_FOOT, R_TOE, R_TOE_END,
	L_CLAVICLE, L_SHOULDER, L_ELBOW, L_WRIST, L_PALM, L_PALM_END,
	R_CLAVICLE, R_SHOULDER, R_ELBOW, R_WRIST, R_PALM, R_PALM_END,
	R_HAND_THUMB1, R_HAND_THUMB2, R_HAND_THUMB3, R_HAND_THUMB4,
	R_HAND_INDEX1, R_HAND_INDEX2, R_HAND_INDEX3, R_HAND_INDEX4,
	R_HAND_MIDDLE1, R_HAND_MIDDLE2, R_HAND_MIDDLE3, R_HAND_MIDDLE4,
	R_HAND_RING1, R_HAND_RING2, R_HAND_RING3, R_HAND_RING4,
	R_HAND_PINKY1, R_HAND_PINKY2, R_HAND_PINKY3, R_HAND_PINKY4,
	L_HAND_THUMB1, L_HAND_THUMB2, L_HAND_THUMB3, L_HAND_THUMB4,
	L_HAND_INDEX1, L_HAND_INDEX2, L_HAND_INDEX3, L_HAND_INDEX4,
	L_HAND_MIDDLE1, L_HAND_MIDDLE2, L_HAND_MIDDLE3, L_HAND_MIDDLE4,
	L_HAND_RING1, L_HAND_RING2, L_HAND_RING3, L_HAND_RING4,
	L_HAND_PINKY1, L_HAND_PINKY2, L_HAND_PINKY3, L_HAND_PINKY4,
	R_FOREARM_END, R_ARM_END, L_FOREARM_END, L_ARM_END
};


struct FMyBoneMapConfig
{
	public:
		FString Bvh_Joint;
		FString UE_Bone;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFrame, const int, FrameIndex);

UCLASS()
class BVHCONVERTER_API ABvhController : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	ABvhController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	ml::Motion Motion;
	ml::UE4Poser Poser;
	ml::BVHReader Reader;
	float InternalTimeAccumulator = 0.0f;
	bool bLoopAnimation = false;
	int FrameOffset = 0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	bool Play(FString TPoseFile, FString BVHFile, bool bLoop);
	
	UFUNCTION(BlueprintCallable)
	bool Pause(bool bPause);

	UFUNCTION(BlueprintCallable)
	TMap<FName, FTransform> GetCurrentFrame(USkeletalMeshComponent* mesh);

	UFUNCTION(BlueprintCallable)
	void SetSkeleton(USkeletalMeshComponent* mesh);

	UFUNCTION(BlueprintCallable)
	void AddMotion(const FString motion);

	UPROPERTY(BlueprintAssignable)
	FOnFrame OnFramePlayed;

	float GetTotalLength();

	TMap<EMyPresetJointTag, FMyBoneMapConfig> BoneMapping;

public:
	bool bPlay = false;
};
