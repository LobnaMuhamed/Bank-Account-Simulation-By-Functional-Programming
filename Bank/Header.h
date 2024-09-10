#pragma once
#include <iostream>
#include <string>
#include <cctype>
#include <iomanip>
#include <vector>
#include <fstream>

using namespace std;

namespace MyLib {
	const string ClientsFileName = "Clients.txt";
	const string UsersFileName = "Users.txt";
	struct stClient {
		string AccountNumber;
		string PinCode;
		string Name;
		string Phone;
		double AccountBalance;
		bool MarkToDelete = false;
	};
	struct stUser {
		string UserName;
		string Password;
		int Permission;
		bool MarkToDelete = false;
	};

	stUser CurrentUser;

	enum enMainMenuOptions {
		eListClients = 1,
		eAddNewClient = 2,
		eDeleteClient = 3,
		eUpdateClient = 4,
		eFindClient = 5,
		eTransactionsMenu = 6,
		eManageUsers = 7,
		eLogout = 8,
	};
	enum enTransactionsMenuOption {
		eDeposit = 1,
		eWithdraw = 2,
		eTotalBalances = 3,
		eMainMenue = 4,
	};
	enum enManageUsersMenuOptions {
		eListUsers = 1,
		eAddNewUser = 2,
		eDeleteUser = 3,
		eUpdateUser = 4,
		eFindUser = 5,
		eMainMenu = 6,
	};
	enum enMainMenuePermisssions {
		eAll = -1,
		pListClients = 1,
		pAddNewClient = 2,
		pDeleteClient = 4,
		pUpdateClients = 8,
		pFindClient = 16,
		pTransactions = 32,
		pManageUsers = 64,
	};

	void ShowMainMenu();
	void ShowTransactionMenu();
	void ShowManageUsersMenu();
	void Login();
	void GoBackToManageUsersMenu();
	bool CheckAccessPermissions(enMainMenuePermisssions Permission);

	vector <string> SplitString(string String, string Delim)
	{
		vector <string> vString;
		string sWord;
		short pos = 0;
		while ((pos = String.find(Delim)) != std::string::npos)
		{
			sWord = String.substr(0, pos);
			if (sWord != "")
				vString.push_back(sWord);
			String.erase(0, pos + Delim.length());
		}
		if (String != "")
			vString.push_back(String);
		return vString;
	}
	void AddDataLineToFile(string FileName, string stDataLine)
	{
		fstream MyFile;
		MyFile.open(FileName, ios::out | ios::app);
		if (MyFile.is_open())
		{
			MyFile << stDataLine << endl;
			MyFile.close();
		}
	}
	//***************************** User Functions *************************//

