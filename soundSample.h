#pragma once
#include <vector>
#include "files.h"

//sert à itérer sur un son. se sert d'un pointeur pour donner la bonne adresse à lire
class soundSampleIterator {
public:
	unsigned int m_currentOffset = 0;
	unsigned int m_SamplesSize;
	char* m_Samples;
	soundSampleIterator(const unsigned int nQtesample, char* nSamples) {
		m_SamplesSize = nQtesample;
		m_Samples = nSamples;
	}
	soundSampleIterator() {
		m_SamplesSize = 0;
		m_currentOffset = 1U;
		m_Samples = nullptr;
	}
	char* getBufferOffsetPointer() {
		return &m_Samples[m_currentOffset];
	}
	void moveBufferPointer(const unsigned int nqteSample) {
		m_currentOffset += nqteSample * 6;
	}
	bool testOverflow() {
		return m_currentOffset >= m_SamplesSize;
	}
	~soundSampleIterator() {
		cout << "iterator got deleted\n";
	}
};


//contien les donnés du son, on pourrait dire que c'est le .wav lui même
class soundSample {
public:
	unsigned int m_SamplesSize = 1;
	unsigned int m_ChunkSize = 512;
	unsigned int m_samplerate = 44100;
	string m_name = "jimbo";
	char* m_Samples = nullptr;
	soundSample(const unsigned int nQtesample, unsigned int nSampleRate, unsigned int nChunkSize, string nName) {
		m_SamplesSize = nQtesample;
		m_ChunkSize = nChunkSize;
		m_samplerate = nSampleRate;
		m_name = nName;
		m_Samples = new char[(m_SamplesSize / m_ChunkSize + 1) * m_ChunkSize]{};
	}

	soundSample(string nName, const unsigned int nChunkSize) {
		m_name = nName;
		m_ChunkSize = nChunkSize;
		wavMetadata fileMetadata = getFileMetadata(m_name);
		m_SamplesSize = fileMetadata.Subchunk2Size;
		m_samplerate = fileMetadata.SampleRate;
		m_Samples = new char[(m_SamplesSize / m_ChunkSize + 1) * m_ChunkSize]{};
		getAudioData(m_name, m_Samples, &m_SamplesSize, &adresses::data, 24);
	}

	soundSample() {}

	~soundSample() {
		delete[] m_Samples;
	}

	void init(string nName, const unsigned int nChunkSize) {
		m_name = nName;
		m_ChunkSize = nChunkSize;
		wavMetadata fileMetadata = getFileMetadata(m_name);
		m_SamplesSize = fileMetadata.Subchunk2Size;
		m_samplerate = fileMetadata.SampleRate;
		m_Samples = new char[(m_SamplesSize / m_ChunkSize + 1) * m_ChunkSize]{};
		getAudioData(m_name, m_Samples, &m_SamplesSize, &adresses::data, 24);
	}
};

//file d'attente des son à jouer
class soundQueue {
public:
	static const unsigned int m_queueSize = 1<<6;
	unsigned int m_size = 0;
	soundSampleIterator m_queue[m_queueSize];

	bool append(soundSample& nSound) {
		for (unsigned int i = 0; i < m_queueSize; i++) {
			if (m_queue[i].testOverflow()) {
				m_queue[i].m_SamplesSize = nSound.m_SamplesSize;
				m_queue[i].m_Samples = nSound.m_Samples;
				m_size++;
				return true;
			}
		}
		return false;
	}

	void remove(const unsigned int element) {
		m_queue[element].m_SamplesSize = 0;
		m_queue[element].m_currentOffset = 1U;
	}

	/*unsigned int size() {
		return m_size;
	}*/

	/*void append(soundSample<char>& nSound) {
		m_queue.push_back(soundSampleIterator<char>(nSound.m_SamplesSize, nSound.m_Samples, nSound.m_rightSamples));
	}*/

	vector<char*> getOffsetQueue(const unsigned int nqteSample) {
		vector<char*> offsetQueue;
		for (unsigned int i = 0; i < m_queueSize; i++) {
			if (!m_queue[i].testOverflow()) {
				offsetQueue.push_back(m_queue[i].getBufferOffsetPointer());
				m_queue[i].moveBufferPointer(nqteSample);
			}
			else {
				this->remove(i);
				m_size--;
			}
		}
		return offsetQueue;
	}

	vector<char*> getRightOffsetQueue(const unsigned int nqteSample) {
		vector<char*> offsetQueue;
		for (unsigned int i = 0; i < m_queueSize; i++) {
			if (!m_queue[i].testOverflow()) {
				offsetQueue.push_back(m_queue[i].getBufferOffsetPointer());
				m_queue[i].moveBufferPointer(nqteSample);
			}
			else {
				this->remove(i);
				m_size--;
			}
		}
		return offsetQueue;
	}
};