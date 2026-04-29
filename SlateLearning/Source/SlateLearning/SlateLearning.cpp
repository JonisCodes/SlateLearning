// Copyright Epic Games, Inc. All Rights Reserved.

#include "SlateLearning.h"
#include "Modules/ModuleManager.h"
#include "FSlatePracticeStyle.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FSlateLearningModule, SlateLearning, "SlateLearning");

void FSlateLearningModule::StartupModule()
{
	FSlatePracticeStyle::Initialize();
}

void FSlateLearningModule::ShutdownModule()
{
	FSlatePracticeStyle::Shutdown();
}