	string  ConvertUserRecordToLine(stUser UserDate, string Separator = "#//#")
	{
		string StUserDate = "";
		StUserDate += UserDate.UserName + Separator;
		StUserDate += UserDate.Password + Separator;
		StUserDate += to_string(UserDate.Permission);
		return StUserDate;
	}
	stUser ConvertUserLineToRecord(string Line, string Separator = "#//#")
	{
		stUser User;
		vector <string> vUserData = SplitString(Line, Separator);
		User.UserName = vUserData[0];
		User.Password = vUserData[1];
		User.Permission = stoi(vUserData[2]);
		return User;
	}
	vector <stUser> LoadUserDateFromFile(string FileName)
	{
		vector <stUser> vUsers;
		fstream MyFile;
		MyFile.open(FileName, ios::in);
		if (MyFile.is_open())
		{
			stUser User;
			string Line;
			while (getline(MyFile, Line))
			{
				User = ConvertUserLineToRecord(Line);
				vUsers.push_back(User);
			}
			MyFile.close();
		}
		return vUsers;
	}
	bool UserExitsByUserName(string UserName, string FileName)
	{
		fstream MyFile;
		MyFile.open(FileName, ios::in);
		if (MyFile.is_open())
		{
			string Line;
			stUser User;
			while (getline(MyFile, Line))
			{
				User = ConvertUserLineToRecord(Line);
				if (User.UserName == UserName)
				{
					MyFile.close();
					return true;
				}
			}
			MyFile.close();
		}
		return false;
	}
	int ReadPermissionsToSet()
	{
		int Permissions = 0;
		char Answer = 'n';

		cout << "Do You want to give full access? y/n? ";
		cin >> Answer;
		if (toupper(Answer) == 'Y')
			return -1;
		cout << "\nDo You Want To Give Access To:\n";

		cout << "\nShow Clients List? y/n? ";
		cin >> Answer;
		if (toupper(Answer) == 'Y')
			Permissions += enMainMenuePermisssions::pListClients;

		cout << "\nAdd New Client? y/n? ";
		cin >> Answer;
		if (toupper(Answer) == 'Y')
			Permissions += enMainMenuePermisssions::pAddNewClient;

		cout << "\nDelete Client? y/n? ";
		cin >> Answer;
		if (toupper(Answer) == 'Y')
			Permissions += enMainMenuePermisssions::pDeleteClient;

		cout << "\nUpdate Client? y/n? ";
		cin >> Answer;
		if (toupper(Answer) == 'Y')
			Permissions += enMainMenuePermisssions::pUpdateClients;

		cout << "\nFind Client? y/n? ";
		cin >> Answer;
		if (toupper(Answer) == 'Y')
			Permissions += enMainMenuePermisssions::pFindClient;

		cout << "\nTransaction? y/n? ";
		cin >> Answer;
		if (toupper(Answer) == 'Y')
			Permissions += enMainMenuePermisssions::pTransactions;

		cout << "\nManage Users? y/n? ";
		cin >> Answer;
		if (toupper(Answer) == 'Y')
			Permissions += enMainMenuePermisssions::pManageUsers;

		return Permissions;
	}
	stUser ReadNewUser()
	{
		stUser User;

		cout << "Enter Username? ";
		getline(cin >> ws, User.UserName);
		while (UserExitsByUserName(User.UserName, UsersFileName))
		{
			cout << "\nUser with [" << User.UserName << "] already exists, Enter another Username? ";
			getline(cin >> ws, User.UserName);
		}

		cout << "Enter Password? ";
		getline(cin, User.Password);

		User.Permission = ReadPermissionsToSet();

		return User;
	}
	void AddNewUser()
	{
		stUser User;
		User = ReadNewUser();
		AddDataLineToFile(UsersFileName, ConvertUserRecordToLine(User));
	}
	void AddNewUsers()
	{
		char AddMore = 'Y';
		do {
			cout << "Adding New User:\n\n";
			AddNewUser();
			cout << "\nUser Added Successfully, Do You Want To Add More Clients? Y/N ? ";

			cin >> AddMore;
		} while (toupper(AddMore) == 'Y');
	}

	string ReadUserName()
	{
		string UserName = "";
		cout << "\nPlease Enter Username? \n\n";
		cin >> UserName;
		return UserName;
	}
	void PrintUserRecord(stUser User)
	{
		cout << "\nThe Following Is The User Details:";
		cout << "\nUsername      : " << User.UserName;
		cout << "\nPassword      : " << User.Password;
		cout << "\nPermission    : " << User.Permission;
	}
	void PrintUserRecordInList(stUser User)
	{
		cout << "| " << setw(15) << left << User.UserName;
		cout << "| " << setw(10) << left << User.Password;
		cout << "| " << setw(40) << left << User.Permission;
	}
	void ShowAccessDeniedMessage()
	{
		cout << "\n------------------------------------\n";
		cout << "Access Denied, \nYou don't Have Permission To Do this, \nPlease Contact Your Admin.";
		cout << "\n------------------------------------\n";
	}
	void ShowAllUsersScreen()
	{
		vector <stUser> vUsers = LoadUserDateFromFile(UsersFileName);
		cout << "\n\t\t\t\t\tUsers List (" << vUsers.size() << ") User(s).";
		cout << "\n_______________________________________________________";
		cout << "_______________________________________________________\n" << endl;
		cout << "| " << left << setw(15) << "User Name";
		cout << "| " << left << setw(10) << "Password";
		cout << "| " << left << setw(15) << "Permissions";
		cout << "\n_______________________________________________________";
		cout << "_______________________________________________________\n" << endl;
		if (vUsers.size() == 0)
			cout << "\t\t\t\tNo Users Available In the System!";
		else
			for (stUser& User : vUsers)
			{
				PrintUserRecordInList(User);
				cout << endl;
			}
		cout << "\n_______________________________________________________";
		cout << "_______________________________________________________\n" << endl;

	}
	void ShowListUsersScreen()
	{
		ShowAllUsersScreen();
	}

