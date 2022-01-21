# OpenGL CMake Template - M1 Mac

This is a project template for CMake, you can use this project template to create Modern OpenGL apps with C++. Before using it make sure you follow the guidelines bellow, If you need more information about how to use this, I made a whole article about this which has all the details. [Link to article](https://shrainu.github.io/posts/openGLonM1mac.html).

## How to use it

 - Download [homebrew](https://brew.sh/).
 - From the terminal use these commands
     - `git clone https://github.com/jlevallois/m1-cmake-opengl-template.git`
	 - `cd m1-cmake-opengl-template`
	 - `mkdir build`
	 - `cd build`
	 - `cmake ..`
	 - `cmake --build .`
	 - `./main`

## Using with Visual Studio Code

 In order to use this template with Visual Studio Code you'll these extensions
 - [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
 - [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)
 - [CMake](https://marketplace.visualstudio.com/items?itemName=twxs.cmake)

 After you install all the extensions and enable them close the vscode and project folder and open them again.

## FAQ

### My code doesn't works what should i do?

Make sure you followed all the steps correctly. If you're still having problems read my about this topic [here](https://shrainu.github.io/posts/openGLonM1mac.html). I explained how to use this in depth there.

### My code used to work but it doesn't now

Make sure that you include GLEW before GLFW, and before including GLEW you have this line `#define GLEW_STATIC`. If you already have that, and you're still having problems you can contact me from this e mail : **batuhanyigit1705@gmail.com**. Before contacting just make sure that you didn't make anything wrong in your code.
