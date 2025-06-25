#pragma once
#include<vector>
#include<fstream>
using namespace std;

struct Header {
    char idLength;
    char colorMapType;
    char imageType;
    short colorMapOrigin;
    short colorMapLength;
    char colorMapDepth;
    short xOrigin;
    short yOrigin;
    short width;
    short height;
    char bitsPerPixel;
    char imageDescriptor;
    Header(char idLength, char colorMapType, char imageType, short colorMapOrigin,
        short colorMapLength, char colorMapDepth, short xOrigin, short yOrigin,
        short width, short height, char bitsPerPixel, char imageDescriptor);
    Header();
    Header& operator=(const Header& other);
};

struct Pixel {
    unsigned char blue;
    unsigned char green;
    unsigned char red;
    Pixel(unsigned char blue, unsigned char green, unsigned char red);
};

class Image {
    Header header;
    vector<Pixel> data;
public:
    Image(ifstream& file);
    Image(Header header, vector<Pixel>& data);
    void Write(ofstream& file);
    Image& operator=(const Image& other);
    Image operator+(const Image& bottom) const;
    Image operator-(const Image& bottom) const;
    Image operator*(const Image& bottom) const;
    Image Screen(const Image& top) const;
    Image Overlay(const Image& bottom) const;
    Image AddToChannel(string color, int value) const;
    Image ScaleChannel(string color, float value) const;
    Image TakeChannel(string color) const;
    Image Combine(const Image& Green, const Image& Blue);
    Image Flip();
};

