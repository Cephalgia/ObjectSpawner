#include "ObjectSpawnerActor.h"

#include "DreamGameMode.h"
#include "ObjectSpawnerManager.h"

AObjectSpawnerActor::AObjectSpawnerActor(const FObjectInitializer& ObjectInitializer) 
	:Super(ObjectInitializer) 
{
	SpawnVolume = CreateDefaultSubobject<UBoxComponent>("SpawnVolume");
}

void AObjectSpawnerActor::BeginPlay()
{
	Super::BeginPlay();

	if (ADreamGameMode * GameMode = Cast<ADreamGameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (UObjectSpawnerManager * ObjectSpawnerManager = GameMode->GetManager<UObjectSpawnerManager>())
		{
			ObjectSpawnerManager->RegisterSpawner(this);
		}
	}
}

void AObjectSpawnerActor::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	if (ADreamGameMode * GameMode = Cast<ADreamGameMode>(GetWorld()->GetAuthGameMode()))
	{
		if (UObjectSpawnerManager * ObjectSpawnerManager = GameMode->GetManager<UObjectSpawnerManager>())
		{
			ObjectSpawnerManager->UnRegisterSpawner(this);
		}
	}
	if (SpawnedObject)
	{
		SpawnedObject->Destroy();
	}
	Super::EndPlay(EndPlayReason);
}

bool AObjectSpawnerActor::CanSpawnObject(TSubclassOf<ASpawnableObjectBase> ObjectToSpawn) const
{
	if (SpawnedObject != nullptr)
	{
		return false;
	}

	if (ASpawnableObjectBase * DefaultObj = ObjectToSpawn.GetDefaultObject())
	{
		FVector Origin;
		FVector ObjectExtent;
		DefaultObj->GetActorBounds(true, Origin, ObjectExtent);

		FBoxSphereBounds SpawnerBounds = SpawnVolume->CalcLocalBounds();
		FVector SpawnerExtent = SpawnerBounds.GetBox().GetExtent();

		bool bFitsInBox = ObjectExtent.X <= SpawnerExtent.X && ObjectExtent.Y <= SpawnerExtent.Y && ObjectExtent.Z <= SpawnerExtent.Z;
		
		if (!bFitsInBox)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s DOES NOT FIT IN BOX %s"), *ObjectToSpawn->GetName(), *SpawnerExtent.ToString());
			return false;
		}

		//visibility check; which locations to check? one point per n units of width length and height later, center point for now
		if (ADreamGameMode::IsLocationVisible(this, { SpawnerBounds.Origin }))
		{
			return false;
		}

		return true;
	}

	return false;
}

ASpawnableObjectBase* AObjectSpawnerActor::SpawnObject(TSubclassOf<ASpawnableObjectBase> ObjectToSpawn)
{
	return Cast<ASpawnableObjectBase>(GetWorld()->SpawnActor(ObjectToSpawn.Get(), &SpawnVolume->GetComponentTransform()));
}