#pragma once
#include <vector>

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
	T* getLeftBufferOffsetPointer() {
		return &m_leftSamples[m_currentLeftOffset];
	}
	T* getRightBufferOffsetPointer() {
		return &m_rightSamples[m_currentRightOffset];
	}
	bool moveLeftBufferPointer(const unsigned int nqteSample) {
		m_currentRightOffset += nqteSample;
		return m_currentRightOffset >= m_qteSamples;
	}
	bool moveRightBufferPointer(const unsigned int nqteSample) {
		m_currentRightOffset += nqteSample;
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
	unsigned int m_qteSamples;
	unsigned int m_ChunkSize;
	bool m_stereo = 1;
	unsigned int m_samplerate;
	T* m_leftSamples;
	T* m_rightSamples;
	soundSample(const unsigned int nQtesample, bool nStereo, unsigned int nSampleRate, unsigned int nChunkSize) {
		m_qteSamples = nQtesample;
		m_ChunkSize = nChunkSize;
		m_stereo = nStereo;
		m_samplerate = nSampleRate;
		m_rightSamples = new T[(m_qteSamples / m_ChunkSize + 1) * m_ChunkSize]{};
		m_leftSamples = new T[(m_qteSamples / m_ChunkSize + 1) * m_ChunkSize]{};
	}
	~soundSample() {
		delete[] m_leftSamples, m_rightSamples;
	}
};

//file d'attente des son à jouer
template<class T>
class soundQueue {
public:
	vector<soundSampleIterator<T>*> m_queue;
	unsigned int m_size = 0;
	soundQueue() {}

	unsigned int size() {
		return m_size;
	}

	void append(soundSample<T>& nSound) {
		m_queue.push_back(new soundSampleIterator<T>(nSound.m_qteSamples, nSound.m_leftSamples, nSound.m_rightSamples));
		m_size++;
	}

	T* getLeftOffsetQueue(const unsigned int nqteSample, unsigned int nqueueElement) {
		T** offsetQueue = new T * [m_size];
		for (unsigned int i = 0; i < m_size; i++) {
			offsetQueue[i] = m_queue[i]->getLeftBufferOffsetPointer();
			if (m_queue[i]->moveLefttBufferPointer(nqteSample)) {
				m_queue.erase(m_queue.begin() + i);
				m_size--;
			}
		}
		return offsetQueue;
	}
	T** getRightOffsetQueue(const unsigned int nqteSample) {
		T** offsetQueue = new T*[m_size];
		for (unsigned int i = 0; i < m_size; i++) {
			offsetQueue[i] = m_queue[i]->getRightBufferOffsetPointer();
			if (m_queue[i]->moveRightBufferPointer(nqteSample)) {
				m_queue.erase(m_queue.begin() + i);
				m_size--;
			}
		}
		return offsetQueue;
	}
};