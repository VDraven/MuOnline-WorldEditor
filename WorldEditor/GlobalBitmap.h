// GlobalBitmap.h: interface for the CGlobalBitmap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOBALBITMAP_H__D28A9680_1E6A_49C8_995B_1E90654A074E__INCLUDED_)
#define AFX_GLOBALBITMAP_H__D28A9680_1E6A_49C8_995B_1E90654A074E__INCLUDED_

#pragma once

#include <setjmp.h>
#include "jpeg-6b\jpeglib.h"


#undef LoadImage

#define MAX_BITMAP_FILE_NAME 256

#pragma pack(push, 1)
class BITMAP_t
{
public:
	BITMAP_t() { memset(this, 0, sizeof(BITMAP_t)); };

	GLuint	BitmapIndex;
	char	FileName[MAX_BITMAP_FILE_NAME];
	float	Width;
	float	Height;
	float	RealWidth;
	float	RealHeight;
	char	Components;
	GLuint	TextureNumber;
	BYTE	Ref;
	BYTE* Buffer;

private:
	DWORD	dwCallCount;
};
#pragma pack(pop)


class CGlobalBitmap
{
	struct my_error_mgr
	{
		struct jpeg_error_mgr pub;
		jmp_buf setjmp_buffer;
	};
	typedef struct my_error_mgr* my_error_ptr;

	enum
	{
		MAX_WIDTH = 1024,
		MAX_HEIGHT = 1024,
	};

	typedef std::map<GLuint, BITMAP_t*, std::less<GLuint> >	type_bitmap_map;
	typedef std::list<GLuint> type_index_list;

	type_bitmap_map	m_mapBitmap;

	DWORD	m_dwUsedTextureMemory;

	void Init();
public:
	static GLuint NextBitmapIndex;

	CGlobalBitmap();
	virtual ~CGlobalBitmap();

	bool LoadImage(GLuint uiBitmapIndex, const std::string& filename, GLuint uiFilter = GL_NEAREST, GLuint uiWrapMode = GL_CLAMP);
	void UnloadImage(GLuint uiBitmapIndex, bool bForce = false);
	void UnloadAllImages();

	BITMAP_t* GetTexture(GLuint uiBitmapIndex);
	BITMAP_t* FindTexture(GLuint uiBitmapIndex);
	BITMAP_t* FindTexture(const std::string& filename);
	BITMAP_t* FindTextureByName(const std::string& name);

	DWORD GetUsedTextureMemory() const;
	size_t GetNumberOfTexture() const;

	bool Convert_Format(const std::string& filename);

	inline BITMAP_t& operator [] (GLuint uiBitmapIndex) { return *GetTexture(uiBitmapIndex); }

protected:
	bool OpenJpeg(GLuint uiBitmapIndex, const std::string& filename, GLuint uiFilter = GL_NEAREST, GLuint uiWrapMode = GL_CLAMP);
	bool OpenTga(GLuint uiBitmapIndex, const std::string& filename, GLuint uiFilter = GL_NEAREST, GLuint uiWrapMode = GL_CLAMP);
	void SplitFileName(IN const std::string& filepath, OUT std::string& filename, bool bIncludeExt);
	void SplitExt(IN const std::string& filepath, OUT std::string& ext, bool bIncludeDot);
	void ExchangeExt(IN const std::string& in_filepath, IN const std::string& ext, OUT std::string& out_filepath);

	bool Save_Image(const std::string& src, const std::string& dest, int cDumpHeader);

	static void my_error_exit(j_common_ptr cinfo);
};

#endif // !defined(AFX_GLOBALBITMAP_H__D28A9680_1E6A_49C8_995B_1E90654A074E__INCLUDED_)
