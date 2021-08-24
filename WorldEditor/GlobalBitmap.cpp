// GlobalBitmap.cpp: implementation of the CGlobalBitmap class.
//
//////////////////////////////////////////////////////////////////////

#include "GlobalBitmap.h"

#define SAFE_DELETE_ARRAY(p)  { if(p) { delete [] (p);     (p)=NULL; } }

////////////////////////////////////////////////////////////////////////// CGlobalBitmap

GLuint CGlobalBitmap::NextBitmapIndex = 1;

CGlobalBitmap::CGlobalBitmap()
{
	Init();
}
CGlobalBitmap::~CGlobalBitmap()
{
	UnloadAllImages();
}
void CGlobalBitmap::Init()
{
	m_dwUsedTextureMemory = 0;
}

bool CGlobalBitmap::LoadImage(GLuint uiBitmapIndex, const std::string& filename, GLuint uiFilter, GLuint uiWrapMode)
{
	unsigned int UICLAMP = GL_CLAMP;
	unsigned int UIREPEAT = GL_REPEAT;

	NextBitmapIndex = max(NextBitmapIndex + 1, uiBitmapIndex + 1);

	type_bitmap_map::iterator mi = m_mapBitmap.find(uiBitmapIndex);
	if (mi != m_mapBitmap.end())
	{
		BITMAP_t* pBitmap = (*mi).second;
		if (pBitmap->Ref > 0)
		{
			if (0 == _stricmp(pBitmap->FileName, filename.c_str()))
			{
				pBitmap->Ref++;
				return true;
			}
			else
			{
				UnloadImage(uiBitmapIndex, true);	//. 강제로 지운다.
			}
		}
	}

	std::string ext;
	SplitExt(filename, ext, false);

	if (0 == _stricmp(ext.c_str(), "jpg"))
		return OpenJpeg(uiBitmapIndex, filename, uiFilter, uiWrapMode);
	else if (0 == _stricmp(ext.c_str(), "tga"))
		return OpenTga(uiBitmapIndex, filename, uiFilter, uiWrapMode);

	return false;
}
void CGlobalBitmap::UnloadImage(GLuint uiBitmapIndex, bool bForce)
{
	type_bitmap_map::iterator mi = m_mapBitmap.find(uiBitmapIndex);
	if (mi != m_mapBitmap.end())
	{
		BITMAP_t* pBitmap = (*mi).second;

		if (--pBitmap->Ref == 0 || bForce)
		{
			// OpenGL 텍스쳐 삭제
			glDeleteTextures(1, &(pBitmap->TextureNumber));

			m_dwUsedTextureMemory -= (DWORD)(pBitmap->Width * pBitmap->Height * pBitmap->Components);

			delete[] pBitmap->Buffer;
			delete pBitmap;
			m_mapBitmap.erase(mi);
		}
	}
}
void CGlobalBitmap::UnloadAllImages()
{
	type_bitmap_map::iterator mi = m_mapBitmap.begin();
	for (; mi != m_mapBitmap.end(); mi++)
	{
		BITMAP_t* pBitmap = (*mi).second;

		delete[] pBitmap->Buffer;
		delete pBitmap;
	}

	m_mapBitmap.clear();

	Init();
}

BITMAP_t* CGlobalBitmap::GetTexture(GLuint uiBitmapIndex)
{
	BITMAP_t* pBitmap = NULL;

	type_bitmap_map::iterator mi = m_mapBitmap.find(uiBitmapIndex);	//. 맵에서 찾는다.
	if (mi != m_mapBitmap.end())
		pBitmap = (*mi).second;

	if (NULL == pBitmap)	//. 없는 인덱스라면
	{
		static BITMAP_t s_Error;
		memset(&s_Error, 0, sizeof(BITMAP_t));
		strcpy(s_Error.FileName, "CGlobalBitmap::GetTexture Error!!!");
		pBitmap = &s_Error;
	}
	return pBitmap;
}
BITMAP_t* CGlobalBitmap::FindTexture(GLuint uiBitmapIndex)
{
	BITMAP_t* pBitmap = NULL;
	type_bitmap_map::iterator mi = m_mapBitmap.find(uiBitmapIndex);	//. 맵에서 찾는다.
	if (mi != m_mapBitmap.end())
		pBitmap = (*mi).second;
	return pBitmap;
}

