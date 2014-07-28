/*  src/psp/font.c: Simple font used for PSP menu (public-domain 6x13 font
                    from X.Org)
    Copyright 2009 Andrew Church

    This file is part of Yabause.

    Yabause is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Yabause is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Yabause; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/

#include "common.h"

#include "font.h"
#include "gu.h"

/*************************************************************************/
/****************************** Local data *******************************/
/*************************************************************************/

/* Character width (the font is fixed-width) */
#define CHAR_WIDTH   6

/* First and last characters in font data */
#define FONTDATA_FIRSTCHAR  32
#define FONTDATA_LASTCHAR  127

/* Characters (not pixels!) in one row of font data */
#define FONTDATA_STRIDE     32

/* Default character to use for out-of-range characters */
#define FONTDATA_DEFCHAR   '?'

/* Font data definition (1 byte per pixel, 32 6x13 characters per row).
 * This wastes 6 bits per pixel, but it's only 7k anyway so no big deal. */

static const __attribute__((aligned(64))) uint8_t fontdata[] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,1,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,
    0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,2,0,0,0,1,0,0,0,0,0,1,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,
    0,0,1,0,0,0,0,0,1,0,0,0,0,1,1,1,0,0,1,1,1,1,1,0,0,0,0,1,0,0,1,1,
    1,1,1,0,0,1,1,1,0,0,1,1,1,1,1,0,0,1,1,1,0,0,0,1,1,1,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,1,0,0,
    0,0,0,0,0,0,0,0,1,2,0,0,0,1,2,1,2,0,0,1,0,1,0,0,0,1,1,1,1,0,1,0,
    1,0,1,2,0,1,0,0,0,0,0,0,1,2,0,0,0,0,1,2,0,0,0,0,1,2,0,0,1,0,1,2,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,
    0,1,0,1,0,0,0,1,1,2,0,0,1,0,2,2,1,0,0,2,2,2,1,2,0,0,0,1,2,0,1,2,
    2,2,2,2,1,0,2,2,1,0,0,2,2,2,1,2,1,0,2,2,1,0,1,0,2,2,1,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,1,0,2,0,0,0,0,0,0,0,1,0,0,0,0,1,0,2,2,1,0,
    0,0,0,0,0,0,0,0,1,2,0,0,0,1,2,1,2,0,0,1,2,1,2,0,1,0,1,2,2,2,0,1,
    0,1,0,2,1,0,1,0,0,0,0,0,1,2,0,0,0,1,0,2,0,0,0,0,0,1,0,0,0,1,1,1,
    0,2,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,2,
    1,0,2,0,1,0,1,0,1,2,0,0,1,2,0,0,1,2,0,0,0,1,0,2,0,0,1,1,2,0,1,2,
    0,0,0,0,1,2,0,0,0,2,0,0,0,1,0,2,1,2,0,0,1,2,1,2,0,0,1,2,0,0,1,0,
    0,0,0,0,1,0,0,0,0,0,1,0,2,0,0,0,0,0,0,0,0,0,1,0,0,0,1,2,0,0,1,2,
    0,0,0,0,0,0,0,0,1,2,0,0,0,0,2,0,2,0,1,1,1,1,1,0,1,2,1,2,0,0,0,0,
    2,1,2,0,1,2,1,2,0,0,0,0,0,2,0,0,0,1,2,0,0,0,0,0,0,1,2,0,1,0,1,2,
    1,0,0,0,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,0,
    1,2,0,0,1,2,0,2,1,2,0,0,0,2,0,0,1,2,0,0,1,0,2,0,0,1,0,1,2,0,1,2,
    1,1,0,0,1,2,0,0,0,0,0,0,0,1,2,0,1,2,0,0,1,2,1,2,0,0,1,2,0,1,1,1,
    0,0,0,1,1,1,0,0,0,1,0,2,0,0,1,1,1,1,1,0,0,0,0,1,0,0,0,2,0,0,1,2,
    0,0,0,0,0,0,0,0,1,2,0,0,0,0,0,0,0,0,0,1,2,1,2,2,0,1,1,1,0,0,0,0,
    1,0,2,0,0,1,0,2,0,0,0,0,0,0,0,0,0,1,2,0,0,0,0,0,0,1,2,0,0,2,1,2,
    0,2,1,1,1,1,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,0,2,0,
    1,2,0,0,1,2,0,0,1,2,0,0,0,0,0,1,0,2,0,1,1,1,0,0,0,1,2,1,2,0,1,1,
    0,2,1,0,1,1,1,1,0,0,0,0,1,0,2,0,0,1,1,1,0,2,0,1,1,1,1,2,0,0,1,2,
    2,0,0,0,1,2,2,0,1,0,2,0,0,0,0,2,2,2,2,2,0,0,0,0,1,0,0,0,0,1,0,2,
    0,0,0,0,0,0,0,0,1,2,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,1,2,1,0,0,1,
    0,2,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,2,0,0,0,0,0,0,1,2,0,0,0,0,2,
    0,0,0,2,1,2,2,2,0,0,0,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0,0,1,0,2,0,0,
    1,2,0,0,1,2,0,0,1,2,0,0,0,0,1,0,2,0,0,0,2,2,1,0,1,0,2,1,2,0,0,2,
    2,0,1,2,1,2,2,2,1,0,0,0,1,2,0,0,1,0,2,2,1,0,0,0,2,2,1,2,0,0,0,2,
    0,0,0,0,0,2,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,2,0,0,1,0,2,0,
    0,0,0,0,0,0,0,0,1,2,0,0,0,0,0,0,0,0,0,1,2,1,2,2,0,0,1,2,1,2,0,1,
    2,1,0,0,1,2,0,1,1,0,0,0,0,0,0,0,0,1,2,0,0,0,0,0,0,1,2,0,0,0,0,0,
    0,0,0,0,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,0,0,0,
    1,2,0,0,1,2,0,0,1,2,0,0,0,1,0,2,0,0,0,0,0,0,1,2,1,1,1,1,1,0,0,0,
    0,0,1,2,1,2,0,0,1,2,0,1,0,2,0,0,1,2,0,0,1,2,0,0,0,0,1,2,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,1,1,1,1,0,0,0,1,0,2,0,0,0,1,2,0,0,
    0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,1,2,1,2,0,1,1,1,1,0,2,1,0,
    1,0,1,0,1,2,0,1,2,2,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,2,0,0,0,0,0,
    0,0,0,0,0,2,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,2,0,0,0,
    0,1,0,1,0,2,0,0,1,2,0,0,1,0,2,0,0,0,1,0,0,0,1,2,0,2,2,1,2,2,1,0,
    0,0,1,2,1,2,0,0,1,2,0,1,2,0,0,0,1,2,0,0,1,2,1,0,0,0,1,2,0,0,1,0,
    0,0,0,0,1,1,0,0,0,0,0,1,0,0,0,2,2,2,2,2,0,1,0,2,0,0,0,0,0,2,0,0,
    0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,2,0,2,0,0,2,1,2,2,0,1,2,
    0,1,0,2,0,1,1,0,1,0,0,0,0,0,0,0,0,0,1,2,0,0,0,0,1,2,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,1,2,2,0,0,0,0,0,0,0,0,1,1,1,0,0,1,2,0,0,0,0,
    0,0,1,0,2,0,1,1,1,1,1,0,1,1,1,1,1,0,0,1,1,1,0,2,0,0,0,1,2,0,0,1,
    1,1,0,2,0,1,1,1,0,2,0,1,2,0,0,0,0,1,1,1,0,2,0,1,1,1,0,2,0,1,1,1,
    0,0,0,0,1,2,2,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,2,0,0,0,0,0,1,0,0,0,
    0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,2,
    0,0,2,0,0,0,2,2,0,2,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,2,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,1,0,2,0,0,0,0,0,0,0,0,0,0,1,2,2,0,0,2,0,0,0,0,
    0,0,0,2,0,0,0,2,2,2,2,2,0,2,2,2,2,2,0,0,2,2,2,0,0,0,0,0,2,0,0,0,
    2,2,2,0,0,0,2,2,2,0,0,0,2,0,0,0,0,0,2,2,2,0,0,0,2,2,2,0,0,0,1,2,
    2,0,0,1,0,2,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,2,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
    0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,1,1,1,0,0,0,0,1,0,0,0,1,1,1,1,0,0,0,1,1,1,0,0,1,1,1,1,0,0,1,1,
    1,1,1,0,1,1,1,1,1,0,0,1,1,1,0,0,1,0,0,0,1,0,0,1,1,1,0,0,0,0,1,1,
    1,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,1,1,1,0,0,
    1,1,1,1,0,0,0,1,1,1,0,0,1,1,1,1,0,0,0,1,1,1,0,0,1,1,1,1,1,0,1,0,
    0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,1,1,1,
    1,0,0,1,2,2,2,0,1,0,0,0,0,0,0,0,2,1,2,0,0,0,1,0,0,0,0,0,0,0,0,0,
    1,0,2,2,1,0,0,1,0,1,0,0,0,1,2,2,1,0,1,0,2,2,1,0,0,1,2,2,1,0,1,2,
    2,2,2,2,1,2,2,2,2,2,1,0,2,2,1,0,1,2,0,0,1,2,0,0,1,2,2,0,0,0,0,1,
    2,2,1,2,0,0,1,2,1,2,0,0,0,0,1,2,0,0,1,2,1,1,0,0,1,2,1,0,2,2,1,0,
    1,2,2,2,1,0,1,0,2,2,1,0,1,2,2,2,1,0,1,0,2,2,1,0,0,2,1,2,2,2,1,2,
    0,0,1,2,1,2,0,0,1,2,1,2,0,0,1,2,1,2,0,0,1,2,1,2,0,0,1,2,0,2,2,2,
    1,2,0,1,2,0,0,0,1,2,0,0,0,0,0,0,0,1,2,0,0,1,0,1,0,0,0,0,0,0,0,0,
    1,2,0,0,1,2,1,0,2,0,1,0,0,1,2,0,1,2,1,2,0,0,0,2,0,1,2,0,1,2,1,2,
    0,0,0,0,1,2,0,0,0,0,1,2,0,0,0,2,1,2,0,0,1,2,0,0,1,2,0,0,0,0,0,1,
    2,0,1,2,0,1,0,2,1,2,0,0,0,0,1,1,0,1,1,2,1,1,2,0,1,2,1,2,0,0,1,2,
    1,2,0,0,1,2,1,2,0,0,1,2,1,2,0,0,1,2,1,2,0,0,0,2,0,0,1,2,0,0,1,2,
    0,0,1,2,1,2,0,0,1,2,1,2,0,0,1,2,0,1,0,1,0,2,0,1,0,1,0,2,0,0,0,1,
    0,2,0,1,2,0,0,0,0,1,0,0,0,0,0,0,0,1,2,0,1,0,2,0,1,0,0,0,0,0,0,0,
    1,2,0,1,1,2,1,2,0,0,1,2,0,1,2,0,1,2,1,2,0,0,0,0,0,1,2,0,1,2,1,2,
    0,0,0,0,1,2,0,0,0,0,1,2,0,0,0,0,1,2,0,0,1,2,0,0,1,2,0,0,0,0,0,1,
    2,0,1,2,1,0,2,0,1,2,0,0,0,0,1,2,1,0,1,2,1,2,1,0,1,2,1,2,0,0,1,2,
    1,2,0,0,1,2,1,2,0,0,1,2,1,2,0,0,1,2,1,2,0,0,0,0,0,0,1,2,0,0,1,2,
    0,0,1,2,1,2,0,0,1,2,1,2,0,0,1,2,0,1,2,1,2,0,0,1,2,1,2,0,0,0,0,1,
    2,0,0,1,2,0,0,0,0,1,2,0,0,0,0,0,0,1,2,0,0,2,0,0,0,2,0,0,0,0,0,0,
    1,2,1,0,1,2,1,2,0,0,1,2,0,1,1,1,0,2,1,2,0,0,0,0,0,1,2,0,1,2,1,1,
    1,1,0,0,1,1,1,1,0,0,1,2,0,0,0,0,1,1,1,1,1,2,0,0,1,2,0,0,0,0,0,1,
    2,0,1,1,0,2,0,0,1,2,0,0,0,0,1,2,1,2,1,2,1,2,1,2,1,2,1,2,0,0,1,2,
    1,1,1,1,0,2,1,2,0,0,1,2,1,1,1,1,0,2,0,1,1,1,0,0,0,0,1,2,0,0,1,2,
    0,0,1,2,0,1,0,1,0,2,1,2,1,0,1,2,0,0,1,0,2,0,0,0,1,0,2,0,0,0,1,0,
    2,0,0,1,2,0,0,0,0,0,1,0,0,0,0,0,0,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,2,1,2,1,2,1,1,1,1,1,2,0,1,2,2,1,0,1,2,0,0,0,0,0,1,2,0,1,2,1,2,
    2,2,2,0,1,2,2,2,2,0,1,2,0,1,1,0,1,2,2,2,1,2,0,0,1,2,0,0,0,0,0,1,
    2,0,1,2,1,0,0,0,1,2,0,0,0,0,1,2,0,2,1,2,1,2,0,1,1,2,1,2,0,0,1,2,
    1,2,2,2,2,0,1,2,0,0,1,2,1,2,1,2,2,0,0,0,2,2,1,0,0,0,1,2,0,0,1,2,
    0,0,1,2,0,1,2,1,2,0,1,2,1,2,1,2,0,1,0,1,0,0,0,0,1,2,0,0,0,1,0,2,
    0,0,0,1,2,0,0,0,0,0,0,1,0,0,0,0,0,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,2,1,1,0,2,1,2,2,2,1,2,0,1,2,0,1,2,1,2,0,0,0,0,0,1,2,0,1,2,1,2,
    0,0,0,0,1,2,0,0,0,0,1,2,0,0,1,2,1,2,0,0,1,2,0,0,1,2,0,0,0,0,0,1,
    2,0,1,2,0,1,0,0,1,2,0,0,0,0,1,2,0,0,1,2,1,2,0,1,1,2,1,2,0,0,1,2,
    1,2,0,0,0,0,1,2,0,0,1,2,1,2,0,1,0,0,0,0,0,0,1,2,0,0,1,2,0,0,1,2,
    0,0,1,2,0,1,2,1,2,0,1,2,1,2,1,2,0,1,2,1,2,0,0,0,1,2,0,0,0,1,2,0,
    0,0,0,1,2,0,0,0,0,0,0,1,2,0,0,0,0,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,2,0,2,2,0,1,2,0,0,1,2,0,1,2,0,1,2,1,2,0,0,1,0,0,1,2,0,1,2,1,2,
    0,0,0,0,1,2,0,0,0,0,1,2,0,0,1,2,1,2,0,0,1,2,0,0,1,2,0,0,1,0,0,1,
    2,0,1,2,0,0,1,0,1,2,0,0,0,0,1,2,0,0,1,2,1,2,0,0,1,2,1,2,0,0,1,2,
    1,2,0,0,0,0,1,2,1,0,1,2,1,2,0,0,1,0,1,0,0,0,1,2,0,0,1,2,0,0,1,2,
    0,0,1,2,0,0,1,0,2,0,1,2,1,2,1,2,1,0,2,0,1,0,0,0,1,2,0,0,1,0,2,0,
    0,0,0,1,2,0,0,0,0,0,0,0,1,0,0,0,0,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,1,1,1,1,0,1,2,0,0,1,2,1,1,1,1,0,2,0,1,1,1,0,2,1,1,1,1,0,2,1,1,
    1,1,1,0,1,2,0,0,0,0,0,1,1,1,0,2,1,2,0,0,1,2,0,1,1,1,0,0,0,1,1,0,
    2,0,1,2,0,0,1,2,1,1,1,1,1,0,1,2,0,0,1,2,1,2,0,0,1,2,0,1,1,1,0,2,
    1,2,0,0,0,0,0,1,1,1,0,2,1,2,0,0,1,2,0,1,1,1,0,2,0,0,1,2,0,0,0,1,
    1,1,0,2,0,0,1,2,0,0,0,1,0,1,0,2,1,2,0,0,1,2,0,0,1,2,0,0,1,1,1,1,
    1,0,0,1,2,0,0,0,0,0,0,0,1,2,0,0,0,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,2,2,2,2,0,2,0,0,0,2,0,2,2,2,2,0,0,0,2,2,2,0,0,2,2,2,2,0,0,2,
    2,2,2,2,0,2,0,0,0,0,0,0,2,2,2,0,0,2,0,0,0,2,0,0,2,2,2,0,0,0,2,2,
    0,0,0,2,0,0,0,2,0,2,2,2,2,2,0,2,0,0,0,2,0,2,0,0,0,2,0,0,2,2,2,0,
    0,2,0,0,0,0,0,0,2,2,1,0,0,2,0,0,0,2,0,0,2,2,2,0,0,0,0,2,0,0,0,0,
    2,2,2,0,0,0,0,2,0,0,0,0,2,0,2,0,0,2,0,0,0,2,0,0,0,2,0,0,0,2,2,2,
    2,2,0,1,1,1,0,0,0,0,0,0,0,2,0,1,1,1,2,0,0,0,0,0,0,0,1,1,1,1,1,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,2,2,2,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0,0,0,0,0,0,2,2,2,2,2,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,
    0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,1,0,2,2,0,0,1,0,0,0,0,2,1,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,
    0,0,0,0,2,0,0,0,0,0,0,0,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,0,0,
    0,0,0,0,0,1,0,2,1,0,0,0,0,0,0,0,1,2,0,0,0,0,0,0,1,0,0,0,0,0,0,1,
    0,0,1,2,0,0,0,0,0,0,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,1,2,0,0,0,0,1,2,0,0,0,0,1,2,0,0,1,0,1,0,1,2,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,0,0,
    0,0,0,0,0,1,2,0,0,2,0,0,0,0,0,0,1,2,0,0,0,0,0,0,0,2,0,0,0,0,0,0,
    2,0,1,2,0,0,0,0,0,0,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,1,2,0,0,0,0,1,2,0,0,0,0,1,2,0,0,1,2,0,1,0,2,0,0,0,0,0,0,
    0,0,0,0,0,0,0,1,1,1,0,0,1,1,1,1,0,0,0,1,1,1,0,0,0,1,1,1,1,2,0,1,
    1,1,0,0,0,1,2,0,0,0,0,1,1,1,0,0,1,2,1,1,0,0,0,1,1,0,0,0,0,0,1,1,
    0,0,1,2,0,1,0,0,0,0,1,2,0,0,1,1,0,1,0,0,1,0,1,1,0,0,0,1,1,1,0,0,
    1,1,1,1,0,0,0,1,1,1,1,0,1,0,1,1,0,0,0,1,1,1,0,0,1,1,1,1,0,0,1,0,
    0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,1,1,1,
    1,0,0,0,1,2,0,0,0,0,1,2,0,0,0,0,1,2,0,0,0,2,0,0,2,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,2,2,1,0,1,2,2,2,1,0,1,0,2,2,1,0,1,0,2,2,1,2,1,0,
    2,2,1,0,1,1,1,1,0,0,1,0,2,2,1,0,1,1,0,2,1,0,0,0,1,2,0,0,0,0,0,1,
    2,0,1,2,1,0,2,0,0,0,1,2,0,0,1,2,1,0,1,0,1,1,0,2,1,0,1,0,2,2,1,0,
    1,2,2,2,1,0,1,0,2,2,1,2,1,1,0,2,1,0,1,0,2,2,1,0,0,1,2,2,2,0,1,2,
    0,0,1,2,1,2,0,0,1,2,1,2,0,0,1,2,0,1,0,1,0,2,1,2,0,0,1,2,0,2,2,1,
    2,2,1,1,0,2,0,0,0,0,1,2,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,1,1,1,1,2,1,2,0,0,1,2,1,2,0,0,0,2,1,2,0,0,1,2,1,1,
    1,1,1,2,0,1,2,2,2,0,1,2,0,0,1,2,1,2,2,0,1,2,0,0,1,2,0,0,0,0,0,1,
    2,0,1,1,0,2,0,0,0,0,1,2,0,0,1,2,1,2,1,2,1,2,2,0,1,2,1,2,0,0,1,2,
    1,2,0,0,1,2,1,2,0,0,1,2,1,2,2,0,0,2,0,1,1,0,0,2,0,1,2,0,0,0,1,2,
    0,0,1,2,1,2,0,0,1,2,1,2,1,0,1,2,0,0,1,0,2,0,1,2,0,0,1,2,0,0,1,0,
    2,0,0,2,1,0,0,0,0,0,1,2,0,0,0,0,1,0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,1,0,2,2,1,2,1,2,0,0,1,2,1,2,0,0,0,0,1,2,0,0,1,2,1,2,
    2,2,2,2,0,1,2,0,0,0,1,2,0,0,1,2,1,2,0,0,1,2,0,0,1,2,0,0,0,0,0,1,
    2,0,1,2,1,0,0,0,0,0,1,2,0,0,1,2,1,2,1,2,1,2,0,0,1,2,1,2,0,0,1,2,
    1,2,0,0,1,2,1,2,0,0,1,2,1,2,0,0,0,0,0,0,2,1,0,0,0,1,2,0,0,0,1,2,
    0,0,1,2,0,1,0,1,0,2,1,2,1,2,1,2,0,0,1,2,0,0,1,2,0,1,1,2,0,1,0,2,
    0,0,0,0,1,2,0,0,0,0,1,2,0,0,0,0,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,1,2,0,1,1,2,1,2,0,0,1,2,1,2,0,0,1,0,1,2,0,0,1,2,1,2,
    0,0,1,0,0,1,2,0,0,0,0,1,1,1,1,2,1,2,0,0,1,2,0,0,1,2,0,0,0,0,0,1,
    2,0,1,2,0,1,0,0,0,0,1,2,0,0,1,2,1,2,1,2,1,2,0,0,1,2,1,2,0,0,1,2,
    1,1,1,1,0,2,0,1,1,1,1,2,1,2,0,0,0,0,1,0,0,0,1,0,0,1,2,0,1,0,1,2,
    0,1,1,2,0,1,2,1,2,0,1,2,1,2,1,2,0,1,0,1,0,0,0,1,1,0,1,2,1,0,2,0,
    0,0,0,0,1,2,0,0,0,0,1,2,0,0,0,0,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,1,1,0,1,2,1,1,1,1,0,2,0,1,1,1,0,2,0,1,1,1,1,2,0,1,
    1,1,0,2,0,1,2,0,0,0,0,0,2,2,1,2,1,2,0,0,1,2,0,1,1,1,0,0,1,0,0,1,
    2,0,1,2,0,0,1,0,0,1,1,1,0,0,1,2,0,2,1,2,1,2,0,0,1,2,0,1,1,1,0,2,
    1,2,2,2,2,0,0,0,2,2,1,2,1,2,0,0,0,0,0,1,1,1,0,2,0,0,1,1,0,2,0,1,
    1,0,1,2,0,0,1,0,2,0,0,1,0,1,0,2,1,0,2,0,1,0,0,0,2,2,1,2,1,1,1,1,
    1,0,0,0,1,2,0,0,0,0,1,2,0,0,0,0,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,2,2,0,2,0,2,2,2,2,0,0,0,2,2,2,0,0,0,2,2,2,2,0,0,
    2,2,2,0,0,0,2,0,0,0,1,0,0,0,1,2,0,2,0,0,0,2,0,0,2,2,2,0,1,2,0,1,
    2,0,0,2,0,0,0,2,0,0,2,2,2,0,0,2,0,0,0,2,0,2,0,0,0,2,0,0,2,2,2,0,
    1,2,0,0,0,0,0,0,0,0,1,2,0,2,0,0,0,0,0,0,2,2,2,0,0,0,0,2,2,0,0,0,
    2,2,0,2,0,0,0,2,0,0,0,0,2,0,2,0,0,2,0,0,0,2,1,0,0,0,1,2,0,2,2,2,
    2,2,0,0,0,1,1,0,0,0,0,2,0,0,1,1,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,
    2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    1,2,0,0,0,0,0,0,0,0,1,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,2,0,0,0,0,
    0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,2,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

