#pragma once
#include <string>
#include <iostream>

struct wavMetadata
{
	char ChunkID[5] = "";
	int ChunkSize;
	char Format[5] = "";
	char Subchunk1ID[5] = "";
	int Subchunk1Size;
	int AudioFormat;
	int NumChannels;
	int SampleRate;
	int ByteRate;
	int BlockAlign;
	int BitsPerSample;
	char Subchunk2ID[5] = "";
	int Subchunk2Size;
};

struct wavMetadataAdress {
	int ChunkID = 0x0;
	int ChunkSize = 0x4;
	int Format = 0x8;
	int Subchunk1ID = 0x2d2;
	int Subchunk1Size = 0x2d6;
	int AudioFormat = 0x2da;
	int NumChannels = 0x2dc;
	int SampleRate = 0x2de;
	int ByteRate = 0x2e2;
	int BlockAlign = 0x2e6;
	int BitsPerSample = 0x2e8;
	int Subchunk2ID = 0x3f8;
	int Subchunk2Size = 0x3fc;
	int data = 0x400;
};

wavMetadata getFileMetadata(const string fileName) {
	wavMetadata fileMetadata{};
	wavMetadataAdress adresses;
	std::ifstream ifile(fileName,std::ifstream::binary);
	if (ifile.is_open()) {
		ifile.seekg(adresses.ChunkID);
		ifile.read(fileMetadata.ChunkID,4);
		int tmpint = 0;
		ifile.seekg(adresses.ChunkSize);
		for (byte i = 0; i < 4; i++) {
			tmpint += ifile.get() << 8 * i;
		}
		fileMetadata.ChunkSize = tmpint;
		ifile.seekg(adresses.Format);
		ifile.read(fileMetadata.Format, 4);
		ifile.seekg(adresses.Subchunk1ID);
		ifile.read(fileMetadata.Subchunk1ID, 4);
		ifile.seekg(adresses.Subchunk1Size);
		tmpint = 0;
		for (byte i = 0; i < 4; i++) {
			tmpint += ifile.get() << 8 * i;
		}
		fileMetadata.Subchunk1Size = tmpint;
		tmpint = 0;
		ifile.seekg(adresses.AudioFormat);
		for (byte i = 0; i < 2; i++) {
			tmpint += ifile.get() << 8 * i;
		}
		fileMetadata.AudioFormat = tmpint;
		tmpint = 0;
		ifile.seekg(adresses.NumChannels);
		for (byte i = 0; i < 2; i++) {
			tmpint += ifile.get() << 8 * i;
		}
		fileMetadata.NumChannels = tmpint;
		tmpint = 0;
		ifile.seekg(adresses.SampleRate);
		for (byte i = 0; i < 4; i++) {
			tmpint += ifile.get() << 8 * i;
		}
		fileMetadata.SampleRate = tmpint;
		tmpint = 0;
		ifile.seekg(adresses.ByteRate);
		for (byte i = 0; i < 4; i++) {
			tmpint += ifile.get() << 8 * i;
		}
		fileMetadata.ByteRate = tmpint;
		tmpint = 0;
		ifile.seekg(adresses.BlockAlign);
		for (byte i = 0; i < 2; i++) {
			tmpint += ifile.get() << 8 * i;
		}
		fileMetadata.BlockAlign = tmpint;
		tmpint = 0;
		ifile.seekg(adresses.BitsPerSample);
		for (byte i = 0; i < 2; i++) {
			tmpint += ifile.get() << 8 * i;
		}
		fileMetadata.BitsPerSample = tmpint;
		ifile.seekg(adresses.Subchunk2ID);
		ifile.read(fileMetadata.Subchunk2ID, 4);
		tmpint = 0;
		ifile.seekg(adresses.Subchunk2Size);
		for (byte i = 0; i < 4; i++) {
			tmpint += ifile.get() << 8 * i;
		}
		fileMetadata.Subchunk2Size = tmpint;
		ifile.close();
	}
	return fileMetadata;
}

void printMetadata(const wavMetadata fileMetaData) {
	//wavMetadata fileMetaData = getFileMetadata("S:/Projets/YYZ/Ludwig Sparkle Multi-Velocity/24 Bit WAV Files/Rack/Wet/IAR_Wet_Rack4.wav");
	cout << "ChunkID: " << fileMetaData.ChunkID << endl
		<< "ChunkSize: " << fileMetaData.ChunkSize << endl
		<< "Format: " << fileMetaData.Format << endl
		<< "Subchunk1ID: " << fileMetaData.Subchunk1ID << endl
		<< "Subchunk1Size: " << fileMetaData.Subchunk1Size << endl
		<< "AudioFormat: " << fileMetaData.AudioFormat << endl
		<< "NumChannels: " << fileMetaData.NumChannels << endl
		<< "SampleRate: " << fileMetaData.SampleRate << endl
		<< "ByteRate: " << fileMetaData.ByteRate << endl
		<< "BlockAlign: " << fileMetaData.BlockAlign << endl
		<< "BitsPerSample: " << fileMetaData.BitsPerSample << endl
		<< "Subchunk2ID: " << fileMetaData.Subchunk2ID << endl
		<< "Subchunk2Size: " << fileMetaData.Subchunk2Size << endl;
}

// extrait les données de son des fichier wav du kit de sample.
void getAudioData(string *fileName, int* chunkl, int* chunkr, const unsigned int* qteSamples, const int* dataAdress) {
	std::ifstream ifile(*fileName, std::ifstream::binary);
	if (ifile.is_open()) {
		ifile.seekg(*dataAdress);
		int tmpint = 0;
		for (unsigned int i = 0; i < *qteSamples; i++) {
			chunkl[i] = ((ifile.get() << 8) & 0xff00) | ((ifile.get() << 16) & 0xff0000) | ((ifile.get() << 24) & 0xff000000);
			chunkr[i] = ((ifile.get() << 8) & 0xff00) | ((ifile.get() << 16) & 0xff0000) | ((ifile.get() << 24) & 0xff000000);
		}
		ifile.close();
	}
}

void storeSamples(int* chunkl, const unsigned int* qteSamples) {
	std::ofstream ifile("samples.txt");
	if (ifile.is_open()) {
		for (unsigned int i = 0; i < *qteSamples; i++) {
			ifile << chunkl[i] << endl;
		}
		ifile.close();
	}
}
