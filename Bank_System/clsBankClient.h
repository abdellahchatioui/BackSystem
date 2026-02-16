#pragma once
#include <iostream>;
#include <string>;
#include "clsPerson.h";
#include "clsString.h";
#include <vector>
#include <fstream>

using namespace std;

class clsBankClient : public clsPerson
{

private:
	enum enMode
	{
		EmptyMode = 0,
		UpdateMode = 1,
		AddNewMode = 3,
		DeleteMode = 4,
	};
	static clsBankClient _ConvertLinetoClientObject(string Line ,  string seperator = "#//#") {
		vector<string> vClinetData;
		vClinetData = clsString::Split(Line, seperator);

		return clsBankClient(enMode::UpdateMode, vClinetData[0], vClinetData[1], vClinetData[2], vClinetData[3], vClinetData[4], vClinetData[5], stod(vClinetData[6]),false);

	}
	static clsBankClient _GetEmptyClientObject(){
		return clsBankClient(enMode::EmptyMode, "", "", "", "", "", "", 0,false);
	};
	
	static vector<clsBankClient> _LoadClientsDataFromFile() {
		vector<clsBankClient> Clients;
		ifstream MyFile("Clients.txt");
		if (MyFile.is_open()) {
			string Line;
			while (getline(MyFile,Line)) {
				Clients.push_back(_ConvertLinetoClientObject(Line));
			}
			return Clients;
		}
	};

	string _ConverClientObjectToLine(clsBankClient Client, string Seperator = "#//#") {
		string Line;
		
		Line += Client.FirstName + Seperator;
		Line += Client.LastName + Seperator;
		Line += Client.Email + Seperator;
		Line += Client.Phone + Seperator;
		Line += Client.GetAccountNumber() + Seperator;
		Line += Client.PinCode + Seperator;
		Line += to_string(Client.AccountBalance);
		return Line;
		
	}
	void _SaveCleintsDataToFile(vector<clsBankClient> vClients) {
		ofstream MyFile("Clients.txt");
		string Line;
		// convert vector to string Lines
		for (clsBankClient Client : vClients) {
			if (MyFile.is_open()) {
				if (!Client._MarkForDelete) {
					Line = _ConverClientObjectToLine(Client);
					MyFile << Line << endl;
				}
			}
		}
	}

	void _Update() {
		vector<clsBankClient> vClients;
		vClients = _LoadClientsDataFromFile();
		for (clsBankClient& c :vClients)
		{
			if (c.GetAccountNumber() == _AccountNumber) {
				c = *this;
				break;
			}
		}
		
		_SaveCleintsDataToFile(vClients);

	}

	void _SaveCleintsDataToFile(string NewClient) {
		fstream MyFile("Clients.txt", ios::in | ios::out | ios::app);
		if (MyFile.is_open()) {

			// Move to end of file
			MyFile.seekg(0, ios::end);

			if (MyFile.tellg() > 0) {  // file is not empty
				MyFile.seekg(-1, ios::end);  // go to last character
				char lastChar;
				MyFile.get(lastChar); // get the last character

				// if last char is NOT newline, add one
				if (lastChar != '\n') {
					MyFile << "\n";
				}
			}

			// Write the new client
			MyFile << NewClient << "\n";
		}
	}

	void _AddNew() {
		_SaveCleintsDataToFile(_ConverClientObjectToLine(*this));
	}

	enMode _Mode;
	string _AccountNumber;
	string _PinCode;
	float _AccountBalance;
	bool _MarkForDelete = false;

public:

	clsBankClient(enMode Mode,  string FirstName, string LastName, string Email, string Phone, string AccountNumber, string PinCode, float AccountBalance,bool MarkForDelete)
		: clsPerson(FirstName, LastName, Email, Phone)
	{
		_Mode = Mode;
		_AccountNumber = AccountNumber;
		_PinCode = PinCode;
		_AccountBalance = AccountBalance;
		_MarkForDelete = MarkForDelete;
	}
	
	bool IsEmpty() {
		return (_Mode == enMode::EmptyMode);
	}

	string GetAccountNumber() {
		return _AccountNumber;
	}


	void setPinCode(string PinCode) {
		_PinCode = PinCode;
	}

