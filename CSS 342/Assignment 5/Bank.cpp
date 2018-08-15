#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <cmath>

#include "Bank.h"
#include "Account.h"
#include "Transaction.h"
#include "BSTree.h"
#include "Funds.h"

using namespace std;

static vector < string > tokenize(string s)
{
	string b;
	stringstream ss(s);
	vector < string > tokens;
	while (ss >> b)
		tokens.push_back(b);
	return tokens;
}

Bank::Bank()
{
}

Bank::~Bank()
{
	DestroyBST();
}

void Bank::DisplayAccounts()
{
	accounts.Display();
}

bool Bank::AccountExists(int aid)
{
	pair < int, int >acctInfo = getAccountIDAndFund(aid);
	if (accounts.Retrieve(acctInfo.first) != NULL)
	{
		return true;
	}
	return false;
}

void Bank::Process(string strToParse)
{
	vector<string> tokens = tokenize(strToParse);

	if (strToParse[0] == 'O')
	{
		Open(tokens[1], tokens[2], stoi(tokens[3]));
	}
	else if (strToParse[0] == 'D')
	{
		Deposit(stoi(tokens[1]), stoi(tokens[2]));
	}
	else if (strToParse[0] == 'W')
	{
		Withdraw(stoi(tokens[1]), stoi(tokens[2]));
	}
	else if (strToParse[0] == 'T')
	{
		Transfer(stoi(tokens[1]), stoi(tokens[3]), stoi(tokens[2]));
	}
	else if (strToParse[0] == 'H')
	{
		int aid = stoi(tokens[1]);
		int len = ((int)log10(aid) + 1);
		if (len == 4)
		{
			GetHistory(aid);
		}
		else if (len == 5)
		{
			pair < int, int >acctInfo = getAccountIDAndFund(aid);
			GetHistory(acctInfo.first, acctInfo.second);
		}
		else
		{
			cerr << "Invalid account ID." << endl;
		}
	}
}

void Bank::DestroyBST()
{
	accounts.Destroy();
}

void Bank::GetBalances(int aid)
{
	int accountId = getAccountIDAndFund(aid).first;
	Account* client = accounts.Retrieve(accountId);
	cout << client->getName() << " Account ID: " << client->getAccountId() << endl;
	for (int i = 0; i < 10; i++)
	{
		cout << "  " << FundTypes[i] << ": $" << client->Balance(i) << endl;
	}
	cout << endl;
}

void Bank::GetHistory(int aid)
{
	// entire account history
	int accountId = getAccountIDAndFund(aid).first;
	Account* client = accounts.Retrieve(accountId);
	cout << "Transaction History for " << client->getName() << " by fund." << endl;
	for (int i = 0; i < 10; i++)
	{
		if (!client->GetHistoryEntry(i).empty())
		{
			cout << FundTypes[i] << ": $" << client->Balance(i) << endl;
			for (auto tx : client->GetHistoryEntry(i))
			{
				if (tx.getType() != 'T')
				{
					cout << "  " << tx.getType() << " " << accountId << 
						tx.getFund() << " " << tx.getAmount() << 
						(tx.getSuccess() ? "" : " (Failed)") << endl;
				}
				else
				{
					cout << "  " << tx.getType() << " " << accountId << tx.getFund() 
						<< " " << tx.getAmount() << " " << tx.getDestinationAccount() 
						<< (tx.getSuccess() ? "" : " (Failed)") << endl;
				}
			}
		}
	}	return;
}

void Bank::GetHistory(int aid, int fund)
{
	// history for single fund
	pair < int, int >acctInfo = getAccountIDAndFund(aid);
	int accountId = getAccountIDAndFund(aid).first;
	Account* client = accounts.Retrieve(accountId);
	cout << "Transaction History for " << client->getName() << " " << FundTypes[fund] 
		<< ": $" << client->Balance(fund) << endl;
	for (auto tx : client->GetHistoryEntry(fund))
	{
		if (tx.getType() != 'T')
		{
			cout << "  " << tx.getType() << " " << accountId << tx.getFund() << " " 
				<< tx.getAmount() << (tx.getSuccess() ? "" : " (Failed)") << endl;
		}
		else
		{
			cout << "  " << tx.getType() << " " << accountId << tx.getFund() << " " 
				<< tx.getAmount() << " " << tx.getDestinationAccount() 
				<< (tx.getSuccess() ? "" : " (Failed)") << endl;
		}
	}

	return;
}

void Bank::Open(string last, string first, int aid)
{
	pair < int, int >acctInfo = getAccountIDAndFund(aid);
	if (AccountExists(acctInfo.first))
	{
		cerr << "ERROR: Account " << acctInfo.first
			<< " is already open. Transaction refused." << endl;
		return;
	}

	Account* a = new Account(acctInfo.first, last, first);
	accounts.Insert(a);
}

void Bank::Deposit(int aid, int amount)
{
	pair < int, int >acctInfo = getAccountIDAndFund(aid);

	if (!AccountExists(aid))
	{
		cerr << "[ERR] Account " << acctInfo.first << " not found. Transaction refused." << endl;
		return;
	}
	else if (amount < 0)
	{
		cerr << "[ERR] You can't deposit a negative amount." << endl;
		return;
	}
	else
	{
		Account* currentClient = accounts.Retrieve(acctInfo.first);
		currentClient->Increment(amount, acctInfo.second);
		Transaction tx = Transaction(acctInfo.second, amount, 'D', true);
		currentClient->AddHistoryEntry(tx);
	}
}

