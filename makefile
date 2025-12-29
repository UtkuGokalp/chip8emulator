run: build_emulator
	dotnet run --project ./Chip8EmulatorLauncher/Chip8EmulatorLauncher.csproj

init:
	cmake -S ./chip8emulator/ -B ./chip8emulator/build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

clean:
	cmake --build ./chip8emulator/build --target clean

build_emulator:
	cmake --build ./chip8emulator/build

build_launcher:
	dotnet build --project ./Chip8EmulatorLauncher/Chip8EmulatorLauncher.csproj
