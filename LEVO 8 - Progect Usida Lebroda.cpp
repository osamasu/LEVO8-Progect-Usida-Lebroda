#include "Osama.h"


const string UserFileName = "Users.txt";
const string ClientFileName = "Clients.txt";

struct strUser {

	string UserName;
	string Password;
	bool MarkToDelete = false;
	int Permissions = 0;
};
struct strClient {

	string AccountNumber;
	string Password;
	string FullName;
	string PhoneNumber;
	double Balance;

	bool MarkToDelete = false;
};

enum enPerformMainMenuOptions { eShowClientsList = 1 , eAddNewClient , eDeleteClient , eUpdateClientInfo , eFindClient , eTransactionsMenu , eManageUsers , eLogout };
enum enPerformTransactionMenuOptions { eDeposit = 1, eWithdraw, eTotal_Balances, eBackToMainMenu };
enum enPerformManageUsersMenuScreen { eListUsers = 1, eAddNewUser, eDeleteUser, eUpdateUser, eFindUser, eeBackToMainMenu };
enum enMainMenuPermissions { pAll = -1, pListClients = 1, pAddNewClient = 2, pDeleteClient = 4, pUpdateClient = 8, pFindClient = 16, pTransactionsMenu = 32, pManageUsers = 64 };

strUser CurrentUser;

strClient CurrentClient;
void MainMenuScreen(void);
void TransactionsMenuScreen(void);

void PerformMainMenuOptions(enPerformMainMenuOptions);
void ManageUsersMenuScreen(void);

void BackToMainMenu() {

	printf("Press Any Key To go Back To Main Menu...");
	system("pause>0");

	MainMenuScreen();
}

void LoginScreen() {

	system("cls");

	printf("----------------------------------------");
	printf("\n              LOGIN SCREEN              ");
	printf("\n----------------------------------------\n");
}

vector < string > SplitString(string S1, string Seperator = "#//#") {

	short Pos = 0; string sWord;

	vector < string > vString;

	while ((Pos = S1.find(Seperator)) != std::string::npos)  {

		sWord = S1.substr(0, Pos);

		if (sWord != "") {

			vString.push_back(sWord);
		}

		S1.erase(0, Pos + Seperator.length());
	}

	if (S1 != "") {

		vString.push_back(S1);
	}

	return vString;
}

strUser ConvertUserLineToRecord(string LineOfRecord, string Seperator = "#//#") {

	strUser User;

	vector < string > vUser = SplitString(LineOfRecord, Seperator);

	User.UserName = vUser[0];
	User.Password = vUser[1];
	User.Permissions = stoi(vUser[2]);

	return User;
}

string ConvertUserRecordToLine(strUser User, string Seperator = "#//#") {

	return User.UserName + Seperator + User.Password + Seperator + to_string(User.Permissions);
}

vector <strUser> LoadUserDate_FromFileToVector() {

	vector <strUser> vUser;

	fstream MyFile;
	MyFile.open(UserFileName, ios::in);

	if (MyFile.is_open()) {

		string LineOfRecord;
		
		
		while (getline(MyFile,LineOfRecord)) {

			vUser.push_back(ConvertUserLineToRecord(LineOfRecord));
		}

		MyFile.close();
	}

	return vUser;
}


bool FindUserBy_UsernameAndPassword(string Username, string Password,strUser &User) {

	vector < strUser > vUser = LoadUserDate_FromFileToVector();

	for (strUser& U : vUser) {

		if (U.UserName == Username && U.Password == Password) {

			User = U;
			return true;
		}
	}

	return false;
}

bool LoadUserInfo(string Username, string Password) {

	return FindUserBy_UsernameAndPassword(Username, Password, CurrentUser);
}

enPerformMainMenuOptions ReadMainMenuOption() {

	return (enPerformMainMenuOptions)legend::ReadNumber_InRange::ReadNumber_InRange_short_Ver(" Choose What Do You Want To Do ?   { 1 To 8 } |  ", 1, 8);
}

