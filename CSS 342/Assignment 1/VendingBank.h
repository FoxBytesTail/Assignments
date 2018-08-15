#pragma once
#include <string>
using namespace std;
const int    COIN_LIMIT    = 1000;
const double DOLLAR_VALUE  = 1.00;
const double QUARTER_VALUE = 0.25;
const double DIME_VALUE    = 0.10;
const double NICKEL_VALUE  = 0.05;
const double PENNY_VALUE   = 0.01;

//Specifies the minimum amount of coins the bank must contain to make exact change.
const int MIN_QUARTERS = 3;
const int MIN_DIMES    = 2;
const int MIN_NICKELS  = 1;
const int MIN_PENNIES  = 4;

using namespace std;
class VendingBank {
public:
	VendingBank(); 
	VendingBank(int id);
	VendingBank(int id, int numDollarCoins, int numQuarters, int numDimes, int numNickels, int numPennies);
	~VendingBank();

	void purchaseItem(int itemPrice);
	bool insertCurrency(double currency);
	void coinReturn();

	friend ostream& operator<<(ostream &outstream, const VendingBank &vb);
	friend istream& operator>>(istream &instream, VendingBank &vb);

	int getVendingBankID() const;
	double getUserInputtedCurrency() const;

private:
	double userInputtedCurrency, changeToReturn;
	int numDollarCoin, numQuarters, numDimes, numNickels, numPennies;
	int bankID, numCoinsInBank;

	bool validateCurrency();
	bool checkBankForChange();

	void dispenseChange(); 
	void disableBank();
	
	double calculateNumCoins();
	double calculateAmountOfCurrencyInBank();
};