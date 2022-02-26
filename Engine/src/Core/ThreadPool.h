#pragma once

#include <vector>
#include <thread>
#include <functional>
#include <queue>
#include <mutex>

namespace Engine {
	class ThreadPool {
	public:
		enum class Priority
		{
			HIGH,
			MEDIUM,
			LOW
		};

		~ThreadPool();
		void SpawnJob(std::function<void()> job, Priority priority);

		static ThreadPool& Get();
	private:
		std::vector<std::thread> m_Threads;
		std::queue<std::function<void()>> m_Jobs;
		std::mutex m_JobsMutex;
		std::condition_variable m_JobSignal;
		std::atomic<bool> m_IsRunning;
		void Loop();
		ThreadPool();
	};
}