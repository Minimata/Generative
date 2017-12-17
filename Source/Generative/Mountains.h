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

	ASimplexNoise Noise;

	const int32 HeightOfRocks = 500;
	int32 HeightFactor = 1300;
	int Octaves = 6;
};
