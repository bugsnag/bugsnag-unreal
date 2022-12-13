#include "Scenario.h"

void notify1LevelDeep()
{
	UBugsnagFunctionLibrary::Notify(TEXT("Non-fatal"), TEXT("Calling notify from multiple threads"));
}

void notify2LevelsDeep()
{
	notify1LevelDeep();
}

void notify3LevelsDeep()
{
	notify2LevelsDeep();
}

void notify4LevelsDeep()
{
	notify3LevelsDeep();
}

class NotifyThread : public FRunnable
{
public:
	uint32 Run() override
	{
		for (int i = 0; i < 50; i++)
		{
			notify4LevelsDeep();
			notify1LevelDeep();
			notify3LevelsDeep();
			notify2LevelsDeep();
		}
		return 0;
	}
};

static void createThread()
{
	FRunnableThread::Create(new NotifyThread, TEXT("Some Thread"), 0, TPri_BelowNormal);
}

static void createThreads()
{
	for (int i = 0; i < 300; i++)
	{
		createThread();
	}
}

class NotifyMultithreadedScenario : public Scenario
{
public:
	void Configure() override
	{
		Configuration->SetAutoTrackSessions(false);
	}

	void Run() override
	{
		createThreads();
		FPlatformProcess::Sleep(8);
		UBugsnagFunctionLibrary::StartSession();
		FPlatformProcess::Sleep(1);
	}
};

REGISTER_SCENARIO(NotifyMultithreadedScenario);
