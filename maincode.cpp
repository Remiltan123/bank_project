#include <iostream>
#include <conio.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <time.h>
#include <fstream>
#include <sstream>

using namespace std;

class Administrator;
class Employee;
class Customer;
class Transaction;

class Bank
{
public:
    vector < Employee > employeeArray;
    vector < string > employeeNameArray;
    vector < Customer > customerArray;
    vector < string > customerNameArray;
    vector <Transaction> transactions;
    long bankBalance = 1000000;
};

class Administrator
{
private:
    string dateToday;
    float annualSavingInterest = 20;
    float overdraftCharge = 2;
    int dateIncremented = 0;

public:
    void create_Employee(Bank& bank);
    void set_Date();
    void increase_Date();
    void set_annualSavingInterest();
    void set_overdraftCharge();
    void update_Customer_Overdraft(Customer & customer);
    void update_Customer_Interest(Customer & customer,Bank& bank,ofstream& BankFile);
    string get_Employee_Name(Bank& bank);
    string get_Date();
    float get_annualSavingInterest();
    float get_overdraftCharge();
    int employee_Number(Bank& bank);
    vector<string> get_Employee_Name_Array(Bank& bank);
};

class Employee
{
private:
    string employeeName;
    string dateToday;
public:
    void set_Date(Administrator & Admin);
    void set_Employee_Name(string employeeName);
    string get_Employee_Name();
    void create_Customer(Customer & customerCreated,Bank& bank,ofstream& MyFile);
    void deposit_Money(Customer & customer,Bank& bank,ofstream& MyFile,ofstream& BankFile);
    void withdraw_Money(Customer & customer,Bank& bank,ofstream& MyFile,ofstream& BankFile);
    void view_Transactions(Bank& bank);
    void close_Customer_Account(Customer & customer,ofstream& MyFile);
};

class Customer
{
public:
    string CustomerName;
    string accountType;
    string customerContact;
    long moneyDeposit;
    bool isCurrentAccount = false;
    long overdraftMaximum = 0;
    bool isClosed = false;
    long overDraftNow = 0;
    string dateToday;
    string dateCreated;
    vector <Transaction> transactions;

    void view_Transaction();
    void deposit_Money_Function(Bank& bank,ofstream& MyFile,ofstream& BankFile);
    void set_Date(Administrator& Admin);
    void withdraw_Money(Bank& bank,ofstream& MyFile,ofstream& BankFile);
};

class Transaction
{
private:
    string dateToday;
    string description;
public:

    void setTransactionDate(string dateToday)
    {
        this->dateToday=dateToday;
    }

    void setDescription(string customer_name,string debitOrCredit,long amount)
    {
        this->description=this->dateToday+" - "+customer_name+" : "+debitOrCredit+" : "+to_string(amount)+" Rs.";
    }
    string getDescription()
    {
        return this->description;
    }

};

int Administrator::employee_Number(Bank& bank)
{
    return bank.employeeArray.size();
};

vector<string> Administrator::get_Employee_Name_Array(Bank& bank)
{
    return bank.employeeNameArray;
};

void Administrator::create_Employee(Bank& bank)
{
    string startLetter="";
    if(bank.employeeArray.size()<9)
    {
        startLetter="00";
    }
    else if(bank.employeeArray.size()<99)
    {
        startLetter="0";
    }
    string employeeName=("EMPLOYEE" +startLetter+to_string((bank.employeeArray.size()) + 1));
    Employee employee;
    employee.set_Employee_Name(employeeName);
    bank.employeeArray.push_back(employee);
    bank.employeeNameArray.push_back(employeeName);
    Employee lastCreatedEmployee=bank.employeeArray.back();
    cout<<lastCreatedEmployee.get_Employee_Name()+" : EMPLOYEE IS CREATED."<<endl;;
};

void Administrator::set_Date()
{
    string dateToday;
    time_t ttime = time(0);
    tm * local_time = localtime( & ttime);
    dateToday = to_string(1900 + local_time -> tm_year) + ":" + to_string(1 + local_time -> tm_mon) + ":" + to_string(local_time -> tm_mday);
    this -> dateToday = dateToday;
};

void Administrator::increase_Date()
{
    this->dateIncremented+=1;
    time_t ttime = time(0);
    ttime += (24 * 60 * 60)*(this->dateIncremented);
    tm * local_time = localtime( & ttime);
    this->dateToday = to_string(1900 + local_time -> tm_year) + ":" + to_string(1+local_time -> tm_mon) + ":" + to_string(local_time -> tm_mday);
};

void Administrator::set_annualSavingInterest()
{
    float annualSavingInterest;
    cout << "ENTER THE NEW ANNUAL SAVING INTEREST : ";
    cin >> annualSavingInterest;
    this -> annualSavingInterest = annualSavingInterest;
    cout<<"NEW ANNUAL SAVINGS INTEREST IS "<< annualSavingInterest<<endl;
};

void Administrator::set_overdraftCharge()
{
    float overdraftCharge;
    cout << "ENTER THE NEW OVERDRAFT CHARGE(% PER DAY) : ";
    cin >> overdraftCharge;
    this -> overdraftCharge = overdraftCharge;
    cout<<"NEW OVERDRAFT CHARGE IS "<< overdraftCharge<<endl;
};