strClient ConvertClientLineToRecord(string LineOfRecord , string Seperator = "#//#") {
	
	vector <string> vClientString = SplitString(LineOfRecord,Seperator);

	strClient Client;

	Client.AccountNumber = vClientString[0];
	Client.Password = vClientString[1];
	Client.FullName = vClientString[2];
	Client.PhoneNumber = vClientString[3];
	Client.Balance = stod(vClientString[4]);

	return Client;
}

vector <strClient> LoadClientData_FromFileToVector() {

	vector <strClient> vClient;

	fstream MyFile;
	MyFile.open(ClientFileName, ios::in);

	if (MyFile.is_open()) {

		string LineOfRecord;

		while (getline(MyFile,LineOfRecord)) {

			vClient.push_back(ConvertClientLineToRecord(LineOfRecord));
		}

		MyFile.close();
	}

	return vClient;
}

void PrintClientRecordLine(strClient& Client) {

	cout << "|  " << left << setw(16) << Client.AccountNumber;
	cout << "|  " << setw(11) << Client.Password;
	cout << "|  " << setw(38) << Client.FullName;
	cout << "|  " << setw(20) << Client.PhoneNumber;
	cout << "|  " << setw(20) << Client.Balance;

}

void ShowClientsList() {

	vector <strClient> vClients = LoadClientData_FromFileToVector();

	system("cls");

	printf("\n\t\t\t\t\t Client List (%d)  Client(s). \n", vClients.size());

	printf("\n_____________________________________________________________________________________________________________\n\n");

	cout << "|  " << left << setw(16) << "Account Number";
	cout << "|  " << setw(11) << "Pin Code";
	cout << "|  " << setw(38) << "Client Name";
	cout << "|  " << setw(20) << "Phone Number";
	cout << "|  " << setw(20) << "Balance";

	printf("\n_____________________________________________________________________________________________________________\n\n");

	if (vClients.size() == 0) {

		printf("\t\t\t\t No  Clients  Available  In  The  System !! \n");
	}
	else {

		for (strClient& C : vClients) {

			PrintClientRecordLine(C);
			cout << endl;
		}
	}

	printf("_____________________________________________________________________________________________________________\n\n");
}

bool CheckAccessPermission(enMainMenuPermissions Permission) {

	if (CurrentUser.Permissions == enMainMenuPermissions::pAll)
		return false;

	return ((CurrentUser.Permissions & Permission) != Permission);
}

void ShowAccessDeniedMessage() {

	system("cls");

	printf("\n ---------------------------------------------------\n");
	printf("Access Denied , \n You Don't Have Permission To Do This ,\n");
	printf("Please Contact Your Admin\n");
	printf("\n ---------------------------------------------------\n");
}


void ClientsList() {

	if (CheckAccessPermission(enMainMenuPermissions::pListClients)) {

		ShowAccessDeniedMessage();
		BackToMainMenu();
	}


	ShowClientsList();
}





bool IsClientExistByAccountNumber(string AccountNumber,strClient &Client) {

	vector <strClient> vClient = LoadClientData_FromFileToVector();

	for (strClient& C : vClient) {

		if (C.AccountNumber == AccountNumber) {

			Client = C;
			return true;
		}
	}

	return false;
}



strClient ReadClient() {

	strClient Client;

	printf("\nPlease , Enter An Account Number ?  | ");
	getline(cin >> ws, Client.AccountNumber);

	while (IsClientExistByAccountNumber(Client.AccountNumber,::CurrentClient)) {

		cout << "\nClient With[" << Client.AccountNumber << "] Account Number, Is Already Exist.";
		Client.AccountNumber = legend::Strings::ReadString("|  Enter Another Account Number ?   ");
	}

	Client.Password = legend::Strings::ReadString("Please , Enter The Password ? | ");

	Client.FullName = legend::Strings::ReadString("Please , Enter Client Name ? | ");

	Client.PhoneNumber = legend::Strings::ReadString("Please , Enter The Phone Number ? | ");

	Client.Balance = legend::ReadNumber::ReadNumber_float_Version("Please , Enter The Account Balance ? | ");

	return Client;
}