void Bank::Withdraw(int aid, int amount)
{
	pair < int, int >acctInfo = getAccountIDAndFund(aid);

	if (!(AccountExists(acctInfo.first)))
	{
		cerr << "[ERR] Account " << acctInfo.
			first << " Not found. Transaction Refused." << endl;
		return;
	}
	else if (amount < 0)
	{
		cerr << "[ERR] You can't withdraw a negative amount." << endl;
		return;
	}
	else
	{
		if (acctInfo.second > 3)
		{
			Transaction tx;

			Account* currentClient = accounts.Retrieve(acctInfo.first);
			if ((currentClient->Balance(acctInfo.second) - amount) > 0)
			{
				currentClient->Decrement(amount, acctInfo.second);
				tx = Transaction(acctInfo.second, amount, 'W', true);
			}
			else
			{
				cerr << "ERROR: Not enough funds to withdraw " << amount 
					<< " from " << currentClient->getName() << " " 
					<< FundTypes[acctInfo.second] << endl;
				tx = Transaction(acctInfo.second, amount, 'W', false);
			}

			currentClient->AddHistoryEntry(tx);
			return;
		}
		else if (acctInfo.second >= 0 && acctInfo.second <= 3)
		{
			// this is for the 0-1-2-3 edge case, checking for pairs [0,1],[2,3] balances.
			Account* currentClient = accounts.Retrieve(acctInfo.first);
			// Enough to meet it anyway
			if ((currentClient->Balance(acctInfo.second) - amount) > 0)
			{
				currentClient->Decrement(amount, acctInfo.second);
				currentClient->AddHistoryEntry(Transaction(acctInfo.second, amount, 'W', true));
				return;
			}
			else
			{
				if ((acctInfo.second == 0 || acctInfo.second == 1) 
					&& (currentClient->Balance(0) + currentClient->Balance(1) - amount) > 0)
				{
					// Enough to cover the amount in the combined balance
					int accountToBackfill = ((acctInfo.first == 1) ? 1 : 0);
					int accountToTakeFrom = ((acctInfo.first == 1) ? 0 : 1);
					int amountNeeded = amount - currentClient->Balance(accountToBackfill);
					currentClient->Decrement(amountNeeded, accountToBackfill); // withdraw needed from opposite fund
					currentClient->AddHistoryEntry(Transaction(accountToBackfill, amountNeeded, 'W', true));
					currentClient->Decrement((amount - amountNeeded), accountToTakeFrom); // wjtddrddd the rest fropm original
					currentClient->AddHistoryEntry(Transaction(accountToTakeFrom, (amount - amountNeeded), 'W', true));
				}
				else if ((acctInfo.second == 2 || acctInfo.second == 3) 
					&& (currentClient->Balance(2) + currentClient->Balance(3) - amount) > 0)
				{
					// Enough to cover the amount in the combined balance
					int accountToBackfill = ((acctInfo.first == 2) ? 2 : 3);
					int accountToTakeFrom = ((acctInfo.first == 2) ? 3 : 2);
					int amountNeeded = amount - currentClient->Balance(accountToBackfill);
					currentClient->Decrement(amountNeeded, accountToBackfill); // withdraw needed from opposite fund 
					currentClient->AddHistoryEntry(Transaction(accountToBackfill, amountNeeded, 'W', true));
					currentClient->Decrement((amount - amountNeeded), accountToTakeFrom); // withdraw the rest from original
					currentClient->AddHistoryEntry(Transaction(accountToTakeFrom, (amount - amountNeeded), 'W', true));
				}
			}
		}
	}
}

void Bank::Transfer(int origin, int destination, int amount)
{
	pair < int, int >originAccountId = getAccountIDAndFund(origin);
	pair < int, int >destAccountId = getAccountIDAndFund(destination);

	if (AccountExists(originAccountId.first) == false)
	{
		cerr << "ERROR: Account " << originAccountId.first << " not found. Transferal refused." << endl;
		return;
	}

	if (AccountExists(destAccountId.first) == false)
	{
		cerr << "ERROR: Account " << destAccountId.first << " not found. Transferal refused." << endl;
		return;
	}

	Account* orig = accounts.Retrieve(originAccountId.first);
	Account* dest = accounts.Retrieve(destAccountId.first);
	bool isSameAccount = false;
	if (orig == dest) { isSameAccount = true; }

	// apply this to [01][23] but only if not paired
	if (originAccountId.second >= 0)
	{
		if ((orig->Balance(originAccountId.second) - amount) < 0)
		{
			cerr << "[ERR] Account " << originAccountId.first << " insufficient balance." << endl;
			orig->AddHistoryEntry(Transaction(originAccountId.second, destination, amount, 'T', false));
		}
		else
		{
			// Standard working transfer
			orig->Decrement(amount, originAccountId.second);
			dest->Increment(amount, destAccountId.second);
			orig->AddHistoryEntry(Transaction(originAccountId.second, destination, amount, 'T', true));
			dest->AddHistoryEntry(Transaction(originAccountId.second, destination, amount, 'T', true), destAccountId.second);
		}
	}
}

//helper function to split 4 and 5 character account id's into a tuple
pair<int, int> Bank::getAccountIDAndFund(int aid)
{
	int account_id;
	int fund_index;

	//trim aid (account id) if it is 5 digits long otherwise set aid and default fund to money market.
	if (log10(aid) + 1 >= 5)
	{
		account_id = (aid / 10);
		fund_index = (aid % 10);
	}
	else
	{
		account_id = aid;
		fund_index = 0;
	}

	return pair <int, int>(account_id, fund_index);
}
