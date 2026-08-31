#include "GISCityAssetAnchor.h"
#include "Components/BillboardComponent.h"
#include "Components/TextRenderComponent.h"
#include "GISDigitalTwinSubsystem.h"

AGISCityAssetAnchor::AGISCityAssetAnchor()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	Icon = CreateDefaultSubobject<UBillboardComponent>(TEXT("Icon"));
	Icon->SetupAttachment(SceneRoot);

	Label = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Label"));
	Label->SetupAttachment(SceneRoot);
	Label->SetHorizontalAlignment(EHTA_Center);
	Label->SetWorldSize(120.0f);

	AssetDisplayName = FText::FromString(TEXT("Manama Bahrain 3D City Map Model"));
	SourceAssetName = TEXT("Manama Bahrain 3D City Map Model");
	SourceAssetUrl = TEXT("https://www.fab.com/listings/5510a570-4a07-481a-879e-ef3f49892221");
	AnchorCoordinate.Longitude = 50.5860;
	AnchorCoordinate.Latitude = 26.2285;
	AnchorCoordinate.ElevationMeters = 0.0;
	ApproximateCoverageMeters = FVector2D(3000.0f, 3000.0f);
	bEnableHLOD = true;
	bUseNaniteForStaticMeshes = true;
	ExpectedBuildingCount = 0;
}

void AGISCityAssetAnchor::BeginPlay()
{
	Super::BeginPlay();

	ApplyGeoReference();
	UpdateLabel();

	if (UGISDigitalTwinSubsystem* Subsystem = GetWorld()->GetSubsystem<UGISDigitalTwinSubsystem>())
	{
		Subsystem->RegisterCityAsset(this);
	}
}

void AGISCityAssetAnchor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UGISDigitalTwinSubsystem* Subsystem = GetWorld()->GetSubsystem<UGISDigitalTwinSubsystem>())
	{
		Subsystem->UnregisterCityAsset(this);
	}

	Super::EndPlay(EndPlayReason);
}

void AGISCityAssetAnchor::ApplyGeoReference()
{
	if (UGISDigitalTwinSubsystem* Subsystem = GetWorld()->GetSubsystem<UGISDigitalTwinSubsystem>())
	{
		SetActorLocation(Subsystem->GeoToWorld(AnchorCoordinate));
	}
}

void AGISCityAssetAnchor::UpdateLabel()
{
	if (Label)
	{
		Label->SetText(AssetDisplayName);
	}
}