strClient ChangeClientRecord(string AccountNumber) {

	strClient Client;

	Client.AccountNumber = AccountNumber;

	printf("Please , Enter The Password ? | ");
	getline(cin >> ws, Client.Password);

	Client.FullName = legend::Strings::ReadString("Please , Enter Client Name ? | ");

	Client.PhoneNumber = legend::Strings::ReadString("Please , Enter The Phone Number ? | ");

	Client.Balance = legend::ReadNumber::ReadNumber_float_Version("Please , Enter The Balance ? | ");

	return Client;
}

string ConvertClientRecordToLine(strClient Client , string Seperator = "#//#") {

	return Client.AccountNumber + Seperator + Client.Password + Seperator + Client.FullName + Seperator + Client.PhoneNumber + Seperator + to_string(Client.Balance);
}

void AddNewClient() {

	strClient Client = ReadClient();
	legend::Files::AddDataLineToFile(ClientFileName, ConvertClientRecordToLine(Client));
}


void AddNewClients() {

	char AddMoreClients;

	do {

		AddNewClient();

		AddMoreClients = legend::Strings::ReadChar("\nDo You Want To Add more Clients ?  | ");

	} while (toupper(AddMoreClients) == 'Y');
}

void ShowAddNewClientsScreen() {

	if (CheckAccessPermission(enMainMenuPermissions::pAddNewClient)) {

		ShowAccessDeniedMessage();
		BackToMainMenu();
	}

	system("cls");

	printf("--------------------------------------------\n");
	printf("            Add New Client Screen\n");
	printf("--------------------------------------------\n");
	printf(" Adding New Client: \n");

	AddNewClients();
}

bool MarkClientForDelete_ByAccountNumber(vector <strClient> &vClient) {

	for (strClient& C : vClient) {

		if (C.AccountNumber == CurrentClient.AccountNumber) {

			C.MarkToDelete = true;
			return true;
		}
	}
	return false;
}

void LoadClientsToFile(vector <strClient>& vClient) {

	fstream MyFile;

	MyFile.open(ClientFileName, ios::out);

	if (MyFile.is_open()) {

		for (strClient& C : vClient) {

			if (C.MarkToDelete != true) {

				MyFile << ConvertClientRecordToLine(C) << endl;
			}
		}

		MyFile.close();
	}
}

void PrintClientCardInfo(strClient Client) {

	cout << "\nThe Following Are The Client Details:\n";
	cout << "---------------------------------------\n";
	cout << "Account Number : " << Client.AccountNumber << endl;
	cout << "Password       : " << Client.Password << endl;
	cout << "Client Name    : " << Client.FullName << endl;
	cout << "Phone Number   : " << Client.PhoneNumber << endl;
	cout << "Account Balance: " << Client.Balance << endl;
	cout << "---------------------------------------\n\n";
}

bool DeleteClient(vector <strClient> &vClients,string AccountNumber) {

	if (!IsClientExistByAccountNumber(AccountNumber, CurrentClient)) {

		printf("\nClient With [%s] Account Number , Is Already Exist.\n\n", AccountNumber.c_str());
		return false;
	}

	PrintClientCardInfo(CurrentClient);
	char SureToPerform = false;
	SureToPerform = legend::Strings::ReadChar("Are You Sure To Perform Deleteing This Client ?  [Y / N] |  ");

	if (toupper(SureToPerform) == 'Y') {

		MarkClientForDelete_ByAccountNumber(vClients);
		LoadClientsToFile(vClients);

		printf("\nClient Deleted Successfully\n\n");
		return true;
	}
	
}

string ReadAccountNumber() {

	string AccountNumber;
	printf("Please , Enter An Account Number ?  | ");
	getline(cin >> ws, AccountNumber);
	return AccountNumber;
}

