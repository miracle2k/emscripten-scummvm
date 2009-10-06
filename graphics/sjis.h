/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL$
 * $Id$
 */

// The code in this file is currently only used in KYRA and SCI.
// So if neither of those is enabled, we will skip compiling it.
// We also enable this code for ScummVM builds including support
// for dynamic engine plugins.
// If you plan to use this code in another engine, you will have
// to add the proper define check here.
#if !(defined(ENABLE_KYRA) || defined(ENABLE_SCI) || defined(DYNAMIC_MODULES))

// If neither of the above mentioned is enabled, do not include the SJIS code.

#else

#ifndef GRAPHICS_SJIS_H
#define GRAPHICS_SJIS_H

#include "common/scummsys.h"
#include "common/stream.h"
#include "common/util.h"

#include "graphics/surface.h"

namespace Graphics {

/**
 * A font that is able to draw SJIS encoded characters.
 */
class FontSJIS {
public:
	virtual ~FontSJIS() {}

	/**
	 * Creates the first SJIS font, which ROM/font file is present.
	 * It will also call loadData, so the user can just start
	 * using the font.
	 *
	 * It'll prefer the platform specific ROM file, when platform
	 * is set to a value, which's font ROM is supported.
	 * So far that is only kPlatformFMTowns.
	 *
	 * The last file tried is ScummVM's SJIS.FNT file.
	 */
	static FontSJIS *createFont(const Common::Platform platform = Common::kPlatformUnknown);

	/**
	 * Load the font data.
	 */
	virtual bool loadData() = 0;

	/**
	 * Enable outline drawing.
	 */
	virtual void enableOutline(bool enable) {}

	/**
	 * Toggle values returned by getFontHeight and getMaxFontWidth / getCharWidth.
	 * These methods have to return different values when emulating	PC-98 text mode.
	 * We cannot simply match this with enableOutline(), since there are situations
	 * where outlines get disabled in graphic mode, too. In these admittedly rare
	 * cases (Kyra 1: Brynn's note, Kyra 2: spell book) the values returned by
	 * getFontHeight and getMaxFontWidth / getCharWidth have to remain the same.
	 */
	virtual void toggleCharSize(bool textMode) {}

	/**
	 * Returns the height of the font.
	 */
	virtual uint getFontHeight() const = 0;

	/**
	 * Returns the max. width of the font.
	 */
	virtual uint getMaxFontWidth() const = 0;

	/**
	 * Returns the width of a specific character.
	 */
	virtual uint getCharWidth(uint16 ch) const { return getMaxFontWidth(); }

	/**
	 * Draws a SJIS encoded character on the given surface.
	 *
	 * TODO: Currently there is no assurance, that this method will only draw within
	 * the surface boundaries. Thus the caller has to assure the glyph will fit at
	 * the specified position.
	 */
	void drawChar(Graphics::Surface &dst, uint16 ch, int x, int y, uint32 c1, uint32 c2) const {
		drawChar(dst.getBasePtr(x, y), ch, c1, c2, dst.pitch, dst.bytesPerPixel);
	}

	/**
	 * Draws a SJIS char on the given raw buffer.
	 *
	 * @param dst   pointer to the destination
	 * @param ch    character to draw (in little endian)
	 * @param pitch pitch of the destination buffer (size in *bytes*)
	 * @param bpp   bytes per pixel of the destination buffer
	 * @param c1    forground color
	 * @param c2    outline color
	 */
	virtual void drawChar(void *dst, uint16 ch, int pitch, int bpp, uint32 c1, uint32 c2) const = 0;
};

/**
 * A base class to render 16x16 monochrome SJIS fonts.
 */
class FontSJIS16x16 : public FontSJIS {
public:
	FontSJIS16x16() : _outlineEnabled(false), _pc98TextModeCharSize(false) {}

	void enableOutline(bool enable) { _outlineEnabled = enable; }
	void toggleCharSize(bool textMode) { _pc98TextModeCharSize = textMode; }

	uint getFontHeight() const { return _pc98TextModeCharSize ? 16 : 18; }
	uint getMaxFontWidth() const { return _pc98TextModeCharSize ? 16 : 18; }

	virtual void drawChar(void *dst, uint16 ch, int pitch, int bpp, uint32 c1, uint32 c2) const;

private:
	template<typename Color>
	void drawCharInternOutline(const uint16 *glyph, uint8 *dst, int pitch, Color c1, Color c2) const;

	template<typename Color>
	void drawCharIntern(const uint16 *glyph, uint8 *dst, int pitch, Color c1) const;

protected:
	bool _outlineEnabled;
	bool _pc98TextModeCharSize;

	virtual const uint16 *getCharData(uint16 c) const = 0;
};

/**
 * FM-TOWNS ROM based SJIS compatible font.
 *
 * This is used in KYRA and SCI.
 *
 * TODO: This implementation does not support any 8x16 ASCII or half-width katakana chars.
 */
class FontTowns : public FontSJIS16x16 {
public:
	/**
	 * Loads the ROM data from "FMT_FNT.ROM".
	 */
	bool loadData();

private:
	enum {
		kFontRomSize = 262144
	};

	uint16 _fontData[kFontRomSize / 2];

	const uint16 *getCharData(uint16 c) const;
};

/**
 * Our custom SJIS FNT.
 */
class FontSjisSVM : public FontSJIS16x16 {
public:
	FontSjisSVM() : _fontData16x16(0), _fontData16x16Size(0), _fontData8x16(0), _fontData8x16Size(0) {}
	~FontSjisSVM() { delete[] _fontData16x16; delete[] _fontData8x16; }

	/**
	 * Load the font data from "SJIS.FNT".
	 */
	bool loadData();

	void drawChar(void *dst, uint16 ch, int pitch, int bpp, uint32 c1, uint32 c2) const;

	uint getCharWidth(uint16 ch) const;
private:
	uint16 *_fontData16x16;
	uint _fontData16x16Size;

	uint8 *_fontData8x16;
	uint _fontData8x16Size;

	bool is8x16(uint16 ch) const;

	const uint16 *getCharData(uint16 c) const;
	const uint8 *getCharData8x16(uint16 c) const;

	template<typename Color>
	void drawCharInternOutline(const uint8 *glyph, uint8 *dst, int pitch, Color c1, Color c2) const;

	template<typename Color>
	void drawCharIntern(const uint8 *glyph, uint8 *dst, int pitch, Color c1) const;
};

// TODO: Consider adding support for PC98 ROM

} // End of namespace Graphics

#endif

#endif // engine and dynamic plugins guard

