// Fill out your copyright notice in the Description page of Project Settings.


#include "NarrativeGraphFactory.h"

#include "NarrativeGraph.h"

UNarrativeGraphFactory::UNarrativeGraphFactory()
{
	// Tell Unreal what class this factory creates
	SupportedClass = UNarrativeGraph::StaticClass();
	
	// This factory creates new objects from scratch, not from importing files
	bCreateNew = true;
	
	// Open the asset editor immediately after creation
	bEditAfterNew = true;
}

UObject* UNarrativeGraphFactory::FactoryCreateNew(UClass* InClass, 
	UObject* InParent, FName InName, EObjectFlags Flags, 
	UObject* Context, FFeedbackContext* Warn)
{
	UNarrativeGraph* NewGraph = NewObject<UNarrativeGraph>(
		InParent, InClass, InName, Flags);
	
	return NewGraph;
}
