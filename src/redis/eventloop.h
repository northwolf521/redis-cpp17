#pragma once
#include "all.h"
#include "channel.h"
#include "socket.h"
#ifdef __APPLE__
#include "poll.h"
#endif

#ifdef __linux__
#include "epoll.h"
#endif

#ifdef _WIN32
#include "select.h"
#endif

#include "timerqueue.h"
#include "callback.h"

class EventLoop
{
public:
	typedef std::function<void()> Functor;
	EventLoop();
	~EventLoop();

	void quit();
	void run();
	void handleRead();
	void runInLoop(Functor &&cb);
	void queueInLoop(Functor &&cb);
	void wakeup();
	void updateChannel(Channel *channel);
	void removeChannel(Channel *channel);
	bool hasChannel(Channel *channel);
	void cancelAfter(const TimerPtr &timer);
	void assertInLoopThread();

	TimerPtr runAfter(double when, bool repeat, TimerCallback &&cb);
	TimerQueuePtr getTimerQueue() { return timerQueue; }
	void handlerTimerQueue() { timerQueue->handleRead(); }
	bool isInLoopThread() const { return threadId == std::this_thread::get_id(); }
	bool geteventHandling() const { return eventHandling; }
	std::thread::id getThreadId() const { return threadId; }

private:
	EventLoop(const EventLoop&);
	void operator=(const EventLoop&);

	void abortNotInLoopThread();
	void doPendingFunctors();

	std::thread::id threadId;
	mutable std::mutex mutex;
#ifdef __APPLE__
	PollPtr epoller;
	int32_t op;
	int32_t wakeupFd[2];
#endif

#ifdef __linux__
	EpollPtr epoller;
	int32_t wakeupFd;
#endif

#ifdef _WIN32
	SelectPtr epoller;
	int32_t op;
	int wakeupFd[2];
#endif

	TimerQueuePtr timerQueue;
	ChannelPtr wakeupChannel;

	typedef std::vector<Channel*> ChannelList;
	ChannelList activeChannels;
	Channel *currentActiveChannel;

	bool running;
	bool eventHandling;
	bool callingPendingFunctors;
	std::vector<Functor> functors;
	std::vector<Functor> pendingFunctors;
};

