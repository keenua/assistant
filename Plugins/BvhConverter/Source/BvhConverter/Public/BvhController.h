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
	R_CLAVICLE, R_SHOULDER, R_ELBOW, R_WRIST, R_PALM, R_PALM_END
};


struct FMyBoneMapConfig
{
	public:
		FString Bvh_Joint;
		FString UE_Bone;
};

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
	float InternalTimeAccumulator = 0.0f;
	bool bLoopAnimation = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	bool Play(FString TPoseFile, FString BVHFile, bool bLoop);
	
	UFUNCTION(BlueprintCallable)
	bool Pause(bool bPause);

	UFUNCTION(BlueprintCallable)
	TMap<FName, FTransform> GetCurrentFrame(USkeletalMeshComponent* mesh);

	float GetTotalLength();

	TMap<EMyPresetJointTag, FMyBoneMapConfig> BoneMapping;

public:
	bool bPlay = false;
};