	bool FindUserByUsernameAndPassword(string Username, string Password, stUser& User)
	{
		vector <stUser> vUsers = LoadUserDateFromFile(UsersFileName);
		for (stUser& U : vUsers)
		{
			if (U.UserName == Username && U.Password == Password)
			{
				User = U;
				return true;
			}
		}
		return false;
	}
	bool LoadUserData(string Username, string Password)
	{
		if (FindUserByUsernameAndPassword(Username, Password, CurrentUser))
			return true;
		else
			return false;
	}
	bool FindUserByUserName(string UserName, vector <stUser> vUsers, stUser& User)
	{
		for (stUser& U : vUsers)
		{
			if (U.UserName == UserName)
			{
				User = U;
				return true;
			}
		}
		return false;
	}
	bool MarkUserForDeleteByUserName(string UserName, vector <stUser>& vUsers)
	{
		for (stUser& U : vUsers)
		{
			if (U.UserName == UserName)
			{
				U.MarkToDelete = true;
				return true;
			}
		}
		return false;
	}
	vector <stUser> SaveUsersToFile(string FileName, vector <stUser> vUsers)
	{
		fstream MyFile;
		string Line;
		MyFile.open(FileName, ios::out);
		if (MyFile.is_open())
		{
			for (stUser& U : vUsers)
			{
				if (U.MarkToDelete == false)
				{
					Line = ConvertUserRecordToLine(U);
					MyFile << Line << endl;
				}
			}
			MyFile.close();
		}
		return vUsers;
	}
	bool DeleteUserFromFileByUserName(string UserName, vector <stUser>& vUsers)
	{
		if (UserName == "Admin")
		{
			cout << "\n\nYou Can't Delete This User.";
			return false;
		}
		stUser User;
		char Answer = 'n';
		if (FindUserByUserName(UserName, vUsers, User))
		{
			PrintUserRecord(User);
			cout << "\n\nAre You Sure You Want To Delete This Account? Y/N ? ";
			cin >> Answer;
			if (toupper(Answer) == 'Y')
			{
				MarkUserForDeleteByUserName(UserName, vUsers);
				SaveUsersToFile(UsersFileName, vUsers);

				vUsers = LoadUserDateFromFile(UsersFileName);
				cout << "\nUser Deleted Successfully.\n";
				return true;
			}
		}
		else
		{
			cout << "\nUser With Username (" << UserName << ") is Not Found!\n\n";
			return false;
		}
	}
	stUser ChangeUserRecord(string UserName)
	{
		stUser User;
		User.UserName = UserName;
		cout << "\nEnter Password ? ";
		getline(cin >> ws, User.Password);
		User.Permission = ReadPermissionsToSet();
		return User;
	}
	bool UpdateUserDateByUserName(string UserName, vector <stUser>& vUsers)
	{
		stUser User;
		char Answer = 'n';
		if (FindUserByUserName(UserName, vUsers, User))
		{
			PrintUserRecord(User);
			cout << "\nAre You Sure You Want To Update This Client ? Y/N ? ";
			cin >> Answer;
			if (toupper(Answer) == 'Y')
			{
				for (stUser& U : vUsers)
				{
					if (U.UserName == UserName)
					{
						U = ChangeUserRecord(UserName);
						break;
					}
				}
				SaveUsersToFile(UsersFileName, vUsers);
				cout << "\nUser Updated Successfully.\n";
				return true;
			}
		}
		else
		{
			cout << "\nUser With Username (" << UserName << ") is Not Found!\n\n";
			return false;
		}
	}

	void ShowAddNewUserScreen()
	{
		cout << "\n-------------------------------------------\n";
		cout << "\tAdd New Users Screen";
		cout << "\n-------------------------------------------\n";
		AddNewUsers();
	}
	void ShowDeleteUserScreen()
	{
		cout << "\n-------------------------------------------\n";
		cout << "\tDelete User Screen";
		cout << "\n-------------------------------------------\n";
		vector <stUser> vUsers = LoadUserDateFromFile(UsersFileName);
		string UserName = ReadUserName();
		DeleteUserFromFileByUserName(UserName, vUsers);
	}
	void ShowFindUserScreen()
	{
		cout << "\n-------------------------------------------\n";
		cout << "\tFind User Info Screen";
		cout << "\n-------------------------------------------\n";
		vector <stUser> vUsers = LoadUserDateFromFile(UsersFileName);
		stUser User;
		string UserName = ReadUserName();
		if (FindUserByUserName(UserName, vUsers, User))
			PrintUserRecord(User);
		else
			cout << "\nClient with Account Number[" << UserName << "] is not Found!";
	}