void Administrator::update_Customer_Overdraft(Customer & customer)
{
    if (customer.overDraftNow > 0)
    {
        customer.overDraftNow += ((this -> overdraftCharge) / 100) * (customer.overDraftNow);
        customer.moneyDeposit=(customer.overDraftNow)*(-1);

    }
}

void Administrator::update_Customer_Interest(Customer & customer,Bank& bank,ofstream& BankFile)
{
    float dailyInterest = (this -> annualSavingInterest) / 365;
    if (customer.moneyDeposit > 0)
    {
        cout<<"LINE-1"<<endl;
        customer.moneyDeposit += ((dailyInterest) / 100) * (customer.moneyDeposit);
        bank.bankBalance-=((dailyInterest) / 100) * (customer.moneyDeposit);
        BankFile<<this->dateToday<<" - "<<customer.CustomerName<<" : INTEREST DEBITED TO BANK ACCOUNT : "<<to_string(((dailyInterest) / 100) * (customer.moneyDeposit))<<" Rs."<<endl;
        BankFile<<this->dateToday<<" - "<<"BANK BALANCE : "<<bank.bankBalance<<endl;
        Transaction transaction_now;
        transaction_now.setTransactionDate(this->dateToday);
        transaction_now.setDescription(customer.CustomerName,"INTEREST CREDITED TO CUSTOMER ACCOUNT",((dailyInterest) / 100) * (customer.moneyDeposit));
        bank.transactions.push_back(transaction_now);
        customer.transactions.push_back(transaction_now);

    }
}

string Administrator::get_Employee_Name(Bank& bank)
{
    return bank.employeeArray.back().get_Employee_Name();
}

string Administrator::get_Date()
{
    return this->dateToday;
}

float Administrator::get_annualSavingInterest()
{
    return (this->annualSavingInterest);
};

float Administrator::get_overdraftCharge()
{
    return (this->overdraftCharge);
};

void Customer::view_Transaction()
{
    int numberOfTransactions=this->transactions.size();
    for(int i=0; i<numberOfTransactions; i++)
    {
        cout<<this->transactions[i].getDescription()<<endl;
    }
};

void Customer::deposit_Money_Function(Bank& bank,ofstream& MyFile,ofstream& BankFile)
{
    int moneyAdded;
    cout << "ENTER THE AMOUNT OF MONEY TO DEPOSIT: ";
    cin >> moneyAdded;
    this -> moneyDeposit += moneyAdded;
    MyFile<<this->dateToday<<" - "<<this->CustomerName<<" : DEPOSITED "<<to_string(moneyAdded)<<endl;
    Transaction transaction_now_1;
    transaction_now_1.setTransactionDate(this->dateToday);
    transaction_now_1.setDescription(this->CustomerName,"MONEY CREDITED TO CUSTOMER ACCOUNT",moneyAdded);
    this->transactions.push_back(transaction_now_1);

    if (this -> moneyDeposit >= 0)
    {
        bank.bankBalance+=overDraftNow;
        BankFile<<this->dateToday<<" - "<<this->CustomerName<<" : OVERDRAFT CREDITED TO BANK ACCOUNT : "<<to_string(overDraftNow)<<" Rs."<<endl;
        BankFile<<this->dateToday<<" - "<<"BANK BALANCE : "<<bank.bankBalance<<endl;
        Transaction transaction_now_2;
        transaction_now_2.setTransactionDate(this->dateToday);
        transaction_now_2.setDescription(this->CustomerName,"OVERDRAFT CREDITED TO BANK ACCOUNT",overDraftNow);
        bank.transactions.push_back(transaction_now_2);
        this -> overDraftNow = 0;
    }
    else
    {
        bank.bankBalance+=moneyAdded;
        BankFile<<this->dateToday<<" - "<<this->CustomerName<<" : OVERDRAFT CREDITED TO BANK ACCOUNT : "<<to_string(moneyAdded)<<" Rs."<<endl;
        BankFile<<this->dateToday<<" - "<<"BANK BALANCE : "<<bank.bankBalance<<endl;
        this -> overDraftNow -= moneyAdded;
        Transaction transaction_now_2;
        transaction_now_2.setTransactionDate(this->dateToday);
        transaction_now_2.setDescription(this->CustomerName,"OVERDRAFT CREDITED TO BANK ACCOUNT",moneyAdded);
        bank.transactions.push_back(transaction_now_2);
    }
    for(int i=0; i<=bank.customerNameArray.size(); i++)
    {
        if(this->CustomerName==bank.customerNameArray[i])
        {
            bank.customerArray[i].moneyDeposit=this->moneyDeposit;
            bank.customerArray[i].overDraftNow=this->overDraftNow;
        }
    }
}

void Customer::set_Date(Administrator & Admin)
{
    this -> dateToday = Admin.get_Date();
};

