#include "GISMeasurementTool.h"

AGISMeasurementTool::AGISMeasurementTool()
{
	PrimaryActorTick.bCanEverTick = false;
	UnrealUnitsPerMeter = 100.0f;
	bHasMeasurement = false;
	StartLocation = FVector::ZeroVector;
	EndLocation = FVector::ZeroVector;
}

void AGISMeasurementTool::BeginMeasurement(const FVector& InStartLocation)
{
	StartLocation = InStartLocation;
	EndLocation = InStartLocation;
	bHasMeasurement = false;
}

float AGISMeasurementTool::FinishMeasurement(const FVector& InEndLocation)
{
	EndLocation = InEndLocation;
	bHasMeasurement = true;
	return GetDistanceMeters();
}

float AGISMeasurementTool::GetDistanceMeters() const
{
	return FVector::Distance(StartLocation, EndLocation) / UnrealUnitsPerMeter;
}

float AGISMeasurementTool::GetHorizontalDistanceMeters() const
{
	const FVector2D Start2D(StartLocation.X, StartLocation.Y);
	const FVector2D End2D(EndLocation.X, EndLocation.Y);
	return FVector2D::Distance(Start2D, End2D) / UnrealUnitsPerMeter;
}
