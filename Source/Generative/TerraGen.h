// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RuntimeMeshComponent.h"
#include "TerraGen.generated.h"

UCLASS(abstract)
class GENERATIVE_API ATerraGen : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATerraGen();

	// virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	virtual void InitArrays();
	virtual void SetMaterials() {}
	virtual void LoadSceneReferences();

	virtual void Generate();

	virtual void PopulateVertices() {}
	virtual void GenerateGeometry() {}
	virtual void CreateVertex(int index, int32 x, int32 y, int32 z, int XIndex, int YIndex) {}

	virtual int32 Height(int32 x, int32 y) { return 0; }
	virtual void GenerateFromPlane();

	virtual void DisplacePlane();

	void Seed();
	FVector SetNormal(int32 x, int32 y, int XIndex, int YIndex);
	FVector GetFaceNormal(FVector center, FVector i, FVector j);

	UPROPERTY(EditAnywhere)
		URuntimeMeshComponent* RuntimeMesh;
	UPROPERTY(EditAnywhere)
		TArray<UMaterialInterface*> Materials;

	ACharacter* Player;

	FBox BoundingBox;

	UPROPERTY(EditAnywhere)
	int32 HalfWidth = 1025;
	UPROPERTY(EditAnywhere)
	float CellSize = 5;
	int32 NumberOfTerrains = 1;

	TArray<TArray<int32>> VerticesPos;

	TArray<TArray<FRuntimeMeshVertexDualUV>> Vertices;
	TArray<int32> Triangles;
	template<typename T>
	void InitSingleArray(TArray<TArray<T>>& array, T base, int size1, int size2) {
		TArray<T> baseArray;
		baseArray.Init(base, size1);
		array.Init(baseArray, size2);
	}

	URuntimeMeshLibrary* Lib;
};
