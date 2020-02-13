#include "Core/RuntimeChunkActor.h"
#include "RuntimeMeshComponent.h"
#include "Providers/RuntimeMeshProviderStatic.h"

DEFINE_LOG_CATEGORY(LogRuntimeMesh);

ARuntimeChunkActor::ARuntimeChunkActor()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.SetTickFunctionEnable(false);
}

void ARuntimeChunkActor::GenerateMesh()
{
	/** Do not create Mesh if Box Scale == 0  or Chunk Size == 0*/
	if (BoxRadius.Size() <= 0.f || RuntimeChunkData.ChunkWidth <= 0 || RuntimeChunkData.ChunkHeight <= 0)
		return;

	/** Create new Mesh Provider if no existing */
	if (!RuntimeMeshProviderStatic)
		RuntimeMeshProviderStatic = NewObject<URuntimeMeshProviderStatic>();

	/** Important! Runtime Component must be initialized with Mesh Provider */
	GetRuntimeMeshComponent()->Initialize(RuntimeMeshProviderStatic);

	uint8 VertexCount = 0;
	TInlineComponentArray<FVector, 8> BoxVertices;
	FRuntimeMeshRenderableMeshData RenderableMeshData(false, false, 1, false);

	FRuntimeMeshSectionProperties RuntimeMeshSectionProperties;
	RuntimeMeshSectionProperties.bCastsShadow = true;
	RuntimeMeshSectionProperties.bIsVisible = true;
	RuntimeMeshSectionProperties.MaterialSlot = 0;
	RuntimeMeshSectionProperties.UpdateFrequency = ERuntimeMeshUpdateFrequency::Infrequent;

	for (int8 X = 0; X < RuntimeChunkData.ChunkWidth; ++X)
	{
		for (int8 Y = 0; Y < RuntimeChunkData.ChunkWidth; ++Y)
		{
			for (int8 Z = 0; Z < RuntimeChunkData.ChunkHeight; ++Z)
			{
				BoxVertices.Reset();

				/** Generate verts */
				BoxVertices.Add(FVector(-BoxRadius.X + (X * BoxRadius.X * 2.f), BoxRadius.Y + (Y * BoxRadius.Y * 2.f), BoxRadius.Z + (Z * BoxRadius.Z * 2.f)));
				BoxVertices.Add(FVector(BoxRadius.X + (X * BoxRadius.X * 2.f), BoxRadius.Y + (Y * BoxRadius.Y * 2.f), BoxRadius.Z + (Z * BoxRadius.Z * 2.f)));
				BoxVertices.Add(FVector(BoxRadius.X + (X * BoxRadius.X * 2.f), -BoxRadius.Y + (Y * BoxRadius.Y * 2.f), BoxRadius.Z + (Z * BoxRadius.Z * 2.f)));
				BoxVertices.Add(FVector(-BoxRadius.X + (X * BoxRadius.X * 2.f), -BoxRadius.Y + (Y * BoxRadius.Y * 2.f), BoxRadius.Z + (Z * BoxRadius.Z * 2.f)));

				BoxVertices.Add(FVector(-BoxRadius.X + (X * BoxRadius.X * 2.f), BoxRadius.Y + (Y * BoxRadius.Y * 2.f), -BoxRadius.Z + (Z * BoxRadius.Z * 2.f)));
				BoxVertices.Add(FVector(BoxRadius.X + (X * BoxRadius.X * 2.f), BoxRadius.Y + (Y * BoxRadius.Y * 2.f), -BoxRadius.Z + (Z * BoxRadius.Z * 2.f)));
				BoxVertices.Add(FVector(BoxRadius.X + (X * BoxRadius.X * 2.f), -BoxRadius.Y + (Y * BoxRadius.Y * 2.f), -BoxRadius.Z + (Z * BoxRadius.Z * 2.f)));
				BoxVertices.Add(FVector(-BoxRadius.X + (X * BoxRadius.X * 2.f), -BoxRadius.Y + (Y * BoxRadius.Y * 2.f), -BoxRadius.Z + (Z * BoxRadius.Z * 2.f)));

				GenerateMeshData(BoxVertices, VertexCount, RenderableMeshData);
			}
		}
	}
	/** Mesh Generation */
	RuntimeMeshProviderStatic->CreateSection(/*LODIndex*/ 0, /*SectionId*/ 0, RuntimeMeshSectionProperties, RenderableMeshData);
	/** Apply Material to Generated mesh */
	if (Material)
		RuntimeMeshProviderStatic->SetupMaterialSlot(0, TEXT("Chunk Base"), Material);

	Super::GenerateMesh();
}

