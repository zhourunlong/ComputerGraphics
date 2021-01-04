#pragma once
#include <bits/stdc++.h>
#include <png.h> //LibPNG header
#include <jpeglib.h>
#include <setjmp.h>

class ClmbsImg_Data
{
public:
    ClmbsImg_Data() {}

    ClmbsImg_Data(const ClmbsImg_Data &t) {
        w = t.w;
        h = t.h;
        bpp = t.bpp;
        data = t.data;
    }

    void print() const {
        std::cout << "w = " << w << " h = " << h << " bpp = " << bpp << "\n";
        std::cout << "data = " << &data << "\n";
    }

    unsigned int w; //Image width
    unsigned int h; //Image height
    unsigned int bpp; //Bytes per pixel: 3, 4
    unsigned char* data; //Image pixel data
};

typedef struct
{
    uint8_t magic[2]; //Magic Bytes 'B' and 'M'
    uint32_t size; //Size of whole file
    uint32_t unused; //Should be 0
    uint32_t offset; //Offset to bitmap data
} BMP_HEADER;

typedef struct
{
    uint32_t infosize; //Size of info struct (40 bytes)
    int32_t width; //Width of image
    int32_t height; //Height of image
    uint16_t planes; //Should be 1
    uint16_t bits; //Bits per pixel (1, 4, 8, 16, 24, 32)
    uint32_t compression; //0 = none, 1 = 8-bit RLE, 2 = 4-bit RLE
    uint32_t size_data; //The image size
    uint32_t hres; //Horizontal resolution (pixel per meter)
    uint32_t vres; //Vertical resolution (pixel per meter)
    uint32_t colors; //Number of palette colors
    uint32_t important_colors; //Number of important colors;
} BMP_INFO;

void ClmbsImg_Free(ClmbsImg_Data* data);

bool ClmbsImg_IsBMP(const char* file); //Check file format, if BMP, returns true
bool ClmbsImg_IsTGA(const char* file); //Check file format, if TGA, returns true
bool ClmbsImg_IsPNG(const char* file); //Check file format, if PNG, returns true
bool ClmbsImg_IsJPG(const char* file); //Check file format, if JPG, returns true

ClmbsImg_Data ClmbsImg_LoadBMP(const char* file); //Load BMP image WITHOUT check
ClmbsImg_Data ClmbsImg_LoadTGA(const char* file); //Load TGA image WITHOUT check
ClmbsImg_Data ClmbsImg_LoadPNG(const char* file); //Load PNG image WITHOUT check
ClmbsImg_Data ClmbsImg_LoadJPG(const char* file); //Load JPG image WITHOUT check
ClmbsImg_Data ClmbsImg_Load(const char* file); //Check and load all supported iamge formats

bool ClmbsImg_SaveBMP(const char* file, ClmbsImg_Data data);
bool ClmbsImg_SaveTGA(const char* file, ClmbsImg_Data data);
bool ClmbsImg_SavePNG(const char* file, ClmbsImg_Data data);
bool ClmbsImg_SaveJPG(const char* file, ClmbsImg_Data data, unsigned int quality);

void ClmbsImg_Free(ClmbsImg_Data* data)
{
    if (data == NULL) return;

    data->w = 0;
    data->h = 0;
    data->bpp = 0;

    if (data->data == NULL) return;
    free(data->data);
}

ClmbsImg_Data ClmbsImg_Load(const char* file) {

    ClmbsImg_Data err = ClmbsImg_Data();
    FILE* fp = fopen(file, "rb");
    if (fp == NULL) {
        std::cerr << "cannot open texture image file!!!\n";
        return err;
    }

    if (ClmbsImg_IsBMP(file))
        return ClmbsImg_LoadBMP(file);

    if (ClmbsImg_IsPNG(file))
        return ClmbsImg_LoadPNG(file);

    if (ClmbsImg_IsJPG(file))
        return ClmbsImg_LoadJPG(file);

    //if (ClmbsImg_IsTIF(file))
    //    return ClmbsImg_LoadTIF(file);

    //if (ClmbsImg_IsTGA(file))
    //    return ClmbsImg_LoadTGA(file);

    std::cerr << "cannot determine format of " << file << "!!!\n";

    return err;
}

typedef struct
{
    uint8_t b; //Blue channel
    uint8_t g; //Green channel
    uint8_t r; //Red channel
    uint8_t a; //Alpha channel
} BMP_PALETTE;

