///////////////////////////////////////////////////////////////////////////////
// Jpeg, Tga 이미지 파일을 읽어들이는 함수
// 수정할 일이 거의 없을것 같음
//
// *** 함수 레벨: 1
///////////////////////////////////////////////////////////////////////////////

#include <setjmp.h>
#include "ZzzTexture.h"
#include "jpeg-6b\jpeglib.h"
#include "libpng/lpng1637/png.h"

#define SAFE_DELETE_ARRAY(p)  { if(p) { delete [] (p);     (p)=NULL; } }

CGlobalBitmap Bitmaps;

struct my_error_mgr {
	struct jpeg_error_mgr pub;
	jmp_buf setjmp_buffer;
};

typedef struct my_error_mgr* my_error_ptr;

METHODDEF(void) my_error_exit(j_common_ptr cinfo)
{
	my_error_ptr myerr = (my_error_ptr)cinfo->err;
	(*cinfo->err->output_message) (cinfo);
	longjmp(myerr->setjmp_buffer, 1);
}

bool WriteJpeg(const char* filename, int Width, int Height, unsigned char* Buffer, int quality)
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;
	FILE* outfile;
	JSAMPROW row_pointer[1];
	int row_stride;

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);

	if ((outfile = fopen(filename, "wb")) == NULL)
	{
		//fprintf(stderr, "can't open %s\n", filename);
		//exit(1);
		return FALSE;
	}
	jpeg_stdio_dest(&cinfo, outfile);

	cinfo.image_width = Width;
	cinfo.image_height = Height;
	cinfo.input_components = 3;
	cinfo.in_color_space = JCS_RGB;
	jpeg_set_defaults(&cinfo);
	jpeg_set_quality(&cinfo, quality, TRUE);
	jpeg_start_compress(&cinfo, TRUE);
	row_stride = cinfo.image_width * 3;

	while (cinfo.next_scanline < cinfo.image_height)
	{
		row_pointer[0] = &Buffer[(Height - 1 - cinfo.next_scanline) * row_stride];
		(void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	jpeg_finish_compress(&cinfo);
	fclose(outfile);
	jpeg_destroy_compress(&cinfo);
	return TRUE;
}

void SaveImage(int HeaderSize, char* Ext, char* filename, BYTE* PakBuffer, int Size)
{
	if (PakBuffer == NULL || Size == 0)
	{
		char OpenFileName[256];
		strcpy(OpenFileName, "Data2\\");
		strcat(OpenFileName, filename);
		FILE* fp = fopen(OpenFileName, "rb");
		if (fp == NULL)
		{
			return;
		}
		fseek(fp, 0, SEEK_END);
		Size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		PakBuffer = new unsigned char[Size];
		fread(PakBuffer, 1, Size, fp);
		fclose(fp);
	}

	char Header[24];
	memcpy(Header, PakBuffer, HeaderSize);

	char NewFileName[256];

	int iTextcnt = 0;
	for (int i = 0; i < (int)strlen(filename); i++)
	{
		iTextcnt = i;
		NewFileName[i] = filename[i];
		if (filename[i] == '.') break;
	}
	NewFileName[iTextcnt + 1] = NULL;

	strcat(NewFileName, Ext);
	char SaveFileName[256];
	strcpy(SaveFileName, "Data\\");
	strcat(SaveFileName, NewFileName);
	FILE* fp = fopen(SaveFileName, "wb");
	if (fp == NULL) return;
	fwrite(Header, 1, HeaderSize, fp);
	fwrite(PakBuffer, 1, Size, fp);
	fclose(fp);

	if (PakBuffer == NULL || Size == 0)
	{
		SAFE_DELETE_ARRAY(PakBuffer);
	}
}

bool OpenJpegBuffer(char* filename, float* BufferFloat)
{
	struct jpeg_decompress_struct cinfo;
	struct my_error_mgr jerr;
	FILE* infile;
	JSAMPARRAY buffer;
	int row_stride;

	char FileName[256];

	char NewFileName[256];
	int iTextcnt = 0;
	for (int i = 0; i < (int)strlen(filename); i++)
	{
		iTextcnt = i;
		NewFileName[i] = filename[i];
		if (filename[i] == '.') break;
	}
	NewFileName[iTextcnt + 1] = NULL;
	strcpy(FileName, "Data\\");
	strcat(FileName, NewFileName);
	strcat(FileName, "OZJ");

	if ((infile = fopen(FileName, "rb")) == NULL)
	{
		char Text[256];
		sprintf(Text, "%s - File not exist.", FileName);
		OutputDebugStringA(Text);
		return false;
	}

	fseek(infile, 24, SEEK_SET);

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
	row_stride = cinfo.output_width * cinfo.output_components;
	buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);

	unsigned char* Buffer = (unsigned char*) new BYTE[cinfo.output_width * cinfo.output_height * cinfo.output_components];
	while (cinfo.output_scanline < cinfo.output_height)
	{
		(void)jpeg_read_scanlines(&cinfo, buffer, 1);
		memcpy(Buffer + (cinfo.output_height - cinfo.output_scanline) * row_stride, buffer[0], row_stride);
	}
	int Index = 0;
	for (unsigned int y = 0; y < cinfo.output_height; y++)
	{
		for (unsigned int x = 0; x < cinfo.output_width; x++)
		{
			BufferFloat[Index] = (float)Buffer[Index] / 255.f;
			BufferFloat[Index + 1] = (float)Buffer[Index + 1] / 255.f;
			BufferFloat[Index + 2] = (float)Buffer[Index + 2] / 255.f;
			Index += 3;
		}
	}
	SAFE_DELETE_ARRAY(Buffer);

	(void)jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	fclose(infile);
	return true;
}

