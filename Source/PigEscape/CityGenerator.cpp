// Fill out your copyright notice in the Description page of Project Settings.


#include "CityGenerator.h"

// Sets default values
ACityGenerator::ACityGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACityGenerator::BeginPlay()
{
	Super::BeginPlay();
	InitializeGrid();
	FIntPoint BottomStartCell = { 0, gridResolution / 2 + 1 };
	FIntPoint TopStartCell = { gridResolution - 1, FMath::RandRange(0, gridResolution - 1)};
	FIntPoint RightStartCell = { FMath::RandRange(0, gridResolution - 1), gridResolution - 1 };
	FIntPoint LeftStartCell = { FMath::RandRange(0, gridResolution - 1), 0 };

	GenerateRoadNetwork(BottomStartCell.X, BottomStartCell.Y, EDirection::Up);
	GenerateRoadNetwork(RightStartCell.X, RightStartCell.Y, EDirection::Left);
	GenerateRoadNetwork(LeftStartCell.X, LeftStartCell.Y, EDirection::Right);
	//SpawnRoadTiles();
}

// Called every frame
void ACityGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACityGenerator::InitializeGrid()
{
	Grid.SetNum(gridResolution);
	for (int i = 0; i < gridResolution; i++)
	{
		Grid[i].SetNum(gridResolution);

		for (int j = 0; j < gridResolution; j++)
		{
			Grid[i][j] = false;
		}
	}
}

void ACityGenerator::GenerateRoadNetwork(const int X, const int Y, EDirection PreviousDirection)
{
	Grid[X][Y] = true;

	FVector Location = FVector(X * cellSize, Y * cellSize, 100);

	SpawnRoadTile(Location, PreviousDirection);

	TArray<FIntPoint> PossibleDirections;


	switch(PreviousDirection)
	{
		case EDirection::Up :
			PossibleDirections = {  {1,0}, {0,1}, {0, -1} };
			break;

		case EDirection::Right :
			PossibleDirections = {  {1,0}, {0,1} };
			break;
		
		case EDirection::Left :
			PossibleDirections = {  {1,0}, {0, -1} };
			break;
	}

	FIntPoint PickedDirection = PossibleDirections[FMath::RandRange(0, PossibleDirections.Num() - 1)];

	int NextX = X + PickedDirection.X;
	int NextY = Y + PickedDirection.Y;

	EDirection NextPreviousDirection = Directions[PickedDirection];

	if (NextX >= 0 && NextX < gridResolution && NextY >= 0 && NextY < gridResolution && !Grid[NextX][NextY])
	{
		GenerateRoadNetwork(NextX, NextY, NextPreviousDirection);
	}

	float secondPathChance = 0.8f;
	if (FMath::Rand() <= secondPathChance)
	{
		FIntPoint AlternatePickedDirection = PossibleDirections[FMath::RandRange(0, PossibleDirections.Num() - 1)];

		if (AlternatePickedDirection != PickedDirection)
		{
			NextX = X + AlternatePickedDirection.X;
			NextY = Y + AlternatePickedDirection.Y;
			NextPreviousDirection = Directions[AlternatePickedDirection];

			if (NextX >= 0 && NextX < gridResolution && NextY >= 0 && NextY < gridResolution && !Grid[NextX][NextY])
				GenerateRoadNetwork(NextX, NextY, NextPreviousDirection);
		}

	}
	
}

void ACityGenerator::SpawnRoadTiles()
{
	for (int i = 0; i < gridResolution; i++)
	{
		for (int j = 0; j < gridResolution; j++)
		{
			if (Grid[i][j])
			{
				FVector Location = FVector(i * cellSize, j * cellSize, 100);
				//SpawnRoadTile(Location);
			}
		}
	}
}

void ACityGenerator::SpawnRoadTile(const FVector& Location, const EDirection Direction)
{
	FActorSpawnParameters spawnParameters;

	FRotator SpawnRotation;

	switch (Direction)
	{
	case EDirection::Left :
		SpawnRotation = FRotator(0, 90, 0);
		break;
	case EDirection::Right :
		SpawnRotation = FRotator(0, -90, 0);
		break;
	default: 
		SpawnRotation = FRotator::ZeroRotator;
		break;
	}

	GetWorld()->SpawnActor<AActor>(RoadTileClass, Location, SpawnRotation, spawnParameters);
}

