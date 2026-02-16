#pragma once
#include "clsBankClient.h"
#include "clsInputValidate.h"
#include "clsUtil.h"
#include <iostream>
#include <iomanip>
using namespace std;

int ReadClient(clsBankClient& Client1) {
	cout << "Enter FirstName : ";
	Client1.FirstName = clsInputValidate::ReadString();
	cout << "Enter LastName  : ";
	Client1.LastName = clsInputValidate::ReadString();
	cout << "Enter Email     : ";
	Client1.Email = clsInputValidate::ReadString();
	cout << "Enter Phone     : ";
	Client1.Phone = clsInputValidate::ReadString();
	cout << "Enter PinCode   : ";
	Client1.PinCode = clsInputValidate::ReadString();
	cout << "Enter AccBalance : ";
	Client1.AccountBalance = clsInputValidate::ReadFloatNumber();

	return 0;
}

void UpdateClient() {
	
	string AccountNumber;
	do
	{
		cout << "Enter a Account Number : ";
		AccountNumber = clsInputValidate::ReadString();
	} while (!clsBankClient::IsClientExist(AccountNumber));
	clsBankClient UpdatedClient = clsBankClient::Find(AccountNumber);

	UpdatedClient.Print();
	cout << endl << endl;
	cout << "__________Update Menu____________" << endl;
	ReadClient(UpdatedClient);
	UpdatedClient.Save();
	
	clsBankClient::enSaveResults SaveResult;
	SaveResult = UpdatedClient.Save();

	switch (SaveResult)
	{
	case  clsBankClient::enSaveResults::svSucceeded:
	{
		cout << "\nAccount Updated Successfully :-)\n";
		UpdatedClient.Print();
		break;
	}
	case clsBankClient::enSaveResults::svFaildEmptyObject:
	{
		cout << "\nError account was not saved because it's Empty";
		break;

	}

	}

}

void AddNewClient() {

	string AccountNumber;
		
	cout << "Enter a Account Number : ";
	AccountNumber = clsInputValidate::ReadString();
	while (clsBankClient::IsClientExist(AccountNumber))
	{
		cout << "Acc Number is exist ,Enter another Account Number : ";
		AccountNumber = clsInputValidate::ReadString();
	} 
	clsBankClient NewClient = clsBankClient::GetAddNewClientObject(AccountNumber);
	ReadClient(NewClient);
	clsBankClient::enSaveResults svResult;
	svResult = NewClient.Save();

	switch (svResult) {
		case clsBankClient::svFaildEmptyObject : {
			cout << "\nError Account is Empty. " << endl;
			break;
		}
		case clsBankClient::svFaildAccountNumberExists : {
			cout << "\nError Account is alredy exists. " << endl;
			break;
		}
		case clsBankClient::svSucceeded : {
			cout << "\nAccount added seccesfuly. " << endl;
			NewClient.Print();
			break;
		}
	}
}

void DeleteClient() {
	cout << "Enter Account Number : ";
	string AccountNumber = clsInputValidate::ReadString();
	while (!clsBankClient::IsClientExist(AccountNumber))
	{
		cout << "Enter Another Account Number : ";
		AccountNumber = clsInputValidate::ReadString();
	}
	clsBankClient DeleteClient = clsBankClient::Find(AccountNumber);

	if (DeleteClient.Delete())
	{	
		cout << "\nClient Deleted Successfully. " << endl;
		DeleteClient.Print();
	}
	else
	{
		cout << "\nError Client was not Deleted. " << endl;
	}


}

void PrintClientRecordLine(clsBankClient Client)
{

	cout << "| " << setw(15) << left << Client.GetAccountNumber();
	cout << "| " << setw(20) << left << Client.Fullname();
	cout << "| " << setw(12) << left << Client.Phone;
	cout << "| " << setw(20) << left << Client.Email;
	cout << "| " << setw(10) << left << Client.PinCode;
	cout << "| " << setw(12) << left << Client.AccountBalance;

}

void ShowClientsList()
{

	vector <clsBankClient> vClients = clsBankClient::GetClientsList();

	cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	cout << "| " << left << setw(15) << "Accout Number";
	cout << "| " << left << setw(20) << "Client Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(20) << "Email";
	cout << "| " << left << setw(10) << "Pin Code";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	if (vClients.size() == 0)
		cout << "\t\t\t\tNo Clients Available In the System!";
	else

		for (clsBankClient Client : vClients)
		{

			PrintClientRecordLine(Client);
			cout << endl;
		}

	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

}

void PrintClientRecordBalanceLine(clsBankClient Client)
{

	cout << "| " << setw(15) << left << Client.GetAccountNumber();
	cout << "| " << setw(40) << left << Client.Fullname();
	cout << "| " << setw(12) << left << Client.AccountBalance;

}


void ShowTotalBalances()
{

	vector <clsBankClient> vClients = clsBankClient::GetClientsList();

	cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	cout << "| " << left << setw(15) << "Accout Number";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;

	double TotalBalances = clsBankClient::GetTotalBalances();

	if (vClients.size() == 0)
		cout << "\t\t\t\tNo Clients Available In the System!";
	else

		for (clsBankClient Client : vClients)
		{
			PrintClientRecordBalanceLine(Client);
			cout << endl;
		}

	cout << "\n_______________________________________________________";
	cout << "_________________________________________\n" << endl;
	cout << "\t\t\t\t\t   Total Balances = " << TotalBalances << endl;
	cout << "\t\t\t\t\t   ( " << clsUtil::NumberToText(TotalBalances) << ")";
}

int main()
{
	/*
	
	//clsBankClient client1(clsBankClient::enMode::EmptyMode , "John", "Doe", "dakjlzd@dazd", "0123456789", "1234567890", "1234", 1000.0);
	//client1.Print();
	//cout << endl << client1.IsEmpty();
	//client1.Print();
	clsBankClient Client1 = clsBankClient::Find("A101");
	if (!Client1.IsEmpty())
	{
		cout << "\nClient Found :-)\n";
	}
	else
	{
		cout << "\nClient Was not Found :-(\n";
	}

	clsBankClient Client2 = clsBankClient::Find("A101","12345");
	if (!Client2.IsEmpty())
	{
		cout << "\nClient Found :-)\n";
	}
	else
	{
		cout << "\nClient Was not Found :-(\n";
	}

	// cout << endl << clsBankClient::IsClientExist("A101","1234");

	*/
	UpdateClient();
	AddNewClient();
	DeleteClient();
	ShowClientsList();
	ShowTotalBalances();
	cout << endl << "\n==\n";
	return 0;


}