void Customer::withdraw_Money(Bank& bank,ofstream& MyFile,ofstream& BankFile)
{
    int moneyToWithdraw;
    cout << "ENTER THE AMOUNT OF MONEY TO WITHDRAW: ";
    cin >> moneyToWithdraw;
    if (this -> isCurrentAccount == false && moneyToWithdraw > this -> moneyDeposit)
    {
        cout << "SORRY INSUFFICIENT BALANCE" << endl;
    }
    else if (this -> isCurrentAccount == false && moneyToWithdraw <= this -> moneyDeposit)
    {
        this -> moneyDeposit -= moneyToWithdraw;
        cout << "SUCESSFULL WITHDRAWAL" << endl;
        MyFile<<this->dateToday<<" - "<<this->CustomerName<<" : WITHDRAWN "<<to_string(moneyToWithdraw)<<endl;
        Transaction transaction_now_1;
        transaction_now_1.setTransactionDate(this->dateToday);
        transaction_now_1.setDescription(this->CustomerName,"MONEY WITHDRAWN FROM CUSTOMER ACCOUNT",moneyToWithdraw);
        this->transactions.push_back(transaction_now_1);
    }
    else if (this -> isCurrentAccount == true && moneyToWithdraw > this -> moneyDeposit)
    {
        if (((this -> moneyDeposit - moneyToWithdraw) * (-1)) <= this -> overdraftMaximum && ((this -> moneyDeposit - moneyToWithdraw) * (-1)) <= bank.bankBalance )
        {
            this -> overDraftNow = moneyToWithdraw - (this -> moneyDeposit);
            bank.bankBalance-=this->overDraftNow;
            this -> moneyDeposit -= moneyToWithdraw;
            cout << "SUCESSFULL WITHDRAWAL" << endl;
            BankFile<<this->dateToday<<" - "<<this->CustomerName<<" : OVERDRAFT DEBITED TO BANK ACCOUNT : "<<to_string(this->overDraftNow)<<" Rs."<<endl;
            BankFile<<this->dateToday<<" - "<<"BANK BALANCE : "<<bank.bankBalance<<endl;
            MyFile<<this->dateToday<<" - "<<this->CustomerName<<" : WITHDRAWN "<<to_string(moneyToWithdraw)<<endl;
            Transaction transaction_now_2;
            Transaction transaction_now_3;
            transaction_now_2.setTransactionDate(this->dateToday);
            transaction_now_3.setTransactionDate(this->dateToday);
            transaction_now_2.setDescription(this->CustomerName,"MONEY WITHDRAWN FROM CUSTOMER ACCOUNT",moneyToWithdraw);
            transaction_now_3.setDescription(this->CustomerName,"OVERDRAFT DEBITED TO BANK ACCOUNT",this->overDraftNow);
            this->transactions.push_back(transaction_now_2);
            bank.transactions.push_back(transaction_now_3);
        }
        else if(((this -> moneyDeposit - moneyToWithdraw) * (-1)) <= this -> overdraftMaximum && ((this -> moneyDeposit - moneyToWithdraw) * (-1)) > bank.bankBalance)
        {
            cout << "CAN'T WITHDRAW AMOUNT GREATER THAN BALANCE IN THE BANK.";
        }
        else
        {
            cout << "CAN'T WITHDRAW AMOUNT GREATER THAN MAXIMUM OVERDRAFT.";
        }
    }
    else if (this -> isCurrentAccount == true && moneyToWithdraw <= this -> moneyDeposit)
    {
        this -> moneyDeposit -= moneyToWithdraw;
        cout << "SUCESSFULL WITHDRAWAL" << endl;
        MyFile<<this->dateToday<<" - "<<this->CustomerName<<" : WITHDRAWN "<<to_string(moneyToWithdraw)<<endl;
        Transaction transaction_now_4;
        transaction_now_4.setTransactionDate(this->dateToday);
        transaction_now_4.setDescription(this->CustomerName,"MONEY WITHDRAWN FROM CUSTOMER ACCOUNT",moneyToWithdraw);
        this->transactions.push_back(transaction_now_4);
    }
    for(int i=0; i<=bank.customerNameArray.size(); i++)
    {
        if(this->CustomerName==bank.customerNameArray[i])
        {
            bank.customerArray[i].moneyDeposit=this->moneyDeposit;
            bank.customerArray[i].overDraftNow=this->overDraftNow;
        }
    }

};

void Employee::set_Date(Administrator & Admin)
{
    this -> dateToday = Admin.get_Date();
};

void Employee::set_Employee_Name(string employeeName)
{
    this->employeeName = employeeName;
};


