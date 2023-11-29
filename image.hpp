#ifndef __IMAGE__
#define __IMAGE__

#define PNG_SETJMP_NOT_SWUPPORTED
#include <png.h>
#include <string>

#define COLOR_DEPTH 8

struct Pixel {
	png_byte r, g, b, a;
    Pixel() : r(0), g(0), b(0), a(0) {}
    Pixel(png_byte _r, png_byte _g, png_byte _b, png_byte _a = 255) : r(_r), g(_g), b(_b), a(_a) {}
};

class image {
public:
    image(const image& img) : height(img.height), width(img.width) {
        data = new Pixel*[height];
        for (int i=0; i<height; i++) {
            data[i] = new Pixel[width];
            for (int j=0; j<width; j++) {
                data[i][j] = img.data[i][j];
            }
        }
    }

    image(image&& img) : height(img.height), width(img.width) {
        data = img.data;
        img.data = NULL;
    }
    
    image(int _height, int _width) : height(_height), width(_width) {
        /* allocate image data */
        data = new Pixel*[_height];
        for (int i=0; i<_height; i++) {
            data[i] = new Pixel[_width];
        }
    }

    Pixel* operator[](int index) {
        return data[index];
    }

    // referred lines.cpp
    void save(const std::string& path) {
        FILE *f = fopen(path.c_str(), "wb");    
        png_structp png_ptr;
        png_infop info_ptr;

        /* initialize png data structures */
        png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png_ptr) {
            fprintf(stderr, "could not initialize png struct\n");\
        }

        info_ptr = png_create_info_struct(png_ptr);
        if (!info_ptr) {
            png_destroy_write_struct(&png_ptr, (png_infopp)NULL);\
        }

        /* begin writing PNG File */
        png_init_io(png_ptr, f);
        png_set_IHDR(png_ptr, info_ptr, width, height, COLOR_DEPTH,
                    PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
                    PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
        png_write_info(png_ptr, info_ptr);

        /* write image data to disk */
        png_write_image(png_ptr, (png_byte **)data);

        /* finish writing PNG file */
        png_write_end(png_ptr, NULL);

        /* clean up PNG-related data structures */
        png_destroy_write_struct(&png_ptr, &info_ptr);

        /* close the file */
        fclose(f);
    }

    ~image() {
        if (data==NULL) return;
        for (int i=0; i<height; i++) {
            delete[] data[i];
        }
        delete[] data;
    }
private:
    int height, width;
    struct Pixel** data;
};


#endif