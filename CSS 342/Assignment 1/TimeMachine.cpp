#include <iostream>
#include <cassert>
#include "TimeSpan.h"
using namespace std;
int main()
{
	//Tests for timespan constructors
	cout << "TimeSpan constructor test" << endl;
	TimeSpan dur1, dur2(5), dur3(60), dur4(7, 3), dur5(60, 60), dur6(1.5, 4, -10), dur7(0, 1000000, 100000000);
	cout << dur1 << dur2 << dur3 << dur4 << dur5 << dur6 << dur7 << endl;

	//Tests for addition operator
	cout << "Tests for addition operator " << endl;
	TimeSpan ts1, ts2, ts3, ts4(60), ts5(60), ts6(1.5, 2, -10), ts7(1.3333, 20, 2.12345);
	ts1 = ts4 + ts5;
	ts2 = ts5 + ts6;
	ts3 = ts5 + ts7;

	cout << ts1 << ts2 << ts3 << endl;

	//Tests for subtraction
	cout << "Tests for subtraction operator " << endl;
	TimeSpan ts8, ts9, ts10;
	ts8 = ts4 - ts5;
	ts9 = ts5 - ts6;
	ts10 = ts5 - ts7;

	cout << ts8 << ts9 << ts10 << endl;

	//Test for subtraction & addition
	cout << "Tests for addition and subtraction" << endl;
	TimeSpan ts11, ts12(60), ts13(60, 60);
	ts11 = ts12 - ts12 + ts13 - ts13;
	cout << ts11 << endl;

	//Test for unary negation
	cout << "Tests for unary negation" << endl;
	TimeSpan ts14, ts15(-1, -60, -60), ts16(1, 2, 3);
	cout << ts15 << -ts15 << ts16 << -ts16 << endl;

	//Test for -= assignment operator
	cout << "Tests for -= operator" << endl;
	TimeSpan ts17, ts18(1, 60, 60), ts19(60, 60);
	ts18 -= ts19;
	ts17 -= ts19;
	cout << ts18 << ts17 << endl;

	//Test for += assignemnt operator
	cout << "Tests for += operator" << endl;
	TimeSpan ts20(1, 0, 0), ts21(1, 0, 0), ts22(-2, 0, 0);
	ts20 += ts21;
	ts22 += ts20;
	cout << ts20 << ts22 << endl;

	//Test for != operator
	cout << "Tests for != operator" << endl;
	TimeSpan ts23(1, 2, 3), ts24(4, 5, 6), ts25(1, 2, 3);
	if (ts23 != ts24) {
		cout << "Durations are different" << endl;
	} 
	else {
	cout << "Durations are same" << endl;
	}

	if (ts23 != ts25) {
		cout << "Durations are different" << endl;
	}
	else {
		cout << "Durations are same" << endl;
	}

	//Test for == operator
	cout << "Tests for == operator" << endl;
	TimeSpan ts26(1, 2, 3), ts27(4, 5, 6), ts28(1, 2, 3);
	if (ts26 == ts27) {
		cout << "Durations are same" << endl;
	}
	else {
		cout << "Durations are different" << endl;
	}

	if (ts26 == ts28) {
		cout << "Durations are same" << endl;
	}
	else {
		cout << "Durations are different" << endl;
	}
	
	//Test for << operator
	cout << "Tests for << operator" << endl;
	TimeSpan ts29;
	cin >> ts29;
	cout << ts29 << endl;

	system("pause");
}