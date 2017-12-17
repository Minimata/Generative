// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class ASimplexNoise
{
public:
	ASimplexNoise();
	ASimplexNoise(float frequency,
		float amplitude,
		float lacunarity,
		float persistence);

	static float noise(float x, float y);
	float fractal(size_t octaves, float x, float y) const;

protected:
	float mFrequency;   ///< Frequency ("width") of the first octave of noise (default to 1.0)
	float mAmplitude;   ///< Amplitude ("height") of the first octave of noise (default to 1.0)
	float mLacunarity;  ///< Lacunarity specifies the frequency multiplier between successive octaves (default to 2.0).
	float mPersistence; ///< Persistence is the loss of amplitude between successive octaves (usually 1/lacunarity)

};
