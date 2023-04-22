// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Animation/AnimNodeBase.h"
#include "BvhController.h"
#include "BoneControllers/AnimNode_SkeletalControlBase.h"
#include "Motion/ml.h"			

#include "AnimNode_BvhConvert.generated.h"

class USkeletalMeshComponent;

/**
 *	Convert Bvh Data to Pose Data
 */
USTRUCT(BlueprintInternalUseOnly)
struct BVHCONVERTER_API FAnimNode_BvhConvert : public FAnimNode_SkeletalControlBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Settings, meta = (PinShownByDefault))
	ABvhController* Controller;

	FAnimNode_BvhConvert();
	virtual ~FAnimNode_BvhConvert() override;

	virtual void OnInitializeAnimInstance(const FAnimInstanceProxy* InProxy, const UAnimInstance* InAnimInstance) override;
	virtual bool NeedsOnInitializeAnimInstance() const override { return true; }

	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;
	virtual void UpdateInternal(const FAnimationUpdateContext& Context) override;
	//virtual void Evaluate_AnyThread(FPoseContext& Output) override;
	virtual void EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms) override;
	virtual void CacheBones_AnyThread(const FAnimationCacheBonesContext& Context)  override;
	virtual void GatherDebugData(FNodeDebugData& DebugData) override;

	virtual bool IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones) override;
};
