#pragma once

class Time {
	static double lastTime;

public:
	static double TotalTime;
	static double DeltaTime;
	static double UnscaledDeltaTime;
	static double TimeScale;
	static double ScaledTime;
	static double OneTime;

	static void Update() {
		double currentTime = (double)glfwGetTime();
		double amount = currentTime - lastTime;

		UnscaledDeltaTime = amount;

		DeltaTime = amount * TimeScale;
		OneTime = DeltaTime * 60;
		
		
		TotalTime += amount;
		ScaledTime += amount * TimeScale;
		
		lastTime = currentTime;
	}
};

double Time::OneTime = 0.0;
double Time::lastTime = 0.0;
double Time::TotalTime = 0.0;
double Time::DeltaTime = 0.0;
double Time::UnscaledDeltaTime = 0.0;
double Time::TimeScale = 1.0;
double Time::ScaledTime = 0.0;