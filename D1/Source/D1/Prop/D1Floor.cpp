#include "Prop/D1Floor.h"

// Sets default values
AD1Floor::AD1Floor()
{
	const int32 Rows = 10; 
	const int32 Columns = 10
	const float FloorSpacing = 300.f;

	// 메쉬 배열 초기화
	for (int32 i = 0; i < Rows * Columns; ++i)
	{
		TObjectPtr<UStaticMeshComponent> NewMesh = CreateDefaultSubobject<UStaticMeshComponent>(*FString::Printf(TEXT("Mesh_%d"), i));

		if (i == 0)
		{
			NewMesh->SetupAttachment(GetRootComponent()); 
		}
		else
		{
			NewMesh->SetupAttachment(Meshes[0]); 
		}

		static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRef(
			TEXT("/Script/Engine.StaticMesh'/Game/RestaurantScene/Meshes/SM_Floor.SM_Floor'"));

		if (MeshRef.Succeeded())
		{
			NewMesh->SetStaticMesh(MeshRef.Object);
		}

		int32 Row = i / Columns;  
		int32 Col = i % Columns; 
		FVector Location(Row * FloorSpacing, Col * FloorSpacing, 0.f); // X, Y축으로 배치

		NewMesh->SetRelativeLocation(Location);

		Meshes.Add(NewMesh);
	}
}
