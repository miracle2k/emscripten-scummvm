/* Copyright (C) 1994-2003 Revolution Software Ltd
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * $Header$
 */

#include "stdafx.h"
#include "sword2/driver/driver96.h"
#include "sword2/sword2.h"
#include "sword2/debug.h"
#include "sword2/console.h"
#include "sword2/defs.h"
#include "sword2/events.h"			// for CountEvents()
#include "sword2/layers.h"
#include "sword2/logic.h"
#include "sword2/maketext.h"
#include "sword2/mouse.h"
#include "sword2/protocol.h"
#include "sword2/resman.h"
#include "sword2/router.h"			// for plotWalkGrid()
#include "sword2/speech.h"			// for '_officialTextNumber'
						// and '_speechScriptWaiting'

namespace Sword2 {

void Debugger::clearDebugTextBlocks(void) {
	uint8 blockNo = 0;

	while (blockNo < MAX_DEBUG_TEXT_BLOCKS && _debugTextBlocks[blockNo] > 0) {
		// kill the system text block
		fontRenderer.killTextBloc(_debugTextBlocks[blockNo]);

		// clear this element of our array of block numbers
		_debugTextBlocks[blockNo] = 0;

		blockNo++;
	}
}

void Debugger::makeDebugTextBlock(char *text, int16 x, int16 y) {
	uint8 blockNo = 0;

	while (blockNo < MAX_DEBUG_TEXT_BLOCKS && _debugTextBlocks[blockNo] > 0)
		blockNo++;

	assert(blockNo < MAX_DEBUG_TEXT_BLOCKS);

	_debugTextBlocks[blockNo] = fontRenderer.buildNewBloc((uint8 *) text, x, y, 640 - x, 0, RDSPR_DISPLAYALIGN, CONSOLE_FONT_ID, NO_JUSTIFICATION);
}

void Debugger::buildDebugText(void) {
	char buf[128];

	int32 showVarNo;		// for variable watching
	int32 showVarPos;
	int32 varNo;
	int32 *varTable;

	// clear the array of text block numbers for the debug text
	clearDebugTextBlocks();

	// mouse coords
/*
	// print mouse coords beside mouse-marker, if it's being displayed
	if (displayMouseMarker) {
		sprintf(buf, "%d,%d", mousex + _thisScreen.scroll_offset_x, mousey + _thisScreen.scroll_offset_y);
		if (mousex>560)
			makeDebugTextBlock(buf, mousex - 50, mousey - 15);
		else
			makeDebugTextBlock(buf, mousex + 5, mousey - 15);
	}
*/

	// mouse area coords

	// defining a mouse area the easy way, by creating a box on-screen
	if (_draggingRectangle || SYSTEM_TESTING_ANIMS) {
		// so we can see what's behind the lines
		_rectFlicker = !_rectFlicker;

		sprintf(buf, "x1=%d", _rectX1);
		makeDebugTextBlock(buf, 0, 120);

		sprintf(buf, "y1=%d", _rectY1);
		makeDebugTextBlock(buf, 0, 135);

		sprintf(buf, "x2=%d", _rectX2);
		makeDebugTextBlock(buf, 0, 150);

		sprintf(buf, "y2=%d", _rectY2);
		makeDebugTextBlock(buf, 0, 165);
	}

	// testingSnR indicator

	if (_testingSnR) {		// see fnAddHuman()
		sprintf(buf, "TESTING LOGIC STABILITY!");
		makeDebugTextBlock(buf, 0, 105);
	}

	// speed-up indicator

	if (g_sword2->_renderSkip) {		// see sword.cpp
		sprintf(buf, "SKIPPING FRAMES FOR SPEED-UP!");
		makeDebugTextBlock(buf, 0, 120);
	}

	// debug info at top of screen - enabled/disabled as one complete unit

	if (_displayTime) {
		int32 time = SVM_timeGetTime();

		if ((time - _startTime) / 1000 >= 10000)
			_startTime = time;

		time -= _startTime;
		sprintf(buf, "Time %.2d:%.2d:%.2d.%.3d", (time / 3600000) % 60, (time / 60000) % 60, (time / 1000) % 60, time % 1000);
		makeDebugTextBlock(buf, 500, 360);
		sprintf(buf, "Game %d", g_sword2->_gameCycle);
		makeDebugTextBlock(buf, 500, 380);
	}

   	// current text number & speech-sample resource id

	if (_displayTextNumbers) {
		if (_textNumber) {
			if (SYSTEM_TESTING_TEXT) {
				if (SYSTEM_WANT_PREVIOUS_LINE)
					sprintf(buf, "backwards");
				else
 					sprintf(buf, "forwards");

				makeDebugTextBlock(buf, 0, 340);
			}

			sprintf(buf, "res: %d", _textNumber / SIZE);
			makeDebugTextBlock(buf, 0, 355);

			sprintf(buf, "pos: %d", _textNumber & 0xffff);
			makeDebugTextBlock(buf, 0, 370);

 			sprintf(buf, "TEXT: %d", g_logic->_officialTextNumber);
			makeDebugTextBlock(buf, 0, 385);
		}
	}

	// resource number currently being checking for animation

	if (SYSTEM_TESTING_ANIMS) {
		sprintf(buf, "trying resource %d", SYSTEM_TESTING_ANIMS);
		makeDebugTextBlock(buf, 0, 90);
	}

	// general debug info

	if (_displayDebugText) {
/*
		// CD in use
		sprintf(buf, "CD-%d", currentCD);
		makeDebugTextBlock(buf, 0, 0);
*/

		// mouse coords & object pointed to

		if (CLICKED_ID)
			sprintf(buf, "last click at %d,%d (id %d: %s)",
				MOUSE_X, MOUSE_Y, CLICKED_ID,
				g_sword2->fetchObjectName(CLICKED_ID));
		else
			sprintf(buf, "last click at %d,%d (---)",
				MOUSE_X, MOUSE_Y);

 		makeDebugTextBlock(buf, 0, 15);

		if (g_sword2->_mouseTouching)
			sprintf(buf, "mouse %d,%d (id %d: %s)",
				g_display->_mouseX + g_sword2->_thisScreen.scroll_offset_x,
				g_display->_mouseY + g_sword2->_thisScreen.scroll_offset_y,
				g_sword2->_mouseTouching,
				g_sword2->fetchObjectName(g_sword2->_mouseTouching));
		else
			sprintf(buf, "mouse %d,%d (not touching)",
				g_display->_mouseX + g_sword2->_thisScreen.scroll_offset_x,
				g_display->_mouseY + g_sword2->_thisScreen.scroll_offset_y);

		makeDebugTextBlock(buf, 0, 30);

 		// player coords & graphic info
		// if player objct has a graphic

		if (_playerGraphic.anim_resource)
			sprintf(buf, "player %d,%d %s (%d) #%d/%d",
				g_sword2->_thisScreen.player_feet_x,
				g_sword2->_thisScreen.player_feet_y,
				g_sword2->fetchObjectName(_playerGraphic.anim_resource),
				_playerGraphic.anim_resource,
				_playerGraphic.anim_pc,
				_playerGraphicNoFrames);
		else
			sprintf(buf, "player %d,%d --- %d",
				g_sword2->_thisScreen.player_feet_x,
				g_sword2->_thisScreen.player_feet_y,
				_playerGraphic.anim_pc);

		makeDebugTextBlock(buf, 0, 45);

 		// frames-per-second counter

		sprintf(buf, "fps %d", g_sword2->_fps);
		makeDebugTextBlock(buf, 440, 0);

 		// location number

		sprintf(buf, "location=%d", LOCATION);
		makeDebugTextBlock(buf, 440, 15);

 		// "result" variable

		sprintf(buf, "result=%d", RESULT);
		makeDebugTextBlock(buf, 440, 30);

 		// no. of events in event list

		sprintf(buf, "events=%d", g_sword2->countEvents());
		makeDebugTextBlock(buf, 440, 45);

		// sprite list usage

		sprintf(buf, "bgp0: %d/%d", g_sword2->_curBgp0, MAX_bgp0_sprites);
		makeDebugTextBlock(buf, 560, 0);

		sprintf(buf, "bgp1: %d/%d", g_sword2->_curBgp1, MAX_bgp1_sprites);
		makeDebugTextBlock(buf, 560, 15);

		sprintf(buf, "back: %d/%d", g_sword2->_curBack, MAX_back_sprites);
		makeDebugTextBlock(buf, 560, 30);

		sprintf(buf, "sort: %d/%d", g_sword2->_curSort, MAX_sort_sprites);
		makeDebugTextBlock(buf, 560, 45);

		sprintf(buf, "fore: %d/%d", g_sword2->_curFore, MAX_fore_sprites);
		makeDebugTextBlock(buf, 560, 60);

		sprintf(buf, "fgp0: %d/%d", g_sword2->_curFgp0, MAX_fgp0_sprites);
		makeDebugTextBlock(buf, 560, 75);

		sprintf(buf, "fgp1: %d/%d", g_sword2->_curFgp1, MAX_fgp1_sprites);
		makeDebugTextBlock(buf, 560, 90);

		// largest layer & sprite

		// NB. Strings already constructed in Build_display.cpp
		makeDebugTextBlock(g_sword2->_largestLayerInfo, 0, 60);
		makeDebugTextBlock(g_sword2->_largestSpriteInfo, 0, 75);

		// "waiting for person" indicator - set form fnTheyDo and
		// fnTheyDoWeWait

		if (g_logic->_speechScriptWaiting) {
			sprintf(buf, "script waiting for %s (%d)",
				g_sword2->fetchObjectName(g_logic->_speechScriptWaiting),
				g_logic->_speechScriptWaiting);
			makeDebugTextBlock(buf, 0, 90);
		}

		// variable watch display

		showVarPos = 115;	// y-coord for first showVar

		// res 1 is the global variables resource
		varTable = (int32 *) (res_man->openResource(1) + sizeof(_standardHeader));

		for (showVarNo = 0; showVarNo < MAX_SHOWVARS; showVarNo++) {
			varNo = _showVar[showVarNo];	// get variable number

			// if non-zero ie. cannot watch 'id' but not needed
			// anyway because it changes throughout the logic loop

			if (varNo) {
				sprintf(buf, "var(%d) = %d", varNo, varTable[varNo]);
				makeDebugTextBlock(buf, 530, showVarPos);
				showVarPos += 15;	// next line down
			}
		}

		res_man->closeResource(1);	// close global variables resource

		// memory indicator - this should come last, to show all the
		// sprite blocks above!

		memory->memoryString(buf);
		makeDebugTextBlock(buf, 0, 0);
	}
}

void Debugger::drawDebugGraphics(void) {
	// walk-grid

	if (_displayWalkGrid)
		router.plotWalkGrid(); 

	// player feet coord marker

	if (_displayPlayerMarker)
		plotCrossHair(g_sword2->_thisScreen.player_feet_x, g_sword2->_thisScreen.player_feet_y, 215);

	// mouse marker & coords

	if (_displayMouseMarker)
		plotCrossHair(g_display->_mouseX + g_sword2->_thisScreen.scroll_offset_x, g_display->_mouseY + g_sword2->_thisScreen.scroll_offset_y, 215);

   	// mouse area rectangle / sprite box rectangle when testing anims

	if (SYSTEM_TESTING_ANIMS) {
		// draw box around current frame
		drawRect(_rectX1, _rectY1, _rectX2, _rectY2, 184);
	} else if (_draggingRectangle) {
		// defining a mouse area the easy way, by creating a box
		// on-screen
		if (_rectFlicker)
			drawRect(_rectX1, _rectY1, _rectX2, _rectY2, 184);
	}
}

void Debugger::plotCrossHair(int16 x, int16 y, uint8 pen) {
	g_display->plotPoint(x, y, pen);		// driver function

	g_display->drawLine(x - 2, y, x - 5, y, pen);	// driver function
	g_display->drawLine(x + 2, y, x + 5, y, pen);

	g_display->drawLine(x, y - 2, x, y - 5, pen);
	g_display->drawLine(x, y + 2, x, y + 5, pen);
}

void Debugger::drawRect(int16 x1, int16 y1, int16 x2, int16 y2, uint8 pen) {
	g_display->drawLine(x1, y1, x2, y1, pen);	// top edge
	g_display->drawLine(x1, y2, x2, y2, pen);	// bottom edge
	g_display->drawLine(x1, y1, x1, y2, pen);	// left edge
	g_display->drawLine(x2, y1, x2, y2, pen);	// right edge
}

void Debugger::printCurrentInfo(void) {
	// prints general stuff about the screen, etc.

	if (g_sword2->_thisScreen.background_layer_id) {
		Debug_Printf("background layer id %d\n", g_sword2->_thisScreen.background_layer_id);
		Debug_Printf("%d wide, %d high\n", g_sword2->_thisScreen.screen_wide, g_sword2->_thisScreen.screen_deep);
		Debug_Printf("%d normal layers\n", g_sword2->_thisScreen.number_of_layers);

		g_logic->examineRunList();
	} else
		Debug_Printf("No screen\n");
}

} // End of namespace Sword2