int ReadBytes(void* data, size_t size, FILE* fp)
{
    if (fread(data, size, 1, fp) != 1) return 0;
    return 1;
}

int WriteBytes(const void* data, size_t size, FILE* fp)
{
    if (fwrite(data, size, 1, fp) != 1) return 0;
    return 1;
}

int ReadUint8(uint8_t* data, FILE* fp)
{
    return ReadBytes(data, sizeof(uint8_t), fp);
}

int WriteUint8(const uint8_t* data, FILE* fp)
{
    return WriteBytes(data, sizeof(uint8_t), fp);
}

int ReadUint16(uint16_t* data, FILE* fp)
{
    return ReadBytes(data, sizeof(uint16_t), fp);
}

int WriteUint16(const uint16_t* data, FILE* fp)
{
    return WriteBytes(data, sizeof(uint16_t), fp);
}

int ReadUint32(uint32_t* data, FILE* fp)
{
    return ReadBytes(data, sizeof(uint32_t), fp);
}

int WriteUint32(const uint32_t* data, FILE* fp)
{
    return WriteBytes(data, sizeof(uint32_t), fp);
}

int ReadInt32(int32_t* data, FILE* fp)
{
    return ReadBytes(data, sizeof(int32_t), fp);
}

int WriteInt32(const int32_t* data, FILE* fp)
{
    return WriteBytes(data, sizeof(int32_t), fp);
}

int bgr2rgb(uint8_t* data, size_t size)
{
    if (data == NULL) return 0;

    uint8_t bgr[3];
    for (int i = 0; i < size; i += 3)
    {
        bgr[0] = data[i + 0];
        bgr[1] = data[i + 1];
        bgr[2] = data[i + 2];

        data[i + 0] = bgr[2];
        data[i + 1] = bgr[1];
        data[i + 2] = bgr[0];
    }

    return 1;
}

int bgra2rgba(uint8_t* data, size_t size)
{
    if (data == NULL) return 0;

    uint8_t bgr[3];
    for (int i = 0; i < size; i += 4)
    {
        bgr[0] = data[i + 0];
        bgr[1] = data[i + 1];
        bgr[2] = data[i + 2];

        data[i + 0] = bgr[2];
        data[i + 1] = bgr[1];
        data[i + 2] = bgr[0];
        data[i + 3] = data[i + 3];
    }

    return 1;
}

int rgb2bgr(uint8_t* data, size_t size)
{
    return bgr2rgb(data, size);
}

int rgba2bgra(uint8_t* data, size_t size)
{
    return bgra2rgba(data, size);
}

int flipX(uint8_t* aData, size_t aWidth, size_t aHeight, size_t aBPP)
{
    if (aData == NULL) return 0;

    const size_t stride = aWidth * aBPP;
    uint8_t* row;
    uint8_t* pixel = (uint8_t*)malloc(aBPP);

    for (size_t i = 0; i < aHeight; i++)
    {
        row = &aData[i * stride];

        for (size_t j = 0; j < aWidth / 2; j++)
        {
            memcpy(pixel, &row[j * aBPP], aBPP);
            memcpy(&row[j * aBPP], &row[(aWidth - j) * aBPP], aBPP);
            memcpy(&row[(aWidth - j) * aBPP], pixel, aBPP);
        }
    }

    return 1;
}

int flipY(uint8_t* aData, size_t aWidth, size_t aHeight, size_t aBPP)
{
    if (aData == NULL) return 0;

    const size_t stride = aWidth * aBPP;
    uint8_t* row = (uint8_t*)malloc(stride);

    for (size_t i = 0; i < aHeight / 2; i++)
    {
        memcpy(row, &aData[i * stride], stride);
        memcpy(&aData[i * stride], &aData[(aHeight - i) * stride], stride);
        memcpy(&aData[(aHeight - i) * stride], row, stride);
    }
    free(row);

    return 1;
}

