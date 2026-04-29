#pragma once

struct FStatRow
{
	FText StatName;
	FText StatValue;
	float NormalizedValue;
	
	FStatRow(FText InName, FText InValue, float InNormalized)
	: StatName(InName), StatValue(InValue), NormalizedValue(InNormalized) {}
};