void ARuntimeChunkActor::GenerateMeshData(const TInlineComponentArray<FVector, 8>& InBoxVertices, uint8& InVertexCount, FRuntimeMeshRenderableMeshData& InRenderableMeshData)
{
	FVector Normal, Tangent;
	uint8 InternalVertexCount = 0;

	/** Helper function (lambda) to add vertex to the mesh data */
	auto AddVertex = [&InRenderableMeshData](const FVector& InPosition, const FVector& InTangent, const FVector& InNormal, const FVector2D& InTexCoord)
	{
		InRenderableMeshData.Positions.Add(InPosition);
		InRenderableMeshData.Tangents.Add(InNormal, InTangent);
		InRenderableMeshData.Colors.Add(FColor::White);
		InRenderableMeshData.TexCoords.Add(InTexCoord);
	};

	/** Front (Positive X) */
	Normal = FVector(1.0f, 0.0f, 0.0f);
	Tangent = FVector(0.0f, 1.0f, 0.0f);
	AddVertex(InBoxVertices[6], Tangent, Normal, FVector2D(0.0f, 0.0f));
	AddVertex(InBoxVertices[2], Tangent, Normal, FVector2D(0.0f, 1.0f));
	AddVertex(InBoxVertices[1], Tangent, Normal, FVector2D(1.0f, 1.0f));
	AddVertex(InBoxVertices[5], Tangent, Normal, FVector2D(1.0f, 0.0f));
	InRenderableMeshData.Triangles.AddTriangle(12 + InVertexCount, 13 + InVertexCount, 15 + InVertexCount);
	InRenderableMeshData.Triangles.AddTriangle(13 + InVertexCount, 14 + InVertexCount, 15 + InVertexCount);
	InternalVertexCount += 4;

	/** Back (Negative X) */
	Normal = FVector(-1.0f, 0.0f, 0.0f);
	Tangent = FVector(0.0f, -1.0f, 0.0f);
	AddVertex(InBoxVertices[4], Tangent, Normal, FVector2D(0.0f, 0.0f));
	AddVertex(InBoxVertices[0], Tangent, Normal, FVector2D(0.0f, 1.0f));
	AddVertex(InBoxVertices[3], Tangent, Normal, FVector2D(1.0f, 1.0f));
	AddVertex(InBoxVertices[7], Tangent, Normal, FVector2D(1.0f, 0.0f));
	InRenderableMeshData.Triangles.AddTriangle(4 + InVertexCount, 5 + InVertexCount, 7 + InVertexCount);
	InRenderableMeshData.Triangles.AddTriangle(5 + InVertexCount, 6 + InVertexCount, 7 + InVertexCount);
	InternalVertexCount += 4;

	/** Right (Positive Y) */
	Normal = FVector(0.0f, 1.0f, 0.0f);
	Tangent = FVector(-1.0f, 0.0f, 0.0f);
	AddVertex(InBoxVertices[5], Tangent, Normal, FVector2D(0.0f, 0.0f));
	AddVertex(InBoxVertices[1], Tangent, Normal, FVector2D(0.0f, 1.0f));
	AddVertex(InBoxVertices[0], Tangent, Normal, FVector2D(1.0f, 1.0f));
	AddVertex(InBoxVertices[4], Tangent, Normal, FVector2D(1.0f, 0.0f));
	InRenderableMeshData.Triangles.AddTriangle(8 + InVertexCount, 9 + InVertexCount, 11 + InVertexCount);
	InRenderableMeshData.Triangles.AddTriangle(9 + InVertexCount, 10 + InVertexCount, 11 + InVertexCount);
	InternalVertexCount += 4;

	/** Left (Negative Y) */
	Normal = FVector(0.0f, -1.0f, 0.0f);
	Tangent = FVector(1.0f, 0.0f, 0.0f);
	AddVertex(InBoxVertices[7], Tangent, Normal, FVector2D(0.0f, 0.0f));
	AddVertex(InBoxVertices[3], Tangent, Normal, FVector2D(0.0f, 1.0f));
	AddVertex(InBoxVertices[2], Tangent, Normal, FVector2D(1.0f, 1.0f));
	AddVertex(InBoxVertices[6], Tangent, Normal, FVector2D(1.0f, 0.0f));
	InRenderableMeshData.Triangles.AddTriangle(16 + InVertexCount, 17 + InVertexCount, 19 + InVertexCount);
	InRenderableMeshData.Triangles.AddTriangle(17 + InVertexCount, 18 + InVertexCount, 19 + InVertexCount);
	InternalVertexCount += 4;

	/** Top (Positive Z) */
	Normal = FVector(0.0f, 0.0f, 1.0f);
	Tangent = FVector(0.0f, -1.0f, 0.0f);
	AddVertex(InBoxVertices[0], Tangent, Normal, FVector2D(0.0f, 0.0f));
	AddVertex(InBoxVertices[1], Tangent, Normal, FVector2D(0.0f, 1.0f));
	AddVertex(InBoxVertices[2], Tangent, Normal, FVector2D(1.0f, 1.0f));
	AddVertex(InBoxVertices[3], Tangent, Normal, FVector2D(1.0f, 0.0f));
	InRenderableMeshData.Triangles.AddTriangle(0 + InVertexCount, 1 + InVertexCount, 3 + InVertexCount);
	InRenderableMeshData.Triangles.AddTriangle(1 + InVertexCount, 2 + InVertexCount, 3 + InVertexCount);
	InternalVertexCount += 4;

	/** Bottom (Negative Z) */
	Normal = FVector(0.0f, 0.0f, -1.0f);
	Tangent = FVector(0.0f, 1.0f, 0.0f);
	AddVertex(InBoxVertices[7], Tangent, Normal, FVector2D(0.0f, 0.0f));
	AddVertex(InBoxVertices[6], Tangent, Normal, FVector2D(0.0f, 1.0f));
	AddVertex(InBoxVertices[5], Tangent, Normal, FVector2D(1.0f, 1.0f));
	AddVertex(InBoxVertices[4], Tangent, Normal, FVector2D(1.0f, 0.0f));
	InRenderableMeshData.Triangles.AddTriangle(20 + InVertexCount, 21 + InVertexCount, 23 + InVertexCount);
	InRenderableMeshData.Triangles.AddTriangle(21 + InVertexCount, 22 + InVertexCount, 23 + InVertexCount);
	InternalVertexCount += 4;

	/** Fill Vertex Count */
	InVertexCount += InternalVertexCount;
}
