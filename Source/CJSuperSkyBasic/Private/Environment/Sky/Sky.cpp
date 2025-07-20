// Created by Cássio J. M Lima (CJ's Studios), don't forget to rate this project, you can find me on www.github.com/cjmlima //


#include "Environment/Sky/Sky.h"

// Sets default values
ASky::ASky()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DayDuration = 0.5f;
	DayCycleDuration = 1.0f; //In minutes
	TimeOfDay = 12.0f;

	USceneComponent* SkyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SkyRoot"));
	RootComponent = SkyRoot;

	// SkyAtmosphere
	SkyAtmosphereComponent = CreateDefaultSubobject<USkyAtmosphereComponent>(TEXT("SkyAtmosphere"));
	SkyAtmosphereComponent->SetupAttachment(RootComponent);

	// Skylight
	SkyLightComponent = CreateDefaultSubobject<USkyLightComponent>(TEXT("SkyLight"));
	SkyLightComponent->SetupAttachment(RootComponent);
	SkyLightComponent->bRealTimeCapture = true;

	// Height Fog
	SkyFogComponent = CreateDefaultSubobject<UExponentialHeightFogComponent>(TEXT("SkyFog"));
	SkyFogComponent->SetupAttachment(RootComponent);

	// Clouds
	SkyCloudsComponent = CreateDefaultSubobject<UVolumetricCloudComponent>(TEXT("SkyClouds"));
	SkyCloudsComponent->SetupAttachment(RootComponent);

	// Sky Sphere
	SkyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkyMesh"));
	SkyMeshComponent->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SkyDomeMeshAsset(TEXT("StaticMesh'/CJSuperSkyBasic/Environment/Sky/StaticMeshes/SM_Sky.SM_Sky'"));
	if (SkyDomeMeshAsset.Succeeded())
	{
		SkyMeshComponent->SetStaticMesh(SkyDomeMeshAsset.Object);
	}
	SkyMeshComponent->SetRelativeScale3D(FVector(100000000.0f));
	StarsRotation = FRotator(-90.0f, 180.0f, 180.0f);
	SkyMeshComponent->SetWorldRotation(StarsRotation);
	SkyMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SkyMeshComponent->SetCastShadow(false);

	// Sun
	SunLightComponent = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("SunLight"));
	SunLightComponent->SetupAttachment(RootComponent);
	SunLightComponent->bCastCloudShadows = true;
	SunLightComponent->bCastShadowsOnClouds = true;

	static ConstructorHelpers::FObjectFinder<UCurveLinearColor> SunLightColorCurveAsset(TEXT("CurveLinearColor'/CJSuperSkyBasic/Environment/Sky/Curves/C_SunLightColorCurve.C_SunLightColorCurve'"));
	if (SunLightColorCurveAsset.Succeeded())
	{
		SunLightColorCurve = SunLightColorCurveAsset.Object;
	}

	SunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SunMesh"));
	SunMesh->SetupAttachment(SunLightComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMeshAsset(TEXT("StaticMesh'/CJSuperSkyBasic/Environment/Sky/StaticMeshes/SM_Sphere.SM_Sphere'"));
	if (SphereMeshAsset.Succeeded())
	{
		SunMesh->SetStaticMesh(SphereMeshAsset.Object);
	}
	SunMesh->SetCastShadow(false);
	SunMesh->SetRelativeScale3D(FVector(3000.0f));
	SunMesh->SetRelativeLocation(FVector(-10000000.0f, 0.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> SunMaterialAsset(TEXT("Material'/CJSuperSkyBasic/Environment/Sky/Materials/MI_Sun.MI_Sun'"));
	if (SunMaterialAsset.Succeeded())
	{
		SunMesh->SetMaterial(0, SunMaterialAsset.Object);
	}

	// Moon
	MoonLightComponent = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("MoonLight"));
	MoonLightComponent->SetupAttachment(RootComponent);
	MoonLightComponent->bCastCloudShadows = true;
	MoonLightComponent->bCastShadowsOnClouds = true;

	static ConstructorHelpers::FObjectFinder<UCurveLinearColor> MoonLightColorCurveAsset(TEXT("CurveLinearColor'/CJSuperSkyBasic/Environment/Sky/Curves/C_MoonLightColorCurve.C_MoonLightColorCurve'"));
	if (MoonLightColorCurveAsset.Succeeded())
	{
		MoonLightColorCurve = MoonLightColorCurveAsset.Object;
	}

	MoonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MoonMesh"));
	MoonMesh->SetupAttachment(MoonLightComponent);
	if (SphereMeshAsset.Succeeded())
	{
		MoonMesh->SetStaticMesh(SphereMeshAsset.Object);
	}
	MoonMesh->SetCastShadow(false);
	MoonMesh->SetRelativeScale3D(FVector(5000.0f));
	MoonMesh->SetRelativeLocation(FVector(-10000000.0f, 0.0f, 0.0f));
	MoonMesh->SetRelativeRotation(FRotator(0,-90,0));

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MoonMaterialAsset(TEXT("Material'/CJSuperSkyBasic/Environment/Sky/Materials/MI_Moon.MI_Moon'"));
	if (MoonMaterialAsset.Succeeded())
	{
		MoonMesh->SetMaterial(0, MoonMaterialAsset.Object);
	}

	SkyIcon = CreateDefaultSubobject<UBillboardComponent>(TEXT("SkyIcon"));
	SkyIcon->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconTexture(TEXT("Texture2D'/CJSuperSkyBasic/Environment/Sky/UI/Icons/S_Sky.S_Sky'"));
	if (IconTexture.Succeeded())
	{
		SkyIcon->SetSprite(IconTexture.Object);
		SkyIcon->bIsScreenSizeScaled = true;
		SkyIcon->SetRelativeScale3D(FVector(1.0f));
	}
	
	DayNightCycle(0);
	UpdateSky();
}

// Called when the game starts or when spawned
void ASky::BeginPlay()
{
	Super::BeginPlay();
	
}

// ASky.cpp

void ASky::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SunLightComponent->ForwardShadingPriority = 1;
	MoonLightComponent->ForwardShadingPriority = 0;

	if (SunMesh && SunMesh->GetMaterial(0))
	{
		if (SunMaterialInstance == nullptr)
		{
			SunMaterialInstance = UMaterialInstanceDynamic::Create(SunMesh->GetMaterial(0), this);
			SunMesh->SetMaterial(0, SunMaterialInstance);
		}
	}

	if (MoonMesh && MoonMesh->GetMaterial(0))
	{
		if (MoonMaterialInstance == nullptr)
		{
			MoonMaterialInstance = UMaterialInstanceDynamic::Create(MoonMesh->GetMaterial(0), this);
			MoonMesh->SetMaterial(0, MoonMaterialInstance);
		}
	}

	DayNightCycle(0);
	UpdateSky();
}