void Employee::create_Customer(Customer & customerCreated,Bank& bank,ofstream& MyFile)
{
    string startLetter="";
    if(bank.customerArray.size()<9)
    {
        startLetter="00";
    }
    else if(bank.customerArray.size()<99)
    {
        startLetter="0";
    }
    string customer_contact;
    int account_type;
    customerCreated.dateCreated = dateToday;
    string customerName= "CUSTOMER" +startLetter+to_string((bank.customerArray.size()) + 1);
    cout << "CUSTOMER NAME : " << "CUSTOMER" +startLetter+to_string((bank.customerArray.size()) + 1)<<endl;
    cout << "ENTER MOBILE NUMBER : ";
    cin >> customer_contact;
    customerCreated.CustomerName=customerName;
    customerCreated.customerContact = customer_contact;
    cout << "SELECT THE ACCOUNT TYPE : " << endl;
    cout << "1. CURRENT ACCOUNT" << endl;
    cout << "2. SAVING ACCOUNT" << endl;
    cin >> account_type;

    while (account_type < 0 || account_type > 3)
    {
        cout << "INVALID INPUT!";
        cout << "SELECT THE ACCOUNT TYPE : " << endl;
        cout << "1. CURRENT ACCOUNT" << endl;
        cout << "2. SAVING ACCOUNT" << endl;
        cin >> account_type;
    }

    if (account_type == 1)
    {
        customerCreated.accountType = "CURRENT";
        customerCreated.isCurrentAccount = true;
        cout << "ENTER THE OVERDRAFT LIMIT : ";
        cin >> customerCreated.overdraftMaximum;
    }
    else if (account_type == 2)
    {
        customerCreated.accountType = "SAVING";
    }

    long moneyDepositNow;
    cout << "ENTER THE INITIAL AMOUNT DEPOSITED : ";
    cin>>moneyDepositNow;
    customerCreated.moneyDeposit=moneyDepositNow;
    bank.customerNameArray.push_back(customerName);
    bank.customerArray.push_back(customerCreated);
    MyFile<<customerCreated.dateToday<<" - "<<customerCreated.CustomerName<<" : ACCOUNT CREATED."<<endl;
    MyFile<<customerCreated.dateToday<<" - "<<customerCreated.CustomerName<<" : DEPOSITED "<<moneyDepositNow<<endl;
    Transaction transaction_now_1;
    transaction_now_1.setTransactionDate(customerCreated.dateToday);
    transaction_now_1.setDescription(customerCreated.CustomerName,"MONEY CREDITED TO CUSTOMER ACCOUNT",moneyDepositNow);
    customerCreated.transactions.push_back(transaction_now_1);

};

void Employee::deposit_Money(Customer& customer,Bank& bank,ofstream& MyFile,ofstream& BankFile)
{
    if (customer.isClosed == false)
    {
        long account_balance = customer.moneyDeposit;
        long now_deposited;
        cout << "ENTER THE AMOUNT TO BE DEPOSITED: ";
        cin >> now_deposited;
        customer.moneyDeposit = account_balance + now_deposited;
        MyFile<<customer.dateToday<<" - "<<customer.CustomerName<<" : DEPOSITED "<<now_deposited<<endl;
        Transaction transaction_now_1;
        transaction_now_1.setTransactionDate(customer.dateToday);
        transaction_now_1.setDescription(customer.CustomerName,"MONEY CREDITED TO CUSTOMER ACCOUNT",now_deposited);
        customer.transactions.push_back(transaction_now_1);

        if (customer.moneyDeposit >= 0)
        {
            bank.bankBalance+=customer.overDraftNow;
            BankFile<<customer.dateToday<<" - "<<customer.CustomerName<<" : OVERDRAFT CREDITED TO BANK ACCOUNT : "<<to_string(customer.overDraftNow)<<" Rs."<<endl;
            BankFile<<customer.dateToday<<" - "<<"BANK BALANCE : "<<bank.bankBalance<<endl;
            Transaction transaction_now_2;
            transaction_now_2.setTransactionDate(customer.dateToday);
            transaction_now_2.setDescription(customer.CustomerName,"OVERDRAFT CREDITED TO BANK ACCOUNT",customer.overDraftNow);
            bank.transactions.push_back(transaction_now_2);
            customer.overDraftNow = 0;

        }
        else
        {
            bank.bankBalance+=now_deposited;
            customer.overDraftNow -= now_deposited;
            BankFile<<customer.dateToday<<" - "<<customer.CustomerName<<" : OVERDRAFT CREDITED TO BANK ACCOUNT : "<<to_string(now_deposited)<<" Rs."<<endl;
            BankFile<<customer.dateToday<<" - "<<"BANK BALANCE : "<<bank.bankBalance<<endl;
            Transaction transaction_now_2;
            transaction_now_2.setTransactionDate(customer.dateToday);
            transaction_now_2.setDescription(customer.CustomerName,"OVERDRAFT CREDITED TO BANK ACCOUNT",now_deposited);
            bank.transactions.push_back(transaction_now_2);
        }
    }
    else
    {
        cout << "ACCOUNT IS ALREADY CLOSED" << endl;
    }
};