BITMAP_t* CGlobalBitmap::FindTexture(const std::string& filename)
{
	type_bitmap_map::iterator mi = m_mapBitmap.begin();
	for (; mi != m_mapBitmap.end(); mi++)
	{
		BITMAP_t* pBitmap = (*mi).second;
		if (0 == _stricmp(filename.c_str(), pBitmap->FileName))
			return pBitmap;
	}
	return NULL;
}

BITMAP_t* CGlobalBitmap::FindTextureByName(const std::string& name)
{
	type_bitmap_map::iterator mi = m_mapBitmap.begin();
	for (; mi != m_mapBitmap.end(); mi++)
	{
		BITMAP_t* pBitmap = (*mi).second;
		std::string texname;
		SplitFileName(pBitmap->FileName, texname, true);
		if (0 == _stricmp(texname.c_str(), name.c_str()))
			return pBitmap;
	}
	return NULL;
}

DWORD CGlobalBitmap::GetUsedTextureMemory() const
{
	return m_dwUsedTextureMemory;
}
size_t CGlobalBitmap::GetNumberOfTexture() const
{
	return m_mapBitmap.size();
}

bool CGlobalBitmap::OpenJpeg(GLuint uiBitmapIndex, const std::string& filename, GLuint uiFilter, GLuint uiWrapMode)
{
	std::string filename_ozj;
	ExchangeExt(filename, "OZJ", filename_ozj);

	FILE* infile = fopen(filename_ozj.c_str(), "rb");
	if (infile == NULL)
	{
		return false;
	}

	fseek(infile, 24, SEEK_SET);	//. Skip Dump

	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr;
	cinfo.err = jpeg_std_error(&jerr.pub);
	jerr.pub.error_exit = my_error_exit;
	if (setjmp(jerr.setjmp_buffer))
	{
		jpeg_destroy_decompress(&cinfo);
		fclose(infile);

		return false;
	}

	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, infile);
	(void)jpeg_read_header(&cinfo, TRUE);
	(void)jpeg_start_decompress(&cinfo);

	if (cinfo.output_width <= MAX_WIDTH && cinfo.output_height <= MAX_HEIGHT)
	{
		int Width, Height;
		for (int i = 1; i <= MAX_WIDTH; i <<= 1)
		{
			Width = i;
			if (i >= (int)cinfo.output_width) break;
		}
		for (int i = 1; i <= MAX_HEIGHT; i <<= 1)
		{
			Height = i;
			if (i >= (int)cinfo.output_height) break;
		}

		BITMAP_t* pNewBitmap = new BITMAP_t;
		memset(pNewBitmap, 0, sizeof(BITMAP_t));

		pNewBitmap->BitmapIndex = uiBitmapIndex;
		filename._Copy_s(pNewBitmap->FileName, MAX_BITMAP_FILE_NAME, MAX_BITMAP_FILE_NAME);

		pNewBitmap->Width = (float)Width;
		pNewBitmap->Height = (float)Height;
		pNewBitmap->Components = 3;
		pNewBitmap->Ref = 1;

		size_t BufferSize = Width * Height * pNewBitmap->Components;
		pNewBitmap->Buffer = new BYTE[BufferSize];
		m_dwUsedTextureMemory += BufferSize;

		size_t offset = 0;
		size_t row_stride = cinfo.output_width * cinfo.output_components;
		JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);
		while (cinfo.output_scanline < cinfo.output_height)
		{
			if (offset + row_stride > BufferSize)
				break;

			(void)jpeg_read_scanlines(&cinfo, buffer, 1);
			memcpy(pNewBitmap->Buffer + (cinfo.output_scanline - 1) * Width * 3, buffer[0], row_stride);
			offset += row_stride;
		}

		m_mapBitmap.insert(type_bitmap_map::value_type(uiBitmapIndex, pNewBitmap));

		glGenTextures(1, &(pNewBitmap->TextureNumber));

		glBindTexture(GL_TEXTURE_2D, pNewBitmap->TextureNumber);

		glTexImage2D(GL_TEXTURE_2D, 0, 3, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, pNewBitmap->Buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, uiFilter);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, uiFilter);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, uiWrapMode);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, uiWrapMode);

	}
	(void)jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(infile);

	return true;
}
bool CGlobalBitmap::OpenTga(GLuint uiBitmapIndex, const std::string& filename, GLuint uiFilter, GLuint uiWrapMode)
{
	std::string filename_ozt;
	ExchangeExt(filename, "OZT", filename_ozt);

	// 파일 오픈과 실패시 처리.
	FILE* fp = fopen(filename_ozt.c_str(), "rb");
	if (fp == NULL)
	{
		return false;
	}

	// 파일 크기를 얻음.
	fseek(fp, 0, SEEK_END);
	int Size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	// 오픈한 파일 내용을 담을 메모리 할당 후 파일을 읽음.
	unsigned char* PakBuffer = new unsigned char[Size];
	fread(PakBuffer, 1, Size, fp);
	fclose(fp);

	// OZT확장자 헤더정보를 읽음.
	// 인덱스 16(12 + 4)부터.
	int index = 12;
	index += 4;
	// 인덱스 16은 이미지 가로크기.
	short nx = *((short*)(PakBuffer + index)); index += 2;
	// 인덱스 18은 이미지 세로크기.
	short ny = *((short*)(PakBuffer + index)); index += 2;
	// 인덱스 20은 이미지 도트의 color bit.(예: 32bit, 16bit)
	char bit = *((char*)(PakBuffer + index)); index += 1;
	index += 1;	// 현재 인덱스는 22.

	// 32bit color가 아니거나 이미지 최대 크기를 넘으면 실패.
	if (bit != 32 || nx > MAX_WIDTH || ny > MAX_HEIGHT)
	{
		SAFE_DELETE_ARRAY(PakBuffer);

		return false;
	}

	// 텍스처를 만들기 위해 텍스처 크기 정함. 2의 자승으로 증가(<<=1)
	int Width = 0, Height = 0;
	for (int i = 1; i <= MAX_WIDTH; i <<= 1)
	{
		Width = i;
		if (i >= nx) break;
	}
	for (int i = 1; i <= MAX_HEIGHT; i <<= 1)
	{
		Height = i;
		if (i >= ny) break;
	}

	// BITMAP_t 구조체 채움.
	BITMAP_t* pNewBitmap = new BITMAP_t;
	memset(pNewBitmap, 0, sizeof(BITMAP_t));

	pNewBitmap->BitmapIndex = uiBitmapIndex;
	filename._Copy_s(pNewBitmap->FileName, MAX_BITMAP_FILE_NAME, MAX_BITMAP_FILE_NAME);

	pNewBitmap->Width = (float)Width;
	pNewBitmap->Height = (float)Height;
	pNewBitmap->Components = 4;	//ARGB 등의 요소 수(4이므로 32bit이며 4byte임) 
	pNewBitmap->Ref = 1;

	size_t BufferSize = Width * Height * pNewBitmap->Components;
	pNewBitmap->Buffer = (unsigned char*)new BYTE[BufferSize];
	// m_dwUsedTextureMemory는 게임엔 사용하지 않고 개발자 확인용. #define ENABLE_EDIT 관련.
	m_dwUsedTextureMemory += BufferSize;

	for (int y = 0; y < ny; y++)
	{
		// 파일에서 읽어들인 데이터.
		unsigned char* src = &PakBuffer[index];
		index += nx * 4;
		// 텍스처 데이터. 밑에서부터 위로 채움.(TGA 파일의 특성 때문인 듯)
		unsigned char* dst = &pNewBitmap->Buffer[(ny - 1 - y) * Width * pNewBitmap->Components];

		// TGA와 비트맵의 ARGB형식이 다르므로.
		for (int x = 0; x < nx; x++)
		{
			dst[0] = src[2];
			dst[1] = src[1];
			dst[2] = src[0];
			dst[3] = src[3];
			src += 4;
			dst += pNewBitmap->Components;
		}
	}
	SAFE_DELETE_ARRAY(PakBuffer);

	m_mapBitmap.insert(type_bitmap_map::value_type(uiBitmapIndex, pNewBitmap));

	glGenTextures(1, &(pNewBitmap->TextureNumber));		// 텍스처 ID(pNewBitmap->TextureNumber) 얻음.

	glBindTexture(GL_TEXTURE_2D, pNewBitmap->TextureNumber);	// 텍스처 바인드.

	// 텍스처 이미지 정의.
	glTexImage2D(GL_TEXTURE_2D, 0, 4, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pNewBitmap->Buffer);

	// 텍스처 효과 정의.
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// 텍스처 필터링.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, uiFilter);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, uiFilter);

	// 텍스처 랩핑.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, uiWrapMode);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, uiWrapMode);

	return true;
}

