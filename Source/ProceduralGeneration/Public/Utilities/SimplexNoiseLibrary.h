#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SimplexNoiseLibrary.generated.h"

UCLASS()
class PROCEDURALGENERATION_API USimplexNoiseLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	static unsigned char perm[];
	static double  _grad(int hash, double x);
	static double  _grad(int hash, double x, double y);
	static double  _grad(int hash, double x, double y, double z);
	static double  _grad(int hash, double x, double y, double z, double t);

	static double _simplexNoise1D(double x);
	static double _simplexNoise2D(double x, double y);
	static double _simplexNoise3D(double x, double y, double z);
	static double _simplexNoise4D(double x, double y, double z, double w);

public:
	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
	static void setNoiseSeed(const int32& newSeed);

	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
	static float SimplexNoise1D(float x, float inFactor = 1.f);

	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
	static float SimplexNoise2D(float x, float y, float inFactor = 1.f);

	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
	static float SimplexNoise3D(float x, float y, float z, float inFactor = 1.f);

	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
	static float SimplexNoise4D(float x, float y, float z, float w, float inFactor = 1.f);

	/** Scaled by float value */
	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
	static float SimplexNoiseScaled1D(float x, float scaleOut = 1.f, float inFactor = 1.f);

	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
	static float SimplexNoiseScaled2D(float x, float y, float scaleOut = 1.f, float inFactor = 1.f);

	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
	static float SimplexNoiseScaled3D(float x, float y, float z, float scaleOut = 1.f, float inFactor = 1.f);

	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
	static float SimplexNoiseScaled4D(float x, float y, float z, float w, float scaleOut = 1.f, float inFactor = 1.f);

	/**
	* Return value in Range between two float numbers
	* Return Value is scaled by difference between rangeMin & rangeMax value
	*/
	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
	static float SimplexNoiseInRange1D(float x, float rangeMin, float rangeMax, float inFactor = 1.f);

	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
	static float SimplexNoiseInRange2D(float x, float y, float rangeMin, float rangeMax, float inFactor = 1.f);

	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
	static float SimplexNoiseInRange3D(float x, float y, float z, float rangeMin, float rangeMax, float inFactor = 1.f);

	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
	static float SimplexNoiseInRange4D(float x, float y, float z, float w, float rangeMin, float rangeMax, float inFactor = 1.f);
};