	string GetPinCode() {
		return _PinCode;
	}

	__declspec(property(get = GetPinCode, put = setPinCode)) string PinCode;

	void setAccountBalance(float AccountBalance) {
		_AccountBalance = AccountBalance;
	}

	float GetAccountBalance() {
		return _AccountBalance;
	}

	__declspec(property(get = GetAccountBalance, put = setAccountBalance)) float AccountBalance;

	void Print()
	{
		cout << "\n____________________________";
		cout << "\nFristName       : " << FirstName;
		cout << "\nLastName        : " << LastName;
		cout << "\nFullname        : " << Fullname();
		cout << "\nEmail           : " << Email;
		cout << "\nPhone           : " << Phone;
		cout << "\nAccount Number  : " << GetAccountNumber();
		cout << "\nPin Code        : " << GetPinCode();
		cout << "\nAccount Balance : " << GetAccountBalance();
		// cout << "\nMode            : " << _Mode;
		cout << "\n____________________________";

	}

	static clsBankClient Find(string AccountNumber) {
		ifstream Myfile("Clients.txt");
		// Myfile.open("Clients.txt", ios::in);
		
		if (Myfile.is_open()) {
			string Line;
			while (getline(Myfile, Line)) {
				clsBankClient Client = _ConvertLinetoClientObject(Line);
				if (Client.GetAccountNumber() == AccountNumber) {
					Myfile.close();
					return Client;
				}

			}
			Myfile.close();
			return _GetEmptyClientObject();
		}
	}

	static clsBankClient Find(string AccountNumber, string PinCode) {
		ifstream Myfile("Clients.txt");
		string Line;
		if (Myfile.is_open())
		{
			while (getline(Myfile, Line)) {
				clsBankClient Client = _ConvertLinetoClientObject(Line);
				if (Client.GetAccountNumber() == AccountNumber && Client.GetPinCode() == PinCode)
				{
					Myfile.close();
					return Client;
				}
			};

			Myfile.close();
			return _GetEmptyClientObject();
		}
	}

	bool Delete() {
		vector<clsBankClient> vClients;
		vClients = _LoadClientsDataFromFile();
		for (clsBankClient& c : vClients)
		{
			if (c.GetAccountNumber() == _AccountNumber) {
				c._MarkForDelete = true;
				break;
			}
		}

		_SaveCleintsDataToFile(vClients);
		*this = _GetEmptyClientObject();
		return true;
	}

	
	static clsBankClient GetAddNewClientObject(string AccountNumber) {
		return clsBankClient(enMode::AddNewMode, "", "", "", "", AccountNumber, "", 0,false);
	}
	static bool IsClientExist(string AccountNumber) {
		clsBankClient Client = clsBankClient::Find(AccountNumber);
		return (!Client.IsEmpty());
	}

	static bool IsClientExist(string AccountNumber,string Pincode) {
		clsBankClient Client = clsBankClient::Find(AccountNumber, Pincode);
		return (!Client.IsEmpty());
	}

	static vector <clsBankClient> GetClientsList() {
		return _LoadClientsDataFromFile();
	}

	static float GetTotalBalances()
	{
		vector<clsBankClient> vClient = clsBankClient::_LoadClientsDataFromFile();
		float TotalBalances = 0;
		for (clsBankClient Client : vClient)
		{
			TotalBalances += Client.AccountBalance;
		}
		return TotalBalances;
	}

	enum enSaveResults { svFaildEmptyObject = 0, svSucceeded = 1, svFaildAccountNumberExists = 2 };

	enSaveResults Save() {
		switch (_Mode)
		{
			case enMode::EmptyMode:  {
				if (IsEmpty())
				{
					return enSaveResults::svFaildEmptyObject;
				}
				break;
			}
			case enMode::UpdateMode :  {
				_Update();
				return enSaveResults::svSucceeded;
				break;
			}
			case enMode::AddNewMode:  {
				if (clsBankClient::IsClientExist(_AccountNumber))
				{
					return enSaveResults::svFaildAccountNumberExists;
				}
				else {
					// call ADD new function
					_AddNew();
					_Mode = enMode::UpdateMode;
					return enSaveResults::svSucceeded;
				}
				break;
			}
		}
	}
};

