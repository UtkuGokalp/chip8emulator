run: build_emulator
	dotnet run --project Chip8EmulatorLauncher/Chip8EmulatorLauncher.csproj

init_build_system:
	cmake -S ./chip8emulator/ -B build

build_emulator:
	cmake --build chip8emulator/build

build_launcher:
	dotnet build --project Chip8EmulatorLauncher/Chip8EmulatorLauncher.csproj
