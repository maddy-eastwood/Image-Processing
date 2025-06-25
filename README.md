# Image Processing Photo Editor

**By: Madeleine Eastwood**

**Video Demo:** coming soon

**About:** The goal of this project was to practice reading, manipulating, and writing
binary image files (.tga) in C++, while gaining hands-on experience with image processing algorithms.
Through this project, I developed a deeper understanding of how computers represent the world we live in using pixels
and how color information is stored and manipulated at the binary level. 
It was exciting to visualize the effects of my algorithms on the input images. 
Additionally, I became more comfortable using the command line and executing tasks with Bash commands.

## Photo Editor Methods

I implemented 15 image manipulation methods that the user can call on an input image of their choosing 
from the CLI (command-line interface):

* `multiply`
* `subtract`
* `overlay`
* `screen`
* `combine`
* `flip`
* `onlyred`
* `onlygreen`
* `onlyblue`
* `addred`
* `addgreen`
* `addblue`
* `scalered`
* `scalegreen`
* `scaleblue`

## How to use the photo editor

**macOS/Linux users**

You can use the prebuilt `photoeditor.out` executable provided in this repo,
or build the executable yourself (requires g++ compiler to be installed) with this command:
`g++ -std=c++11 -o photoeditor.out src/*.cpp`

Then run the program using: `./photoeditor.out <args>`

**Windows users**
1. Install MinGW: https://www.mingw-w64.org/
2. Open the MinGW terminal or Command Prompt (ensure g++ is available in your PATH).
3. Compile the project: `g++ -std=c++11 -o photoeditor.exe src/*.cpp`
4. Then run the program using: `photoeditor.exe <args>`