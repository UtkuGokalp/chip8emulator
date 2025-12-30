# Licensing
This project is licensed under the MIT license. See the [license](LICENSE.txt) file.

# Chip-8 Emulator
This is a project I started to learn more about emulation, something that I am curious about. I chose Chip-8 because it is a simple system, which I thought was appropriate for my first emulation project. I am using olcPixelGameEngine for rendering and olcSoundWaveEngine for sound, which have their respective GitHub repositories [here](https://github.com/OneLoneCoder/olcPixelGameEngine) and [here](https://github.com/OneLoneCoder/olcSoundWaveEngine).

I developed the emulator itself in C++. Once I found Chip-8 ROMs [here](https://github.com/kripod/chip8-roms) and added them to the project, it became apparent that I would need a launcher because of the amount of ROMs. That's when I decided to write a launcher in C#, which scans the folders, lists all the ROMs and runs the emulator with the selected ROM.

The following is a screenshot of the launcher. It is configured so that you can only choose a single ROM, trying to choose another one deselects the currently selected ROM. The ROMs in the "Hires" category doesn't work at all, since they require extra features that exist on extended versions of Chip-8 and the emulator doesn't currently support them.

![Launcher](Launcher.jpg)

Finally, the following is a demo of the emulator running the Pong (1 player) ROM, which can be found under the Games category.

![EmulatorDemo](Pong.gif)

# Building on Windows
On Windows, project files of Visual Studio should contain all the necessary information to build and run the emulator. Just click the Start button.

# Building on Linux (Ubuntu)
On Ubuntu, you first need to install the necessary libraries.

```
sudo apt install make cmake libgl-dev libpng-dev libpulse-dev dotnet8
```

Once you install these, you can go to the directory you cloned the project into to build the project.
The project internally uses CMake, but in order to simplify the build commands I added a Makefile layer on top of CMake. Once you are in the project's directory, run:

```
make init
```

This will create the build folder for CMake. You only need to do this once unless you change CMakeLists.txt.

After the build system is initialized, you can simply run:

```
make
```

This will build the emulator first, then the launcher. Finally, it will start the launcher. From there, you can choose a ROM and start the emulator.

If you just want to build the emulator (and not the launcher), you can run the following command after initializing the build system:

```
make build_emulator
```

Similarly, if you just want to build the launcher (and not run it as well as not build the emulator), you can run the following command after initializing the build system:

```
make build_launcher
```

And finally, to clean everything you can run:

```
make clean
```
