// Fill out your copyright notice in the Description page of Project Settings.


#include "SStatInspectorPanel.h"

#include "Selection.h"
#include "SlateOptMacros.h"
#include "MySlateCharacter.h"
#include "SStatList.h"
#include "FSlatePracticeStyle.h"
#include "Widgets/Layout/SWidgetSwitcher.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SStatInspectorPanel::Construct(const FArguments& InArgs)
{
	SelectionChangedHandle = USelection::SelectionChangedEvent.AddSP(
		this, &SStatInspectorPanel::OnEditorSelectionChanged);

	ChildSlot
	[
		SNew(SVerticalBox)

		// Toolbar
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			BuildToolbar()
		]

		// Divider
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SSeparator)
			.Orientation(Orient_Horizontal)
		]

		// Main content
		+ SVerticalBox::Slot()
		.FillHeight(1.f)
		.Padding(4.f)
		[
			SNew(SSplitter)
			.Orientation(Orient_Vertical)

			+ SSplitter::Slot()
			.Value(0.4f)
			[
				SAssignNew(ContentSwitcher, SWidgetSwitcher)

				+ SWidgetSwitcher::Slot()
				[
					BuildEmptyState()
				]

				+ SWidgetSwitcher::Slot()
				[
					SAssignNew(StatList, SStatList)
					.StatRows(this, &SStatInspectorPanel::GetStatRows)
				]
			]

			+ SSplitter::Slot()
			.Value(0.6f)
			[
				BuildDetailsView()
			]
		]

		// Status bar
		+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(4.f, 2.f)
		[
			SNew(STextBlock)
			.Text(INVTEXT("No character selected"))
			.TextStyle(FSlatePracticeStyle::Get(), "SlatePractice.Text.Default")
		]
	];
}

SStatInspectorPanel::~SStatInspectorPanel()
{
	USelection::SelectionChangedEvent.Remove(SelectionChangedHandle);
}

TSharedRef<SBox> SStatInspectorPanel::BuildToolbar()
{
	return SNew(SBox)
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.AutoWidth()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(SButton)
				.Text(INVTEXT("Refresh"))
			]
		];
}

TSharedRef<SBox> SStatInspectorPanel::BuildEmptyState()
{
	return SNew(SBox)
		[
			SNew(STextBlock)
			.Text(INVTEXT("Select a character first"))
		];
}

void SStatInspectorPanel::OnEditorSelectionChanged(UObject* NewSelection)
{
	AMySlateCharacter* FoundCharacter = nullptr;

	const auto Selection = GEditor->GetSelectedActors();
	for (FSelectionIterator It(*Selection); It; ++It)
	{
		if (const auto AsCharacter = Cast<AMySlateCharacter>(*It))
		{
			FoundCharacter = AsCharacter;
			break;
		}
	}

	SelectedCharacter = FoundCharacter;
	RefreshPanel();

	UE_LOG(LogTemp, Warning, TEXT("Selected character: %s"),
	       SelectedCharacter.IsValid() ? *SelectedCharacter->GetName() : TEXT("None"));
}

TSharedRef<SWidget> SStatInspectorPanel::BuildDetailsView()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	FDetailsViewArgs Args;
	Args.bHideSelectionTip = true;
	Args.bAllowSearch = true;
	Args.bShowOptions = false;
	Args.bUpdatesFromSelection = false;

	DetailsView = PropertyModule.CreateDetailView(Args);
	return DetailsView.ToSharedRef();
}

TArray<TSharedPtr<FStatRow>> SStatInspectorPanel::GetStatRows() const
{
	if (!SelectedCharacter.IsValid()) return {};

	return SelectedCharacter->GetStatRows();
}

void SStatInspectorPanel::RefreshPanel()
{
	const auto bHasCharacter = SelectedCharacter.IsValid();

	ContentSwitcher->SetActiveWidgetIndex(bHasCharacter ? 1 : 0);
	
	if (DetailsView.IsValid())
	{
		DetailsView->SetObject(bHasCharacter ? SelectedCharacter.Get() : nullptr);
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
