#include "AnimGraph/AnimNode_BvhConvert.h"
#include "AnimationRuntime.h"
#include "Animation/AnimInstanceProxy.h"

FAnimNode_BvhConvert::FAnimNode_BvhConvert()
{
}

FAnimNode_BvhConvert::~FAnimNode_BvhConvert()
{
}


void FAnimNode_BvhConvert::CacheBones_AnyThread(const FAnimationCacheBonesContext& Context)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(CacheBones_AnyThread)
	FAnimNode_SkeletalControlBase::CacheBones_AnyThread(Context);
}

void FAnimNode_BvhConvert::GatherDebugData(FNodeDebugData& DebugData)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(GatherDebugData)
	const FString DebugLine = DebugData.GetNodeName(this);
	DebugData.AddDebugItem(DebugLine);
	//Todo: Add more debug information here!
}

void FAnimNode_BvhConvert::OnInitializeAnimInstance(const FAnimInstanceProxy* InProxy, const UAnimInstance* InAnimInstance)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_FUNC()
	Super::OnInitializeAnimInstance(InProxy, InAnimInstance);
}

void FAnimNode_BvhConvert::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(Initialize_AnyThread)
	FAnimNode_SkeletalControlBase::Initialize_AnyThread(Context);
	GetEvaluateGraphExposedInputs().Execute(Context);
}

void FAnimNode_BvhConvert::UpdateInternal(const FAnimationUpdateContext& Context)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(UpdateInternal)

	GetEvaluateGraphExposedInputs().Execute(Context);
}

void FAnimNode_BvhConvert::FAnimNode_BvhConvert::EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms)
{
	DECLARE_SCOPE_HIERARCHICAL_COUNTER_ANIMNODE(EvaluateSkeletalControl_AnyThread)
	Output.ResetToRefPose();

	USkeletalMeshComponent* Component = Output.AnimInstanceProxy->GetSkelMeshComponent();
	if (Component && Component->GetSkeletalMeshAsset() && Controller)
	{
		const FBoneContainer& BoneContainer = Output.Pose.GetPose().GetBoneContainer();
		TMap<FName, FTransform> CurframePose = Controller->GetCurrentFrame(Component);

		for (TMap<FName, FTransform>::TIterator ItrMap(CurframePose); ItrMap; ++ItrMap)
		{
			int32 Index = Component->GetBoneIndex(ItrMap.Key());
			if (Index != INDEX_NONE)
			{
				FTransform SourceTM = ItrMap.Value();

				FCompactPoseBoneIndex CompactPoseBone = BoneContainer.GetCompactPoseIndexFromSkeletonPoseIndex(FSkeletonPoseBoneIndex(Index));

				OutBoneTransforms.Add(FBoneTransform(CompactPoseBone, SourceTM));
			}
		}

		OutBoneTransforms.Sort([](const FBoneTransform& A, const FBoneTransform& B) {
			return (A.BoneIndex <= B.BoneIndex);
			});
	}
}


bool FAnimNode_BvhConvert::IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones)
{
	return true;
}
