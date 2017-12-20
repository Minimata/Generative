// Fill out your copyright notice in the Description page of Project Settings.

#include "TerraGen.h"
#include "ParallelFor.h"
// #include "RuntimeMeshLibrary.h"


// Sets default values
ATerraGen::ATerraGen() :
	BoundingBox(FVector(-100000, -100000, -10000), FVector(100000, 100000, 10000))
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RuntimeMesh = CreateDefaultSubobject<URuntimeMeshComponent>(TEXT("Runtime Mesh"));
	RootComponent = RuntimeMesh;
}

// Called when the game starts or when spawned
void ATerraGen::BeginPlay()
{
	Super::BeginPlay();
	Seed();
	LoadSceneReferences();
	InitArrays();
	Generate();
	SetMaterials();
}

void ATerraGen::LoadSceneReferences() {
	Player = UGameplayStatics::GetPlayerCharacter(this, 0);
}

void ATerraGen::Seed() {
	double secs = FTimespan(FDateTime::Now().GetTicks()).GetTotalSeconds();
	int32 seed = (int32)(((int64)secs) % INT_MAX);
	FMath::RandInit(seed);
}

void ATerraGen::InitArrays() {
	InitSingleArray(VerticesPos, -1000, HalfWidth * 2, HalfWidth * 2);
	InitSingleArray(Vertices, FRuntimeMeshVertexDualUV(FVector(0, 0, 0), 
														FVector(0, 0, 1), 
														FRuntimeMeshTangent(-1, 0, 0),
														FColor(0, 0, 0),
														FVector2D(0, 0)), FMath::Pow(HalfWidth * 2., 2.), NumberOfTerrains);
}


void ATerraGen::Generate() {

	PopulateVertices();

	int32 lowestX = 0;
	int32 lowestY = 0;
	int32 lowestZ = INT_MIN;

	int index = 0;
	for (int YIndex = 0; YIndex < VerticesPos.Num(); ++YIndex) {
		for (int XIndex = 0; XIndex < VerticesPos[YIndex].Num(); ++XIndex) {
			int32 x = (XIndex - HalfWidth) * CellSize;
			int32 y = (YIndex - HalfWidth) * CellSize;
			int32 z = VerticesPos[XIndex][YIndex];
			CreateVertex(index, x, y, z, XIndex, YIndex);
			index++;

			if (z > lowestZ) {
				lowestX = x;
				lowestY = y;
				lowestZ = z;
			}
		}
	}

	Player->SetActorLocation(FVector(lowestX, lowestY, lowestZ + this->GetActorLocation().Z + 500));

	GenerateGeometry();
}

void ATerraGen::GenerateFromPlane() {
	URuntimeMeshLibrary::CreateGridMeshTriangles(HalfWidth * 2, HalfWidth * 2, true, Triangles);
	for (int i = 0; i < NumberOfTerrains; ++i) {
		RuntimeMesh->CreateMeshSection(i, Vertices[i], Triangles, BoundingBox, true);
	}
}

FVector ATerraGen::SetNormal(int32 x, int32 y, int XIndex, int YIndex) {

	TArray<FVector> vertices;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			int32 posX = x + i * CellSize;
			int32 posY = y + j * CellSize;
			int32 z = 0;
			if (XIndex == 0 || YIndex == 0 || XIndex == 2 * HalfWidth - 1 || YIndex == 2 * HalfWidth - 1) z = 0;
			else z = VerticesPos[XIndex + i][YIndex + j];
			vertices.Add(FVector(posX, posY, z));
		}
	}

	FVector normal(0, 0, 0);
	FVector center = vertices[4];

	normal += GetFaceNormal(center, vertices[0], vertices[1]);
	normal += GetFaceNormal(center, vertices[1], vertices[2]);
	normal += GetFaceNormal(center, vertices[2], vertices[5]);
	normal += GetFaceNormal(center, vertices[5], vertices[8]);
	normal += GetFaceNormal(center, vertices[8], vertices[7]);
	normal += GetFaceNormal(center, vertices[7], vertices[6]);
	normal += GetFaceNormal(center, vertices[6], vertices[3]);
	normal += GetFaceNormal(center, vertices[3], vertices[0]);

	normal.Normalize();
	return normal;
}

FVector ATerraGen::GetFaceNormal(FVector center, FVector i, FVector j) {
	FVector first = i - center;
	FVector second = j - center;
	return FVector::CrossProduct(second, first);
}


void ATerraGen::DisplacePlane()
{
	/*
	if (!Generated)
	{
	GeneratePlane();
	}

	// Start position only update
	TArray<FVector>& Positions = *RuntimeMesh->BeginMeshSectionPositionUpdate(0);
	int32 randFactor = FMath::Rand();

	int32 lowestX = 0;
	int32 lowestY = 0;
	int32 lowestZ = -99999;

	ParallelFor((HalfWidth * 2)*(HalfWidth * 2), [&](int32 Index)
	{
	int32 PlaneWidth = HalfWidth * 2;
	int32 XIndex = Index % PlaneWidth;
	int32 YIndex = Index / PlaneWidth;
	int32 x = XIndex * CellSize;
	int32 y = YIndex * CellSize;

	// Positions[Index].Z = HeightFactor * FastNoise->GetSimplexFractal(XIndex * CellSize, YIndex * CellSize);
	int32 z = HeightFactor * Noise.fractal(6, x + randFactor, y + randFactor);
	Positions[Index].Z = z;
	if (z > lowestZ) {
	lowestX = Positions[Index].X;
	lowestY = Positions[Index].Y;
	lowestZ = z;
	}
	});

	// Commit position only update (sends updates to GPU)
	RuntimeMesh->EndMeshSectionPositionUpdate(0, BoundingBox);

	Player->SetActorLocation(FVector(lowestX, lowestY, lowestZ + 1000));
	*/
}