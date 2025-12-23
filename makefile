CC=g++
CFLAGS=-std=c++20
SRC=chip8emulator/src/*.cpp
INC=chip8emulator/include/
LIBS=-lX11 -lGL -lpthread -lpng -lasound
TARGET_DIR=chip8emulator/bin/
TARGET_BIN=chip8emulator
TARGET=$(TARGET_DIR)$(TARGET_BIN)
LAUNCHER_FOLDER=Chip8EmulatorLauncher/
LAUNCHER_PROJ_FILE=Chip8EmulatorLauncher.csproj
LAUNCHER_PROJ=$(LAUNCHER_FOLDER)$(LAUNCHER_PROJ_FILE)

run:
	mkdir -p $(TARGET_DIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) -I$(INC) $(LIBS)
	dotnet run --project $(LAUNCHER_PROJ)

build:
	mkdir -p $(TARGET_DIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) -I$(INC) $(LIBS)
	dotnet build $(LAUNCHER_PROJ)

clean:
	rm $(TARGET)
	dotnet clean $(LAUNCHER_PROJ)