void CGlobalBitmap::SplitFileName(IN const std::string& filepath, OUT std::string& filename, bool bIncludeExt)
{
	char __fname[_MAX_FNAME] = { 0, };
	char __ext[_MAX_EXT] = { 0, };
	_splitpath(filepath.c_str(), NULL, NULL, __fname, __ext);
	filename = __fname;
	if (bIncludeExt)
		filename += __ext;
}
void CGlobalBitmap::SplitExt(IN const std::string& filepath, OUT std::string& ext, bool bIncludeDot)
{
	char __ext[_MAX_EXT] = { 0, };
	_splitpath(filepath.c_str(), NULL, NULL, NULL, __ext);
	if (bIncludeDot) {
		ext = __ext;
	}
	else {
		if ((__ext[0] == '.') && __ext[1])
			ext = __ext + 1;
	}
}
void CGlobalBitmap::ExchangeExt(IN const std::string& in_filepath, IN const std::string& ext, OUT std::string& out_filepath)
{
	char __drive[_MAX_DRIVE] = { 0, };
	char __dir[_MAX_DIR] = { 0, };
	char __fname[_MAX_FNAME] = { 0, };
	_splitpath(in_filepath.c_str(), __drive, __dir, __fname, NULL);

	out_filepath = __drive;
	out_filepath += __dir;
	out_filepath += __fname;
	out_filepath += '.';
	out_filepath += ext;
}

