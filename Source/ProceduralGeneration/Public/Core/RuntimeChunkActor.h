#pragma once

#include "CoreMinimal.h"
#include "RuntimeMeshActor.h"
#include "RuntimeChunkActor.generated.h"

USTRUCT(BlueprintType)
struct FRuntimeChunkData
{
	GENERATED_BODY()

	FRuntimeChunkData()
		: ChunkWidth(0)
		, ChunkHeight(0)
	{
	}

	FRuntimeChunkData(const int32 InChunkWidth, const int32 InChunkHeight)
		: ChunkWidth(InChunkWidth)
		, ChunkHeight(InChunkHeight)
	{
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 ChunkWidth = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 ChunkHeight = 0;
};

UCLASS(Blueprintable)
class PROCEDURALGENERATION_API ARuntimeChunkActor : public ARuntimeMeshActor
{
	GENERATED_BODY()

public:
	ARuntimeChunkActor();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Runtime Chunk Parameters")
	FRuntimeChunkData RuntimeChunkData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Runtime Chunk Parameters")
	UMaterialInterface* Material = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Runtime Chunk Parameters")
	FVector BoxRadius;

protected:
	void GenerateMesh() override;

	void GenerateMeshData(const TInlineComponentArray<FVector, 8>& InBoxVertices, uint8& InVertexCount, FRuntimeMeshRenderableMeshData& InRenderableMeshData);

private:
	UPROPERTY()
	class URuntimeMeshProviderStatic* RuntimeMeshProviderStatic = nullptr;
};
