run: build_emulator
	dotnet run --project ./Chip8EmulatorLauncher/Chip8EmulatorLauncher.csproj

init_build_system:
	cmake -S ./chip8emulator/ -B ./chip8emulator/build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

build_emulator:
	cmake --build ./chip8emulator/build

build_launcher:
	dotnet build --project ./Chip8EmulatorLauncher/Chip8EmulatorLauncher.csproj
