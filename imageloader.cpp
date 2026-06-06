#include <assert.h>
#include <fstream>
#include "imageloader.h"

using namespace std;

Image::Image(char* ps, int w, int h) : pixels(ps), width(w), height(h) {}

Image::~Image(){
    delete[] pixels;
}

namespace {
    int toInt(const char* bytes)
    {
        return (int)(
            ((unsigned char)bytes[3] << 24) |
            ((unsigned char)bytes[2] << 16) |
            ((unsigned char)bytes[1] << 8)  |
            (unsigned char)bytes[0]
        );
    }

    short toShort(const char* bytes)
    {
        return (short)(
            ((unsigned char)bytes[1] << 8) |
            (unsigned char)bytes[0]
        );
    }

    int readInt(ifstream& input)
    {
        char buffer[4];
        input.read(buffer, 4);
        return toInt(buffer);
    }

    short readShort(ifstream& input)
    {
        char buffer[2];
        input.read(buffer, 2);
        return toShort(buffer);
    }

    template<class T>
    class auto_array {
    private:
        T* array;
        mutable bool isReleased;

    public:
        explicit auto_array(T* array = 0) : array(array), isReleased(false) {}

        auto_array(const auto_array<T>& aarray)
        {
            array = aarray.array;
            isReleased = aarray.isReleased;
            aarray.isReleased = true;
        }

        ~auto_array()
        {
            if (!isReleased && array != 0) {
                delete[] array;
            }
        }

        T* get() const
        {
            return array;
        }

        T& operator*() const
        {
            return *array;
        }

        void operator=(const auto_array<T>& aarray)
        {
            if (!isReleased && array != 0) {
                delete[] array;
            }

            array = aarray.array;
            isReleased = aarray.isReleased;
            aarray.isReleased = true;
        }

        T* operator->() const
        {
            return array;
        }

        T* release()
        {
            isReleased = true;
            return array;
        }

        void reset(T* newArray = 0)
        {
            if (!isReleased && array != 0) {
                delete[] array;
            }

            array = newArray;
            isReleased = false;
        }

        T* operator+(int i)
        {
            return array + i;
        }

        T& operator[](int i)
        {
            return array[i];
        }
    };
}

Image* loadBMP(const char* filename)
{
    ifstream input;
    input.open(filename, ifstream::binary);
    assert(!input.fail() || !"File tidak ditemukan!!!");

    char buffer[2];
    input.read(buffer, 2);
    assert((buffer[0] == 'B' && buffer[1] == 'M') || !"Bukan file bitmap!!!");

    input.ignore(8);

    int dataOffset = readInt(input);
    int headerSize = readInt(input);

    int width = 0;
    int height = 0;
    int bitsPerPixel = 0;
    int compression = 0;

    if (headerSize == 40) {
        width = readInt(input);
        height = readInt(input);

        input.ignore(2); // color planes

        bitsPerPixel = readShort(input);
        compression = readInt(input);

        assert((bitsPerPixel == 24 || bitsPerPixel == 32) || !"Gambar harus 24-bit atau 32-bit BMP!");
        assert((compression == 0 || compression == 3) || !"Gambar BMP dikompres dan tidak bisa dibaca!");
    }
    else if (headerSize == 12) {
        width = readShort(input);
        height = readShort(input);

        input.ignore(2); // color planes

        bitsPerPixel = readShort(input);

        assert((bitsPerPixel == 24 || bitsPerPixel == 32) || !"Gambar harus 24-bit atau 32-bit BMP!");
    }
    else {
        assert(!"Format bitmap ini tidak diketahui!");
    }

    int absHeight = height < 0 ? -height : height;
    int bytesPerPixel = bitsPerPixel / 8;
    int bytesPerRow = ((width * bytesPerPixel + 3) / 4) * 4;
    int size = bytesPerRow * absHeight;

    auto_array<char> pixels(new char[size]);

    input.seekg(dataOffset, ios_base::beg);
    input.read(pixels.get(), size);

    auto_array<char> pixels2(new char[width * absHeight * 3]);

    for (int row = 0; row < absHeight; row++) {
        int srcRow = height > 0 ? row : absHeight - 1 - row;

        for (int col = 0; col < width; col++) {
            int srcIndex = bytesPerRow * srcRow + bytesPerPixel * col;
            int dstIndex = 3 * (width * row + col);

            // BMP menyimpan warna sebagai BGR atau BGRA.
            pixels2[dstIndex + 0] = pixels[srcIndex + 2]; // R
            pixels2[dstIndex + 1] = pixels[srcIndex + 1]; // G
            pixels2[dstIndex + 2] = pixels[srcIndex + 0]; // B
        }
    }

    input.close();

    return new Image(pixels2.release(), width, absHeight);
}