#include "data.h"

// parameterized Header constructor
Header::Header(char idLength, char colorMapType, char imageType, short colorMapOrigin,
        short colorMapLength, char colorMapDepth, short xOrigin, short yOrigin,
        short width, short height, char bitsPerPixel, char imageDescriptor) {
        this->idLength = idLength;
        this->colorMapType = colorMapType;
        this->imageType = imageType;
        this->colorMapOrigin = colorMapOrigin;
        this->colorMapLength = colorMapLength;
        this->colorMapDepth = colorMapDepth;
        this->xOrigin = xOrigin;
        this->yOrigin = yOrigin;
        this->width = width;
        this->height = height;
        this->bitsPerPixel = bitsPerPixel;
        this->imageDescriptor = imageDescriptor;
}
Header::Header() {}

Header& Header::operator=(const Header& other) {
        if (this == &other) {
                return *this;
        }
        this->idLength = other.idLength;
        this->colorMapType = other.colorMapType;
        this->imageType = other.imageType;
        this->colorMapOrigin = other.colorMapOrigin;
        this->colorMapLength = other.colorMapLength;
        this->colorMapDepth = other.colorMapDepth;
        this->xOrigin = other.xOrigin;
        this->yOrigin = other.yOrigin;
        this->width = other.width;
        this->height = other.height;
        this->bitsPerPixel = other.bitsPerPixel;
        this->imageDescriptor = other.imageDescriptor;
        return *this;
}

// Pixel constructor
Pixel::Pixel(unsigned char blue, unsigned char green, unsigned char red) {
        this->blue = blue;
        this->green = green;
        this->red = red;
}

// Image constructor - reads a tga file
Image::Image(ifstream& file) {
        // in main check that the file exists and is opened properly & close file after this constructor is called
        // Step 1. Read in the header info (first 18 bytes in file)
        char idLength;
        file.read(&idLength, 1);
        char colorMapType;
        file.read(&colorMapType, 1);
        char imageType;
        file.read(&imageType, 1);
        short colorMapOrigin;
        file.read(reinterpret_cast<char *>(&colorMapOrigin), 2);
        short colorMapLength;
        file.read(reinterpret_cast<char *>(&colorMapLength), 2);
        char colorMapDepth;
        file.read(&colorMapDepth, 1);
        short xOrigin;
        file.read(reinterpret_cast<char *>(&xOrigin), 2);
        short yOrigin;
        file.read(reinterpret_cast<char *>(&yOrigin), 2);
        short width;
        file.read(reinterpret_cast<char *>(&width), 2);
        short height;
        file.read(reinterpret_cast<char *>(&height), 2);
        char bitsPerPixel;
        file.read(&bitsPerPixel, 1);
        char imageDescriptor;
        file.read(&imageDescriptor, 1);

        // put info into header object:
        Header h(idLength, colorMapType, imageType, colorMapOrigin, colorMapLength, colorMapDepth, xOrigin,
                 yOrigin, width, height, bitsPerPixel, imageDescriptor);
        this->header = h;

        // Step 2. Read in the raw data and store it in a vector of pixels
        int numPixels = width * height;
        for (int i = 0; i < numPixels; i++) {
                unsigned char blue, green, red;
                file.read(reinterpret_cast<char*>(&blue), 1);
                file.read(reinterpret_cast<char*>(&green), 1);
                file.read(reinterpret_cast<char*>(&red), 1);
                Pixel pixel(blue, green, red);
                this->data.push_back(pixel);
        }
}

// Second constructor for Image object (for when an image is formed through manipulations of other existing images)
Image::Image(Header header, vector<Pixel>& data) {
        this->header = header;
        this->data = data;
}