void ShowDeleteClientScreen() {

	if (CheckAccessPermission(enMainMenuPermissions::pDeleteClient)) {

		ShowAccessDeniedMessage();
		BackToMainMenu();
	}


	system("cls");
	printf("--------------------------------------------\n");
	printf("            Delete Client Screen\n");
	printf("--------------------------------------------\n\n");

	vector <strClient> vClients = LoadClientData_FromFileToVector();
	string AccountNumber = ReadAccountNumber();
	DeleteClient(vClients,AccountNumber);
}

bool UpdateClient(vector <strClient> vClients , string AccountNumber) {

	if (!IsClientExistByAccountNumber(AccountNumber, CurrentClient)) {

		printf("\nClient With [%s] Account Number is not Exist.\n\n", AccountNumber.c_str());
		return false;
	}

	PrintClientCardInfo(CurrentClient);
	char SureToPerform = false;
	SureToPerform = legend::Strings::ReadChar("Are You Sure To Perform Updating This Client ?  [Y / N] |  ");

	if (toupper(SureToPerform) == 'Y') {

		for (strClient & C : vClients) {

			if (C.AccountNumber == AccountNumber) {

				C = ChangeClientRecord(AccountNumber);
				break;
			}
		}

		printf("\nClient Updated Successfully \n\n");
		LoadClientsToFile(vClients);

		return true;
	}
}

void ShowUpdateClientScreen() {

	if (CheckAccessPermission(enMainMenuPermissions::pUpdateClient)) {

		ShowAccessDeniedMessage();
		BackToMainMenu();
	}


	system("cls");
	printf("--------------------------------------------\n");
	printf("            Update Client Screen\n");
	printf("--------------------------------------------\n\n");

	vector <strClient> vClients = LoadClientData_FromFileToVector();
	string AccountNumber = ReadAccountNumber();
	UpdateClient(vClients, AccountNumber);
}

bool FindClient(string AccountNumber) {

	if (!IsClientExistByAccountNumber(AccountNumber,CurrentClient)) {
		
		printf("\nClient With [%s] Account Number is not Exist.\n\n", AccountNumber.c_str());
		return false;
	}

	PrintClientCardInfo(CurrentClient);

	return 0;
}

void ShowFindClientScreen() {

	if (CheckAccessPermission(enMainMenuPermissions::pFindClient)) {

		ShowAccessDeniedMessage();
		BackToMainMenu();
	}


	system("cls");
	printf("--------------------------------------------\n");
	printf("             Find Client Screen\n");
	printf("--------------------------------------------\n\n");

	string AccountNumber = ReadAccountNumber();
	FindClient(AccountNumber);
}

void DepositBalanceToClientByAccountNumber(string AccountNumber, double DepositAmount) {

	vector <strClient> vClients = LoadClientData_FromFileToVector();

	char SureToPerform = legend::Strings::ReadChar("\nAre You Sure To Perform This Transaction [Y / N] ?  |  ");

	if (toupper(SureToPerform) == 'Y') {

		for (strClient& C : vClients) {

			if (C.AccountNumber == AccountNumber) {

				C.Balance += DepositAmount;
				printf("\nDone Successfully, New Balance is: { %4.f }\n\n", C.Balance);
				break;
			}
		}

		LoadClientsToFile(vClients);
	}
}

void ShowDepositScreen() {

	system("cls");
	printf("--------------------------------------------\n");
	printf("             Deposit Screen\n");
	printf("--------------------------------------------\n\n");

	string AccountNumber = ReadAccountNumber();

	while (!IsClientExistByAccountNumber(AccountNumber, CurrentClient)) {

		cout << "Client With Account Number [" << AccountNumber << "] is Not Found !\n\n";
		AccountNumber = ReadAccountNumber();
	}

	PrintClientCardInfo(CurrentClient);

	double AmountOfDeposit = legend::ReadPositiveNumbers::ReadPositiveNumber_float_Ver("Please , Enter Deposit Amount ? |  ");

	DepositBalanceToClientByAccountNumber(AccountNumber, AmountOfDeposit);
}