	//***************************** Client Functions *************************//

	string ConvertClientRecordToLine(stClient ClientData, string Separator = "#//#")
	{
		string StClientData = "";
		StClientData += ClientData.AccountNumber + Separator;
		StClientData += ClientData.PinCode + Separator;
		StClientData += ClientData.Name + Separator;
		StClientData += ClientData.Phone + Separator;
		StClientData += to_string(ClientData.AccountBalance);
		return StClientData;
	}
	stClient ConvertClientLineToRecord(string Line, string Separator = "#//#")
	{
		stClient Client;
		vector <string> vClientData;
		vClientData = SplitString(Line, Separator);
		Client.AccountNumber = vClientData[0];
		Client.PinCode = vClientData[1];
		Client.Name = vClientData[2];
		Client.Phone = vClientData[3];
		Client.AccountBalance = stod(vClientData[4]);
		return Client;
	}

	vector <stClient> LoadClientDataFromFile(string FileName)
	{
		vector <stClient> vClients;
		fstream MyFile;
		MyFile.open(FileName, ios::in);
		if (MyFile.is_open())
		{
			stClient Client;
			string Line;
			while (getline(MyFile, Line))
			{
				Client = ConvertClientLineToRecord(Line);
				vClients.push_back(Client);
			}
			MyFile.close();
		}
		return vClients;
	}
	bool ClientExitsByAccountNumber(string AccountNumber, string FileName)
	{
		vector <stClient> vClients;
		fstream MyFile;
		MyFile.open(FileName, ios::in);
		if (MyFile.is_open())
		{
			string Line;
			stClient Client;
			while (getline(MyFile, Line))
			{
				Client = ConvertClientLineToRecord(Line);
				if (Client.AccountNumber == AccountNumber)
				{
					MyFile.close();
					return true;
				}
				vClients.push_back(Client);
			}
			MyFile.close();
		}
		return false;
	}
	stClient ReadNewClient()
	{
		stClient Client;
		cout << "Enter Account Number? ";
		getline(cin >> ws, Client.AccountNumber);
		while (ClientExitsByAccountNumber(Client.AccountNumber, ClientsFileName))
		{
			cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
			getline(cin >> ws, Client.AccountNumber);
		}
		cout << "Enter PinCode? ";
		getline(cin, Client.PinCode);
		cout << "Enter Name? ";
		getline(cin, Client.Name);
		cout << "Enter Phone? ";
		getline(cin, Client.Phone);
		cout << "Enter Account Balance? ";
		cin >> Client.AccountBalance;
		return Client;
	}
	void AddNewClient()
	{
		stClient Client;
		Client = ReadNewClient();
		AddDataLineToFile(ClientsFileName, ConvertClientRecordToLine(Client));
	}
	void AddClients()
	{
		char AddMore = 'Y';
		do
		{
			cout << "Adding New Client:\n\n";
			AddNewClient();
			cout << "\nClient Added Successfully, Do You Want To Add More Clients? Y/N ? ";
			cin >> AddMore;
		} while (toupper(AddMore) == 'Y');
	}

	string ReadAccountNumber()
	{
		string AccountNumber = "";
		cout << "\nPlease Enter Account Number? \n\n";
		cin >> AccountNumber;
		return AccountNumber;
	}
	void PrintClientRecord(stClient Client)
	{
		cout << "\nThe Following Is The Extracted Client Record:";
		cout << "\nAccount Number: " << Client.AccountNumber;
		cout << "\nPinCode       : " << Client.PinCode;
		cout << "\nName          : " << Client.Name;
		cout << "\nPhone         : " << Client.Phone;
		cout << "\nAccout Balance: " << Client.AccountBalance << endl;
	}

