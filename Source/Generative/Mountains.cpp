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

void AMountains::PopulateVertices() {
	RandFactor = FMath::Rand();

	for (int32 YIndex = 0; YIndex < HalfWidth * 2; YIndex++) {
		for (int32 XIndex = 0; XIndex < HalfWidth * 2; XIndex++) {
			int32 x = XIndex * CellSize;
			int32 y = YIndex * CellSize;
			VerticesPos[XIndex][YIndex] = Height(x, y);
		}
	}

	/*
	for (int32 YIndex = 0; YIndex < HalfWidth * 2 - DiamondSquareStep; YIndex += DiamondSquareStep)
	{
		for (int32 XIndex = 0; XIndex < HalfWidth * 2 - DiamondSquareStep; XIndex += DiamondSquareStep)
		{
			/*
			int32 endX = XIndex + DiamondSquareStep;
			int32 endY = YIndex + DiamondSquareStep;
			int32 x = XIndex * CellSize;
			int32 y = YIndex * CellSize;
			VerticesPos[XIndex][YIndex] = Height(x, y);
			x = (endX) * CellSize;
			y = YIndex * CellSize;
			VerticesPos[endX][YIndex] = Height(x, y);
			x = XIndex * CellSize;
			y = (endY) * CellSize;
			VerticesPos[XIndex][endY] = Height(x, y);
			x = (endX) * CellSize;
			y = (endY) * CellSize;
			VerticesPos[endX][endY] = Height(x, y);

			int step = DiamondSquareStep;
			while (step > 1) {
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


		}

	} */
	

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
	return /* Gauss(distanceToCenter / HalfWidth, 1) */ HeightFactor * Noise.fractal(Octaves, x + RandFactor, y + RandFactor);
}

float AMountains::Gauss(int32 x, float sigma, float average) {
	check(sigma > 0);
	return ((1 / (sigma * FMath::Sqrt(2 * 3.14159265))) * FMath::Exp(-FMath::Pow((x - average), 2)/(2. * FMath::Pow(sigma, 2))));
}
