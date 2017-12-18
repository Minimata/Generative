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

	virtual void SetMaterials() override;
	virtual int SetVertex(int, int32, int32, int32, int32) override;

protected:
	int32 Height(int32, int32);
	float Gauss(int32 x, float sigma, float average = 0);
	FVector SetNormal(int x, int y, int z);
	FVector GetFaceNormal(FVector center, FVector i, FVector j);

	ASimplexNoise Noise;

	UPROPERTY(EditAnywhere)
	int32 HeightOfRocks = 1000;
	UPROPERTY(EditAnywhere)
	int32 HeightFactor = 2000;
	UPROPERTY(EditAnywhere)
	int Octaves = 6;
};
