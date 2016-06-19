/* 

Amine Rehioui
Created: April 22nd 2012

*/

#include "Shoot.h"

#include "PNGLoader.h"

#include "File.h"

#ifndef SHOOT_FILE_SYSTEM
#include "png.h"
#include "pnginfo.h"
#endif // SHOOT_FILE_SYSTEM

namespace shoot
{

#ifndef SHOOT_FILE_SYSTEM
	//! PNG malloc
	png_voidp PNGMalloc(png_structp png_ptr, png_size_t size)
	{
		return snew u8[size];
	}

	//! PNG free
    void PNGFree(png_structp png_ptr, png_voidp ptr)
	{
		delete[] (u8*)ptr;
	}

	//! PNG error
	void PNGError(png_structp png_ptr, png_const_charp msg)
	{
		SHOOT_ASSERT(false, msg);
	}

	//! PNG warning
	void PNGWarning(png_structp png_ptr, png_const_charp msg)
	{
		SHOOT_WARNING(false, msg);
	}

	//! PNG read
	void PNGRead(png_structp png_ptr, png_bytep data, png_size_t length)
	{
		File* pFile = (File*)png_get_io_ptr(png_ptr);
		pFile->Read(data, length);
	}
#endif // SHOOT_FILE_SYSTEM

	//! loads a texture
	void* PNGLoader::Load(const char* strPath, s32& width, s32& height, s32& channels, u32& bpp)
	{
#ifdef SHOOT_FILE_SYSTEM
		return LoadFS(strPath, width, height, channels, bpp);
#else
		File* pFile = File::Create(strPath, File::M_ReadBinary);
		if(!pFile->Open())
		{
			sdelete(pFile);
			return NULL;
		}

		png_structp png_ptr = png_create_read_struct_2(PNG_LIBPNG_VER_STRING, 
			/*error_ptr*/NULL,
			PNGError,
			PNGWarning,
			/*mem_ptr*/NULL, 
			PNGMalloc,
			PNGFree);

		SHOOT_ASSERT(png_ptr, "png_create_read_struct_2 failed");

		png_infop info_ptr = png_create_info_struct(png_ptr);
		SHOOT_ASSERT(info_ptr, "png_create_info_struct failed");

		png_set_read_fn(png_ptr, pFile, PNGRead);

		u32 sig_read = 0;
		png_set_sig_bytes(png_ptr, sig_read);
		
		png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

		width = info_ptr->width;
		height = info_ptr->height;

		switch (info_ptr->color_type)
		{
		case PNG_COLOR_TYPE_RGBA:
			channels = 4;
			break;

		case PNG_COLOR_TYPE_RGB:
			channels = 3;
			break;

		default: SHOOT_ASSERT(false, "Unsupported PNG format");
		}

		s32 row_bytes = png_get_rowbytes(png_ptr, info_ptr);
		SHOOT_ASSERT(row_bytes == width*channels, "PNG row_bytes inconsistent with image format");
		png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

		u8* pngData = snew u8[row_bytes * height];
		for(s32 i=0; i<height; ++i)
		{
			memcpy(pngData+(row_bytes*i), row_pointers[i], row_bytes);
		}

		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

		pFile->Close();
		sdelete(pFile);
				
		bool bForce32bits = std::string(strPath).rfind("32") != std::string::npos;
		if(bForce32bits)
		{
#ifdef DX11
			bpp = 4; // it seems there is no 24bit RGB format in DX11 so always use RBGA
#else
			bpp = (channels == 3) ? 3 : 4;
#endif
		}
		else		
		{
			// attempt to convert to 16 bit data to save space
#ifdef DX11
			bpp = (channels == 3) ? 2 : 4; // 16bit RGBA (4444) doesnt have mipmapping in DX11, so keep 32bits for RGBA textures.
#else
			bpp = 2;
#endif
		}

		if(bpp != channels)
		{
			if(bpp == 2)
			{
				u16* pData = snew u16[width*height];
				s32 row_bytes = width*channels;
				switch(channels)
				{
				case 4:
					for (s32 i=0; i<height; ++i)
					{
						for (s32 j=0; j<row_bytes; j += channels)
						{
							u8 r = pngData[i*row_bytes + j + 0];
							u8 g = pngData[i*row_bytes + j + 1];
							u8 b = pngData[i*row_bytes + j + 2];
							u8 a = pngData[i*row_bytes + j + 3];
							r = u8((f32(r)*0xf)/0xff)&0xf;
							g = u8((f32(g)*0xf)/0xff)&0xf;
							b = u8((f32(b)*0xf)/0xff)&0xf;
							a = u8((f32(a)*0xf)/0xff)&0xf;
							s32 column = j/channels;
							s32 index = i*width + column;
							pData[index] = (r<<12) | (g<<8) | (b<<4) | a;
						}
					}
					break;

				case 3:
					for (s32 i=0; i<height; ++i)
					{
						for (s32 j=0; j<row_bytes; j += channels)
						{
							u8 r = pngData[i*row_bytes + j + 0];
							u8 g = pngData[i*row_bytes + j + 1];
							u8 b = pngData[i*row_bytes + j + 2];
							r = u8((f32(r)*0x1f)/0xff)&0x1f;
							g = u8((f32(g)*0x3f)/0xff)&0x3f;
							b = u8((f32(b)*0x1f)/0xff)&0x1f;
							s32 column = j/channels;
							s32 index = i*width + column;
							pData[index] = (r<<11) | (g<<5) | b;
						}
					}
					break;
				}

				sdelete_array(pngData);
				return pData;
			}
			else if(bpp == 4)
			{
				// convert to 32 bits
				if(channels == 3)
				{
					u8* pData = snew u8[width*height*bpp];
					s32 row_bytes = width*channels;
					u32 curByte = 0;
					
					for (s32 i=0; i<height; ++i)
					{
						for (s32 j=0; j<row_bytes; j += channels)
						{
							u8 r = pngData[i*row_bytes + j + 0];
							u8 g = pngData[i*row_bytes + j + 1];
							u8 b = pngData[i*row_bytes + j + 2];
							pData[curByte] = r;
							pData[curByte+1] = g;
							pData[curByte+2] = b;
							pData[curByte+3] = 0xff;
							curByte += 4;
						}
					}

					channels = bpp;
					sdelete_array(pngData);
					return pData;
				}				
			}
			
			SHOOT_ASSERT(false, "Texture loading error");
			return 0;
		}

		return pngData;
#endif // SHOOT_FILE_SYSTEM
	}

	//! loads a texture from pre-processed file system
	void* PNGLoader::LoadFS(const char* strPath, s32& width, s32& height, s32& channels, u32& bpp)
	{
		File* pFile = File::Create(strPath, File::M_ReadBinary);
		pFile->Open();
		pFile->Read(&width, sizeof(s32));
		pFile->Read(&height, sizeof(s32));
		pFile->Read(&channels, sizeof(s32));
		pFile->Read(&bpp, sizeof(u32));
		void* pData = snew u8[width*height*bpp];
		pFile->Read(pData, width*height*bpp);
		pFile->Close();
		delete pFile;
		return pData;
	}	
}

