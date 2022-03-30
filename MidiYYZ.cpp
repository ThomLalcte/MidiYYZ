#include <iostream>
using namespace std;

#include "olcNoiseMaker.h"
#include "soundSample.h"

//#TODO fonction pour loader tout les sample

int main() {
    const unsigned int bufferSize = 1<<12;
    const unsigned int chunkSize = 1<<8;
    string fileName[] = { "S:/Projets/YYZ/Ludwig Sparkle Multi-Velocity/24 Bit WAV Files/Rack/Wet/IAR_Wet_Rack4.wav" , "S:/Projets/YYZ/Ludwig Sparkle Multi-Velocity/24 Bit WAV Files/Kicks/Wet/IAR_Wet_Kick5.wav", "S:/Projets/YYZ/Ludwig Sparkle Multi-Velocity/24 Bit WAV Files/Crash 1/Wet/IAR_Wet_Crash3.wav" };
    const unsigned int qteSounds = size(fileName);

    soundSample<int>* sounds = new soundSample<int>[qteSounds];
    

    for (int i = 0; i < qteSounds; i++ ) {
        sounds[i] = soundSample<int>(fileName[i], chunkSize);
    }

    vector<wstring> devices = olcNoiseMaker<int>::Enumerate();

    std::wcout << "Found devices: " << endl;
    for (auto &d : devices) std::wcout << d << endl;

    olcNoiseMaker<int> audioOut(devices[0], 44100, 1, bufferSize / chunkSize, chunkSize);

    storeSamples<int>(sounds[0].m_rightSamples, &sounds[0].m_qteSamples, "rightSamples.txt");
    storeSamples<int>(sounds[0].m_leftSamples, &sounds[0].m_qteSamples,"leftSamples.txt");

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