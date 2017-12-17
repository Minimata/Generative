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

	void Generate();
	void Seed();

	virtual void DisplacePlane();
	virtual void LoadSceneReferences();
	virtual void InitArrays();
	virtual void SetMaterials() {}
	virtual int SetVertex(int, int32, int32, int32, int32) { return 0; }

	UPROPERTY(EditAnywhere)
		URuntimeMeshComponent* RuntimeMesh;
	UPROPERTY(EditAnywhere)
		TArray<UMaterialInterface*> Materials;

	ACharacter* Player;

	bool Generated = false;
	FBox BoundingBox;

	UPROPERTY(EditAnywhere)
	int32 HalfWidth = 1000;
	UPROPERTY(EditAnywhere)
	float CellSize = 5;
	int32 RandFactor = 0;
	int32 NumberOfTerrains = 1;

	TArray<TArray<FRuntimeMeshVertexDualUV>> Vertices;
	TArray<int32> Triangles;
	template<typename T>
	void InitSingleArray(TArray<TArray<T>>& array, T base) {
		TArray<T> baseArray;
		baseArray.Init(base, FMath::Pow(HalfWidth * 2., 2.));
		array.Init(baseArray, NumberOfTerrains);
	}

	URuntimeMeshLibrary* Lib;
};
