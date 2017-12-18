// Fill out your copyright notice in the Description page of Project Settings.

#include "Mountains.h"

using Vertex = FRuntimeMeshVertexDualUV;

AMountains::AMountains() :
	Noise(0.0001f, 10000.0f, 2.0f, 0.5f), 
	ATerraGen() {
	NumberOfTerrains = 2;
}

void AMountains::BeginPlay() {
	Super::BeginPlay();
}

void AMountains::SetMaterials() {
	for (int i = 0; i < NumberOfTerrains; ++i) {
		RuntimeMesh->SetMaterial(i, Materials[i]);
	}
}

int AMountains::SetVertex(int index, int32 x, int32 y, int32 XIndex, int32 YIndex) {
	float Scale = CellSize / 50.0f;

	int32 z = Height(x, y);
	FVector position(x, y, z);

	FVector normal = SetNormal(x, y, z);

	FRuntimeMeshTangent tangent = FVector::CrossProduct(normal, FVector(0, 0, 1));
	FColor color(0, 0, 0);
	FVector2D UV0(XIndex * Scale, YIndex * Scale);

	for (int k = 0; k < NumberOfTerrains; ++k) {
		Vertices[k][index] = Vertex(FVector(x, y, z - 1), normal, tangent, color, UV0);

		if (z < HeightOfRocks) {
			Vertices[0][index] = Vertex(position, normal, tangent, color, UV0);
		}
		else {
			//UE_LOG(LogTemp, Warning, TEXT("NOT ROCKS"));
			Vertices[1][index] = Vertex(position, normal, tangent, color, UV0);
		}
	}

	return z;
}

FVector AMountains::SetNormal(int x, int y, int z) {
	FMatrix heights(FVector(Height(x - CellSize, y - CellSize), Height(x, y - CellSize), Height(x + CellSize, y - CellSize)),
		FVector(Height(x - CellSize, y), z, Height(x + CellSize, y)),
		FVector(Height(x - CellSize, y + CellSize), Height(x, y + CellSize), Height(x + CellSize, y + CellSize)),
		FVector(0, 0, 0)
	);

	TArray<FVector> vertices;
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			int32 posX = x + i * CellSize;
			int32 posY = y + j * CellSize;
			vertices.Add(FVector(posX, posY, Height(posX, posY)));
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

FVector AMountains::GetFaceNormal(FVector center, FVector i, FVector j) {
	FVector first = i - center;
	FVector second = j - center;
	return FVector::CrossProduct(second, first);
}

int32 AMountains::Height(int32 x, int32 y) {
	int32 distanceToCenter = FMath::Sqrt(FMath::Pow(x, 2) + FMath::Pow(y, 2));
	int32 maxDistance = HalfWidth * CellSize;
	return /* Gauss(distanceToCenter / HalfWidth, 1) */ HeightFactor * Noise.fractal(Octaves, x + RandFactor, y + RandFactor);
}

float AMountains::Gauss(int32 x, float sigma, float average) {
	check(sigma > 0);
	return ((1 / (sigma * FMath::Sqrt(2 * 3.14159265))) * FMath::Exp(-FMath::Pow((x - average), 2)/(2. * FMath::Pow(sigma, 2))));
}
