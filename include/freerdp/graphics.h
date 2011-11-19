/**
 * FreeRDP: A Remote Desktop Protocol Client
 * Graphical Objects
 *
 * Copyright 2011 Marc-Andre Moreau <marcandre.moreau@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __GRAPHICS_H
#define __GRAPHICS_H

typedef struct rdp_bitmap rdpBitmap;
typedef struct rdp_pointer rdpPointer;
typedef struct rdp_glyph rdpGlyph;

#include <stdlib.h>
#include <freerdp/api.h>
#include <freerdp/types.h>
#include <freerdp/freerdp.h>

/* Bitmap Class */

typedef void (*pBitmap_New)(rdpContext* context, rdpBitmap* bitmap);
typedef void (*pBitmap_Free)(rdpContext* context, rdpBitmap* bitmap);
typedef void (*pBitmap_Paint)(rdpContext* context, rdpBitmap* bitmap);
typedef void (*pBitmap_Decompress)(rdpContext* context, rdpBitmap* bitmap,
		uint8* data, int width, int height, int bpp, int length, boolean compressed);
typedef void (*pBitmap_SetSurface)(rdpContext* context, rdpBitmap* bitmap, boolean primary);

struct rdp_bitmap
{
	size_t size;
	pBitmap_New New;
	pBitmap_Free Free;
	pBitmap_Paint Paint;
	pBitmap_Decompress Decompress;
	pBitmap_SetSurface SetSurface;
	uint32 paddingA[16 - 12]; /* offset 64 */

	uint32 left;
	uint32 top;
	uint32 right;
	uint32 bottom;
	uint32 width;
	uint32 height;
	uint32 bpp;
	uint32 flags;
	uint32 length;
	uint8* data;
	uint32 paddingB[16 - 12]; /* offset 128 */

	boolean compressed;
	boolean ephemeral;
	uint32 paddingC[16 - 2]; /* offset 192 */
};

FREERDP_API rdpBitmap* Bitmap_Alloc(rdpContext* context);
FREERDP_API void Bitmap_New(rdpContext* context, rdpBitmap* bitmap);
FREERDP_API void Bitmap_Free(rdpContext* context, rdpBitmap* bitmap);
FREERDP_API void Bitmap_Register(rdpContext* context, rdpBitmap* bitmap);
FREERDP_API void Bitmap_Decompress(rdpContext* context, rdpBitmap* bitmap,
		uint8* data, int width, int height, int bpp, int length, boolean compressed);
FREERDP_API void Bitmap_SetRectangle(rdpContext* context, rdpBitmap* bitmap,
		uint16 left, uint16 top, uint16 right, uint16 bottom);
FREERDP_API void Bitmap_SetDimensions(rdpContext* context, rdpBitmap* bitmap, uint16 width, uint16 height);
FREERDP_API void Bitmap_SetSurface(rdpContext* context, rdpBitmap* bitmap, boolean primary);

/* Pointer Class */

typedef void (*pPointer_New)(rdpContext* context, rdpPointer* pointer);
typedef void (*pPointer_Free)(rdpContext* context, rdpPointer* pointer);
typedef void (*pPointer_Set)(rdpContext* context, rdpPointer* pointer);

struct rdp_pointer
{
	size_t size;
	pPointer_New New;
	pPointer_Free Free;
	pPointer_Set Set;
	uint32 paddingA[16 - 8]; /* offset 64 */

	uint32 xPos;
	uint32 yPos;
	uint32 width;
	uint32 height;
	uint32 xorBpp;
	uint32 lengthAndMask;
	uint32 lengthXorMask;
	uint8* xorMaskData;
	uint8* andMaskData;
	uint32 paddingB[16 - 11]; /* offset 128 */
};

FREERDP_API rdpPointer* Pointer_Alloc(rdpContext* context);
FREERDP_API void Pointer_New(rdpContext* context, rdpPointer* pointer);
FREERDP_API void Pointer_Free(rdpContext* context, rdpPointer* pointer);
FREERDP_API void Pointer_Set(rdpContext* context, rdpPointer* pointer);

/* Glyph Class */

typedef void (*pGlyph_New)(rdpContext* context, rdpGlyph* glyph);
typedef void (*pGlyph_Free)(rdpContext* context, rdpGlyph* glyph);
typedef void (*pGlyph_Draw)(rdpContext* context, rdpGlyph* glyph, int x, int y);
typedef void (*pGlyph_BeginDraw)(rdpContext* context, int x, int y, int width, int height, uint32 bgcolor, uint32 fgcolor);
typedef void (*pGlyph_EndDraw)(rdpContext* context, int x, int y, int width, int height, uint32 bgcolor, uint32 fgcolor);

struct rdp_glyph
{
	size_t size;
	pGlyph_New New;
	pGlyph_Free Free;
	pGlyph_Draw Draw;
	pGlyph_BeginDraw BeginDraw;
	pGlyph_EndDraw EndDraw;
	uint32 paddingA[16 - 12]; /* offset 64 */

	sint32 x;
	sint32 y;
	uint32 cx;
	uint32 cy;
	uint32 cb;
	uint8* aj;
	uint32 paddingB[16 - 7]; /* offset 128 */
};

FREERDP_API rdpGlyph* Glyph_Alloc(rdpContext* context);
FREERDP_API void Glyph_New(rdpContext* context, rdpGlyph* glyph);
FREERDP_API void Glyph_Free(rdpContext* context, rdpGlyph* glyph);
FREERDP_API void Glyph_Draw(rdpContext* context, rdpGlyph* glyph, int x, int y);
FREERDP_API void Glyph_BeginDraw(rdpContext* context, int x, int y, int width, int height, uint32 bgcolor, uint32 fgcolor);
FREERDP_API void Glyph_EndDraw(rdpContext* context, int x, int y, int width, int height, uint32 bgcolor, uint32 fgcolor);

/* Graphics Module */

struct rdp_graphics
{
	rdpContext* context;
	rdpBitmap* Bitmap_Prototype;
	rdpPointer* Pointer_Prototype;
	rdpGlyph* Glyph_Prototype;
	uint32 paddingA[16 - 8]; /* offset 64 */
};

FREERDP_API void graphics_register_bitmap(rdpGraphics* graphics, rdpBitmap* bitmap);
FREERDP_API void graphics_register_pointer(rdpGraphics* graphics, rdpPointer* pointer);
FREERDP_API void graphics_register_glyph(rdpGraphics* graphics, rdpGlyph* glyph);

FREERDP_API rdpGraphics* graphics_new(rdpContext* context);
FREERDP_API void graphics_free(rdpGraphics* graphics);

#endif /* __GRAPHICS_H */
