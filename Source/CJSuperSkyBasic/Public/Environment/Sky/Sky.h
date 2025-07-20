// Created by Cássio J. M Lima (CJ's Studios), don't forget to rate this project, you can find me on www.github.com/cjmlima //

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SceneComponent.h"
#include "Curves/CurveLinearColor.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/VolumetricCloudComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Engine/Texture2D.h"
#include "Components/BillboardComponent.h"
#include "CJSuperSkyBasic.h"
#include "Sky.generated.h"

UCLASS(HideCategories = (Collision, Physics, LOD, Cooking, Actor, Input), 
		meta = (
			ClassIcon = "CJSuperSkyBasic.Sky",
			BlueprintIcon = "/CJSuperSkyBasic/Environment/Sky/UI/Icons/S_Sky.S_Sky"
		)
)
class CJSUPERSKYBASIC_API ASky : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASky();

	//==============================================================================================
	// CONTROLS - Main variables for adjusting Sky
	//==============================================================================================

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sky|Time Controls", meta = (UIMin = "0.0", UIMax = "24.0", ClampMin = "0.0", ClampMax = "24.0", DisplayPriority = "-1000000"))
	float TimeOfDay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sky|Time Controls", meta = (Units = "Minutes", DisplayPriority = "-1000000"))
	float DayCycleDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sky|Time Controls", meta = (UIMin = "0.0", UIMax = "1.0", ClampMin = "0.0", ClampMax = "1.0", DisplayPriority = "-1000000"))
	float DayDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sky|Time Controls", meta = (UIMin = "0.0", UIMax = "1.0", ClampMin = "0.0", ClampMax = "1.0", DisplayPriority = "-1000000"))
	float NightDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sky|Sun Controls", meta = (DisplayPriority = "-1000000"))
	UCurveLinearColor* SunLightColorCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sky|Moon Controls", meta = (DisplayPriority = "-1000000"))
	UCurveLinearColor* MoonLightColorCurve;

	//==============================================================================================
	// STATE - State variables (read only)
	//==============================================================================================

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sky|State", meta = (DisplayPriority = "-1000000"))
	float SunHeight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sky|State", meta = (DisplayPriority = "-1000000"))
	float MoonHeight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sky|State", meta = (DisplayPriority = "-1000000"))
	FRotator StarsRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sky|State", meta = (DisplayPriority = "-1000000"))
	FRotator SunRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sky|State", meta = (DisplayPriority = "-1000000"))
	FRotator MoonRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sky|State", meta = (DisplayPriority = "-1000000"))
	UMaterialInstanceDynamic* SunMaterialInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sky|State", meta = (DisplayPriority = "-1000000"))
	UMaterialInstanceDynamic* MoonMaterialInstance;

	//==============================================================================================
	// COMPONENTS - Actor Components
	//==============================================================================================

	UPROPERTY() //
		UBillboardComponent* SkyIcon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sky|Components")
	UStaticMeshComponent* SkyMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sky|Components")
	USkyAtmosphereComponent* SkyAtmosphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sky|Components")
	UExponentialHeightFogComponent* SkyFogComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sky|Components")
	USkyLightComponent* SkyLightComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sky|Components")
	UVolumetricCloudComponent* SkyCloudsComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sky|Components")
	UDirectionalLightComponent* SunLightComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sky|Components")
	UStaticMeshComponent* SunMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sky|Components")
	UDirectionalLightComponent* MoonLightComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sky|Components")
	UStaticMeshComponent* MoonMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Sky|Sky")
	void UpdateSky();

	UFUNCTION(BlueprintCallable, Category = "Sky|Settings")
	void UpdateSunSettings();

	UFUNCTION(BlueprintCallable, Category = "Sky|Settings")
	void UpdateMoonSettings();

	UFUNCTION(BlueprintCallable, Category = "Sky|Sun|Material")
	void UpdateCelestialBodyMaterial(UMaterialInstanceDynamic* DynamicMaterial, float Height);

	void UpdateTickRate();

	void DayNightCycle(float DeltaTime);

};
