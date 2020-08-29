#pragma once
#include "UObject/Object.h"
#include "ObjectSpawnerConfig.generated.h"

UCLASS(config = Game, defaultconfig)
class OBJECTSPAWNER_API UObjectSpawnerConfig : public UObject
{
	GENERATED_BODY()

public:
	UObjectSpawnerConfig(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer) {};
};
