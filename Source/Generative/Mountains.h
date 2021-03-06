// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TerraGen.h"
#include "SimplexNoise.h"
#include "Mountains.generated.h"

UCLASS()
class GENERATIVE_API AMountains : public ATerraGen
{
	GENERATED_BODY()

public:
	AMountains();

	virtual void BeginPlay() override;

	virtual void Seed() override;
	virtual void SetMaterials() override;
	virtual void PopulateVertices() override;
	virtual void GenerateGeometry() override;
	virtual void CreateVertex(int index, int32 x, int32 y, int32 z, int XIndex, int YIndex) override;

protected:
	int32 Height(int32, int32) override;
	float Gauss(int32 x, float sigma, float average = 0);

	ASimplexNoise Noise;

	UPROPERTY(EditAnywhere)
	int32 HeightOfRocks = 700;
	UPROPERTY(EditAnywhere)
	int32 HeightFactor = 1000;
	UPROPERTY(EditAnywhere)
	int Octaves = 6;

	int32 RandFactorBaseHeight = 0;
	int32 RandFactorGradientNoise = 0;
};