void Image::Write(ofstream& file) {
        // 1. Write the header info
        file.write(&header.idLength, 1);
        file.write(&header.colorMapType, 1);
        file.write(&header.imageType, 1);
        file.write(reinterpret_cast<char*>(&header.colorMapOrigin), 2);
        file.write(reinterpret_cast<char*>(&header.colorMapLength), 2);
        file.write(&header.colorMapDepth, 1);
        file.write(reinterpret_cast<char*>(&header.xOrigin), 2);
        file.write(reinterpret_cast<char*>(&header.yOrigin), 2);
        file.write(reinterpret_cast<char*>(&header.width), 2);
        file.write(reinterpret_cast<char*>(&header.height), 2);
        file.write(&header.bitsPerPixel, 1);
        file.write(&header.imageDescriptor, 1);

        // 2. Write the image data
        int numPixels = header.width * header.height;
        for (int i = 0; i < numPixels; i++) {
                file.write(reinterpret_cast<char*>(&data.at(i).blue), 1);
                file.write(reinterpret_cast<char*>(&data.at(i).green), 1);
                file.write(reinterpret_cast<char*>(&data.at(i).red), 1);
        }
}

// copy assignment operator
Image& Image::operator=(const Image& other) {
        if (this == &other) {
                return *this;
        }
        this->header = other.header;
        for (int i = 0; i < data.size(); i++) {
                this->data.at(i) = other.data.at(i);
        }
        return *this;
}

// "Add" two images
Image Image::operator+(const Image& bottom) const {
        // note: "this" is the top layer and "bottom" is the bottom layer
        Header newHeader = header; // copy header from one of the original images
        vector<Pixel> newData;
        for (int i = 0; i < data.size(); i++) {
                unsigned char newBlue, newGreen, newRed;
                int tempBlueTop = (int)data.at(i).blue;
                int tempGreenTop = (int)data.at(i).green;
                int tempRedTop = (int)data.at(i).red;
                int tempBlueBottom = (int)bottom.data.at(i).blue;
                int tempGreenBottom = (int)bottom.data.at(i).green;
                int tempRedBottom = (int)bottom.data.at(i).red;
                int tempNewBlue = tempBlueTop + tempBlueBottom;
                if (tempNewBlue > 255) {
                        tempNewBlue = 255;
                }
                int tempNewGreen = tempGreenTop + tempGreenBottom;
                if (tempNewGreen > 255) {
                        tempNewGreen = 255;
                }
                int tempNewRed = tempRedTop + tempRedBottom;
                if (tempNewRed > 255) {
                        tempNewRed = 255;
                }
                newBlue = (unsigned char)tempNewBlue;
                newGreen = (unsigned char)tempNewGreen;
                newRed = (unsigned char)tempNewRed;
                Pixel newPixel(newBlue, newGreen, newRed);
                newData.push_back(newPixel);
        }
        Image newImage(newHeader, newData);
        return newImage;
}

// "Subtract" : top layer minus bottom layer
Image Image::operator-(const Image& bottom) const {
        Header newHeader = header;
        vector<Pixel> newData;
        for (int i = 0; i < data.size(); i++) {
                unsigned char newBlue, newGreen, newRed;
                int tempBlueTop = (int)data.at(i).blue;
                int tempGreenTop = (int)data.at(i).green;
                int tempRedTop = (int)data.at(i).red;
                int tempBlueBottom = (int)bottom.data.at(i).blue;
                int tempGreenBottom = (int)bottom.data.at(i).green;
                int tempRedBottom = (int)bottom.data.at(i).red;
                int tempNewBlue = tempBlueTop - tempBlueBottom;
                if (tempNewBlue < 0) {
                        tempNewBlue = 0;
                }
                int tempNewGreen = tempGreenTop - tempGreenBottom;
                if (tempNewGreen < 0) {
                        tempNewGreen = 0;
                }
                int tempNewRed = tempRedTop - tempRedBottom;
                if (tempNewRed < 0) {
                        tempNewRed = 0;
                }
                newBlue = (unsigned char)tempNewBlue;
                newGreen = (unsigned char)tempNewGreen;
                newRed = (unsigned char)tempNewRed;
                Pixel newPixel(newBlue, newGreen, newRed);
                newData.push_back(newPixel);
        }
        Image newImage(newHeader, newData);
        return newImage;
}