	void PrintClientBalanceInBalanceList(stClient Client)
	{
		cout << "| " << setw(15) << left << Client.AccountNumber;
		cout << "| " << setw(40) << left << Client.Name;
		cout << "| " << setw(12) << left << Client.AccountBalance;

	}
	void PrintClientRecordInList(stClient Client)
	{
		cout << "| " << setw(15) << left << Client.AccountNumber;
		cout << "| " << setw(10) << left << Client.PinCode;
		cout << "| " << setw(40) << left << Client.Name;
		cout << "| " << setw(12) << left << Client.Phone;
		cout << "| " << setw(12) << left << Client.AccountBalance;
	}

	void ShowAllClientScreen()
	{
		if (!CheckAccessPermissions(enMainMenuePermisssions::pListClients))
		{
			ShowAccessDeniedMessage();
			return;
		}
		vector <stClient> vClients = LoadClientDataFromFile(ClientsFileName);
		cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
		cout << "\n_______________________________________________________";
		cout << "_______________________________________________________\n" << endl;
		cout << "| " << left << setw(15) << "Account Number";
		cout << "| " << left << setw(10) << "Pin Code";
		cout << "| " << setw(15) << left << "Client Name";
		cout << "| " << setw(15) << left << "Phone";
		cout << "| " << setw(15) << left << "Balance";
		cout << "\n_______________________________________________________";
		cout << "_______________________________________________________\n" << endl;
		for (stClient& Client : vClients)
		{
			PrintClientRecordInList(Client);
			cout << endl;
		}
		cout << "\n_______________________________________________________";
		cout << "_______________________________________________________\n" << endl;
	}
	void ShowTotalBalances()
	{
		vector <stClient> vClients = LoadClientDataFromFile(ClientsFileName);
		short TotalBalances = 0;
		cout << "\n\t\t\t\t\tBalances List (" << vClients.size() << ") Client(s).";
		cout << "\n_______________________________________________________";
		cout << "_______________________________________________________\n" << endl;
		cout << "| " << left << setw(15) << "Account Number";
		cout << "| " << setw(40) << left << "Client Name";
		cout << "| " << setw(15) << left << "Balance";
		cout << "\n_______________________________________________________";
		cout << "_______________________________________________________\n" << endl;
		for (stClient& Client : vClients)
		{
			TotalBalances += Client.AccountBalance;
			PrintClientBalanceInBalanceList(Client);
			cout << endl;
		}
		cout << "\n_______________________________________________________";
		cout << "_______________________________________________________\n";
		cout << "\n\n\t\t\t\t\tTotal Balances = " << TotalBalances << endl;

	}

