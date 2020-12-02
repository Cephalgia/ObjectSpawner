#pragma once
#include "GameFramework/Actor.h"
#include "SpawnableObjectBase.generated.h"

UCLASS(config = Game, defaultconfig)
class OBJECTSPAWNER_API ASpawnableObjectBase : public AActor  //maybe this will have to be an interface
{
	GENERATED_BODY()

public:
	ASpawnableObjectBase(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer) {};
};