int flipXY(uint8_t* aData, size_t aWidth, size_t aHeight, size_t aBPP)
{
    if (aData == NULL) return 0;

    const size_t size = aWidth * aHeight;
    //uint8_t* pixel = new uint8_t[aBPP];
    uint8_t* pixel = (uint8_t*)malloc(sizeof(uint8_t) * aBPP);

    for (size_t i = 0; i < size / 2; i++)
    {
        memcpy(pixel, &aData[i * aBPP], aBPP);
        memcpy(&aData[i * aBPP], &aData[(size - i) * aBPP], aBPP);
        memcpy(&aData[(size - i) * aBPP], pixel, aBPP);
    }
    free(pixel);

    return 1;
}

static bool ReadHeader(BMP_HEADER* header, FILE* fp)
{
    if (header == NULL || fp == NULL) return false;

    if (!ReadUint8(&header->magic[0], fp)) return false;
    if (!ReadUint8(&header->magic[1], fp)) return false;
    if (!ReadUint32(&header->size, fp)) return false;
    if (!ReadUint32(&header->unused, fp)) return false;
    if (!ReadUint32(&header->offset, fp)) return false;

    return true;
}

static bool WriteHeader(BMP_HEADER header, FILE* fp)
{
    if (fp == NULL) return false;

    if (!WriteUint8(&header.magic[0], fp)) return false;
    if (!WriteUint8(&header.magic[1], fp)) return false;
    if (!WriteUint32(&header.size, fp)) return false;
    if (!WriteUint32(&header.unused, fp)) return false;
    if (!WriteUint32(&header.offset, fp)) return false;

    return true;
}

static bool ReadInfo(BMP_INFO* info, FILE* fp)
{
    if (info == NULL || fp == NULL) return false;

    if (!ReadUint32(&info->infosize, fp)) return false;
    if (!ReadInt32(&info->width, fp)) return false;
    if (!ReadInt32(&info->height, fp)) return false;
    if (!ReadUint16(&info->planes, fp)) return false;
    if (!ReadUint16(&info->bits, fp)) return false;
    if (!ReadUint32(&info->compression, fp)) return false;
    if (!ReadUint32(&info->size_data, fp)) return false;
    if (!ReadUint32(&info->hres, fp)) return false;
    if (!ReadUint32(&info->vres, fp)) return false;
    if (!ReadUint32(&info->colors, fp)) return false;
    if (!ReadUint32(&info->important_colors, fp)) return false;

    uint8_t* empty = (uint8_t*)malloc(68);
    ReadBytes(empty, 68, fp);
    free(empty);

    return true;
}

static bool WriteInfo(BMP_INFO info, FILE* fp)
{
    if (fp == NULL) return false;

    if (!WriteUint32(&info.infosize, fp)) return false;
    if (!WriteInt32(&info.width, fp)) return false;
    if (!WriteInt32(&info.height, fp)) return false;
    if (!WriteUint16(&info.planes, fp)) return false;
    if (!WriteUint16(&info.bits, fp)) return false;
    if (!WriteUint32(&info.compression, fp)) return false;
    if (!WriteUint32(&info.size_data, fp)) return false;
    if (!WriteUint32(&info.hres, fp)) return false;
    if (!WriteUint32(&info.vres, fp)) return false;
    if (!WriteUint32(&info.colors, fp)) return false;
    if (!WriteUint32(&info.important_colors, fp)) return false;

    return true;
}

static bool ReadPalette(BMP_PALETTE* palette, FILE* fp)
{
    if (palette == NULL || fp == NULL) return false;

    if (!ReadUint8(&palette->b, fp)) return false;
    if (!ReadUint8(&palette->g, fp)) return false;
    if (!ReadUint8(&palette->r, fp)) return false;
    if (!ReadUint8(&palette->a, fp)) return false;

    return true;
}

static bool Decode(uint8_t* data, size_t size, size_t bits)
{
    switch (bits)
    {
    case 24:
        return bgr2rgb(data, size);
        break;
    case 32:
        return bgra2rgba(data, size);
        break;
    };

    return true;
}

bool ClmbsImg_IsBMP(const char* file)
{
    FILE* fp = fopen(file, "rb");
    if (fp == NULL)
        return false;

    unsigned char magic[2];
    fread(magic, sizeof(magic), 1, fp);
    fclose(fp);

    if (magic[0] == 'B' && magic[1] == 'M')
        return true;
    else
        return false;
}