// Called every frame
void ASky::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DayNightCycle(DeltaTime);
	UpdateSky();

}

void ASky::UpdateSky()
{
	if (TimeOfDay >= 24) {
		TimeOfDay = 0;
	}
	else if (TimeOfDay < 0) {
		TimeOfDay = 24;
	}
	UpdateSunSettings();
	UpdateMoonSettings();
	SkyMeshComponent->SetWorldRotation(StarsRotation);
}

void ASky::UpdateSunSettings()
{
	SunLightComponent->SetWorldRotation(SunRotation);
	FVector SunDirection = SunLightComponent->GetForwardVector();
	SunHeight = FVector::DotProduct(SunDirection, FVector::DownVector);
	//SunMesh->SetRelativeScale3D(SunOriginalSize * FMath::Lerp(2.0f, 1.0f, FMath::Abs(SunHeight)));
	UpdateCelestialBodyMaterial(SunMaterialInstance, SunHeight);

	FLinearColor SunLightColor = FLinearColor::White;

	if (SunLightColorCurve)
	{
		SunLightColor = SunLightColorCurve->GetLinearColorValue(SunHeight);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SunLightColorCurve is null. Defaulting to white light."));
	}
	SunLightComponent->SetLightColor(SunLightColor);
	SunLightComponent->SetIntensity(FMath::Lerp(0.0f, 10.0f, FMath::Clamp(SunHeight + 0.1, 0.0f, 1.0f)));
}
void ASky::UpdateMoonSettings()
{
	MoonLightComponent->SetWorldRotation(MoonRotation);
	FVector MoonDirection = MoonLightComponent->GetForwardVector();
	MoonHeight = FVector::DotProduct(MoonDirection, FVector::DownVector);
	//MoonMesh->SetRelativeScale3D(MoonOriginalSize * FMath::Lerp(2.0f, 1.0f, FMath::Abs(MoonHeight)));
	UpdateCelestialBodyMaterial(MoonMaterialInstance, MoonHeight);

	FLinearColor MoonLightColor = FLinearColor::White;

	if (MoonLightColorCurve)
	{
		MoonLightColor = MoonLightColorCurve->GetLinearColorValue(MoonHeight);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MoonLightColorCurve is null. Defaulting to white light."));
	}
	MoonLightComponent->SetLightColor(MoonLightColor);
	MoonLightComponent->SetIntensity(FMath::Lerp(0.0f, 1.5f, FMath::Clamp(MoonHeight + 0.1, 0.0f, 1.0f)));
}


void ASky::UpdateCelestialBodyMaterial(UMaterialInstanceDynamic* DynamicMaterial, float Height)
{
	if (DynamicMaterial)
	{
		DynamicMaterial->SetScalarParameterValue(TEXT("BodyHeight"), Height + 0.1);
	}
}

void ASky::UpdateTickRate() {
	//float NewRate = (DayCycleDuration / 48) / 100;
	//if (PrimaryActorTick.TickInterval != NewRate) {
		//PrimaryActorTick.TickInterval = NewRate;
	//}

	PrimaryActorTick.TickInterval = 0.1f;
}

void ASky::DayNightCycle(float DeltaTime) {

	float SunRiseTime;
	float SunSetTime;

	//Calculate Sunrise and Sunset Times

	SunRiseTime = 12.0f - (DayDuration) * 12.0f;
	SunSetTime = 12.0f + (DayDuration) * 12.0f;

	TimeOfDay += DeltaTime * (24.0f /(DayCycleDuration*60.0f));
	TimeOfDay = FMath::Fmod(TimeOfDay, 24.0f);

	//Calculate Sun Rotation
	if (TimeOfDay > SunRiseTime && TimeOfDay < SunSetTime) { // Daytime
		SunRotation.Pitch = FMath::GetMappedRangeValueClamped(
			FVector2D(SunRiseTime, SunSetTime),
			FVector2D(0.0f, -180.0f),
			TimeOfDay
		);
		MoonRotation.Pitch = SunRotation.Pitch + 180;
	}
	else { // NightTime
		SunRotation.Pitch = FMath::GetMappedRangeValueClamped(
			FVector2D(SunSetTime, SunRiseTime + 24.0f),
			FVector2D(-180.0f, -360.0f),
			TimeOfDay<SunRiseTime? TimeOfDay + 24.0f : TimeOfDay
		);
		MoonRotation.Pitch = SunRotation.Pitch + 180;
	}

	StarsRotation.Pitch = (TimeOfDay * 15) - 90;
}