// "Multiply" top layer x bottom layer
Image Image::operator*(const Image& bottom) const {
        Header newHeader = header;
        vector<Pixel> newData;
        for (int i = 0; i < data.size(); i++) {
                unsigned char newBlue, newGreen, newRed;
                float tempBlueTop = data.at(i).blue;
                float normBlueTop = tempBlueTop / 255.0f;
                float tempBlueBottom = bottom.data.at(i).blue;
                float normBlueBottom = tempBlueBottom / 255.0f;
                float normNewBlue = normBlueTop * normBlueBottom;
                float tempNewBlue = normNewBlue * 255.0f;
                float finalNewBlue = tempNewBlue + 0.5f;
                newBlue = (unsigned char)finalNewBlue;
                
                float tempGreenTop = data.at(i).green;
                float normGreenTop = tempGreenTop / 255.0f;
                float tempGreenBottom = bottom.data.at(i).green;
                float normGreenBottom = tempGreenBottom / 255.0f;
                float normNewGreen = normGreenTop * normGreenBottom;
                float tempNewGreen = normNewGreen * 255.0f;
                float finalNewGreen = tempNewGreen + 0.5f;
                newGreen = (unsigned char)finalNewGreen;

                float tempRedTop = data.at(i).red;
                float normRedTop = tempRedTop / 255.0f;
                float tempRedBottom = bottom.data.at(i).red;
                float normRedBottom = tempRedBottom / 255.0f;
                float normNewRed = normRedTop * normRedBottom;
                float tempNewRed = normNewRed * 255.0f;
                float finalNewRed = tempNewRed + 0.5f;
                newRed = (unsigned char)finalNewRed;

                Pixel newPixel(newBlue, newGreen, newRed);
                newData.push_back(newPixel);
        }
        Image newImage(newHeader, newData);
        return newImage;
}

// "Screen" blending mode
Image Image::Screen(const Image& top) const {
        // "this" is the bottom image
        // 1 - [(1-NP1)x(1-NP2)]
        Header newHeader = header;
        vector<Pixel> newData;
        for (int i = 0; i < data.size(); i++) {
                unsigned char newBlue, newGreen, newRed;
                float tempBlueTop = top.data.at(i).blue;
                float bNP1 = tempBlueTop / 255.0f;
                float tempBlueBottom = data.at(i).blue;
                float bNP2 = tempBlueBottom / 255.0f;
                float tempNewBlue = 1 - ((1-bNP1)*(1-bNP2));
                float finalNewBlue = tempNewBlue * 255.0f + 0.5f;
                newBlue = (unsigned char)finalNewBlue;

                float tempGreenTop = top.data.at(i).green;
                float gNP1 = tempGreenTop / 255.0f;
                float tempGreenBottom = data.at(i).green;
                float gNP2 = tempGreenBottom / 255.0f;
                float tempNewGreen = 1 - ((1-gNP1)*(1-gNP2));
                float finalNewGreen = tempNewGreen * 255.0f + 0.5f;
                newGreen = (unsigned char)finalNewGreen;

                float tempRedTop = top.data.at(i).red;
                float rNP1 = tempRedTop / 255.0f;
                float tempRedBottom = data.at(i).red;
                float rNP2 = tempRedBottom / 255.0f;
                float tempNewRed = 1 - ((1-rNP1)*(1-rNP2));
                float finalNewRed = tempNewRed * 255.0f + 0.5f;
                newRed = (unsigned char)finalNewRed;

                Pixel newPixel(newBlue, newGreen, newRed);
                newData.push_back(newPixel);
        }
        Image newImage(newHeader, newData);
        return newImage;
}

