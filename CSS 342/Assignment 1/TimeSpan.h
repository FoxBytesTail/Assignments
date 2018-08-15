#pragma once;
#include <iostream>

using namespace std;
class TimeSpan{
public:
	TimeSpan(double hours, double minutes, double seconds);
	TimeSpan(double minutes, double seconds);
	TimeSpan(double seconds);
	TimeSpan();
	~TimeSpan();
	bool operator==(const TimeSpan &b) const;
	bool operator!=(const TimeSpan &b) const;
	TimeSpan operator+=(const TimeSpan &b);
	TimeSpan operator-=(const TimeSpan &b);
	TimeSpan operator+(const TimeSpan &b) const;
	TimeSpan operator-(const TimeSpan &b) const;
	TimeSpan operator-() const;
	friend ostream& operator<<(ostream& os, const TimeSpan& ts);
	friend istream& operator>>(istream& is, TimeSpan& ts);
	int getHours() const;
	int getMinutes() const;
	int getSeconds() const;
private:
	void reduce();
	bool setTime(double hours, double minutes, double seconds);
	int m_hours, m_minutes, m_seconds;
	int convertToSeconds(double hours, double minutes, double seconds);
};