void Employee::withdraw_Money(Customer& customer,Bank& bank,ofstream& MyFile,ofstream& BankFile)
{
    if (customer.isClosed == false)
    {
        long now_withdraw;
        cout << "ENTER THE AMOUNT TO BE WITHDRAWN: ";
        cin >> now_withdraw;

        if (customer.isCurrentAccount == false && now_withdraw > customer.moneyDeposit)
        {
            cout << "INSUFFICIENT BALANCE" << endl;
        }
        else if (customer.isCurrentAccount == false && now_withdraw <= customer.moneyDeposit)
        {
            customer.moneyDeposit -= now_withdraw;
            cout << "SUCCESSFUL WITHDRAWAL" << endl;
            MyFile<<customer.dateToday<<" - "<<customer.CustomerName<<" : WITHDRAWN "<<now_withdraw<<endl;
            Transaction transaction_now_1;
            transaction_now_1.setTransactionDate(customer.dateToday);
            transaction_now_1.setDescription(customer.CustomerName,"MONEY DEBITED FROM CUSTOMER ACCOUNT",now_withdraw);
            bank.transactions.push_back(transaction_now_1);
        }
        else if (customer.isCurrentAccount == true && now_withdraw > customer.moneyDeposit)
        {
            if (((customer.moneyDeposit - now_withdraw) * (-1)) <= customer.overdraftMaximum && ((customer.moneyDeposit - now_withdraw) * (-1)) <= bank.bankBalance )
            {
                customer.overDraftNow = now_withdraw - (customer.moneyDeposit);
                bank.bankBalance-=customer.overDraftNow;
                customer.moneyDeposit -= now_withdraw;
                cout << "SUCCESSFUL WITHDRAWAL" << endl;
                BankFile<<customer.dateToday<<" - "<<customer.CustomerName<<" : OVERDRAFT DEBITED TO BANK ACCOUNT : "<<to_string(customer.overDraftNow)<<" Rs."<<endl;
                BankFile<<customer.dateToday<<" - "<<"BANK BALANCE : "<<bank.bankBalance<<endl;
                MyFile<<customer.dateToday<<" - "<<customer.CustomerName<<" : WITHDRAWN "<<now_withdraw<<endl;
                Transaction transaction_now_2;
                Transaction transaction_now_3;
                transaction_now_2.setTransactionDate(customer.dateToday);
                transaction_now_3.setTransactionDate(customer.dateToday);
                transaction_now_2.setDescription(customer.CustomerName,"MONEY WITHDRAWN FROM CUSTOMER ACCOUNT",now_withdraw);
                transaction_now_3.setDescription(customer.CustomerName,"OVERDRAFT DEBITED TO BANK ACCOUNT",customer.overDraftNow);
                customer.transactions.push_back(transaction_now_2);
                bank.transactions.push_back(transaction_now_3);
            }
            else if(((customer.moneyDeposit - now_withdraw) * (-1)) <= customer.overdraftMaximum && ((customer.moneyDeposit - now_withdraw) * (-1)) > bank.bankBalance)
            {
                cout << "CAN'T WITHDRAW AMOUNT GREATER THAN BALANCE IN BANK.";
            }
            else
            {
                cout << "CAN'T WITHDRAW AMOUNT GREATER THAN MAXIMUM OVERDRAFT.";
            }
        }
        else if (customer.isCurrentAccount == true && now_withdraw <= customer.moneyDeposit)
        {
            customer.moneyDeposit -= now_withdraw;
            cout << "SUCCESSFUL WITHDRAWAL" << endl;
            MyFile<<customer.dateToday<<" - "<<customer.CustomerName<<" : WITHDRAWN "<<now_withdraw<<endl;
            Transaction transaction_now_4;
            transaction_now_4.setTransactionDate(customer.dateToday);
            transaction_now_4.setDescription(customer.CustomerName,"MONEY DEBITED FROM CUSTOMER ACCOUNT",now_withdraw);
            customer.transactions.push_back(transaction_now_4);
        }
    }
    else
    {
        cout << "ACCOUNT IS ALREADY CLOSED" << endl;
    }
};

void Employee::view_Transactions(Bank& bank)
{
    int numberOfBankTransactions=bank.transactions.size();
    cout<<"TRANSACTION RELATED WITH MONEY OWNED BY BANK"<<endl<<endl;
    for(int i=0; i<numberOfBankTransactions; i++)
    {
        cout<<bank.transactions[i].getDescription()<<endl;
    }
    for(int i=0; i<bank.customerArray.size(); i++)
    {
        cout<<"TRANSACTION RELATED WITH "+bank.customerArray[i].CustomerName<<endl;
        for(int j=0; j<bank.customerArray[i].transactions.size(); j++)
        {
            cout<<bank.customerArray[i].transactions[j].getDescription()<<endl;
        }
    }
};

string Employee::get_Employee_Name()
{
    return this->employeeName;
};

void Employee::close_Customer_Account(Customer& customer,ofstream& MyFile)
{
    if (customer.moneyDeposit == 0)
    {
        customer.isClosed = true;
        cout << "ACCOUNT CLOSED SUCCESSFULLY." << endl;
        MyFile<<customer.dateToday<<" - "<<customer.CustomerName<<" : ACCOUNT CLOSED."<<endl;
    }
    else if (customer.moneyDeposit > 0)
    {
        string userInputCloseAcc;
        cout << "DO YOU WANT TO WITHDRAW THE REMAINING AMOUNT OF " << customer.moneyDeposit << " AND CLOSE THE ACCOUNT." << endl;
        cout << "PRESS 1 FOR YES \NPRESS 2 FOR NO" << endl; ////////////////////////////////////////////////////////////////
        cin >> userInputCloseAcc;
        if (userInputCloseAcc == "1")
        {
            customer.moneyDeposit == 0;
            customer.isClosed = true;
            cout << "ACCOUNT CLOSED SUCCESSFULLY." << endl;
            MyFile<<customer.dateToday<<" - "<<customer.CustomerName<<" : REMAINING AMOUNT OF"<<customer.moneyDeposit <<" IS TAKEN AND ACCOUNT IS CLOSED."<<endl;
            Transaction transaction_now_2;
            transaction_now_2.setTransactionDate(customer.dateToday);
            transaction_now_2.setDescription(customer.CustomerName,"MONEY DEBITED FROM CUSTOMER ACCOUNT",customer.moneyDeposit);
            customer.transactions.push_back(transaction_now_2);

        }
        else if (userInputCloseAcc == "2")
        {
            cout << "ACCOUNT NOT CLOSED." << endl;
        }
    }
    else if (customer.moneyDeposit < 0)
    {
        cout << "THERE IS AN OVERDRAFT IN THIS ACCOUNT. SO ACCOUNT CANNOT BE CLOSED." << endl;
    }

};

