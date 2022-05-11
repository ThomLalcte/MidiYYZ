#include <iostream>
using namespace std;

#include "olcNoiseMaker.h"
#include "soundSample.h"

//#TODO fonction pour loader tous les samples
//#TODO clamp l'audio
//#TODO améliorer la gestion du volume

int main() {
    const unsigned int bufferSize = 1<<11;
    const unsigned int chunkSize = 1<<8;
    const unsigned int channels = 2;
    const unsigned int bitPerSamples = 24;
    string fileName[] = { "S:/Projets/YYZ/Ludwig Sparkle Multi-Velocity/24 Bit WAV Files/Rack/Wet/IAR_Wet_Rack4.wav" ,
        "S:/Projets/YYZ/Ludwig Sparkle Multi-Velocity/24 Bit WAV Files/Kicks/Wet/IAR_Wet_Kick5.wav",
        "S:/Projets/YYZ/Ludwig Sparkle Multi-Velocity/24 Bit WAV Files/Crash 1/Wet/IAR_Wet_Crash3.wav",
        "S:/Projets/YYZ/Ludwig Sparkle Multi-Velocity/24 Bit WAV Files/Floor/Wet/IAR_Wet_Floor5.wav"
    };
    const unsigned int qteSounds = size(fileName);

    soundSample sounds[qteSounds];
    

    for (int i = 0; i < qteSounds; i++ ) {
        sounds[i].init(fileName[i], chunkSize, bitPerSamples, channels);
    }

    cout << "Done loading files\n";

    vector<wstring> devices = olcNoiseMaker::Enumerate();

    wcout << "Found devices:\n";
    for (auto &d : devices) wcout << d << endl;

    olcNoiseMaker audioOut(devices[0], 44100, channels, bufferSize / chunkSize, chunkSize, bitPerSamples);

    storeSamples(sounds[1].m_Samples, &sounds[1].m_SamplesSizeBytes, "Samples.txt", bitPerSamples, bitPerSamples, channels);

    cout << "Init done\n";

    bool lastState[qteSounds]{};
    byte keys[] = { 0x41, 0x53 , 0x44 , 0x46};
    while (true) {
        for (int i = 0; i < qteSounds; i++) {
            if (GetAsyncKeyState(keys[i]) & 0x8000 and lastState[i] == 0) {
                audioOut.appendQueue(sounds[i]);
                lastState[i] = 1;
            }
            else if (!GetAsyncKeyState(keys[i]) & 0x0001 and lastState[i] == 1) {
                lastState[i] = 0;
            }
        }
    }
    return 0;
}