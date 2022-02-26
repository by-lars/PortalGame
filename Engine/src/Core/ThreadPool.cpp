#include "Core/ThreadPool.h"
#include "Core/Base.h"

#include <thread>

namespace Engine {
	ThreadPool::ThreadPool() {
		int nThreads = std::thread::hardware_concurrency();
		ENGINE_DEBUG("Using " << nThreads << " threads");

		m_IsRunning = true;

		for (int i = 0; i < nThreads; i++) {
			m_Threads.push_back(std::thread(&ThreadPool::Loop, this));
		}
	}

	ThreadPool& ThreadPool::Get() {
		static ThreadPool pool;
		return pool;
	}

	void ThreadPool::SpawnJob(std::function<void()> job, Priority priority) {
		{
			std::unique_lock<std::mutex> lock(m_JobsMutex);
			m_Jobs.push(job);
		}

		m_JobSignal.notify_one();
	}

	ThreadPool::~ThreadPool() {
		{
			std::unique_lock<std::mutex> lock(m_JobsMutex);
			m_Jobs = std::queue<std::function<void()>>();
			m_IsRunning = false;
		}

		m_JobSignal.notify_all();

		for (auto& x : m_Threads) {
			x.join();
		}
	}

	void ThreadPool::Loop() {
		std::function<void()> currentJob;

		while (m_IsRunning) {
			{
				currentJob = nullptr;
				std::unique_lock<std::mutex> lock(m_JobsMutex);
				m_JobSignal.wait(lock, [this]() {
					return !m_Jobs.empty() || !m_IsRunning;
					});

				if (!m_IsRunning) {
					continue; 
				}

				currentJob = m_Jobs.front();
				m_Jobs.pop();	
			}

			if (currentJob != nullptr) {
				currentJob();
			}
		}

		ENGINE_DEBUG("Thread #" << std::this_thread::get_id() << " exited.");
	}
}