#include <iostream>
using namespace std;

#include "olcNoiseMaker.h"
#include "soundSample.h"

//#TODO fonction pour loader tous les samples
//#TODO clamp l'audio
//#TODO Trouver why the fuck est-ce que dans le m_leftSamples ya les données du kick et non du rack4
//#TODO améliorer la gestion du volume

int main() {
    const unsigned int bufferSize = 1<<10;
    const unsigned int chunkSize = 1<<7;
    const unsigned int channels = 1;
    const unsigned int bitPerSamples = 16;
    string fileName[] = { "S:/Projets/YYZ/Ludwig Sparkle Multi-Velocity/24 Bit WAV Files/Rack/Wet/IAR_Wet_Rack4.wav" , "S:/Projets/YYZ/Ludwig Sparkle Multi-Velocity/24 Bit WAV Files/Kicks/Wet/IAR_Wet_Kick5.wav", "S:/Projets/YYZ/Ludwig Sparkle Multi-Velocity/24 Bit WAV Files/Crash 1/Wet/IAR_Wet_Crash3.wav" };
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

    storeSamples(sounds[0].m_Samples, &sounds[0].m_SamplesSizeBytes, "Samples.txt", bitPerSamples, bitPerSamples, channels);

    cout << "Init done\n";

    audioOut.appendQueue(sounds[0]);

    bool lastState[qteSounds]{};
    byte keys[] = { 0x41, 0x53 , 0x44 };
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