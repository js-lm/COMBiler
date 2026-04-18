#include "preview_texture.hpp"

PreviewTexture::~PreviewTexture(){
	unload();
}

PreviewTexture::PreviewTexture(PreviewTexture &&other) noexcept
	: renderTexture_{other.renderTexture_}
	, hasAllocatedTexture_{other.hasAllocatedTexture_}
	, allocatedWidthInPixels_{other.allocatedWidthInPixels_}
	, allocatedHeightInPixels_{other.allocatedHeightInPixels_}
	, cachedPageModificationCounter_{other.cachedPageModificationCounter_}
{
	other.renderTexture_ = RenderTexture2D{};
	other.hasAllocatedTexture_ = false;
	other.allocatedWidthInPixels_ = 0;
	other.allocatedHeightInPixels_ = 0;
	other.cachedPageModificationCounter_ = 0;
}

PreviewTexture &PreviewTexture::operator=(PreviewTexture &&other) noexcept{
	
	if(this == &other) return *this;

	unload();

	renderTexture_ = other.renderTexture_;
	hasAllocatedTexture_ = other.hasAllocatedTexture_;
	allocatedWidthInPixels_ = other.allocatedWidthInPixels_;
	allocatedHeightInPixels_ = other.allocatedHeightInPixels_;
	cachedPageModificationCounter_ = other.cachedPageModificationCounter_;
    
	other.renderTexture_ = RenderTexture2D{};
	other.hasAllocatedTexture_ = false;
	other.allocatedWidthInPixels_ = 0;
	other.allocatedHeightInPixels_ = 0;
	other.cachedPageModificationCounter_ = 0;

	return *this;
}

void PreviewTexture::ensureAllocatedSize(int requestedWidthInPixels, int requestedHeightInPixels){

	if(requestedWidthInPixels <= 0 || requestedHeightInPixels <= 0){

		unload();
		return;
	}

	if(hasAllocatedTexture_ && allocatedWidthInPixels_ == requestedWidthInPixels && allocatedHeightInPixels_ == requestedHeightInPixels) return;

	unload();

	renderTexture_ = LoadRenderTexture(requestedWidthInPixels, requestedHeightInPixels);

	hasAllocatedTexture_ = renderTexture_.id != 0;

	allocatedWidthInPixels_ = hasAllocatedTexture_ ? requestedWidthInPixels : 0;
	allocatedHeightInPixels_ = hasAllocatedTexture_ ? requestedHeightInPixels : 0;

	cachedPageModificationCounter_ = 0;
}

void PreviewTexture::unload(){
	if(hasAllocatedTexture_ && renderTexture_.id != 0) UnloadRenderTexture(renderTexture_);

	renderTexture_ = RenderTexture2D{};
	hasAllocatedTexture_ = false;
	allocatedWidthInPixels_ = 0;
	allocatedHeightInPixels_ = 0;
	cachedPageModificationCounter_ = 0;

}
