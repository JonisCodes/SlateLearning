// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "NarrativeGraphFactory.generated.h"

/**
 * 
 */
UCLASS()
class SLATELEARNINGEDITOR_API UNarrativeGraphFactory : public UFactory
{
	GENERATED_BODY()
	
public:
	UNarrativeGraphFactory();
	
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, 
		FName InName, EObjectFlags Flags, UObject* Context, 
		FFeedbackContext* Warn) override;
	
	virtual bool ShouldShowInNewMenu() const override { return true;}
};
