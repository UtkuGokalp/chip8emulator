#include <fstream>
#include <cmath>
#include <cstdint>

int main()
{
    int sampleRate = 44100;
    float frequency = 440.0f;
    float duration = 0.1f; // 100 ms
    int numSamples = int(sampleRate * duration);
    int16_t amplitude = 8000;

    std::ofstream file("chip8_beep.wav", std::ios::binary);

    // WAV header
    file.write("RIFF", 4);
    uint32_t fileSize = 36 + numSamples * 2;
    file.write(reinterpret_cast<char*>(&fileSize), 4);
    file.write("WAVEfmt ", 8);

    uint32_t fmtSize = 16;
    uint16_t format = 1; // PCM
    uint16_t channels = 1;
    uint32_t byteRate = sampleRate * channels * 2;
    uint16_t blockAlign = channels * 2;
    uint16_t bitsPerSample = 16;

    file.write(reinterpret_cast<char*>(&fmtSize), 4);
    file.write(reinterpret_cast<char*>(&format), 2);
    file.write(reinterpret_cast<char*>(&channels), 2);
    file.write(reinterpret_cast<char*>(&sampleRate), 4);
    file.write(reinterpret_cast<char*>(&byteRate), 4);
    file.write(reinterpret_cast<char*>(&blockAlign), 2);
    file.write(reinterpret_cast<char*>(&bitsPerSample), 2);

    file.write("data", 4);
    uint32_t dataSize = numSamples * 2;
    file.write(reinterpret_cast<char*>(&dataSize), 4);

    // Square wave samples
    for (int i = 0; i < numSamples; ++i)
    {
        float t = float(i) / sampleRate;
        int16_t sample = (fmod(t * frequency, 1.0f) < 0.5f)
            ? amplitude
            : -amplitude;

        file.write(reinterpret_cast<char*>(&sample), 2);
    }

    return 0;
}
