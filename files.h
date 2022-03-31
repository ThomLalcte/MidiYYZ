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

namespace adresses {
	const unsigned int ChunkID = 0x0;
	const unsigned int ChunkSize = 0x4;
	const unsigned int Format = 0x8;
	const unsigned int Subchunk1ID = 0x2d2;
	const unsigned int Subchunk1Size = 0x2d6;
	const unsigned int AudioFormat = 0x2da;
	const unsigned int NumChannels = 0x2dc;
	const unsigned int SampleRate = 0x2de;
	const unsigned int ByteRate = 0x2e2;
	const unsigned int BlockAlign = 0x2e6;
	const unsigned int BitsPerSample = 0x2e8;
	const unsigned int Subchunk2ID = 0x3f8;
	const unsigned int Subchunk2Size = 0x3fc;
	const unsigned int data = 0x400;
}

wavMetadata getFileMetadata(const string fileName) {
	wavMetadata fileMetadata{};
	//wavMetadataAdress adresses;
	std::ifstream ifile(fileName,std::ifstream::binary);
	if (ifile.is_open()) {
		ifile.seekg(adresses::ChunkID);
		ifile.read(fileMetadata.ChunkID,4);
		int tmpint = 0;
		ifile.seekg(adresses::ChunkSize);
		for (byte i = 0; i < 4; i++) {
			tmpint += ifile.get() << 8 * i;
		}
		fileMetadata.ChunkSize = tmpint;
		ifile.seekg(adresses::Format);
		ifile.read(fileMetadata.Format, 4);
		ifile.seekg(adresses::Subchunk1ID);
		ifile.read(fileMetadata.Subchunk1ID, 4);
		ifile.seekg(adresses::Subchunk1Size);
		tmpint = 0;
		for (byte i = 0; i < 4; i++) {
			tmpint += ifile.get() << 8 * i;
		}
		fileMetadata.Subchunk1Size = tmpint;
		tmpint = 0;
		ifile.seekg(adresses::AudioFormat);
		for (byte i = 0; i < 2; i++) {
			tmpint += ifile.get() << 8 * i;
		}
		fileMetadata.AudioFormat = tmpint;
		tmpint = 0;
		ifile.seekg(adresses::NumChannels);
		for (byte i = 0; i < 2; i++) {
			tmpint += ifile.get() << 8 * i;
		}
		fileMetadata.NumChannels = tmpint;
		tmpint = 0;
		ifile.seekg(adresses::SampleRate);
		for (byte i = 0; i < 4; i++) {
			tmpint += ifile.get() << 8 * i;
		}
		fileMetadata.SampleRate = tmpint;
		tmpint = 0;
		ifile.seekg(adresses::ByteRate);
		for (byte i = 0; i < 4; i++) {
			tmpint += ifile.get() << 8 * i;
		}
		fileMetadata.ByteRate = tmpint;
		tmpint = 0;
		ifile.seekg(adresses::BlockAlign);
		for (byte i = 0; i < 2; i++) {
			tmpint += ifile.get() << 8 * i;
		}
		fileMetadata.BlockAlign = tmpint;
		tmpint = 0;
		ifile.seekg(adresses::BitsPerSample);
		for (byte i = 0; i < 2; i++) {
			tmpint += ifile.get() << 8 * i;
		}
		fileMetadata.BitsPerSample = tmpint;
		ifile.seekg(adresses::Subchunk2ID);
		ifile.read(fileMetadata.Subchunk2ID, 4);
		tmpint = 0;
		ifile.seekg(adresses::Subchunk2Size);
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

void getAudioData(string fileName, char* chunk, const unsigned int* SamplesSize, const unsigned int* dataAdress, const unsigned int bitPerOutputSample) {
	std::ifstream ifile(fileName, std::ifstream::binary);
	if (ifile.is_open()) {
		if (bitPerOutputSample == 24) {
			ifile.seekg(*dataAdress);
			ifile.read(chunk, static_cast<std::streamsize>(*SamplesSize));
		}
		else if (bitPerOutputSample == 16) {
			char* buffer = new char[*SamplesSize];
			ifile.seekg(*dataAdress);
			ifile.read(chunk, static_cast<std::streamsize>(*SamplesSize));
			unsigned int chunkOffset = 0;
			for (unsigned int i = 0; i < *SamplesSize; i += 6) {
				memcpy(chunk + chunkOffset, buffer + i, 2);
				chunkOffset += 2;
				memcpy(chunk + chunkOffset, buffer + i + 3, 2);
				chunkOffset += 2;
			}
		}
		ifile.close();
	}
}

void storeSamples(char* chunk, const unsigned int* SamplesSize, string fileName, const unsigned int bitPerOutputSample, const unsigned int bitPerInputSample) {
	std::ofstream ifile(fileName);
	if (ifile.is_open()) {
		if (bitPerOutputSample == 24) {
			int tmpint = 0;
			for (unsigned int i = 0; i < *SamplesSize; i += bitPerInputSample/4) {
				tmpint = 0;
				memcpy(&tmpint, chunk + i, 3);
				tmpint = tmpint | (0xff000000 * ((tmpint & 0x800000) >> 23));
				ifile << tmpint << endl;
			}
		}
		else if (bitPerOutputSample == 16) {
			short tmpint = 0;
			for (unsigned int i = 0; i < *SamplesSize; i += bitPerInputSample/4) {
				tmpint = 0;
				memcpy(&tmpint, chunk + i + 1, 2);
				ifile << tmpint << endl;
			}
		}
		ifile.close();
	}
}