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
	FIntPoint StartCell = { 0, 6 };
	GenerateRoadNetwork(StartCell.X, StartCell.Y);
	SpawnRoadTiles();
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

void ACityGenerator::GenerateRoadNetwork(int X, int Y)
{
	Grid[X][Y] = true;
	
	TArray<FIntPoint> Directions = { {1, 0}, {0, 1}, {0, -1}, {-1, 0 } };

	for (int i = 0; i < Directions.Num(); i++)
	{
		int j = FMath::RandRange(i, Directions.Num() - 1);
		Directions.Swap(i, j);
	}

	for (const FIntPoint& Dir : Directions)
	{
		int NewX = X + Dir.X;
		int NewY = Y + Dir.Y;

		// Check bounds and ensure we haven't already visited the cell
		if (NewX >= 0 && NewX < gridResolution && NewY >= 0 && NewY < gridResolution && !Grid[NewX][NewY])
		{
			GenerateRoadNetwork(NewX, NewY);
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
				SpawnRoadTile(Location);
			}
		}
	}
}

void ACityGenerator::SpawnRoadTile(const FVector& Location)
{
	FActorSpawnParameters spawnParameters;

	GetWorld()->SpawnActor<AActor>(RoadTileClass, Location, FRotator::ZeroRotator, spawnParameters);
}

