#include <iostream>
#include <cstring>
#include <string>
#include "data.h"
using namespace std;

int main(int argc, char* argv[])
{
    // 1. Check if user asked for help
    if (argc == 1 || (argc == 2 && (strcmp(argv[1], "--help") == 0))) {
        cout << "Project 2: Image Processing, Fall 2024" << endl;
        cout << endl;
        cout << "Usage:\n" << "\t" << "./project2.out [output] [firstImage] [method] [...]" << endl;
        return 0;
    }

    // 2. Check that user provided valid output file name
    if (argc == 2) {
        // if an output file name is provided but no further instructions
        cerr << "Invalid file name." << endl;
        return 1;
    }
    string outputName = (string)argv[1];
    if (outputName.find(".tga") == string::npos) {
        cerr << "Invalid file name." << endl;
        return 1;
    }

    // 3. Validate tracking image & initialize tracking image
    string trackingImageName = (string)argv[2];
    if (trackingImageName.find(".tga") == string::npos) {
        cerr << "Invalid file name." << endl;
        return 1;
    }
    ifstream trackingImageFile(trackingImageName, ios_base::binary);
    if (!trackingImageFile.is_open()) {
        cerr << "File does not exist." << endl;
        return 1;
    }
    // Check if no further information was given to terminate program
    if (argc == 3) {
        // input & output names provided, but no method is requested
        cerr << "Invalid method name." << endl;
        return 1;
    }
    Image trackingImage(trackingImageFile);
    try {
        int argIndex = 3;
        // 4. Iterate through each method requested
        while (argIndex < argc) {
            if (strcmp(argv[argIndex], "multiply") == 0) {
                // check that additional argument is provided
                if (argIndex + 1 >= argc) {
                    throw invalid_argument("Missing argument.");
                }
                argIndex++; // moves index to second argument
                string secondImageName = (string)argv[argIndex];
                if (secondImageName.find(".tga") == string::npos) {
                    throw invalid_argument("Invalid argument, invalid file name.");
                }
                ifstream secondImageFile(secondImageName, ios_base::binary);
                if (!secondImageFile.is_open()) {
                    throw invalid_argument("Invalid argument, file does not exist.");
                }
                if (argIndex == 4) {
                    cout << "Multiplying " << trackingImageName << " and " << secondImageName << endl;
                } else {
                    cout << "Multiplying output of previous step and " << secondImageName << endl;
                }
                Image secondImage(secondImageFile);
                Image tempImage = trackingImage * secondImage;
                trackingImage = tempImage;
                secondImageFile.close();
            }
            else if (strcmp(argv[argIndex], "subtract") == 0) {
                if (argIndex + 1 >= argc) {
                    throw invalid_argument("Missing argument.");
                }
                argIndex++;
                string bottomImageName = (string)argv[argIndex];
                if (bottomImageName.find(".tga") == string::npos) {
                    throw invalid_argument("Invalid argument, invalid file name.");
                }
                ifstream bottomImageFile(bottomImageName, ios_base::binary);
                if (!bottomImageFile.is_open()) {
                    throw invalid_argument("Invalid argument, file does not exist.");
                }
                if (argIndex == 4) {
                    cout << "Subtracting " << bottomImageName << " from " << trackingImageName << endl;
                } else {
                    cout << "Subtracting " << bottomImageName << " from output of previous step" << endl;
                }
                Image bottomImage(bottomImageFile);
                Image tempImage = trackingImage - bottomImage;
                trackingImage = tempImage;
                bottomImageFile.close();
            }
            else if (strcmp(argv[argIndex], "overlay") == 0) {
                if (argIndex + 1 >= argc) {
                    throw invalid_argument("Missing argument.");
                }
                argIndex++;
                string bottomImageName = (string)argv[argIndex];
                if (bottomImageName.find(".tga") == string::npos) {
                    throw invalid_argument("Invalid argument, invalid file name.");
                }
                ifstream bottomImageFile(bottomImageName, ios_base::binary);
                if (!bottomImageFile.is_open()) {
                    throw invalid_argument("Invalid argument, file does not exist.");
                }
                if (argIndex == 4) {
                    cout << "Overlaying" << trackingImageName << " onto " << bottomImageName << endl;
                } else {
                    cout << "Overlaying output of previous step onto " << bottomImageName << endl;
                }
                Image bottomImage(bottomImageFile);
                Image tempImage = trackingImage.Overlay(bottomImage);
                trackingImage = tempImage;
                bottomImageFile.close();
            }
            else if (strcmp(argv[argIndex], "screen") == 0) {
                if (argIndex + 1 >= argc) {
                    throw invalid_argument("Missing argument.");
                }
                argIndex++;
                string topImageName = (string)argv[argIndex];
                if (topImageName.find(".tga") == string::npos) {
                    throw invalid_argument("Invalid argument, invalid file name.");
                }
                ifstream topImageFile(topImageName, ios_base::binary);
                if (!topImageFile.is_open()) {
                    throw invalid_argument("Invalid argument, file does not exist.");
                }
                if (argIndex == 4) {
                    cout << "Screening" << trackingImageName << " with " << topImageName << endl;
                } else {
                    cout << "Screening output of previous step with " << topImageName << endl;
                }
                Image topImage(topImageFile);
                Image tempImage = trackingImage.Screen(topImage);
                trackingImage = tempImage;
                topImageFile.close();
            }
            else if (strcmp(argv[argIndex], "combine") == 0) {
                // combine method requires 2 additional arguments
                if (argIndex + 2 >= argc) {
                    throw invalid_argument("Missing argument.");
                }
                // tracking image is the source for the red layer
                argIndex++; // index of the source for the green layer
                string greenLayerName = (string)argv[argIndex];
                if (greenLayerName.find(".tga") == string::npos) {
                    throw invalid_argument("Invalid argument, invalid file name.");
                }
                ifstream greenLayerFile(greenLayerName, ios_base::binary);
                if (!greenLayerFile.is_open()) {
                    throw invalid_argument("Invalid argument, file does not exist.");
                }
                Image greenLayer(greenLayerFile);

                argIndex++; // index of the source for the blue layer
                string blueLayerName = (string)argv[argIndex];
                if (blueLayerName.find(".tga") == string::npos) {
                    throw invalid_argument("Invalid argument, invalid file name.");
                }
                ifstream blueLayerFile(blueLayerName, ios_base::binary);
                if (!blueLayerFile.is_open()) {
                    throw invalid_argument("Invalid argument, file does not exist.");
                }
                Image blueLayer(blueLayerFile);

                Image tempImage = trackingImage.Combine(greenLayer, blueLayer);
                if (argIndex == 5) {
                    cout << "Combining red channel from " << trackingImageName << ", green channel from " << greenLayerName;
                    cout << ", and blue channel from " << blueLayerName << endl;
                } else {
                    cout << "Combining red channel from output of previous step, green channel from ";
                    cout << greenLayerName << ", and blue channel from " << blueLayerName << endl;
                }
                trackingImage = tempImage;
                greenLayerFile.close();
                blueLayerFile.close();
            }
            else if (strcmp(argv[argIndex], "flip") == 0) {
                // doesn't require any additional arguments
                Image tempImage = trackingImage.Flip();
                trackingImage = tempImage;
                if (argIndex == 3) {
                    cout << "Flipping " << trackingImageName << endl;
                } else {
                    cout << "Flipping output of previous step" << endl;
                }
            }
            else if (strcmp(argv[argIndex], "onlyred") == 0) {
                // doesn't require any additional args
                Image tempImage = trackingImage.TakeChannel("red");
                trackingImage = tempImage;
                if (argIndex == 3) {
                    cout << "Retrieving red channel from " << trackingImageName << endl;
                } else {
                    cout << "Retrieving red channel from output of previous step" << endl;
                }
            }
            else if (strcmp(argv[argIndex], "onlygreen") == 0) {
                // doesn't require any additional args
                Image tempImage = trackingImage.TakeChannel("green");
                trackingImage = tempImage;
                if (argIndex == 3) {
                    cout << "Retrieving green channel from " << trackingImageName << endl;
                } else {
                    cout << "Retrieving green channel from output of previous step" << endl;
                }
            }
            else if (strcmp(argv[argIndex], "onlyblue") == 0) {
                // doesn't require any additional args
                Image tempImage = trackingImage.TakeChannel("blue");
                trackingImage = tempImage;
                if (argIndex == 3) {
                    cout << "Retrieving blue channel from " << trackingImageName << endl;
                } else {
                    cout << "Retrieving blue channel from output of previous step" << endl;
                }
            }
            else if (strcmp(argv[argIndex], "addred") == 0) {
                if (argIndex + 1 >= argc) {
                    throw invalid_argument("Missing argument.");
                }
                argIndex++;
                string stringVal = (string)argv[argIndex];
                try {
                    int val = stoi(stringVal);
                    Image tempImage = trackingImage.AddToChannel("red", val);
                    trackingImage = tempImage;
                    if (argIndex == 4) {
                        cout << "Adding " << val << " to red channel of " << trackingImageName << endl;
                    } else {
                        cout << "Adding " << val << " to red channel from output of previous step" << endl;
                    }
                } catch (invalid_argument& e) {
                    throw invalid_argument("Invalid argument, expected number.");
                }
            }
            else if (strcmp(argv[argIndex], "addgreen") == 0) {
                if (argIndex + 1 >= argc) {
                    throw invalid_argument("Missing argument.");
                }
                argIndex++;
                string stringVal = (string)argv[argIndex];
                try {
                    int val = stoi(stringVal);
                    Image tempImage = trackingImage.AddToChannel("green", val);
                    trackingImage = tempImage;
                    if (argIndex == 4) {
                        cout << "Adding " << val << " to green channel of " << trackingImageName << endl;
                    } else {
                        cout << "Adding " << val << " to green channel from output of previous step" << endl;
                    }
                } catch (invalid_argument& e) {
                    throw invalid_argument("Invalid argument, expected number.");
                }
            }
            else if (strcmp(argv[argIndex], "addblue") == 0) {
                if (argIndex + 1 >= argc) {
                    throw invalid_argument("Missing argument.");
                }
                argIndex++;
                string stringVal = (string)argv[argIndex];
                try {
                    int val = stoi(stringVal);
                    Image tempImage = trackingImage.AddToChannel("blue", val);
                    trackingImage = tempImage;
                    if (argIndex == 4) {
                        cout << "Adding " << val << " to blue channel of " << trackingImageName << endl;
                    } else {
                        cout << "Adding " << val << " to blue channel from output of previous step" << endl;
                    }
                } catch (invalid_argument& e) {
                    throw invalid_argument("Invalid argument, expected number.");
                }
            }
            else if (strcmp(argv[argIndex], "scalered") == 0) {
                if (argIndex + 1 >= argc) {
                    throw invalid_argument("Missing argument.");
                }
                argIndex++;
                string stringVal = (string)argv[argIndex];
                try {
                    int intVal = stoi(stringVal);
                    float val = intVal * 1.0f;
                    Image tempImage = trackingImage.ScaleChannel("red", val);
                    trackingImage = tempImage;
                    if (argIndex == 4) {
                        cout << "Scaling red channel of " << trackingImageName << " by " << val << endl;
                    } else {
                        cout << "Scaling red channel of output of previous step by " << val << endl;
                    }
                } catch (invalid_argument& e) {
                    throw invalid_argument("Invalid argument, expected number.");
                }
            }
            else if (strcmp(argv[argIndex], "scalegreen") == 0) {
                if (argIndex + 1 >= argc) {
                    throw invalid_argument("Missing argument.");
                }
                argIndex++;
                string stringVal = (string)argv[argIndex];
                try {
                    int intVal = stoi(stringVal);
                    float val = intVal * 1.0f;
                    Image tempImage = trackingImage.ScaleChannel("green", val);
                    trackingImage = tempImage;
                    if (argIndex == 4) {
                        cout << "Scaling green channel of " << trackingImageName << " by " << val << endl;
                    } else {
                        cout << "Scaling green channel of output of previous step by " << val << endl;
                    }
                } catch (invalid_argument& e) {
                    throw invalid_argument("Invalid argument, expected number.");
                }
            }
            else if (strcmp(argv[argIndex], "scaleblue") == 0) {
                if (argIndex + 1 >= argc) {
                    throw invalid_argument("Missing argument.");
                }
                argIndex++;
                string stringVal = (string)argv[argIndex];
                try {
                    int intVal = stoi(stringVal);
                    float val = intVal * 1.0f;
                    Image tempImage = trackingImage.ScaleChannel("blue", val);
                    trackingImage = tempImage;
                    if (argIndex == 4) {
                        cout << "Scaling blue channel of " << trackingImageName << " by " << val << endl;
                    } else {
                        cout << "Scaling blue channel of output of previous step by " << val << endl;
                    }
                } catch (invalid_argument& e) {
                    throw invalid_argument("Invalid argument, expected number.");
                }
            }
            else {
                throw invalid_argument("Invalid method name.");
            }
            argIndex++;
        }
        // Write final manipulated tracking image to output file and close files
        ofstream outputFile(outputName, ios_base::binary);
        cout << "... and saving output to " << outputName << "!" << endl;
        trackingImage.Write(outputFile);
        trackingImageFile.close();
        outputFile.close();
    } catch (invalid_argument& e) {
        cout << e.what() << endl;
    }

    return 0;
}