Image Image::Overlay(const Image& bottom) const {
        Header newHeader = header;
        vector<Pixel> newData;
        for (int i = 0; i < data.size(); i++) {
                unsigned char newBlue, newGreen, newRed;
                float tempBlueTop = data.at(i).blue;
                float bNP1 = tempBlueTop / 255.0f;
                float tempBlueBottom = bottom.data.at(i).blue;
                float bNP2 = tempBlueBottom / 255.0f;
                float tempNewBlue;
                if (bNP2 <= 0.5f) {
                        tempNewBlue = 2.0f * bNP1 * bNP2;
                }
                else {
                        tempNewBlue = 1 - (2.0f * (1 - bNP1) * (1 - bNP2));
                }
                float finalNewBlue = tempNewBlue * 255.0f + 0.5f;
                newBlue = (unsigned char)finalNewBlue;

                float tempGreenTop = data.at(i).green;
                float gNP1 = tempGreenTop / 255.0f;
                float tempGreenBottom = bottom.data.at(i).green;
                float gNP2 = tempGreenBottom / 255.0f;
                float tempNewGreen;
                if (gNP2 <= 0.5f) {
                        tempNewGreen = 2.0f * gNP1 * gNP2;
                }
                else {
                        tempNewGreen = 1 - (2.0f * (1 - gNP1) * (1 - gNP2));
                }
                float finalNewGreen = tempNewGreen * 255.0f + 0.5f;
                newGreen = (unsigned char)finalNewGreen;

                float tempRedTop = data.at(i).red;
                float rNP1 = tempRedTop / 255.0f;
                float tempRedBottom = bottom.data.at(i).red;
                float rNP2 = tempRedBottom / 255.0f;
                float tempNewRed;
                if (rNP2 <= 0.5f) {
                        tempNewRed = 2.0f * rNP1 * rNP2;
                }
                else {
                        tempNewRed = 1 - (2.0f * (1 - rNP1) * (1 - rNP2));
                }
                float finalNewRed = tempNewRed * 255.0f + 0.5f;
                newRed = (unsigned char)finalNewRed;

                Pixel newPixel(newBlue, newGreen, newRed);
                newData.push_back(newPixel);
        }
        Image newImage(newHeader, newData);
        return newImage;
}

Image Image::AddToChannel(string color, int value) const {
        Header newHeader = header;
        vector<Pixel> newData;
        if (color == "blue") {
                for (int i = 0; i < data.size(); i++) {
                        unsigned char newBlue, newGreen, newRed;
                        newRed = data.at(i).red;
                        newGreen = data.at(i).green;
                        int tempBlue = data.at(i).blue;
                        int tempNewBlue = tempBlue + value;
                        if (tempNewBlue > 255) {
                                tempNewBlue = 255;
                        }
                        if (tempNewBlue < 0) {
                                tempNewBlue = 0;
                        }
                        newBlue = (unsigned char)tempNewBlue;
                        Pixel newPixel(newBlue, newGreen, newRed);
                        newData.push_back(newPixel);
                }
        }
        else if (color == "green") {
                for (int i = 0; i < data.size(); i++) {
                        unsigned char newBlue, newGreen, newRed;
                        newBlue = data.at(i).blue;
                        newRed = data.at(i).red;
                        int tempGreen = data.at(i).green;
                        int tempNewGreen = tempGreen + value;
                        if (tempNewGreen > 255) {
                                tempNewGreen = 255;
                        }
                        if (tempNewGreen < 0) {
                                tempNewGreen = 0;
                        }
                        newGreen = (unsigned char)tempNewGreen;
                        Pixel newPixel(newBlue, newGreen, newRed);
                        newData.push_back(newPixel);
                }
        }
        else if (color == "red") {
                for (int i = 0; i < data.size(); i++) {
                        unsigned char newBlue, newGreen, newRed;
                        newBlue = data.at(i).blue;
                        newGreen = data.at(i).green;
                        int tempRed = data.at(i).red;
                        int tempNewRed = tempRed + value;
                        if (tempNewRed > 255) {
                                tempNewRed = 255;
                        }
                        if (tempNewRed < 0) {
                                tempNewRed = 0;
                        }
                        newRed = (unsigned char)tempNewRed;
                        Pixel newPixel(newBlue, newGreen, newRed);
                        newData.push_back(newPixel);
                }
        }
        Image newImage(newHeader, newData);
        return newImage;
}

