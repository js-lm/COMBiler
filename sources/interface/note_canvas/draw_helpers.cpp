#include "note_canvas.hpp"

#include <algorithm>

#include <cmath>

#include <raygui.h>

#include "constants.hpp"
#include "debug_utilities.hpp"

using namespace interface;
namespace canvas_constants = constants::interface_layout::note_canvas;

void NoteCanvas::drawCenteredTextLine(const Rectangle &columnBounds, float centerY, const std::string &text, Color textColor){
	
	if(text.empty()) return;

	const float availableWidth{std::max(.0f, columnBounds.width - (canvas_constants::commands::HorizontalPadding * 2.0f))};
	if(availableWidth <= .0f) return;


	const auto textSize{
		MeasureTextEx(GetFontDefault(), text.c_str(), static_cast<float>(canvas_constants::commands::TextFontSize), 1.0f)
	};

	const float startX{
		columnBounds.x + canvas_constants::commands::HorizontalPadding + ((availableWidth - textSize.x) * .5f)
	};
	const float startY{centerY - (textSize.y * .5f)};
	const float roundedStartX{std::round(startX)};
	const float roundedStartY{std::round(startY)};


	DrawTextEx(
		GetFontDefault(),
		text.c_str(),
		Vector2{roundedStartX, roundedStartY},
		static_cast<float>(canvas_constants::commands::TextFontSize),
		1.0f,
		textColor
	);

}