void depositFromOutside(Bank& bank,Customer& customer,ofstream& MyFile,ofstream& BankFile)
{
    if (customer.isClosed == false)
    {
        long account_balance = customer.moneyDeposit;
        long now_deposited;
        cout << "ENTER THE AMOUNT TO BE DEPOSITED: ";
        cin >> now_deposited;
        customer.moneyDeposit = account_balance + now_deposited;
        Transaction transaction_now_1;
        transaction_now_1.setTransactionDate(customer.dateToday);
        transaction_now_1.setDescription(customer.CustomerName,"MONEY CREDITED TO CUSTOMER ACCOUNT",now_deposited);
        customer.transactions.push_back(transaction_now_1);

        if (customer.moneyDeposit >= 0)
        {
            bank.bankBalance+=customer.overDraftNow;
            BankFile<<customer.dateToday<<" - "<<customer.CustomerName<<" : OVERDRAFT CREDITED TO BANK ACCOUNT : "<<to_string(customer.overDraftNow)<<" Rs."<<endl;
            BankFile<<customer.dateToday<<" - "<<"BANK BALANCE : "<<bank.bankBalance<<endl;
            Transaction transaction_now_2;
            transaction_now_2.setTransactionDate(customer.dateToday);
            transaction_now_2.setDescription(customer.CustomerName,"OVERDRAFT CREDITED TO BANK ACCOUNT",customer.overDraftNow);
            bank.transactions.push_back(transaction_now_2);
            customer.overDraftNow = 0;
        }
        else
        {
            bank.bankBalance+=now_deposited;
            customer.overDraftNow -= now_deposited;
            BankFile<<customer.dateToday<<" - "<<customer.CustomerName<<" : OVERDRAFT CREDITED TO BANK ACCOUNT : "<<to_string(now_deposited)<<" Rs."<<endl;
            BankFile<<customer.dateToday<<" - "<<"BANK BALANCE : "<<bank.bankBalance<<endl;
            Transaction transaction_now_2;
            transaction_now_2.setTransactionDate(customer.dateToday);
            transaction_now_2.setDescription(customer.CustomerName,"OVERDRAFT CREDITED TO BANK ACCOUNT",now_deposited);
            bank.transactions.push_back(transaction_now_2);

        }

        for(int i=0; i< bank.customerArray.size(); i++)
        {
            if(customer.CustomerName==bank.customerArray[i].CustomerName)
            {
                bank.customerArray[i].moneyDeposit=customer.moneyDeposit;
                bank.customerArray[i].overDraftNow=customer.overDraftNow;
                break;
            }
        }
        cout<<"MONEY SUCCESSFULLY DEPOSITED"<<endl;
        MyFile<<customer.dateToday<<" - "<<customer.CustomerName<<" : DEPOSITED "<<now_deposited<<endl;
    }
    else
    {
        cout << "ACCOUNT IS ALREADY CLOSED" << endl;
    }
}

string getPassword()
{
    cout << "ENTER PASSWORD: ";

    string password;
    char ch;
    while (true)
    {
        ch = _getch();
        if (ch == '\r')
        {
            cout << endl;
            break;
        }
        password += ch;
        cout << '*';
    }

    return password;
}


