# Licensing
This project is licensed under the MIT license. See the [license](LICENSE.txt) file.

# Chip-8 Emulator
This is a project I started to learn more about emulation, something that I am curious about. I chose Chip-8 because it is a simple system, which I thought was appropriate for my first emulation project.

I developed the emulator itself in C++. Once I found free-to-use ROMs for Chip-8 (https://github.com/kripod/chip8-roms) and added them to the project, it became apparent that I would need a helper program to launch the ROMs because of the amount. That's when I decided to write a launcher in C#, which scans the folders, lists all the ROMs and runs the emulator with the selected ROM. I am currently in the process of adding new visualizations for keyboard input and data in RAM.