void ShowWithdrawScreen() {

	system("cls");
	printf("--------------------------------------------\n");
	printf("            Withdraw Screen\n");
	printf("--------------------------------------------\n\n");

	string AccountNumber = ReadAccountNumber();

	while (!IsClientExistByAccountNumber(AccountNumber, CurrentClient)) {

		cout << "Client With Account Number [" << AccountNumber << "] is Not Found !\n\n";
		AccountNumber = ReadAccountNumber();
	}

	PrintClientCardInfo(CurrentClient);
	double AmountOfWithdraw = legend::ReadPositiveNumbers::ReadPositiveNumber_float_Ver("Please , Enter Deposit Amount ? |  ");

	while (AmountOfWithdraw > CurrentClient.Balance)
	{
		cout << "Amount Exceed The Balance , You Can Withdraw UP TO: { " << CurrentClient.Balance << " }\n";
		AmountOfWithdraw = legend::ReadPositiveNumbers::ReadPositiveNumber_float_Ver("Please , Enter Deposit Amount ? |  ");
	}
	DepositBalanceToClientByAccountNumber(AccountNumber, AmountOfWithdraw * -1);
}

void PrintClientRecordLineOfTotalBalances(strClient& Client) {

	cout << "|  " << left << setw(18) << Client.AccountNumber;
	cout << "|  " << setw(44) << Client.FullName;
	cout << "|  " << setw(22) << Client.Balance;

}

void ShowTotalBalancesScreen() {

	vector <strClient> vClients = LoadClientData_FromFileToVector();

	system("cls");

	printf("\n\t\t\t\t\t Client List (%d)  Client(s). \n", vClients.size());

	printf("\n_____________________________________________________________________________________________________________\n\n");

	cout << "|  " << left << setw(18) << "Account Number";
	cout << "|  " << setw(44) << "Client Name";
	cout << "|  " << setw(22) << "Balance";

	printf("\n_____________________________________________________________________________________________________________\n\n");

	double TotalBalances = 0;

	if (vClients.size() == 0) {

		printf("\t\t\t\t No  Clients  Available  In  The  System !! \n");
	}
	else {

		for (strClient& C : vClients) {

			PrintClientRecordLineOfTotalBalances(C);
			cout << endl;

			TotalBalances += C.Balance;
		}
	}

	printf("_____________________________________________________________________________________________________________\n\n");

	cout << "\t\t\t\t Total Balances = { " << TotalBalances << " } \n\n";
}


void BackToTransactionsMenuScreen() {

	printf("Press Any Key To go Back To Transactions Menu...");
	system("pause>0");

	TransactionsMenuScreen();
}

void PerformTransactionsMenuOptions(enPerformTransactionMenuOptions TransactionsMenuOptions) {

	switch (TransactionsMenuOptions)
	{
	case eDeposit:
		ShowDepositScreen();
		BackToTransactionsMenuScreen();
		break;

	case eWithdraw:
		ShowWithdrawScreen();
		BackToTransactionsMenuScreen();
		break;
	
	case eTotal_Balances:
		ShowTotalBalancesScreen();
		BackToTransactionsMenuScreen();
		break;

	case eBackToMainMenu:
		MainMenuScreen();
		break;

	}
}

enPerformTransactionMenuOptions ReadTransactionMenuOption() {

	return (enPerformTransactionMenuOptions)legend::ReadNumber_InRange::ReadNumber_InRange_short_Ver("Choose What Do You Want To Do [ 1 to 4 ] ? |  ",1,4);
}

void TransactionsMenuScreen() {

	if (CheckAccessPermission(enMainMenuPermissions::pTransactionsMenu)) {

		ShowAccessDeniedMessage();
		BackToMainMenu();
	}


	system("cls");
	printf("================================================\n");
	printf("             Transactions Menu Screen           \n");
	printf("================================================\n");
	printf("       [1] - Deposit. \n");
	printf("       [2] - Withdraw. \n");
	printf("       [3] - Total Balances. \n");
	printf("       [4] - Main Menu. \n");
	printf("================================================\n");

	PerformTransactionsMenuOptions(ReadTransactionMenuOption());
}