int main()
{
    Bank bank;
    vector<string> employeeNameArray;
    vector<string> customerNameArray;

    ofstream BankFile("Bank.txt");
    ofstream CustomerFile("Customer_details.txt");
    fstream BankFile_handler;
    fstream CustomerFile_handler;

    BankFile_handler.open("Bank.txt", ios:: in | ios::out);
    CustomerFile_handler.open("Customer_details.txt", ios:: in | ios::out);

    if (!BankFile_handler)
    {
        cout << "BANK FILE DID NOT OPEN!";
        exit(1);
    }

    if (!CustomerFile_handler)
    {
        cout << "CUSTOMER DETAILS FILE DID NOT OPEN!";
        exit(1);
    }

    int input=1,user_input_1, user_input_2,user_input_3;
    Administrator Admin;
    Admin.set_Date();
    string username, password;
    while(input!=0)
    {
        cout << "SELECT AN OPTION BELOW : " << endl;
        cout << "1. DEPOSIT AS AN OUTSIDE SOURCE." << endl;
        cout << "2. LOGIN." << endl;
        cin >> user_input_1;
        if (user_input_1 == 2)
        {
            int login_checker=0;
            while(login_checker!=1)
            {
                cout << "SELECT AN OPTION BELOW : " << endl;
                cout << "1. ENTER AS ADMINISTRATOR." << endl;
                cout << "2. ENTER AS CUSTOMER." << endl;
                cout << "3. ENTER AS EMPLOYEE." << endl;
                cout<<"4. BANK"<<endl;

                cin >> user_input_2;
                if (user_input_2 == 1)
                {
                    cout << "ENTER THE USER NAME : ";
                    cin >> username;
                    cout << endl;
                    password = getPassword();

                    while (username != "Admin" || password != "Password@1234")
                    {
                        cout << "INVALID CREDENTIALS." << endl;
                        cout << "ENTER THE USER NAME : ";
                        cin >> username;
                        cout << endl;
                        password = getPassword();
                    }
                    cout << "SUCCESSFUL LOGIN" << endl<<endl;
                    int checker_admin=0;
                    while(checker_admin!=1)
                    {
                        cout<<"\nDATE : "<<Admin.get_Date()<<endl;
                        float annualInterest=Admin.get_annualSavingInterest();
                        float overdraftCharge=Admin.get_overdraftCharge();
                        cout<<"ANNUAL INTEREST RATE IS "<<annualInterest<<endl;
                        cout<<"OVERDRAFT CHARGE IS "<<overdraftCharge<<endl;
                        cout<<"BANK BALANCE NOW :  "<<bank.bankBalance<<endl<<endl;
                        // cout<<"Overdraft charge per day is " <<" % "<<end;
                        cout<<"1. CREATE AN EMPLOYEE"<<endl;
                        cout<<"2. INCREASE THE DATE"<<endl;
                        cout<<"3. CHANGE OVERDRAFT CHARGE"<<endl;
                        cout<<"4. CHANGE ANNUAL SAVINGS INTEREST"<<endl;
                        cout<<"5. LOGOUT"<<endl;
                        cout <<"SELECT AN OPTION ABOVE"<<endl;
                        cin>>user_input_3;
                        switch(user_input_3)
                        {
                        case 1:
                        {
                            Admin.create_Employee(bank);
                            Employee employee;
                            break;
                        }
                        case 2:
                        {
                            Admin.increase_Date();
                            if(bank.customerArray.size()>0)
                            {
                                for(int i=0; i<bank.customerArray.size(); i++)
                                {
                                    Admin.update_Customer_Interest(bank.customerArray[i],bank,BankFile);
                                    Admin.update_Customer_Overdraft(bank.customerArray[i]);
                                }
                            }
                            break;
                        }
                        case 3:
                        {
                            Admin.set_overdraftCharge();
                            break;
                        }
                        case 4:
                        {
                            Admin.set_annualSavingInterest();
                            break;
                        }
                        case 5:
                        {
                            input=0;
                            checker_admin=1;
                            break;
                        }
                        default:
                        {}
                        }
                    }
                }
                else if(user_input_2==4)
                {
                    login_checker=1;
                    input=1;
                    break;
                }
                else if(user_input_2==3)
                {
                    cout << "ENTER THE EMPLOYEE USER NAME : ";
                    cin >> username;
                    cout << endl;
                    password = getPassword();
                    employeeNameArray=bank.employeeNameArray;
                    Employee employeeNow;
                    for(int i=0; i<employeeNameArray.size(); i++)
                    {
                        if(username==employeeNameArray[i] && password =="Password@1234")
                        {
                            employeeNow=bank.employeeArray[i];
                            login_checker=0;
                            break;
                        }
                        login_checker=1;
                    }
                    if(login_checker==1)
                    {
                        cout<<"LOGIN UNSUCCESSFUL";
                        login_checker=0;
                    }
                    else
                    {
                        cout << "SUCCESSFUL LOGIN" << endl<<endl;
                        int checker_employee=0;
                        while(checker_employee!=1)
                        {
                            cout<<"\nDATE : "<<Admin.get_Date()<<endl;
                            float annualInterest=Admin.get_annualSavingInterest();
                            float overdraftCharge=Admin.get_overdraftCharge();
                            cout<<"ANNUAL INTEREST RATE IS "<<annualInterest<<endl;
                            cout<<"OVERDRAFT CHARGE IS "<<overdraftCharge<<endl;
                            cout<<"1. CREATE A CUSTOMER ACCOUNT"<<endl;
                            cout<<"2. CLOSE THE CUSTOMER ACCOUNT"<<endl;
                            cout<<"3. DEPOSIT MONEY IN ACCOUNT"<<endl;
                            cout<<"4. WITHDRAW MONEY FROM ACCOUNT"<<endl;
                            cout<<"5. VIEW TRANSACTIONS"<<endl;
                            cout<<"6. LOGOUT"<<endl;
                            cout <<"SELECT AN OPTION ABOVE"<<endl;
                            cin>>user_input_3;
                            switch(user_input_3)
                            {
                            case 1:
                            {
                                Customer customerCreated;
                                customerCreated.set_Date(Admin);
                                employeeNow.create_Customer(customerCreated,bank,CustomerFile);
                                break;
                            }
                            case 2:
                            {
                                string customerName;
                                cout<<"ENTER THE NAME OF THE CUSTOMER: ";
                                cin>>customerName;
                                for(int i=0; i<bank.customerArray.size(); i++)
                                {
                                    if(customerName==bank.customerArray[i].CustomerName)
                                    {
                                        bank.customerArray[i].set_Date(Admin);
                                        employeeNow.close_Customer_Account(bank.customerArray[i],CustomerFile);
                                        break;
                                    }
                                }
                                break;
                            }
                            case 3:
                            {
                                string customerName;
                                cout<<"ENTER THE NAME OF THE CUSTOMER: ";
                                cin>>customerName;
                                for(int i=0; i<bank.customerArray.size(); i++)
                                {
                                    if(customerName==bank.customerArray[i].CustomerName)
                                    {
                                        bank.customerArray[i].set_Date(Admin);
                                        employeeNow.deposit_Money(bank.customerArray[i],bank,CustomerFile,BankFile);
                                        break;
                                    }
                                }
                                break;
                            }
                            case 4:
                            {
                                string customerName;
                                cout<<"ENTER THE NAME OF THE CUSTOMER: ";
                                cin>>customerName;
                                for(int i=0; i<bank.customerArray.size(); i++)
                                {
                                    if(customerName==bank.customerArray[i].CustomerName)
                                    {
                                        bank.customerArray[i].set_Date(Admin);
                                        employeeNow.withdraw_Money(bank.customerArray[i],bank,CustomerFile,BankFile);
                                        break;
                                    }
                                }
                                break;
                            }
                            case 5:
                            {
                                employeeNow.view_Transactions(bank);
                                break;
                            }
                            case 6:
                            {
                                input=0;
                                checker_employee=1;
                                break;
                            }
                            default:
                            {}
                            }
                        }
                    }


                }
                else if(user_input_2==2)
                {
                    int customerArrayElement;
                    cout << "ENTER THE CUSTOMER USER NAME : ";
                    cin >> username;
                    cout << endl;
                    password = getPassword();
                    customerNameArray=bank.customerNameArray;
                    Customer customer;
                    for(int i=0; i<customerNameArray.size(); i++)
                    {
                        cout<<customerNameArray[i]<<endl;
                    }
                    for(int i=0; i<customerNameArray.size(); i++)
                    {
                        if(username==customerNameArray[i] && password =="Password@1234")
                        {
                            customer=bank.customerArray[i];
                            customerArrayElement=i;
                            login_checker=0;
                            break;
                        }
                        login_checker=1;
                    }
                    if(login_checker==1)
                    {
                        cout<<"LOGIN UNSUCCESSFUL";
                        login_checker=0;
                    }
                    else
                    {
                        cout << "SUCCESSFUL LOGIN" << endl<<endl;
                        int checker_customer=0;
                        while(checker_customer!=1)
                        {
                            cout<<"\nDATE : "<<Admin.get_Date()<<endl;
                            float annualInterest=Admin.get_annualSavingInterest();
                            float overdraftCharge=Admin.get_overdraftCharge();
                            cout<<"ANNUAL INTEREST RATE IS "<<annualInterest<<endl;
                            cout<<"OVERDRAFT CHARGE IS "<<overdraftCharge<<endl<<endl;
                            cout<<"ACCOUNT NAME : "<<customer.CustomerName<<endl;
                            cout<<"ACCOUNT TYPE : "<<customer.accountType<<endl;
                            cout<<"ACCOUNT CONTACT DETAILS : "<<customer.customerContact<<endl<<endl;
                            cout<<"ACCOUNT BALANCE : "<<customer.moneyDeposit<<endl<<endl;
                            cout<<"ACCOUNT OVERDRAFT : "<<customer.overDraftNow<<endl<<endl;
                            cout<<"1. DEPOSIT MONEY IN ACCOUNT"<<endl;
                            cout<<"2. WITHDRAW MONEY FROM ACCOUNT"<<endl;
                            cout<<"3. VIEW TRANSACTIONS"<<endl;
                            cout<<"4. LOGOUT"<<endl;
                            cout <<"SELECT AN OPTION ABOVE"<<endl;
                            cin>>user_input_3;
                            switch(user_input_3)
                            {
                            case 1:
                            {
                                if(customer.isClosed==false)
                                {
                                    customer.set_Date(Admin);
                                    customer.deposit_Money_Function(bank,CustomerFile,BankFile);
                                    bank.customerArray[customerArrayElement]=customer;
                                }
                                else
                                {
                                    cout<<"ACCOUNT CLOSED ALREADY."<<endl;
                                }
                                break;
                            }
                            case 2:
                            {
                                if(customer.isClosed==false)
                                {
                                    customer.set_Date(Admin);
                                    customer.withdraw_Money(bank,CustomerFile,BankFile);
                                    bank.customerArray[customerArrayElement]=customer;
                                }
                                else
                                {
                                    cout<<"ACCOUNT CLOSED ALREADY."<<endl;
                                }
                                break;
                            }
                            case 3:
                            {
                                customer.view_Transaction();
                                break;
                            }
                            case 4:
                            {
                                input=0;
                                checker_customer=1;
                                break;
                            }
                            default:
                            {}
                            }
                        }
                    }
                }

            }
        }
        else if(user_input_1==1)
        {
            string customerUserName;
            int isSuccess=0;
            cout<<"ENTER THE CUSTOMER NAME : ";
            cin>>customerUserName;
            for(int i=0; i<bank.customerArray.size(); i++)
            {
                if(customerUserName==bank.customerNameArray[i])
                {
                    bank.customerArray[i].set_Date(Admin);
                    depositFromOutside(bank,bank.customerArray[i],CustomerFile,BankFile);
                    isSuccess=1;
                    break;
                }
            }
            if(isSuccess==0)
            {
                cout<<"TRANSACTION UNSUCCESSFUL"<<endl;
            }
        }
    }
    return 0;
}