Image Image::ScaleChannel(string color, float value) const {
        Header newHeader = header;
        vector<Pixel> newData;
        if (color == "blue") {
                for (int i = 0; i < data.size(); i++) {
                        unsigned char newBlue, newGreen, newRed;
                        newRed = data.at(i).red;
                        newGreen = data.at(i).green;
                        float tempBlue = data.at(i).blue;
                        float tempNewBlue = tempBlue * value;
                        if (tempNewBlue > 255) {
                                tempNewBlue = 255;
                        }
                        else if (tempNewBlue < 0) {
                                tempNewBlue = 0;
                        }
                        newBlue = (unsigned char)tempNewBlue;
                        Pixel newPixel(newBlue, newGreen, newRed);
                        newData.push_back(newPixel);
                }
        }
        else if (color == "green") {
                for (int i = 0; i < data.size(); i++) {
                        unsigned char newBlue, newGreen, newRed;
                        newRed = data.at(i).red;
                        newBlue = data.at(i).blue;
                        float tempGreen = data.at(i).green;
                        float tempNewGreen = tempGreen * value;
                        if (tempNewGreen > 255) {
                                tempNewGreen = 255;
                        }
                        else if (tempNewGreen < 0) {
                                tempNewGreen = 0;
                        }
                        newGreen = (unsigned char)tempNewGreen;
                        Pixel newPixel(newBlue, newGreen, newRed);
                        newData.push_back(newPixel);
                }
        }
        else if (color == "red") {
                for (int i = 0; i < data.size(); i++) {
                        unsigned char newBlue, newGreen, newRed;
                        newBlue = data.at(i).blue;
                        newGreen = data.at(i).green;
                        float tempRed = data.at(i).red;
                        float tempNewRed = tempRed * value;
                        if (tempNewRed > 255) {
                                tempNewRed = 255;
                        }
                        else if (tempNewRed < 0) {
                                tempNewRed = 0;
                        }
                        newRed = (unsigned char)tempNewRed;
                        Pixel newPixel(newBlue, newGreen, newRed);
                        newData.push_back(newPixel);
                }
        }
        Image newImage(newHeader, newData);
        return newImage;
}

Image Image::TakeChannel(string color) const {
        Header newHeader = header;
        vector<Pixel> newData;
        if (color == "blue") {
                for (int i = 0; i < data.size(); i++) {
                        unsigned char newBlue, newGreen, newRed;
                        newBlue = data.at(i).blue;
                        newGreen = data.at(i).blue;
                        newRed = data.at(i).blue;
                        Pixel newPixel(newBlue, newGreen, newRed);
                        newData.push_back(newPixel);
                }
        }
        else if (color == "green") {
                for (int i = 0; i < data.size(); i++) {
                        unsigned char newBlue, newGreen, newRed;
                        newBlue = data.at(i).green;
                        newGreen = data.at(i).green;
                        newRed = data.at(i).green;
                        Pixel newPixel(newBlue, newGreen, newRed);
                        newData.push_back(newPixel);
                }
        }
        else if (color == "red") {
                for (int i = 0; i < data.size(); i++) {
                        unsigned char newBlue, newGreen, newRed;
                        newBlue = data.at(i).red;
                        newGreen = data.at(i).red;
                        newRed = data.at(i).red;
                        Pixel newPixel(newBlue, newGreen, newRed);
                        newData.push_back(newPixel);
                }
        }
        Image newImage(newHeader, newData);
        return newImage;
}

Image Image::Combine(const Image& Green, const Image& Blue) {
        Header newHeader = header;
        vector<Pixel> newData;
        for (int i = 0; i < data.size(); i++) {
                unsigned char newBlue, newGreen, newRed;
                newRed = data.at(i).red;
                newBlue = Blue.data.at(i).blue;
                newGreen = Green.data.at(i).green;
                Pixel newPixel(newBlue, newGreen, newRed);
                newData.push_back(newPixel);
        }
        Image newImage(newHeader, newData);
        return newImage;
}

Image Image::Flip() {
        Header newHeader = header;
        vector<Pixel> newData;
        int maxIndex = (int)data.size() - 1;
        for (int i = maxIndex; i >= 0; i--) {
                unsigned char newBlue, newGreen, newRed;
                newBlue = data.at(i).blue;
                newGreen = data.at(i).green;
                newRed = data.at(i).red;
                Pixel newPixel(newBlue, newGreen, newRed);
                newData.push_back(newPixel);
        }
        Image newImage(newHeader, newData);
        return newImage;
}
