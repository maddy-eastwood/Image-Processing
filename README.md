# Image Processing Photo Editor

**By: Madeleine Eastwood**

**About:** The goal of this project was to practice reading, manipulating, and writing
binary image files (.tga) in C++, while gaining hands-on experience with image processing algorithms.
Through this project, I developed a deeper understanding of how computers represent the world we live in using pixels
and how color information is stored and manipulated at the binary level. 
It was exciting to visualize the effects of my algorithms on the input images. 
Additionally, I became more comfortable using the command line and executing tasks with Bash commands.

## How to Build the Executable

**macOS/Linux users**

You can use the prebuilt `photoeditor.out` executable provided in this repo,
or build the executable yourself:
1. Ensure you have a g++ compiler installed.
2. Open a terminal and navigate to the project directory.
3. Run this build command:
    ```bash
    g++ -std=c++11 -o photoeditor.out src/*.cpp
    ```
4. Then run the program using: `./photoeditor.out <arguments>`

**Windows users**

To build and run this project on Windows, follow these steps:

1. Install MinGW: https://www.mingw-w64.org/
2. Open the MinGW terminal or Command Prompt (ensure g++ is in your PATH).
3. Navigate to the project directory.
4. Compile the project: 
    ```
    g++ -std=c++11 -o photoeditor.exe src/*.cpp
    ```
5. Then run the program using: `photoeditor.exe <arguments>`

## Photo Editor Methods and How to Use the CLI + Executable

Here is the input template for using this project in the CLI (command-line interface):

`./photoeditor.out [output_file_name] [path_to_first_image] [method] [...]`

If no arguments are provided or if the only argument provided is `--help`, a help message will
be printed to remind the user of the correct input template.

I have implemented 15 common image manipulation methods that the user can apply to an input (.tga) image of their choosing
via the CLI. Several methods can be performed on a single image; the methods will be performed in order 
from left to right.

* `multiply`: Applies the multiply blending mode to the input image (top layer) by multiplying pixel values with a 
    second image file used as the bottom layer. This results in a darker image where colors combine. Provide the
    path/filename of the second image file as an additional argument following the method "multiply" in the CLI.
* `subtract`: Applies the subtract blending mode by subtracting the bottom layer image from the input image (top layer).
    To clarify, output_image = top_layer - bottom_layer.
    This typically results in a darker image, as areas in the top layer are darkened where the bottom layer
    is brighter.
    This method requires a second image file to use as the bottom layer. Provide the path/filename of the second 
    image file as an additional argument following the method "subtract" in the CLI.
* `overlay`: Applies the overlay blending mode to the input image (top layer) to enhance contrast. This method requires
   a second image file to use as the bottom layer. The brightness of the bottom layer determines whether each pixel in the top layer is darkened or lightened.
   Provide the path/filename of the second image file as an additional argument following the method "overlay" in the CLI.
* `screen`: Lightens the input image (bottom layer) by blending it with a second image (top layer). Bright areas become 
   brighter, making the combined image look more illuminated. Provide the path/filename of the second image 
   to use as the top layer after the method "screen" in the CLI. 
* `combine`: Combines 3 separate grayscale images representing the red, green, and blue color channels into a single full-color image.
   The input image for this method must be the red channel. This method requires 2 additional 
   arguments: the green channel image and the blue channel image. Provide the path/filename of those images in that order,
   with a space to separate them.
   This method will merge all 3 layers into a fully-colored image.
* `flip`: This method rotates the input image 180 degrees, flipping it upside down, and requires no additional arguments.
* `onlyred`: This method retrieves the red channel from the input image and requires no additional arguments.
* `onlygreen`: This method retrieves the green channel from the input image and requires no additional arguments.
* `onlyblue`: This method retrieves the blue channel from the input image and requires no additional arguments.
* `addred`: This method will add a given integer value to the red channel of the input image. This will increase the red
   intensity and overall make the image have a warmer tone. A second argument is 
   required; provide the integer value to be added to the red channel after the method "addred".
* `addgreen`: This method will add a given integer value to the green channel of the input image. This will increase the green
  intensity and overall make the image appear more vibrant or cooler. A second argument is
  required; provide the integer value to be added to the green channel after the method "addgreen".
* `addblue`: This method will add a given integer value to the blue channel of the input image. This will increase the blue
  intensity and overall make the image have a cooler tone. A second argument is
  required; provide the integer value to be added to the blue channel after the method "addblue".
* `scalered`: Applies a scaling factor to the red channel of the input image. This method requires a second argument, 
   the integer value by which the red channel will be multiplied. For example, a value of 2 will double the red
   intensity, whereas a value of 0 will remove all red from the image. Provide the integer scaling factor after the
   method "scalered" in the CLI.
* `scalegreen`: Applies a scaling factor to the green channel of the input image. This method requires a second argument,
  the integer value by which the green channel will be multiplied. For example, a value of 2 will double the green
  intensity, whereas a value of 0 will remove all green from the image. Provide the integer scaling factor after the
  method "scalegreen" in the CLI.
* `scaleblue`: Applies a scaling factor to the blue channel of the input image. This method requires a second argument,
  the integer value by which the blue channel will be multiplied. For example, a value of 2 will double the blue
  intensity, whereas a value of 0 will remove all blue from the image. Provide the integer scaling factor after the
  method "scaleblue" in the CLI.

