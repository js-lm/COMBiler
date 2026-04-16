#pragma once

#include <cstdint>

#include <raylib.h>

class PreviewTexture{
private:
	RenderTexture2D renderTexture_{};
	bool hasAllocatedTexture_{false};

	int allocatedWidthInPixels_{0};
	int allocatedHeightInPixels_{0};

	uint64_t cachedPageModificationCounter_{0};

public:
	PreviewTexture() = default;
	~PreviewTexture();

	PreviewTexture(const PreviewTexture &previewTexture) = delete;
	PreviewTexture &operator=(const PreviewTexture &previewTexture) = delete;

	PreviewTexture(PreviewTexture &&other) noexcept;
	PreviewTexture &operator=(PreviewTexture &&other) noexcept;

public:
	void ensureAllocatedSize(int requestedWidthInPixels, int requestedHeightInPixels);
	void unload();

	bool hasAllocatedTexture() const{return hasAllocatedTexture_;}
	int allocatedWidthInPixels() const{return allocatedWidthInPixels_;}
	int allocatedHeightInPixels() const{return allocatedHeightInPixels_;}

	const RenderTexture2D &renderTexture() const{return renderTexture_;}

	uint64_t cachedPageModificationCounter() const{return cachedPageModificationCounter_;}
	void setCachedPageModificationCounter(uint64_t pageModificationCounter){
		cachedPageModificationCounter_ = pageModificationCounter;
	}
};