// Copyright 2016-2019 Chris Conway (Koderz). All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RuntimeMeshComponent.h"
#include "RuntimeMeshActor.generated.h"

class URuntimeMeshComponent;

UCLASS(HideCategories = (Input), ShowCategories = ("Input|MouseInput", "Input|TouchInput"), ComponentWrapperClass, Meta = (ChildCanTick))
class RUNTIMEMESHCOMPONENT_API ARuntimeMeshActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleDefaultsOnly, Meta = (ExposeFunctionCategories = "Mesh,Rendering,Physics,Components|RuntimeMesh", AllowPrivateAccess = "true"), Category = "RuntimeMeshActor")
	URuntimeMeshComponent* RuntimeMeshComponent = nullptr;

	// This is purposefully not a UPROPERTY as we want it to not store across sessions
	bool bHasGeneratedThisRun = false;

public:
	ARuntimeMeshActor();

	virtual void OnConstruction(const FTransform& Transform) override;

	/** Function to change mobility type */
	void SetMobility(EComponentMobility::Type InMobility);
	EComponentMobility::Type GetMobility();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Meta = (DisplayName = "Generate Mesh"), Category = "RuntimeMeshActor")
	void K2_GenerateMesh();

	UFUNCTION(BlueprintCallable, Meta = (DisplayName = "Get Mobility"), Category = "RuntimeMeshActor")
	ERuntimeMeshMobility GetRuntimeMeshMobility();

	UFUNCTION(BlueprintCallable, Meta = (DisplayName = "Set Mobility"), Category = "RuntimeMeshActor")
	void SetRuntimeMeshMobility(ERuntimeMeshMobility NewMobility);

	/** Returns RuntimeMeshComponent subobject **/
	UFUNCTION(BlueprintCallable, Meta = (DisplayName = "Get Runtime Mesh Component"), Category = "RuntimeMeshActor")
	URuntimeMeshComponent* GetRuntimeMeshComponent() const { return RuntimeMeshComponent; }

protected:
	virtual void BeginPlay() override;

	virtual void GenerateMesh();
};
