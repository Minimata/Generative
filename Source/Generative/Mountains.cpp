// Fill out your copyright notice in the Description page of Project Settings.

#include "Mountains.h"


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

	//FMATRIX OPERATIONS !!!

	FVector normal(0, 0, 1);
	FRuntimeMeshTangent tangent(-1, 0, 0);
	FColor color(0, 0, 0);
	FVector2D UV0(XIndex * Scale, YIndex * Scale);

	for (int k = 0; k < NumberOfTerrains; ++k) {
		Vertices[k][index] = FRuntimeMeshVertexDualUV(FVector(x, y, z - 1), normal, tangent, color, UV0);

		if (z < HeightOfRocks) {
			Vertices[0][index] = FRuntimeMeshVertexDualUV(position, normal, tangent, color, UV0);
		}
		else {
			//UE_LOG(LogTemp, Warning, TEXT("NOT ROCKS"));
			Vertices[1][index] = FRuntimeMeshVertexDualUV(position, normal, tangent, color, UV0);
		}
	}

	return z;
}

int32 AMountains::Height(int32 x, int32 y) {
	return HeightFactor * Noise.fractal(Octaves, x + RandFactor, y + RandFactor);
}