bool LoadBitmap(const char* szFileName, GLuint uiTextureIndex, GLuint uiFilter, GLuint uiWrapMode, bool bCheck, bool bFullPath)
{

	char szFullPath[256] = { 0, };
	if (bFullPath == true)
	{
		strcpy(szFullPath, szFileName);
	}
	else
	{
		strcpy(szFullPath, "Data\\");
		strcat(szFullPath, szFileName);
	}

	if (bCheck)
	{
		if (false == Bitmaps.LoadImage(uiTextureIndex, szFullPath, uiFilter, uiWrapMode))
		{
			char szErrorMsg[256] = { 0, };
			sprintf(szErrorMsg, "LoadBitmap Failed: %s", szFullPath);
			OutputDebugStringA(szErrorMsg);

			return false;
		}
		return true;
	}

	return Bitmaps.LoadImage(uiTextureIndex, szFullPath, uiFilter, uiWrapMode);
}

void DeleteBitmap(GLuint uiTextureIndex, bool bForce)
{
	Bitmaps.UnloadImage(uiTextureIndex, bForce);
}

void WriteMapPng(const char* szFileName, std::vector<BYTE>& buffer)
{
	if (buffer.size() != 4 * 512 * 512) return;

	FILE* fp = fopen(szFileName, "wb");
	if (!fp) return;

	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) return;

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) return;

	if (setjmp(png_jmpbuf(png_ptr))) return;
	png_init_io(png_ptr, fp);

	png_set_IHDR(png_ptr, info_ptr, 512, 512,
		8, PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	png_text title_text;
	title_text.compression = PNG_TEXT_COMPRESSION_NONE;
	title_text.key = (char*)"Title";
	title_text.text = (char*)szFileName;
	png_set_text(png_ptr, info_ptr, &title_text, 1);

	png_write_info(png_ptr, info_ptr);

	for (int i = 0; i < 512; i++)
	{
		png_bytep row = &buffer[(511 - i) * 4 * 512];
		png_write_row(png_ptr, row);
	}

	png_write_end(png_ptr, NULL);

	if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
	if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);

	fclose(fp);
}

