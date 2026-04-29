#pragma once
#include "AssetTypeActions_Base.h"

class FNarrativeGraphAssetTypeActions : public FAssetTypeActions_Base
{
public:
	virtual FText GetName() const override
	{
		return INVTEXT("Narrative Graph");
	}
	
	virtual FColor GetTypeColor() const override
	{
		return FColor(180, 50,50);
	}
	
	virtual UClass* GetSupportedClass() const override;
	
	virtual uint32 GetCategories() override
	{
		return EAssetTypeCategories::Misc;
	}
	
	virtual void OpenAssetEditor(const TArray<UObject*>& InObjects, 
		TSharedPtr<IToolkitHost> EditWithinLevelEditor) override;
};
