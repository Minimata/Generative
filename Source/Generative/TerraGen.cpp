// Fill out your copyright notice in the Description page of Project Settings.

#include "TerraGen.h"
#include "ParallelFor.h"
// #include "RuntimeMeshLibrary.h"


// Sets default values
ATerraGen::ATerraGen() :
	BoundingBox(FVector(-10000, -10000, -10000), FVector(10000, 10000, 10000))
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RuntimeMesh = CreateDefaultSubobject<URuntimeMeshComponent>(TEXT("Runtime Mesh"));
	RootComponent = RuntimeMesh;

	//Lib = CreateDefaultSubobject<URuntimeMeshLibrary>(TEXT("Runtime Mesh Lib"));
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
	RandFactor = FMath::Rand();
}

void ATerraGen::InitArrays() {
	InitSingleArray(Vertices, FRuntimeMeshVertexDualUV(FVector(0, 0, 0), 
														FVector(0, 0, 1), 
														FRuntimeMeshTangent(-1, 0, 0),
														FColor(0, 0, 0),
														FVector2D(0, 0)));
}


void ATerraGen::Generate() {

	int32 x = 0;
	int32 y = 0;

	int32 lowestX = 0;
	int32 lowestY = 0;
	int32 lowestZ = -99999;
	// Create the vertex array
	int index = 0;
	for (int32 YIndex = -HalfWidth; YIndex < HalfWidth; YIndex++)
	{
		for (int32 XIndex = -HalfWidth; XIndex < HalfWidth; XIndex++)
		{
			x = XIndex * CellSize;
			y = YIndex * CellSize;
			int32 z = SetVertex(index, x, y, XIndex, YIndex);

			if (z > lowestZ) {
				lowestX = x;
				lowestY = y;
				lowestZ = z;
			}
			index++;
		}
	}
	// Create the triangle array
	URuntimeMeshLibrary::CreateGridMeshTriangles(HalfWidth * 2, HalfWidth * 2, true, Triangles);

	//Lib->CalculateTangentsForMesh(VerticesRef, Triangles, TextureCoordinates, Normals, Tangents);

	for (int i = 0; i < NumberOfTerrains; ++i) {
		RuntimeMesh->CreateMeshSection(i, Vertices[i], Triangles, BoundingBox, true);
	}
	Player->SetActorLocation(FVector(lowestX, lowestY, lowestZ + this->GetActorLocation().Z + 500));

	Generated = true;
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