#pragma once

#include <cstdint>
#include <deque>
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>
#include <atomic>
#include <functional>
#include <unistd.h>

class	ThreadPool
{
	public:
		ThreadPool() : _active_tasks(0), _stop(false) {}
		/* Construct threadpool and add n threads */
		ThreadPool(uint64_t n) {add(n);}
		~ThreadPool() {if (!_stop) stop();}

		uint64_t	threads() {return (_threads_count);}

		/* Adds N threads to the pool */
		void	add(uint64_t n)
		{
			_threads_count += n;
			for (uint32_t i = 0; i < n; ++i)
				_worker_threads.emplace_back(std::bind(&ThreadPool::_worker, this));
		}

		/* Stop all threads join them */
		void	stop()
		{
			std::unique_lock<std::mutex> latch(_queue_mutex);

			_stop = true;
			_cv_task.notify_all();

			latch.unlock();

			for (std::thread &thread : _worker_threads)
				thread.join();
		}

		/* Waits for all tasks to be treated */
		void	wait_finish()
		{
			while (1)
			{
				_queue_mutex.lock();
				if (_tasks.empty() && _active_tasks == 0)
					break ;
				_queue_mutex.unlock();
				usleep(50);
			}
			_queue_mutex.unlock();
		}
		uint64_t	active_tasks() {return (_active_tasks);}

		/* Queue a task for the threads to generate */
		void	queue_task(std::function<void(void)> task)
		{
			std::unique_lock<std::mutex> lock(_queue_mutex);
			_tasks.emplace_back(task);
			_cv_task.notify_one();
		}
		/* Queue a vector of task for the threads to generate */
		void	queue_task(const std::vector<std::function<void(void)>> &tasks)
		{
			std::unique_lock<std::mutex> lock(_queue_mutex);
			for (auto &task : tasks)
				_tasks.emplace_back(task);
			_cv_task.notify_all();
		}

	private:
		void	_worker()
		{
			while (true)
			{
				std::unique_lock<std::mutex> latch(_queue_mutex);
				_cv_task.wait(latch, [this](){return (_stop || !_tasks.empty());});
				if (_stop)
					break ;
				else if (!_tasks.empty())
				{
					std::function<void(void)>	task = _tasks.front();
					_tasks.pop_front();
					_active_tasks++;

					latch.unlock();

					try
					{
						task();
					}
					catch(const std::exception &e)
					{
						(void)e;
					}

					latch.lock();
					_active_tasks--;
				}
			}
		}

		std::deque<std::function<void(void)>>					_tasks;
		std::atomic_int											_active_tasks;
		std::condition_variable 			_cv_task;
		std::mutex							_queue_mutex;

		std::vector<std::thread>			_worker_threads;

		std::atomic_bool					_stop;

		uint64_t							_threads_count = 0;
};