/*************************************************************************/
/************************** Interface functions **************************/
/*************************************************************************/

/**
 * font_printf:  Draw text to the screen at the given position and with the
 * given color.  The string can contain printf()-style format specifiers.
 *
 * It is assumed that a display list has been started with guStart()
 * before calling this function.
 *
 * [Parameters]
 *       x, y: Upper-left coordinates of first character
 *      align: Horizontal alignment (<0: left, 0: center, >0: right)
 *      color: Text color (0xAABBGGRR)
 *     format: Format string for text
 *        ...: Arguments for format string
 * [Return value]
 *     X coordinate immediately following the last character printed
 */
int font_printf(int x, int y, int align, uint32_t color,
                const char *format, ...)
{
    PRECOND(format != NULL, return x);

    /* Generate the text string to output */
    char buf[1000];  // Plenty long enough even if we start offscreen
    va_list args;
    va_start(args, format);
    vsnprintf(buf, sizeof(buf), format, args);
    va_end(args);
    const int nchars = strlen(buf);

    /* Adjust the starting X coordinate based on the alignment */
    if (align == 0) {
        x -= (nchars * CHAR_WIDTH + 1) / 2;
    } else if (align > 0) {
        x -= nchars * CHAR_WIDTH;
    }

    /* Generate vertices for each character of the string */
    struct {
        uint16_t u, v;
        int16_t x, y, z;
    } *vertices = guGetMemory(sizeof(*vertices) * (2*nchars));
    int i;
    for (i = 0; i < nchars; i++, x += CHAR_WIDTH) {
        uint8_t ch = buf[i];
        if (ch < FONTDATA_FIRSTCHAR || ch > FONTDATA_LASTCHAR) {
            ch = FONTDATA_DEFCHAR;
        }
        const unsigned int index = ch - FONTDATA_FIRSTCHAR;
        vertices[i*2+0].u = (index % FONTDATA_STRIDE) * CHAR_WIDTH;
        vertices[i*2+0].v = (index / FONTDATA_STRIDE) * FONT_HEIGHT;
        vertices[i*2+0].x = x;
        vertices[i*2+0].y = y;
        vertices[i*2+0].z = 0;
        vertices[i*2+1].u = (index % FONTDATA_STRIDE + 1) * CHAR_WIDTH;
        vertices[i*2+1].v = (index / FONTDATA_STRIDE + 1) * FONT_HEIGHT;
        vertices[i*2+1].x = x + CHAR_WIDTH;
        vertices[i*2+1].y = y + FONT_HEIGHT;
        vertices[i*2+1].z = 0;
    }
    sceKernelDcacheWritebackInvalidateRange(vertices,
                                            sizeof(*vertices) * (2*nchars));

    /* Set up drawing parameters and draw the string.  Unfortunately, color
     * lookup tables have to be 64-byte aligned, so we can potentially
     * waste a lot of memory here for just 3 color table entries... */
    uint32_t *clut = guGetMemory(3*4 + 60);
    clut = (uint32_t *)(((uintptr_t)clut + 63) & -64);
    clut[0] = 0x00000000;
    clut[1] = color;
    clut[2] = color & 0xFF000000;  // Shadow pixels
    guClutMode(GU_PSM_8888, 0, 0xFF, 0);
    guClutLoad(8/8, clut);  // We only use 3 of the 8 colors, but we have to
                            // send in a whole block
    guEnable(GU_TEXTURE_2D);
    guTexFlush();
    guTexMode(GU_PSM_T8, 0, 0, 0);
    guTexImage(0, 512, 512, (FONTDATA_STRIDE * CHAR_WIDTH), fontdata);
    guTexFilter(GU_NEAREST, GU_NEAREST);
    guTexWrap(GU_CLAMP, GU_CLAMP);
    guTexFunc(GU_TFX_MODULATE, GU_TCC_RGBA);
    guAmbientColor(0xFFFFFFFF);
    guDrawArray(GU_SPRITES,
                GU_TEXTURE_16BIT | GU_VERTEX_16BIT | GU_TRANSFORM_2D,
                2*nchars, NULL, vertices);
    guDisable(GU_TEXTURE_2D);

    /* Return the next X coordinate for potential further drawing */
    return x;
}

/*************************************************************************/
/*************************************************************************/

/*
 * Local variables:
 *   c-file-style: "stroustrup"
 *   c-file-offsets: ((case-label . *) (statement-case-intro . *))
 *   indent-tabs-mode: nil
 * End:
 *
 * vim: expandtab shiftwidth=4:
 */