//FIXME: I feel wrong with the +2 (TerrainHeight.bmp 64K)
void WriteMapBitmap1(const char* szFileName, std::vector<BYTE>& buffer)
{
	constexpr size_t BITS_SIZE = 256 * 256;
	if (buffer.size() != BITS_SIZE) return;

	constexpr size_t HEADER_SIZE =	// 1078
		sizeof(BITMAPFILEHEADER)
		+ sizeof(BITMAPINFOHEADER)
		+ 0x400;	//bmiColors

	constexpr size_t FILE_SIZE =	// 66616
		HEADER_SIZE
		+ 2							// +2 ??? Webzen's mess
		+ BITS_SIZE;

	constexpr BITMAPFILEHEADER header
	{
		0x4D42,			//bfType		//"BM"
		FILE_SIZE,		//bfSize
		0,				//bfReserved1
		0,				//bfReserved2
		HEADER_SIZE		//bfOffBits
	};

	constexpr BITMAPINFOHEADER bmiHeader
	{
		sizeof(BITMAPINFOHEADER),	//biSize
		256,						//biWidth
		256,						//biHeight
		1,							//biPlanes
		8,							//biBitCount		// 1 BYTE
		BI_RGB,						//biCompression		// 0 (no compression)
		0,							//biSizeImage		// 0 (for BI_RGB)
		2834,						//biXPelsPerMeter	// not important 
		2834,						//biYPelsPerMeter	// (72 DPI × 39.3701 inches per meter yields 2834.6472)
		0,							//biClrUsed
		0							//biClrImportant
	};

	//create colors data only once with static magic
	static std::vector<RGBQUAD> bmiColors;
	if (bmiColors.empty())
	{	//Create ref colors table (biBitCount = 8). 
		bmiColors.resize(256);
		for (int i = 0; i < 256; i++)
		{
			bmiColors[i].rgbRed = bmiColors[i].rgbGreen = bmiColors[i].rgbBlue = (BYTE)i;
			bmiColors[i].rgbReserved = 0;
		}
	}

	FILE* fp = fopen(szFileName, "wb");
	if (!fp) return;
	fwrite(&header, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&bmiHeader, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(bmiColors.data(), bmiColors.size() * sizeof(RGBQUAD), 1, fp);
	fwrite(buffer.data(), 2, 1, fp);
	fwrite(buffer.data(), buffer.size(), 1, fp);
	fclose(fp);
}


void WriteMapBitmap3(const char* szFileName, std::vector<BYTE>& buffer)
{
	constexpr size_t SZ = 256 * 256;
	constexpr size_t BITS_SIZE = SZ * 3;
	if (buffer.size() != BITS_SIZE) return;

	constexpr size_t HEADER_SIZE =	// 54
		sizeof(BITMAPFILEHEADER)
		+ sizeof(BITMAPINFOHEADER);
	constexpr size_t FILE_SIZE =	// 196662
		HEADER_SIZE
		+ BITS_SIZE;

	constexpr BITMAPFILEHEADER header
	{
		0x4D42,			//bfType		//"BM"
		FILE_SIZE,		//bfSize
		0,				//bfReserved1
		0,				//bfReserved2
		HEADER_SIZE		//bfOffBits
	};

	constexpr BITMAPINFOHEADER bmiHeader
	{
		sizeof(BITMAPINFOHEADER),	//biSize
		256,						//biWidth
		256,						//biHeight
		1,							//biPlanes
		24,							//biBitCount		// 3 BYTES
		BI_RGB,						//biCompression		// 0 (no compression)
		0,							//biSizeImage		// 0 (for BI_RGB)
		0,							//biXPelsPerMeter	// not important 
		0,							//biYPelsPerMeter
		0,							//biClrUsed
		0							//biClrImportant
	};

	FILE* fp = fopen(szFileName, "wb");
	if (!fp) return;
	fwrite(&header, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&bmiHeader, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(buffer.data(), buffer.size(), 1, fp);
	fclose(fp);
}