ClmbsImg_Data ClmbsImg_LoadBMP(const char* file)
{
    ClmbsImg_Data ret;

    FILE* fp = fopen(file, "rb");
    if (fp == NULL) return ret;

    BMP_HEADER header;
    BMP_INFO info;

    if (!ReadHeader(&header, fp)) return ret;
    if (!ReadInfo(&info, fp)) return ret;

    uint8_t* data = (uint8_t*)malloc(header.size - 68);
    fread(data, header.size - 68, 1, fp);

    Decode(data, header.size - 68, info.bits);

    ret.w = info.width;
    ret.h = info.height;
    ret.bpp = info.bits / 8;

    ret.data = data;

    fclose(fp);

    return ret;
}

bool ClmbsImg_SaveBMP(const char* file, ClmbsImg_Data data)
{
    FILE* fp = fopen(file, "wb");
    if (fp == NULL) return false;

    BMP_HEADER header;
    BMP_INFO info;

    header.magic[0] = 'B';
    header.magic[1] = 'M';
    header.size = data.w * data.h * data.bpp + 54;
    header.unused = 0;
    header.offset = 54;

    info.infosize = 40;
    info.width = data.w;
    info.height = data.h;
    info.planes = 1;
    info.bits = data.bpp * 8;
    info.compression = 0;
    info.size_data = data.w * data.h * data.bpp;
    info.hres = 0;
    info.vres = 0;
    info.colors = 0;
    info.important_colors = 0;

    if (!WriteHeader(header, fp)) return false;
    if (!WriteInfo(info, fp)) return false;

    uint8_t* buffer = (uint8_t*)malloc(data.w * data.h * data.bpp);
    memcpy(buffer, data.data, data.w * data.h * data.bpp);

    Decode(buffer, data.w * data.h * data.bpp, data.bpp * 8);
    
    fwrite(buffer, data.w * data.h * data.bpp, 1, fp);

    fclose(fp);
    free(buffer);

    return true;
}

bool ClmbsImg_IsJPG(const char* file)
{
    FILE* fp = fopen(file, "rb");
    if (fp == NULL) return false;

    uint8_t magic[4];
    fseek(fp, 6, SEEK_CUR);
    if (!ReadBytes(magic, sizeof(magic), fp)) return false;

    if (magic[0] == 'E' &&
        magic[1] == 'x' &&
        magic[2] == 'i' &&
        magic[3] == 'f')
        std::cerr << "Only support JFIF compression of JPG, manually convert it!!!\n";

    if (magic[0] == 'J' &&
        magic[1] == 'F' &&
        magic[2] == 'I' &&
        magic[3] == 'F') return true;

    return false;
}

ClmbsImg_Data ClmbsImg_LoadJPG(const char* file)
{
    ClmbsImg_Data ret;

    struct jpeg_decompress_struct cinfo;

    struct jpeg_error_mgr pub;
    jmp_buf setjmp_buffer;

    FILE* fp = fopen(file, "rb");
    JSAMPARRAY buffer;
    size_t row_stride;

    if (fp == NULL) return ret;

    cinfo.err = jpeg_std_error(&pub);
    
    if (setjmp(setjmp_buffer))
    {
        jpeg_destroy_decompress(&cinfo);
        fclose(fp);
        return ret;
    }

    jpeg_create_decompress(&cinfo);

    jpeg_stdio_src(&cinfo, fp);

    jpeg_read_header(&cinfo, TRUE);

    ret.w = cinfo.image_width;
    ret.h = cinfo.image_height;
    ret.bpp = 3;

    jpeg_start_decompress(&cinfo);

    row_stride = cinfo.output_width * cinfo.output_components;
    buffer = (*cinfo.mem->alloc_sarray) ((j_common_ptr)&cinfo, JPOOL_IMAGE, row_stride, 1);

    uint8_t* data = (uint8_t*)malloc(cinfo.image_width * cinfo.image_height * 3);
    uint64_t counter = 0;
    uint64_t maxsize = row_stride * cinfo.image_height;

    while (cinfo.output_scanline < cinfo.output_height)
    {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        memcpy(data + (maxsize - counter - row_stride), buffer[0], row_stride);
        counter += row_stride;
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    fclose(fp);

    ret.data = data;

    return ret;
}

bool ClmbsImg_SaveJPG(const char* file, ClmbsImg_Data data, unsigned int quality)
{
    if (data.data == NULL) return false;

    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    
    FILE* fp = fopen(file, "wb");;
    if (fp == NULL) return false;

    JSAMPROW row_pointer[1];
    size_t row_stride;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);

    jpeg_stdio_dest(&cinfo, fp);

    cinfo.image_width = data.w;
    cinfo.image_height = data.h;
    cinfo.input_components = data.bpp;
    cinfo.in_color_space = JCS_RGB;
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE);

    jpeg_start_compress(&cinfo, TRUE);

    row_stride = data.w * data.bpp;
    uint64_t maxsize = row_stride * data.h;

    while (cinfo.next_scanline < cinfo.image_height)
    {
        row_pointer[0] = (JSAMPROW)&data.data[maxsize - cinfo.next_scanline * row_stride - row_stride];
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    fclose(fp);
    jpeg_destroy_compress(&cinfo);

    return true;
}

