#pragma once
#include <vector>

//sert � it�rer sur un son. se sert d'un pointeur pour donner la bonne adresse � lire
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
	bool moveLeftBufferPointer(const unsigned int nqteSample) {
		m_currentRightOffset += nqteSample;
		return m_currentRightOffset >= m_qteSamples;
	}
	void moveRightBufferPointer(const unsigned int nqteSample) {
		m_currentRightOffset += nqteSample;
	}

	bool testRightOverflow() {
		return m_currentRightOffset >= m_qteSamples;
	}
	~soundSampleIterator() {
		cout << "iterator got deleted\n";
	}
};


//contien les donn�s du son, on pourrait dire que c'est le .wav lui m�me
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

//file d'attente des son � jouer
template<class T>
class soundQueue {
public:
	const unsigned int m_queueSize = 16U;
	unsigned int m_size = 0;
	soundSampleIterator<T> m_queue[16];

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