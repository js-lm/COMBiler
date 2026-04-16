#pragma once

#include <vector>

#include <raylib.h>

#include "action_center/action_center.hpp"
#include "program_states/context.hpp"

#include "preview_texture.hpp"

class TimelineManager{
public:
	enum class InteractionMode{
		None,
		DraggingPage,
		SelectingPages,
		WaitingForPasteTarget,
	};

	struct PagePreviewCacheSlot{
		int pageIndex{0};
		PreviewTexture pagePreviewTexture{};
	};

	struct DragAndDropState{
		bool isDraggingPage{false};
		int draggedPageIndex{0};
		int proposedDropInsertionIndex{0};
		Vector2 draggedPreviewTopLeftScreenPosition{0, 0};
	};

	struct SelectionAndPasteState{
		bool isSelectionModeEnabled{false};
		bool isPasteModeEnabled{false};

		int selectedPageStartIndex{0};
		int selectedPageCount{0};
	};

	struct ViewState{
		Rectangle scrollPanelBounds{0, 0, 0, 0};
		Rectangle scrollPanelContentBounds{0, 0, 0, 0};

		Vector2 scrollOffsetInPixels{0, 0};

		bool shouldShowHorizontalScrollBar{false};
	};

private:
	program_states::InterfaceContext context_;

	InteractionMode interactionMode_{InteractionMode::None};
	DragAndDropState dragAndDropState_{};
	SelectionAndPasteState selectionAndPasteState_{};
	ViewState viewState_{};

	std::vector<PagePreviewCacheSlot> pagePreviewCache_{};

public:
	TimelineManager(program_states::InterfaceContext context)
		: context_{context}
	{}

	TimelineManager() = delete;
	~TimelineManager() = default;

public:
	void update(ActionCenter &actionCenter);
	void cancelActiveInteraction();

public:
	const ViewState &viewState() const{return viewState_;}
	const DragAndDropState &dragAndDropState() const{return dragAndDropState_;}
	const SelectionAndPasteState &selectionAndPasteState() const{return selectionAndPasteState_;}
	const std::vector<PagePreviewCacheSlot> &pagePreviewCache() const{return pagePreviewCache_;}

private:
	void updateInteractionMode();
	void handleSelectionModeInput(ActionCenter &actionCenter);
	void handleDragAndDropInput(ActionCenter &actionCenter);
	void handlePasteInput(ActionCenter &actionCenter);

private:
	void synchronizePreviewCacheWithProjectPages();
	void invalidateCacheForModifiedPages();
	void redrawInvalidatedPagePreviews();
};