void MainMenuScreen() {

	system("cls");

	printf("================================================\n");
	printf("                   Main Menu Screen             \n");
	printf("================================================\n");
	printf("       [1] - Show Clients List. \n");
	printf("       [2] - Add New Client. \n");
	printf("       [3] - Delete Client. \n");
	printf("       [4] - Update Client Info. \n");
	printf("       [5] - Find Client. \n");
	printf("       [6] - Transactions. \n");
	printf("       [7] - Manage Users. \n");
	printf("       [8] - Logout. \n");
	printf("================================================\n");

	PerformMainMenuOptions(ReadMainMenuOption());
}


enPerformManageUsersMenuScreen ReadManageUsersMenuOptions() {

	return (enPerformManageUsersMenuScreen)legend::ReadNumber_InRange::ReadNumber_InRange_short_Ver("Choose What Do You Want To Do [ 1 to 6 ] ? |  ", 1, 6);
}

void PrintUserRecordLine(strUser& User) {

	cout << "|  " << setw(20) << User.UserName;
	cout << "|  " << left << setw(20) << User.Password;
	cout << "|  " << setw(11) << User.Permissions;

}
void ShowUsersList() {

	vector <strUser> vUsers = LoadUserDate_FromFileToVector();

	system("cls");

	printf("\n\t\t\t\t\t User List (%d)  Users(s). \n", vUsers.size());

	printf("\n_____________________________________________________________________________________________________________\n\n");

	cout << "|  " << left << setw(20) << "User Name";
	cout << "|  " << setw(20) << "Password";
	cout << "|  " << setw(11) << "Permissions";

	printf("\n_____________________________________________________________________________________________________________\n\n");

	if (vUsers.size() == 0) {

		printf("\t\t\t\t No  Users  Available  In  The  System !! \n");
	}
	else {

		for (strUser& U : vUsers) {

			PrintUserRecordLine(U);
			cout << endl;
		}
	}

	printf("_____________________________________________________________________________________________________________\n\n");

}

void BackToManageUsersMenu() {

	printf("Press Any Key To go Back To Manage Users Menu...");
	system("pause>0");

	ManageUsersMenuScreen();
}

bool IsUserExistByUsername(string Username , strUser &User) {

	vector <strUser> vUsers = LoadUserDate_FromFileToVector();

	for (strUser& U : vUsers) {

		if (U.UserName == Username) {
			
			User = U;
			return true;
		}
	}

	return false;
}

string ReadUsername() {

	string Username;
	cout << "Enter, Username ?  | ";
	getline(cin >> ws, Username);

	return Username;
}


int ReadPermissionsToSet() {

	int Permissions = 0;

	char GiveFullAccess = legend::Strings::ReadChar("\nDo You Want To Give Full Access [Y / N] ?  | ");

	if (toupper(GiveFullAccess) == 'Y') {

		return enMainMenuPermissions::pAll;
	}

	else {

		cout << "Do You Want To Give Access To :\n\n";

		char GiveAccessTo;

		if (toupper(GiveAccessTo = legend::Strings::ReadChar("Show Clients List ?  | ")) == 'Y')
			Permissions += enMainMenuPermissions::pListClients;

		if (toupper(GiveAccessTo = legend::Strings::ReadChar("Add New Client ?  | ")) == 'Y')
			Permissions += enMainMenuPermissions::pAddNewClient;

		if (toupper(GiveAccessTo = legend::Strings::ReadChar("Delete Client ?  | ")) == 'Y')
			Permissions += enMainMenuPermissions::pDeleteClient;

		if (toupper(GiveAccessTo = legend::Strings::ReadChar("Update Client ?  | ")) == 'Y')
			Permissions += enMainMenuPermissions::pUpdateClient;

		if (toupper(GiveAccessTo = legend::Strings::ReadChar("Find Client ?  | ")) == 'Y')
			Permissions += enMainMenuPermissions::pFindClient;

		if (toupper(GiveAccessTo = legend::Strings::ReadChar("Transactions ?  | ")) == 'Y')
			Permissions += enMainMenuPermissions::pTransactionsMenu;

		if (toupper(GiveAccessTo = legend::Strings::ReadChar("Manage Users ?  | ")) == 'Y')
			Permissions += enMainMenuPermissions::pManageUsers;
	
	
		return Permissions;
	
	}
}

