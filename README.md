git clone --recursive https://github.com/v310r/Knight.git OR git clone --recurse-submodules https://github.com/v310r/Knight.git to clone the repo

This project only supports Windows. To support other platforms build settings should be tuned.

To build the project, you should first build the SFML library. Checkout to the most stable version of it, that is present (for this project I used 2.6.x). Run cmake in the build folder as it is shown in the picture below:

![image](https://github.com/v310r/Knight/assets/80487632/7b82b38b-8d23-4f8e-9f0c-4798d6739517)

Then, go to the root folder and run GenerateProject.bat. Open a solution, choose whatever configuration you'd prefer (Debug or Release), and build the whole solution.

Now you should be able to play the game, enjoy :-).

![image](https://github.com/v310r/Knight/assets/80487632/d0d9e186-690f-4490-b252-f0afb07fd8f6)