	bool FindClientByAccountNumber(string AccountNumber, vector <stClient> vClients, stClient& Client)
	{
		if (!CheckAccessPermissions(enMainMenuePermisssions::pFindClient))
		{
			ShowAccessDeniedMessage();
			return false;
		}
		for (stClient& C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				Client = C;
				return true;
			}
		}
		return false;
	}

	bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <stClient>& vClients)
	{
		for (stClient& C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.MarkToDelete = true;
				return true;
			}
		}
		return false;
	}
	vector <stClient> SaveClientsToFile(string FileName, vector <stClient> vClients)
	{
		fstream MyFile;
		string Line;
		MyFile.open(FileName, ios::out);
		if (MyFile.is_open())
		{
			for (stClient& C : vClients)
			{
				if (C.MarkToDelete == false)
				{
					Line = ConvertClientRecordToLine(C);
					MyFile << Line << endl;
				}
			}
			MyFile.close();
		}
		return vClients;
	}

	bool DeleteClientFromFileByAccountNumber(string AccountNumber, vector <stClient>& vClients)
	{
		if (!CheckAccessPermissions(enMainMenuePermisssions::pDeleteClient))
		{
			ShowAccessDeniedMessage();
			return false;
		}
		stClient Client;
		char Answer = 'n';
		if (FindClientByAccountNumber(AccountNumber, vClients, Client))
		{
			PrintClientRecord(Client);
			cout << "\n\nAre You Sure You Want To Delete This Account? Y/N ? ";
			cin >> Answer;
			if (toupper(Answer) == 'Y')
			{
				MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
				SaveClientsToFile(ClientsFileName, vClients);

				vClients = LoadClientDataFromFile(ClientsFileName);
				cout << "\nClient Deleted Successfully.\n";
				return true;
			}
		}
		else
		{
			cout << "\nClient With Account Number (" << AccountNumber << ") is Not Found!\n\n";
			return false;
		}
	}

	stClient ChangeClientRecord(string AccountNumber)
	{
		stClient Client;
		Client.AccountNumber = AccountNumber;
		cout << "\nEnter PinCode ? ";
		getline(cin >> ws, Client.PinCode);
		cout << "\nEnter Name ? ";
		getline(cin, Client.Name);
		cout << "\nEnter Phone ? ";
		getline(cin, Client.Phone);
		cout << "\nEnter Account Balance ? ";
		cin >> Client.AccountBalance;
		return Client;
	}

	bool UpdateClientDataByAccountNumber(string AccountNumber, vector <stClient>& vClients)
	{
		if (!CheckAccessPermissions(enMainMenuePermisssions::pUpdateClients))
		{
			ShowAccessDeniedMessage();
			return false;
		}
		stClient Client;
		char Answer = 'n';
		if (FindClientByAccountNumber(AccountNumber, vClients, Client))
		{
			PrintClientRecord(Client);
			cout << "\nAre You Sure You Want To Update This Client ? Y/N ? ";
			cin >> Answer;
			if (toupper(Answer) == 'Y')
			{
				for (stClient& C : vClients)
				{
					if (C.AccountNumber == AccountNumber)
					{
						C = ChangeClientRecord(AccountNumber);
						break;
					}
				}
				SaveClientsToFile(ClientsFileName, vClients);
				cout << "\nClient Updated Successfully.\n";
				return true;
			}
		}
		else
		{
			cout << "\nClient With Account Number (" << AccountNumber << ") is Not Found!\n\n";
			return false;
		}
	}

	bool DepositBalanceToClientByAccountNumber(string AccountNumber, double Amount, vector <stClient>& vClients)
	{
		char Answer = 'n';
		cout << "\n\nAre You Sure You Want Perform This Transaction? Y/N ? ";
		cin >> Answer;
		if (toupper(Answer) == 'Y')
		{
			for (stClient& C : vClients)
			{
				if (C.AccountNumber == AccountNumber)
				{
					C.AccountBalance += Amount;
					SaveClientsToFile(ClientsFileName, vClients);
					cout << "\n\nDone Successfully. New Balance is: " << C.AccountBalance << endl;
					return true;

				}
			}
			return false;
		}
	}

	bool CheckAccessPermissions(enMainMenuePermisssions Permission)
	{
		if (CurrentUser.Permission == enMainMenuePermisssions::eAll)
			return true;
		if ((Permission & CurrentUser.Permission) == Permission)
			return true;
		else
			return false;
	}
	void ShowAddNewClientsScreen()
	{
		cout << "\n-------------------------------------------\n";
		cout << "\tAdd New Clients Screen";
		cout << "\n-------------------------------------------\n";
		AddClients();
	}

	void ShowDeleteClientScreen()
	{
		cout << "\n-------------------------------------------\n";
		cout << "\tDelete Client Screen";
		cout << "\n-------------------------------------------\n";
		vector <stClient> vClients = LoadClientDataFromFile(ClientsFileName);
		string AccountNumber = ReadAccountNumber();
		DeleteClientFromFileByAccountNumber(AccountNumber, vClients);
	}

	void ShowUpdateClientScreen()
	{
		cout << "\n-------------------------------------------\n";
		cout << "\tUpdate Client Info Screen";
		cout << "\n-------------------------------------------\n";
		vector <stClient> vClients = LoadClientDataFromFile(ClientsFileName);
		string AccountNumber = ReadAccountNumber();
		UpdateClientDataByAccountNumber(AccountNumber, vClients);
	}
	void ShowUpdateUserScreen()
	{
		cout << "\n-------------------------------------------\n";
		cout << "\tUpdate User Info Screen";
		cout << "\n-------------------------------------------\n";
		vector <stUser> vUsers = LoadUserDateFromFile(UsersFileName);
		string UserName = ReadUserName();
		UpdateUserDateByUserName(UserName, vUsers);
	}
	void ShowFindClientScreen()
	{
		cout << "\n-------------------------------------------\n";
		cout << "\tFind Client Info Screen";
		cout << "\n-------------------------------------------\n";
		vector <stClient> vClients = LoadClientDataFromFile(ClientsFileName);
		stClient Client;
		string AccountNumber = ReadAccountNumber();
		if (FindClientByAccountNumber(AccountNumber, vClients, Client))
			PrintClientRecord(Client);
		else
			cout << "\nClient with Account Number[" << AccountNumber << "] is not Found!";
	}

	void ShowDepositScreen()
	{
		cout << "\n-------------------------------------------\n";
		cout << "\tDeposit Transaction Screen";
		cout << "\n-------------------------------------------";
		stClient Client;
		vector <stClient> vClients = LoadClientDataFromFile(ClientsFileName);
		string AccountNumber = ReadAccountNumber();
		while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
		{
			cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
			AccountNumber = ReadAccountNumber();
		}
		PrintClientRecord(Client);
		double Amount = 0;
		cout << "\nPlease enter deposit amount? ";
		cin >> Amount;
		DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vClients);
	}
	void ShowWithdrawScreen()
	{
		cout << "\n-------------------------------------------\n";
		cout << "\tWithdraw Transaction Screen";
		cout << "\n-------------------------------------------";
		stClient Client;
		vector <stClient> vClients = LoadClientDataFromFile(ClientsFileName);
		string AccountNumber = ReadAccountNumber();
		while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
		{
			cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
			AccountNumber = ReadAccountNumber();
		}
		PrintClientRecord(Client);
		double Amount = 0;
		cout << "\nPlease Enter Withdraw Amount? ";
		cin >> Amount;
		while (Amount > Client.AccountBalance)
		{
			cout << "\nAmount Exceeds The Balance, You Can Withdraw Up To: " << Client.AccountBalance << endl;
			cout << "Please Enter Another Amount? ";
			cin >> Amount;
		}
		DepositBalanceToClientByAccountNumber(AccountNumber, Amount * -1, vClients);

	}
	/*
		void ShowEndScreen()
		{
			cout << "\n-------------------------------------------\n";
			cout << "\tProgram Ends :-)";
			cout << "\n-------------------------------------------\n\n";
		}
	*/
	short ReadMainMenuOption()
	{
		cout << "Choose what do you want to do? [1 to 8]? ";
		short Choice = 0;
		cin >> Choice;
		return Choice;
	}
	short ReadTransactionMenuOption()
	{
		cout << "Choose what do you want to do? [1 to 4]? ";
		short Choice = 0;
		cin >> Choice;
		return Choice;
	}
	short ReadManageUserMenuOption()
	{
		cout << "Choose what do you want to do? [1 to 6]? ";
		short Choice = 0;
		cin >> Choice;
		return Choice;
	}
	void GoBackToMainMenu()
	{
		cout << "\n\nPress any key to go back to Main Menu...";
		system("pause>0");
		ShowMainMenu();
	}

	void GoBackToTransactionMenu()
	{
		cout << "\n\nPress any key to go back to Transaction Menu...";
		system("pause>0");
		ShowTransactionMenu();
	}

	void GoBackToManageUsersMenu()
	{
		cout << "\n\nPress any key to go back to Manage Users Menu...";
		system("pause>0");
		ShowManageUsersMenu();
	}
	void PerformTransactionMenuOption(enTransactionsMenuOption TransactionOption)
	{
		switch (TransactionOption)
		{
		case enTransactionsMenuOption::eDeposit:
			system("cls");
			ShowDepositScreen();
			GoBackToTransactionMenu();
			break;
		case enTransactionsMenuOption::eWithdraw:
			system("cls");
			ShowWithdrawScreen();
			GoBackToTransactionMenu();
			break;
		case enTransactionsMenuOption::eTotalBalances:
			system("cls");
			ShowTotalBalances();
			GoBackToTransactionMenu();
			break;
		case enTransactionsMenuOption::eMainMenue:
			ShowMainMenu();
		}
	}
	void ShowTransactionMenu()
	{
		if (!CheckAccessPermissions(enMainMenuePermisssions::pTransactions))
		{
			ShowAccessDeniedMessage();
			GoBackToMainMenu();
			return;
		}
		system("cls");
		cout << "===========================================\n";
		cout << "\t\tTransactions Menu Screen\n";
		cout << "===========================================\n";
		cout << "\t[1] Deposit.\n";
		cout << "\t[2] Withdraw.\n";
		cout << "\t[3] Total Balances.\n";
		cout << "\t[4] Main Menu.\n";
		cout << "===========================================\n";
		PerformTransactionMenuOption((enTransactionsMenuOption)ReadTransactionMenuOption());
	}
	void PerformManageUserMenuOption(enManageUsersMenuOptions ManageUsersMenuOption)
	{
		switch (ManageUsersMenuOption)
		{

		case enManageUsersMenuOptions::eListUsers:
			system("cls");
			ShowListUsersScreen();
			GoBackToManageUsersMenu();
			break;

		case enManageUsersMenuOptions::eAddNewUser:
			system("cls");
			ShowAddNewUserScreen();
			GoBackToManageUsersMenu();
			break;

		case enManageUsersMenuOptions::eDeleteUser:
			system("cls");
			ShowDeleteUserScreen();
			GoBackToManageUsersMenu();
			break;

		case enManageUsersMenuOptions::eUpdateUser:
			system("cls");
			ShowUpdateUserScreen();
			GoBackToManageUsersMenu();
			break;

		case enManageUsersMenuOptions::eFindUser:
			system("cls");
			ShowFindUserScreen();
			GoBackToManageUsersMenu();
			break;

		case enManageUsersMenuOptions::eMainMenu:
			ShowMainMenu();
		}
	}
	void ShowManageUsersMenu()
	{
		if (!CheckAccessPermissions(enMainMenuePermisssions::pManageUsers))
		{
			ShowAccessDeniedMessage();
			GoBackToMainMenu();
			return;
		}
		system("cls");
		cout << "===========================================\n";
		cout << "\t\tManage Users Menu Screen\n";
		cout << "===========================================\n";
		cout << "\t[1] List Users.\n";
		cout << "\t[2] Add New User.\n";
		cout << "\t[3] Delete User.\n";
		cout << "\t[4] Update User.\n";
		cout << "\t[5] Find User.\n";
		cout << "\t[6] Main Menu.\n";
		cout << "===========================================\n";
		PerformManageUserMenuOption((enManageUsersMenuOptions)ReadManageUserMenuOption());
	}
	void PerformMainMenuOption(enMainMenuOptions MainMenuOption)
	{
		switch (MainMenuOption)
		{
		case enMainMenuOptions::eListClients:
			system("cls");
			ShowAllClientScreen();
			GoBackToMainMenu();
			break;

		case enMainMenuOptions::eAddNewClient:
			system("cls");
			ShowAddNewClientsScreen();
			GoBackToMainMenu();
			break;

		case enMainMenuOptions::eDeleteClient:
			system("cls");
			ShowDeleteClientScreen();
			GoBackToMainMenu();
			break;

		case enMainMenuOptions::eUpdateClient:
			system("cls");
			ShowUpdateClientScreen();
			GoBackToMainMenu();
			break;

		case enMainMenuOptions::eFindClient:
			system("cls");
			ShowFindClientScreen();
			GoBackToMainMenu();
			break;
		case enMainMenuOptions::eTransactionsMenu:
			system("cls");
			ShowTransactionMenu();
			break;
		case enMainMenuOptions::eManageUsers:
			system("cls");
			ShowManageUsersMenu();
			break;
		case enMainMenuOptions::eLogout:
			system("cls");
			// ShowEndScreen();
			Login();
			break;
		}
	}
	void ShowMainMenu()
	{
		system("cls");
		cout << "===========================================\n";
		cout << "\t\tMain Menue Screen\n";
		cout << "===========================================\n";
		cout << "\t[1] Show Client List.\n";
		cout << "\t[2] Add New Client.\n";
		cout << "\t[3] Delete Client.\n";
		cout << "\t[4] Update Client Info.\n";
		cout << "\t[5] Find Client.\n";
		cout << "\t[6] Transactions.\n";
		cout << "\t[7] Manage Users.\n";
		cout << "\t[8] Logout.\n";
		cout << "===========================================\n";
		PerformMainMenuOption((enMainMenuOptions)ReadMainMenuOption());
	}
	void Login()
	{
		bool LoginFailed = false;
		string Username, Password;

		do {
			system("cls");
			cout << "===========================================\n";
			cout << "\t\tLogin Screen\n";
			cout << "===========================================\n";
			if (LoginFailed)
				cout << "Invalid Username/Password!\n";

			cout << "Enter Username? ";
			cin >> Username;

			cout << "Enter Password? ";
			cin >> Password;

			LoginFailed = !LoadUserData(Username, Password);
		} while (LoginFailed);

		ShowMainMenu();
	}

	void Start()
	{
		Login();
		system("pause>0");
	}
}