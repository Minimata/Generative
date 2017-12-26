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

void AMountains::Seed() {
	Super::Seed();
	RandFactorBaseHeight = FMath::Rand();
	RandFactorGradientNoise = FMath::Rand();
}

void AMountains::PopulateVertices() {

	int32 XIndex = 0;
	int32 YIndex = 0;
	int32 endX = HalfWidth * 2 - 2;
	int32 endY = HalfWidth * 2 - 2;

	VerticesPos[0][0] = FMath::RandRange(0, HeightFactor);
	VerticesPos[endX][0] = FMath::RandRange(0, HeightFactor);
	VerticesPos[0][endY] = FMath::RandRange(0, HeightFactor);
	VerticesPos[endX][endY] = FMath::RandRange(0, HeightFactor);

	int step = HalfWidth * 2 - 2;
	while (step > 1) {
		UE_LOG(LogTemp, Warning, TEXT("Step : %d"), step);
		int32 halfStep = step / 2;
		for (int i = XIndex + halfStep; i < endX; i += step) {
			for (int j = YIndex + halfStep; j < endY; j += step) {
				int32 average = VerticesPos[i - halfStep][j - halfStep] +
					VerticesPos[i - halfStep][j + halfStep] +
					VerticesPos[i + halfStep][j - halfStep] +
					VerticesPos[i + halfStep][j + halfStep];
				average /= 4;
				VerticesPos[i][j] = average + FMath::FRandRange(-halfStep, halfStep);
			}
		}
		for (int i = XIndex; i < endX; i += halfStep) {
			int offset = 0;
			if(i % step == 0) offset = halfStep;
			for (int j = YIndex + offset; j < endY; j += step) {
				int sum = 0;
				int n = 0;
				if (i >= halfStep + XIndex) {
					sum += VerticesPos[i - halfStep ][j];
					n++;
				}
				if (i + halfStep < endX) {
					sum += VerticesPos[i + halfStep ][j];
					n++;
				}

				if (j >= halfStep + YIndex) {
					sum += VerticesPos[i ][j - halfStep];
					n++;
				}
				if (j + halfStep < endY) {
					sum += VerticesPos[i ][j + halfStep];
					n++;
				}
				VerticesPos[i][j] = sum / n + FMath::FRandRange(-halfStep, halfStep);
			}
		}
		step = halfStep;
	}


	for (int32 YIndex = 0; YIndex < HalfWidth * 2; YIndex++) {
		for (int32 XIndex = 0; XIndex < HalfWidth * 2; XIndex++) {
			int32 x = XIndex * CellSize;
			int32 y = YIndex * CellSize;
			VerticesPos[XIndex][YIndex] += Height(x, y);
		}
	}

}

void AMountains::CreateVertex(int index, int32 x, int32 y, int32 z, int XIndex, int YIndex) {
	FVector normal = SetNormal(x, y, XIndex, YIndex);
	FRuntimeMeshTangent tangent = FVector::CrossProduct(normal, FVector(0, 0, 1));

	FColor color(0, 0, 0);

	float Scale = CellSize / 50.0f;
	FVector2D UV0(XIndex * Scale, YIndex * Scale);

	for (int k = 0; k < NumberOfTerrains; ++k) {
		Vertices[k][index] = Vertex(FVector(x, y, z - 1), normal, tangent, color, UV0);

		if (z < HeightOfRocks) {
			Vertices[0][index] = Vertex(FVector(x, y, z), normal, tangent, color, UV0);
		}
		else {
			//UE_LOG(LogTemp, Warning, TEXT("NOT ROCKS"));
			Vertices[1][index] = Vertex(FVector(x, y, z), normal, tangent, color, UV0);
		}
	}
}

void AMountains::GenerateGeometry() {
	GenerateFromPlane();
}


int32 AMountains::Height(int32 x, int32 y) {
	int32 distanceToCenter = FMath::Sqrt(FMath::Pow(x, 2) + FMath::Pow(y, 2));
	int32 maxDistance = HalfWidth * CellSize;

	FVector2D q = FVector2D(HeightFactor * Noise.fractal(Octaves, x + RandFactorBaseHeight + 0, y + RandFactorBaseHeight + 0),
							HeightFactor * Noise.fractal(Octaves, x + RandFactorBaseHeight + 5200, y + RandFactorBaseHeight + 1300));

	FVector2D r = FVector2D(HeightFactor * Noise.fractal(Octaves, x + RandFactorBaseHeight + 4 * q.X + 1700, y + RandFactorBaseHeight + 4 * q.Y + 9200),
							HeightFactor * Noise.fractal(Octaves, x + RandFactorBaseHeight + 4 * q.X + 8300, y + RandFactorBaseHeight + 4 * q.Y + 2800));

	int32 height = HeightFactor * Noise.fractal(Octaves, x + RandFactorBaseHeight + 4 * q.X, y + RandFactorBaseHeight + 4 * q.Y);

	// height *= Gauss(distanceToCenter / HalfWidth, 1);
	return height;
}

float AMountains::Gauss(int32 x, float sigma, float average) {
	check(sigma > 0);
	return ((1 / (sigma * FMath::Sqrt(2 * 3.14159265))) * FMath::Exp(-FMath::Pow((x - average), 2)/(2. * FMath::Pow(sigma, 2))));
}