strUser ReadUser() {

	strUser User;

	cout << "Enter, Username ?  | ";
	getline(cin >> ws, User.UserName);

	while (IsUserExistByUsername(User.UserName, CurrentUser)) {

		printf("\nUser With [%s] already exists, Enter another Username ?  | ", User.UserName.c_str());
		getline(cin >> ws, User.UserName);
	}

    User.Password = legend::Strings::ReadString("Enter Password ?  | ");

	User.Permissions = ReadPermissionsToSet();

	return User;
}

void AddUser() {

	strUser User = ReadUser();
	legend::Files::AddDataLineToFile(UserFileName, ConvertUserRecordToLine(User));
}

void AddUsers() {

	char AddMoreUsers;
	do {

		AddUser();
		AddMoreUsers = legend::Strings::ReadChar("User Added Successfully, Do You Want To Add More ? |  ");
		cout << "\n\n";

	} while (toupper(AddMoreUsers) == 'Y');
}

void ShowAddUserScreen() {

	system("cls");
	printf("--------------------------------------------\n");
	printf("            Add User Screen\n");
	printf("--------------------------------------------\n");
	printf("Adding New User:\n\n");

	AddUsers();
}

void PrintUserCardInfo(strUser User) {

	cout << "\nThe Following Are The User Details:\n";
	cout << "---------------------------------------\n";
	cout << "Username    : " << User.UserName << endl;
	cout << "Password    : " << User.Password << endl;
	cout << "Permissions : " << User.Permissions << endl;
	cout << "---------------------------------------\n\n";
}

bool ShowFindUserScreen() {

	system("cls");
	printf("--------------------------------------------\n");
	printf("               Find User Screen\n");
	printf("--------------------------------------------\n\n");

	string Username = ReadUsername();

	if (!IsUserExistByUsername(Username, CurrentUser)) {

		printf("User With [%s] Username is Not Exist\n\n",Username.c_str());
		return false;
	}

	PrintUserCardInfo(CurrentUser);
	return true;
}

bool MarkUserForDeleteByUsername(vector <strUser> &vUsers, string Username) {

	for (strUser& U : vUsers) {

		if (U.UserName == Username) {

			U.MarkToDelete = true;
			return true;
		}
	}

	return false;
}

void LoadUsersToFile(vector <strUser> vUsers) {

	fstream MyFile;
	MyFile.open(UserFileName, ios::out);

	if (MyFile.is_open()) {

		string DataLine;

		for (strUser& U : vUsers) {

			if (U.MarkToDelete == false) {

				MyFile << ConvertUserRecordToLine(U) << endl;
			}
		}

		MyFile.close();
	}
}

bool DeleteUser(vector <strUser> &vUsers,string Username) {

	if (Username == "Admin") {

		printf("You Cannot Delete This User.\n\n", Username.c_str());
		return false;
	}

	if (!IsUserExistByUsername(Username, CurrentUser)) {

		printf("User With [%s] Username is Not Exist\n\n", Username.c_str());
		return false;
	}

	PrintUserCardInfo(CurrentUser);

	char SureToDelete = legend::Strings::ReadChar("Are You Sure To Delete This User [Y / N] ?  | ");

	if (toupper(SureToDelete) == 'Y') {

		MarkUserForDeleteByUsername(vUsers, Username);
		LoadUsersToFile(vUsers);

		printf("User Deleted Successfully.\n");
	}
}

void ShowDeleteUserScreen() {

	system("cls");
	printf("--------------------------------------------\n");
	printf("            Delete User Screen\n");
	printf("--------------------------------------------\n\n");
	
	vector <strUser> vUsers = LoadUserDate_FromFileToVector();
	string Username = ReadUsername();

	DeleteUser(vUsers,Username);
}

strUser ChangeUserRecord(string Username) {

	strUser User;

	User.UserName = Username;

	cout << "Enter Password ?  | ";
	getline(cin >> ws, User.Password);

	User.Permissions = ReadPermissionsToSet();

	return User;
}

