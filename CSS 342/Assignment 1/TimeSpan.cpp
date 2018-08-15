#include <iostream>
#include <string>
#include <istream>
#include "TimeSpan.h"
#include <stdlib.h>
using namespace std;

TimeSpan::TimeSpan(double hours, double minutes, double seconds)
{
	setTime(hours, minutes, seconds);
}

TimeSpan::TimeSpan(double minutes, double seconds)
{
	setTime(0, minutes, seconds);
}

TimeSpan::TimeSpan(double seconds) {
	setTime(0, 0, seconds);
}

TimeSpan::TimeSpan()
{
	setTime(0, 0, 0);
}

TimeSpan::~TimeSpan()
{
}

bool TimeSpan::operator==(const TimeSpan & other) const
{
	if ((other.getHours() == m_hours) && (other.getMinutes() == m_minutes) && (other.getSeconds() == m_seconds))
		return true;
	return false;
}

bool TimeSpan::operator!=(const TimeSpan & other) const
{
	if ((other.getHours() == m_hours) && (other.getMinutes() == m_minutes) && (other.getSeconds() == m_seconds))
		return false;
	return true;
}

TimeSpan TimeSpan::operator+=(const TimeSpan & other)
{
	this->m_hours += other.getHours();
	this->m_minutes += other.getMinutes();
	this->m_seconds += other.getSeconds();
	reduce();

	return *this;
}

TimeSpan TimeSpan::operator-=(const TimeSpan & other)
{
	this->m_hours -= other.getHours();
	this->m_minutes -= other.getMinutes();
	this->m_seconds -= other.getSeconds();
	reduce();

	return *this;
}

TimeSpan TimeSpan::operator+(const TimeSpan & other) const
{
	TimeSpan result;
	result = *this;
	result += other;
	return result;
}

TimeSpan TimeSpan::operator-(const TimeSpan & other) const
{
	TimeSpan result;
	result = *this;
	result -= other;
	return result;
}

TimeSpan TimeSpan::operator- () const
{
	TimeSpan ts(-m_hours, -m_minutes, -m_seconds);
	return ts;
}

ostream& operator<<(ostream& os, const TimeSpan& other)
{
	return os << "Hours: " << other.getHours() << " " << "Minutes: " << other.getMinutes() << " " << "Seconds: " << other.getSeconds() << " " << endl;
}

istream& operator >> (istream& is, TimeSpan& other)
{
	is >> other.m_hours >> other.m_minutes >> other.m_seconds;
	other.reduce();
	return is;
}

int TimeSpan::getHours() const
{
	return m_hours;
}

int TimeSpan::getMinutes() const
{
	return m_minutes;
}

int TimeSpan::getSeconds() const
{
	return m_seconds;
}

bool TimeSpan::setTime(double hours, double minutes, double seconds)
{
	int convertedSeconds = (int)convertToSeconds(hours, minutes, seconds);
	m_hours = convertedSeconds / 3600;
	m_minutes = (convertedSeconds % 3600) / 60;
	m_seconds = convertedSeconds % 60;
	return true;
}

void TimeSpan::reduce()
{
	int convertedSeconds = (int)convertToSeconds(m_hours, m_minutes, m_seconds);
	m_hours = convertedSeconds / 3600;
	m_minutes = (convertedSeconds % 3600) / 60;
	m_seconds = convertedSeconds % 60;
}

int TimeSpan::convertToSeconds(double hours, double minutes, double seconds)
{
	int convertedTimeSpan = (int)((hours * 3600) + (minutes * 60) + seconds);
	return convertedTimeSpan;
}