bool ClmbsImg_IsPNG(const char* file)
{
    FILE* fp = fopen(file, "rb");
    if (fp == NULL) return false;

    uint8_t magic[4];
    if (!ReadBytes(magic, sizeof(magic), fp)) return false;
    fclose(fp);

    if (magic[1] == 'P' &&
        magic[2] == 'N' &&
        magic[3] == 'G') return true;
    else return false;
}

ClmbsImg_Data ClmbsImg_LoadPNG(const char* file)
{
    ClmbsImg_Data ret;

    FILE* fp = fopen(file, "rb");
    if (fp == NULL) return ret;

    png_structp	png_ptr;
    png_infop info_ptr;
    png_uint_32 width;
    png_uint_32 height;
    png_uint_32 bpp;
    int bit_depth;
    int color_type;
    int interlace_method;
    int compression_method;
    int filter_method;

    png_bytepp rows;

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) return ret;

    info_ptr = png_create_info_struct(png_ptr);
    if (!png_ptr) return ret;

    png_init_io(png_ptr, fp);
    png_read_png(png_ptr, info_ptr, 0, 0);
    png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth,
        &color_type, &interlace_method, &compression_method, &filter_method);

    rows = png_get_rows(png_ptr, info_ptr);
    int rowbytes;
    rowbytes = png_get_rowbytes(png_ptr, info_ptr);

    if (color_type == PNG_COLOR_TYPE_RGB)
        bpp = 3;
    if (color_type == PNG_COLOR_TYPE_RGBA)
        bpp = 4;

    ret.w = width;
    ret.h = height;
    ret.bpp = bpp;

    uint8_t* data = (uint8_t*)malloc(width * height * bpp);

    for (size_t i = 0; i < height; i++)
        memcpy(&data[rowbytes * i], rows[height - i - 1], rowbytes);

    if (png_ptr && info_ptr)
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    fclose(fp);

    ret.data = data;

    return ret;
}

bool ClmbsImg_SavePNG(const char* file, ClmbsImg_Data data)
{
    FILE* fp = fopen(file, "wb");
    if (fp == NULL) return false;

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) return false;

    png_infop info = png_create_info_struct(png);
    if (!info)
    {
        png_destroy_write_struct(&png, &info);
        return false;
    }

    unsigned int width = data.w;
    unsigned int height = data.h;
    unsigned int bpp = data.bpp;

    png_init_io(png, fp);
    int type = PNG_COLOR_TYPE_RGB;
    if (bpp == 4)
        type = PNG_COLOR_TYPE_RGBA;

    png_set_IHDR(png, info, width, height, 8, type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_colorp palette = (png_colorp)png_malloc(png, PNG_MAX_PALETTE_LENGTH * sizeof(png_color));
    if (!palette)
    {
        fclose(fp);
        png_destroy_write_struct(&png, &info);
        return false;
    }

    png_set_PLTE(png, info, palette, PNG_MAX_PALETTE_LENGTH);
    png_write_info(png, info);
    png_set_packing(png);

    png_bytepp rows = (png_bytepp)png_malloc(png, height * sizeof(png_bytep));
    int rowbytes = width * bpp;
    for (size_t i = 0; i < height; i++)
    {
        rows[i] = (png_bytep)malloc(rowbytes);
        memcpy(rows[i], data.data + (height - i - 1) * rowbytes, rowbytes);
    }

    png_write_image(png, rows);
    png_write_end(png, info);
    png_free(png, palette);
    png_destroy_write_struct(&png, &info);
    fclose(fp);

    for (size_t i = 0; i < height; i++)
        free(rows[i]);

    return true;
}