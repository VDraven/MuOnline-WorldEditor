#ifndef __ZZZTEXTURE_H__
#define __ZZZTEXTURE_H__

#include "GlobalBitmap.h"
extern CGlobalBitmap Bitmaps;

#undef LoadBitmap

extern bool OpenJpegBuffer(char* filename, float* BufferFloat);
extern bool WriteJpeg(const char* filename, int Width, int Height, unsigned char* Buffer, int quality);
extern void SaveImage(int HeaderSize, char* Ext, char* filename, BYTE* PakBuffer, int Size);

bool LoadBitmap(const char* szFileName, GLuint uiTextureIndex, GLuint uiFilter = GL_NEAREST, GLuint uiWrapMode = GL_CLAMP, bool bCheck = true, bool bFullPath = false);
void DeleteBitmap(GLuint uiTextureIndex, bool bForce = false);

#endif// __ZZZTEXTURE_H__
#pragma once
