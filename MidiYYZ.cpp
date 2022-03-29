#include <iostream>
using namespace std;

#include "olcNoiseMaker.h"
#include "files.h"
#include "soundSample.h"

//#TODO une queue pour les request de jouage de sample
//#TODO fonction pour loader tout les sample

int main() {
    string fileName = "S:/Projets/YYZ/Ludwig Sparkle Multi-Velocity/24 Bit WAV Files/Rack/Wet/IAR_Wet_Rack4.wav";

    wavMetadataAdress fileAdresses;
    wavMetadata fileMetadata = getFileMetadata(fileName);
    printMetadata(fileMetadata);

    const unsigned int chunkSize = 512;
    const unsigned int qteSample = fileMetadata.Subchunk2Size / 6 / 2;

    soundSample<int> WetRack4(qteSample, 1, fileMetadata.SampleRate, chunkSize);
    getAudioData(&fileName, WetRack4.m_leftSamples, WetRack4.m_rightSamples, &qteSample, &fileAdresses.data);

    vector<wstring> devices = olcNoiseMaker<int>::Enumerate();

    wcout << "Found devices: " << endl;
    for (auto &d : devices) wcout << d << endl;

    olcNoiseMaker<int> sound(devices[0], 44100, 1, 4, chunkSize);
    /*sound.appendQueue(WetRack4);
    sound.appendQueue(WetRack4);
    sound.appendQueue(WetRack4);*/



    bool lastState = 0;
    while (true) {
        if (GetAsyncKeyState(0x41) & 0x8000 and lastState == 0) {
            sound.appendQueue(WetRack4);
            lastState = 1;
        }
        else if (!GetAsyncKeyState(0x41) & 0x0001 and lastState == 1) {
            lastState = 0;
        }
    }
    return 0;
}