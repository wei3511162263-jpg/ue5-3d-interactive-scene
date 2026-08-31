#include "GISPointMarker.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "GISDigitalTwinSubsystem.h"

AGISPointMarker::AGISPointMarker()
{
	PrimaryActorTick.bCanEverTick = false;

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->InitSphereRadius(50.0f);
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collision->SetCollisionResponseToAllChannels(ECR_Ignore);
	Collision->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	RootComponent = Collision;

	Label = CreateDefaultSubobject<UTextRenderComponent>(TEXT("Label"));
	Label->SetupAttachment(Collision);
	Label->SetRelativeLocation(FVector(0.0f, 0.0f, 140.0f));
	Label->SetHorizontalAlignment(EHTA_Center);
	Label->SetWorldSize(80.0f);

	MarkerId = NAME_None;
	DisplayName = FText::FromString(TEXT("POI"));
	Category = EGISAssetCategory::Landmark;
	MarkerColor = FLinearColor(0.0f, 0.65f, 1.0f, 1.0f);
}

void AGISPointMarker::BeginPlay()
{
	Super::BeginPlay();

	ApplyGeoReference();
	UpdateVisualState(false);

	if (UGISDigitalTwinSubsystem* Subsystem = GetWorld()->GetSubsystem<UGISDigitalTwinSubsystem>())
	{
		Subsystem->RegisterPointMarker(this);
	}
}

void AGISPointMarker::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UGISDigitalTwinSubsystem* Subsystem = GetWorld()->GetSubsystem<UGISDigitalTwinSubsystem>())
	{
		Subsystem->UnregisterPointMarker(this);
	}

	Super::EndPlay(EndPlayReason);
}

void AGISPointMarker::ApplyGeoReference()
{
	if (UGISDigitalTwinSubsystem* Subsystem = GetWorld()->GetSubsystem<UGISDigitalTwinSubsystem>())
	{
		SetActorLocation(Subsystem->GeoToWorld(GeoCoordinate));
	}
}

void AGISPointMarker::UpdateVisualState(bool bSelected)
{
	const float Radius = bSelected ? 80.0f : 50.0f;
	Collision->SetSphereRadius(Radius);

	if (Label)
	{
		Label->SetText(DisplayName);
		Label->SetTextRenderColor(MarkerColor.ToFColor(true));
	}
}