bool UpdateUser(vector <strUser>& vUsers, string Username) {

	if (!IsUserExistByUsername(Username, CurrentUser)) {

		printf("User With [%s] Username is Not Exist\n\n", Username.c_str());
		return false;
	}

	PrintUserCardInfo(CurrentUser);

	char SureToUpdate = legend::Strings::ReadChar("Are You Sure To Update This User [Y / N] ?  | ");

	if (toupper(SureToUpdate) == 'Y') {

		for (strUser& U : vUsers) {

			if (U.UserName == Username) {

				U = ChangeUserRecord(Username);
				break;
			}
		}

		LoadUsersToFile(vUsers);
		cout << "User Updated Successfully.\n";
	}

	return true;
}

void ShowUpdateUserScreen() {

	system("cls");
	printf("--------------------------------------------\n");
	printf("            Delete User Screen\n");
	printf("--------------------------------------------\n\n");

	vector <strUser> vUsers = LoadUserDate_FromFileToVector();
	string Username = ReadUsername();

	UpdateUser(vUsers, Username);
}


void PerformManageUsersMenuOptions(enPerformManageUsersMenuScreen ManageUsersMenuOption) {

	switch (ManageUsersMenuOption)
	{
	case eListUsers:
		ShowUsersList();
		BackToManageUsersMenu();
		break;

	case eAddNewUser:
		ShowAddUserScreen();
		BackToManageUsersMenu();
		break;

	case eDeleteUser:
		ShowDeleteUserScreen();
		BackToManageUsersMenu();
		break;

	case eUpdateUser:
		ShowUpdateUserScreen();
		BackToManageUsersMenu();
		break;

	case eFindUser:
		ShowFindUserScreen();
		BackToManageUsersMenu();
		break;

	case eeBackToMainMenu:
		MainMenuScreen();
		break;

	}
}

void ManageUsersMenuScreen() {

	if (CheckAccessPermission(enMainMenuPermissions::pManageUsers)) {

		ShowAccessDeniedMessage();
		BackToMainMenu();
	}


	system("cls");
	printf("================================================\n");
	printf("            Manage Users Menu Screen            \n");
	printf("================================================\n");
	printf("       [1] - List Users. \n");
	printf("       [2] - Add New User. \n");
	printf("       [3] - Delete User. \n");
	printf("       [4] - Update User. \n");
	printf("       [5] - Find User. \n");
	printf("       [6] - Main Menu. \n");
	printf("================================================\n");

	PerformManageUsersMenuOptions(ReadManageUsersMenuOptions());
}


void Login() {

	string UserName, Password;
	bool LoginFailed = false;

	do {

		system("cls");

		LoginScreen();

		if (LoginFailed) {

			printf("Invalid Username | Password !!!\n");
		}

		cout << "Enter Username ?  | ";
		getline(cin >> ws, UserName);

		Password = legend::Strings::ReadString("Enter Password ?  | ");

		LoginFailed = !LoadUserInfo(UserName, Password);

	} while (LoginFailed);


	MainMenuScreen();
}

void Logout() {

	Login();
}

void PerformMainMenuOptions(enPerformMainMenuOptions MainMenuOption) {

	switch (MainMenuOption)
	{

	case eShowClientsList:
		ClientsList();
		BackToMainMenu();
		break;

	case eAddNewClient:
		ShowAddNewClientsScreen();
		BackToMainMenu();
		break;

	case eDeleteClient:
		ShowDeleteClientScreen();
		BackToMainMenu();
		break;

	case eUpdateClientInfo:
		ShowUpdateClientScreen();
		BackToMainMenu();
		break;

	case eFindClient:
		ShowFindClientScreen();
		BackToMainMenu();
		break;

	case eTransactionsMenu:
		TransactionsMenuScreen();
		break;

	case eManageUsers:
		ManageUsersMenuScreen();
		break;

	case eLogout:
		Logout();
		break;

	}
}


void main() {

	Login();
    system("pause>0");

}