void NoteCanvas::drawBigNote(
	program_states::InterfaceContext &context,
	const Rectangle &columnBounds,
	const BigNote &bigNote,
	float alpha,
	bool shouldCenterIconInColumn
){
	const auto fillColor{Fade(ColorBrightness(bigNote.baseColor, -canvas_constants::notes::InstrumentIndexColorDarkeningFactor), alpha)};
	const auto borderColor{Fade(canvas_constants::notes::NoteBorderColor, alpha)};
	const auto textColor{Fade(WHITE, alpha)};

	DrawRectangleRec(columnBounds, fillColor);
	DrawRectangleLinesEx(columnBounds, canvas_constants::notes::BorderThicknessInPixels, borderColor);

	const auto &camera{context.system.noteCanvas.gridCamera};
	auto screenToWorld{[&](float x, float y){
		return GetScreenToWorld2D(Vector2{x, y}, camera);
	}};
	auto roundedScreenToWorld{[&](float x, float y){
		return screenToWorld(std::round(x), std::round(y));
	}};


	const Vector2 columnLeftTopScreenPosition{
		GetWorldToScreen2D(Vector2{columnBounds.x, columnBounds.y}, camera)
	};
	const Vector2 columnRightTopScreenPosition{
		GetWorldToScreen2D(Vector2{columnBounds.x + columnBounds.width, columnBounds.y}, camera)
	};

	const Rectangle screenRelativeColumnBounds{
		columnLeftTopScreenPosition.x,
		context.interface.noteCanvas.gridAreaTopScreenPositionY,
		columnRightTopScreenPosition.x - columnLeftTopScreenPosition.x,
		context.interface.noteCanvas.gridArea.height
	};

	const float textWidth{std::max(.0f, screenRelativeColumnBounds.width - (canvas_constants::commands::HorizontalPadding * 2.0f))};
	const float columnHeight{screenRelativeColumnBounds.height};
	const std::string trimmedFirstTextLine{
		bigNote.shouldDrawFirstTextLineVertically
			? bigNote.firstTextLine
			: NoteCanvas::trimTextToFitWidth(bigNote.firstTextLine, textWidth, canvas_constants::commands::TextFontSize)
	};
	const std::string trimmedSecondTextLine{NoteCanvas::trimTextToFitWidth(bigNote.secondTextLine, textWidth, canvas_constants::commands::TextFontSize)};
	const std::string trimmedThirdTextLine{NoteCanvas::trimTextToFitWidth(bigNote.thirdTextLine, textWidth, canvas_constants::commands::TextFontSize)};
	auto drawCenteredScreenTextLine{[&](const std::string &text, float centerY){
		if(text.empty()) return;

		const float availableWidth{std::max(.0f, screenRelativeColumnBounds.width - (canvas_constants::commands::HorizontalPadding * 2.0f))};
		if(availableWidth <= .0f) return;

		const auto textSize{
			MeasureTextEx(GetFontDefault(), text.c_str(), static_cast<float>(canvas_constants::commands::TextFontSize), 1.0f)
		};

		const float startX{
			screenRelativeColumnBounds.x + canvas_constants::commands::HorizontalPadding + ((availableWidth - textSize.x) * .5f)
		};
		const float startY{centerY - (textSize.y * .5f)};
		const Vector2 worldPosition{roundedScreenToWorld(startX, startY)};

		
		DrawTextEx(
			GetFontDefault(),
			text.c_str(),
			worldPosition,
			static_cast<float>(canvas_constants::commands::TextFontSize),
			1.0f,
			textColor
		);

	}};

	if(shouldCenterIconInColumn){
		if(bigNote.shouldDrawFirstTextLineVertically){
			const auto firstTextLineSize{
				MeasureTextEx(GetFontDefault(), trimmedFirstTextLine.c_str(), static_cast<float>(canvas_constants::commands::TextFontSize), 1.0f)
			};
			const float centerX{std::round(screenRelativeColumnBounds.x + (screenRelativeColumnBounds.width * .5f))};
			const float iconCenterY{
				std::round(screenRelativeColumnBounds.y + (screenRelativeColumnBounds.height * canvas_constants::commands::InstrumentVerticalIconCenterRatio))
			};
			const float iconLeftX{std::round(centerX - (static_cast<float>(canvas_constants::IconSize) * .5f))};
			const float iconTopY{std::round(iconCenterY - (static_cast<float>(canvas_constants::IconSize) * .5f))};
			const float iconBottomY{iconTopY + static_cast<float>(canvas_constants::IconSize)};
			const float textTopY{
				std::round(iconBottomY + canvas_constants::commands::InstrumentVerticalTextTopPaddingInPixels)
			};

			if(!trimmedFirstTextLine.empty()){

				const float rotatedTextWidth{firstTextLineSize.y};
				const float rotatedTextHalfWidth{rotatedTextWidth * .5f};
				const Vector2 textAnchorWorldPosition{
					roundedScreenToWorld(centerX + rotatedTextHalfWidth, textTopY)
				};

				DrawTextPro(
					GetFontDefault(),
					trimmedFirstTextLine.c_str(),
					textAnchorWorldPosition,
					Vector2{.0f, .0f},
					90.0f,
					static_cast<float>(canvas_constants::commands::TextFontSize),
					1.0f,
					textColor
				);
			}

			const Vector2 iconWorldPosition{roundedScreenToWorld(iconLeftX, iconTopY)};
			GuiDrawIcon(
				bigNote.iconIndex,
				iconWorldPosition.x,
				iconWorldPosition.y,
				1,
				textColor
			);


		}else{

			const Vector2 iconWorldPosition{
				roundedScreenToWorld(
					screenRelativeColumnBounds.x + (screenRelativeColumnBounds.width - canvas_constants::IconSize) * .5f,
					screenRelativeColumnBounds.y + (columnHeight - canvas_constants::IconSize) * .5f
				)
			};

			GuiDrawIcon(
				bigNote.iconIndex,
				iconWorldPosition.x,
				iconWorldPosition.y,
				1,
				textColor
			);
			
		}
		return;
	}

	const float constantStringCenterY{screenRelativeColumnBounds.y + (columnHeight * canvas_constants::commands::ConstantStringCenterRatio)};
	const float iconCenterY{screenRelativeColumnBounds.y + (columnHeight * canvas_constants::commands::IconCenterRatio)};
	const float nameCenterY{screenRelativeColumnBounds.y + (columnHeight * canvas_constants::commands::NameCenterRatio)};
	const float valueCenterY{screenRelativeColumnBounds.y + (columnHeight * canvas_constants::commands::ValueCenterRatio)};
	const float targetCenterY{screenRelativeColumnBounds.y + (columnHeight * canvas_constants::commands::TargetCenterRatio)};

	if(!bigNote.constantName.empty()){
		drawCenteredScreenTextLine(
			NoteCanvas::trimTextToFitWidth(
				bigNote.constantName, 
				textWidth, 
				canvas_constants::commands::TextFontSize
			), 
			constantStringCenterY
		);
	}

	const Vector2 iconWorldPosition{
		roundedScreenToWorld(
			screenRelativeColumnBounds.x + (screenRelativeColumnBounds.width - canvas_constants::IconSize) * .5f,
			iconCenterY - (canvas_constants::IconSize * .5f)
		)
	};

	GuiDrawIcon(
		bigNote.iconIndex,
		iconWorldPosition.x,
		iconWorldPosition.y,
		1,
		textColor
	);

	drawCenteredScreenTextLine(trimmedFirstTextLine, nameCenterY);
	drawCenteredScreenTextLine(trimmedSecondTextLine, valueCenterY);
	drawCenteredScreenTextLine(trimmedThirdTextLine, targetCenterY);
}