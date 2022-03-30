#pragma once
#include <vector>
#include "files.h"

//sert à itérer sur un son. se sert d'un pointeur pour donner la bonne adresse à lire
template<class T>
class soundSampleIterator {
public:
	unsigned int m_currentRightOffset = 0;
	unsigned int m_currentLeftOffset = 0;
	unsigned int m_qteSamples;
	T* m_leftSamples;
	T* m_rightSamples;
	soundSampleIterator(const unsigned int nQtesample, T* nleftSamples, T* nrightSamples) {
		m_qteSamples = nQtesample;
		m_leftSamples = nleftSamples;
		m_rightSamples = nrightSamples;
	}
	soundSampleIterator() {
		m_qteSamples = 0;
		m_currentRightOffset = 1U;
		m_currentLeftOffset = 1U;
		m_leftSamples = nullptr;
		m_rightSamples = nullptr;
	}
	T* getLeftBufferOffsetPointer() {
		return &m_leftSamples[m_currentLeftOffset];
	}
	T* getRightBufferOffsetPointer() {
		return &m_rightSamples[m_currentRightOffset];
	}
	void moveLeftBufferPointer(const unsigned int nqteSample) {
		m_currentLeftOffset += nqteSample;
	}
	void moveRightBufferPointer(const unsigned int nqteSample) {
		m_currentRightOffset += nqteSample;
	}
	bool testLeftOverflow() {
		return m_currentLeftOffset >= m_qteSamples;
	}
	bool testRightOverflow() {
		return m_currentRightOffset >= m_qteSamples;
	}
	~soundSampleIterator() {
		cout << "iterator got deleted\n";
	}
};


//contien les donnés du son, on pourrait dire que c'est le .wav lui même
template<class T>
class soundSample {
public:
	unsigned int m_qteSamples = 1;
	unsigned int m_ChunkSize = 512;
	unsigned int m_samplerate = 44100;
	string m_name = "jimbo";
	T* m_leftSamples;// = new T[(m_qteSamples / m_ChunkSize + 1) * m_ChunkSize]{};
	T* m_rightSamples;// = new T[(m_qteSamples / m_ChunkSize + 1) * m_ChunkSize]{};
	soundSample(const unsigned int nQtesample, unsigned int nSampleRate, unsigned int nChunkSize, string nName) {
		m_qteSamples = nQtesample;
		m_ChunkSize = nChunkSize;
		m_samplerate = nSampleRate;
		m_name = nName;
		m_rightSamples = new T[(m_qteSamples / m_ChunkSize + 1) * m_ChunkSize]{};
		m_leftSamples = new T[(m_qteSamples / m_ChunkSize + 1) * m_ChunkSize]{};
	}

	soundSample(string nName, const unsigned int nChunkSize) {
		m_name = nName;
		m_ChunkSize = nChunkSize;
		wavMetadata fileMetadata = getFileMetadata(m_name);
		m_qteSamples = fileMetadata.Subchunk2Size / fileMetadata.BlockAlign / fileMetadata.NumChannels;
		m_samplerate = fileMetadata.SampleRate;
		m_rightSamples = new T[(m_qteSamples / m_ChunkSize + 1) * m_ChunkSize]{};
		m_leftSamples = new T[(m_qteSamples / m_ChunkSize + 1) * m_ChunkSize]{};
		getAudioData(m_name, m_leftSamples, m_rightSamples, &m_qteSamples, &adresses::data);
		printMetadata(fileMetadata);
	}

	soundSample() {}

	~soundSample() {
		delete[] m_leftSamples, m_rightSamples;
	}
};

//file d'attente des son à jouer
template<class T>
class soundQueue {
public:
	static const unsigned int m_queueSize = 1<<6;
	unsigned int m_size = 0;
	soundSampleIterator<T> m_queue[m_queueSize];

	bool append(soundSample<T>& nSound) {
		for (unsigned int i = 0; i < m_queueSize; i++) {
			if (m_queue[i].testRightOverflow()) {
				m_queue[i].m_qteSamples = nSound.m_qteSamples;
				m_queue[i].m_leftSamples = nSound.m_leftSamples;
				m_queue[i].m_rightSamples = nSound.m_rightSamples;
				m_size++;
				return true;
			}
		}
		return false;
	}

	void remove(const unsigned int element) {
		m_queue[element].m_qteSamples = 0;
		m_queue[element].m_currentRightOffset = 1U;
		m_queue[element].m_currentLeftOffset = 1U;
	}

	/*unsigned int size() {
		return m_size;
	}*/

	/*void append(soundSample<T>& nSound) {
		m_queue.push_back(soundSampleIterator<T>(nSound.m_qteSamples, nSound.m_leftSamples, nSound.m_rightSamples));
	}*/

	/*T* getLeftOffsetQueue(const unsigned int nqteSample, unsigned int nqueueElement) {
		T** offsetQueue = new T * [m_size];
		for (unsigned int i = 0; i < m_size; i++) {
			offsetQueue[i] = m_queue[i]->getLeftBufferOffsetPointer();
			if (m_queue[i]->moveLefttBufferPointer(nqteSample)) {
				m_queue.erase(m_queue.begin() + i);
				m_size--;
			}
		}
		return offsetQueue;
	}*/

	vector<T*> getRightOffsetQueue(const unsigned int nqteSample) {
		vector<T*> offsetQueue;
		for (unsigned int i = 0; i < m_queueSize; i++) {
			if (!m_queue[i].testRightOverflow()) {
				offsetQueue.push_back(m_queue[i].getRightBufferOffsetPointer());
				m_queue[i].moveRightBufferPointer(nqteSample);
			}
			else {
				this->remove(i);
				m_size--;
			}
		}
		return offsetQueue;
	}
};