bool CGlobalBitmap::Convert_Format(const std::string& filename)
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	::_splitpath(filename.c_str(), drive, dir, fname, ext);

	std::string strPath = drive; strPath += dir;
	std::string strName = fname;

	if (_stricmp(ext, ".jpg") == 0)
	{
		const std::string strSaveName = strPath + strName + ".OZJ";
		return Save_Image(filename, strSaveName.c_str(), 24);
	}
	else if (_stricmp(ext, ".tga") == 0)
	{
		const std::string strSaveName = strPath + strName + ".OZT";
		return Save_Image(filename, strSaveName.c_str(), 4);
	}
	else if (_stricmp(ext, ".bmp") == 0)
	{
		const std::string strSaveName = strPath + strName + ".OZB";
		return Save_Image(filename, strSaveName.c_str(), 4);
	}
	else
	{
		// 이미지 포멧이 다릅니다.
	}

	return false;
}

bool CGlobalBitmap::Save_Image(const std::string& src, const std::string& dest, int cDumpHeader)
{
	FILE* fp = fopen(src.c_str(), "rb");
	if (fp == NULL)
	{
		return false;
	}

	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char* pTempBuf = new char[size];
	fread(pTempBuf, 1, size, fp);
	fclose(fp);

	fp = fopen(dest.c_str(), "wb");
	if (fp == NULL)
		return false;

	fwrite(pTempBuf, 1, cDumpHeader, fp);
	fwrite(pTempBuf, 1, size, fp);
	fclose(fp);

	delete[] pTempBuf;

	return true;
}

void CGlobalBitmap::my_error_exit(j_common_ptr cinfo)
{
	my_error_ptr myerr = (my_error_ptr)cinfo->err;
	(*cinfo->err->output_message) (cinfo);
	longjmp(myerr->setjmp_buffer, 1);
}


////////////////////////////////////////////////////////////////////////// CGlobalBitmap