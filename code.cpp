#include <iostream>
#include <conio.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <time.h>
#include <fstream>
#include <sstream>

using namespace std;

class Customer;
class Transaction;
class Administrator;
class Employee;


class Bank
{
public:
    vector < Employee > employeeArray;
    vector < string > employeeNameArray;
    vector < Customer > customerArray;
    vector < string > customerNameArray;
    vector <Transaction> transactions;
    long bank_balance=1000000;
};

class Administrator
{
private:
    string dateTay;
    float annual_saving_interest = 20;
    float overdraftCharge = 2;
    int dateIncreased=0;

public:
    void createEmployee(Bank& bank);
    void setDate();
    void increaseDate();
    void setAnnualSavingInterest();
    void setOverdraftCharge();
    void updateCustomerOverdraft(Customer & customer);
    void updateCustomerInterest(Customer & customer,Bank& bank,ofstream& BankFile);
    string getEmployeeName(Bank& bank);
    string getDate();
    float getAnnualSavingInterest();
    float getOverdraftCharge();
    int employeeNumber(Bank& bank);
    vector<string> getEmployeeNameArray(Bank& bank);
};

class Employee
{
private:
    string employeeName;
    string dateTay;
public:
    void setDate(Administrator & Admin);
    void setEmployeeName(string employeeName);
    string getEmployeeName();
    void createCustomer(Customer & customerCreated,Bank& bank,ofstream& MyFile);
    void depositMoney(Customer & customer,Bank& bank,ofstream& MyFile,ofstream& BankFile);
    void withDrawMoney(Customer & customer,Bank& bank,ofstream& MyFile,ofstream& BankFile);
    void viewTransactions(Bank& bank);
    void closeCustomerAccount(Customer & customer,ofstream& MyFile);
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
    string dateTay;
    string dateCreated;
    vector <Transaction> transactions;

    void viewTransaction();
    void depositMoneyFun(Bank& bank,ofstream& MyFile,ofstream& BankFile);
    void setDate(Administrator& Admin);
    void withdrawMoney(Bank& bank,ofstream& MyFile,ofstream& BankFile);
};

class Transaction
{
private:
    string dateTay;
    string description;
public:

    void setTransactionDate(string dateTay)
    {
        this->dateTay=dateTay;
    }

    void setDescription(string customer_name,string debitOrCredit,long amount)
    {
        this->description=this->dateTay+" - "+customer_name+" : "+debitOrCredit+" : "+to_string(amount)+" Rs.";
    }
    string getDescription()
    {
        return this->description;
    }

};

int Administrator::employeeNumber(Bank& bank)
{
    return bank.employeeArray.size();
};

vector<string> Administrator::getEmployeeNameArray(Bank& bank)
{
    return bank.employeeNameArray;
};

void Administrator::createEmployee(Bank& bank)
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
    string employeeName=("employee" +startLetter+to_string((bank.employeeArray.size()) + 1));
    Employee employee;
    employee.setEmployeeName(employeeName);
    bank.employeeArray.push_back(employee);
    bank.employeeNameArray.push_back(employeeName);
    Employee lastCreatedEmployee=bank.employeeArray.back();
    cout<<lastCreatedEmployee.getEmployeeName()+" : Employee created successfully"<<endl;;
};

void Administrator::setDate()
{
    string dateTay;
    time_t ttime = time(0);
    tm * local_time = localtime( & ttime);
    dateTay = to_string(1900 + local_time -> tm_year) + ":" + to_string(1 + local_time -> tm_mon) + ":" + to_string(local_time -> tm_mday);
    this -> dateTay = dateTay;
};

void Administrator::increaseDate()
{
    this->dateIncreased+=1;
    time_t ttime = time(0);
    ttime += (24 * 60 * 60)*(this->dateIncreased);
    tm * local_time = localtime( & ttime);
    this->dateTay = to_string(1900 + local_time -> tm_year) + ":" + to_string(1+local_time -> tm_mon) + ":" + to_string(local_time -> tm_mday);
};

void Administrator::setAnnualSavingInterest()
{
    float annual_saving_interest;
    cout << "Enter the new annual saving interest : ";
    cin >> annual_saving_interest;
    this -> annual_saving_interest = annual_saving_interest;
    cout<<"New annual savings interest is "<< annual_saving_interest<<endl;
};

void Administrator::setOverdraftCharge()
{
    float overdraftCharge;
    cout << "Enter the new overdraft charge(% per day) : ";
    cin >> overdraftCharge;
    this -> overdraftCharge = overdraftCharge;
    cout<<"New overdraft charge is "<< overdraftCharge<<endl;
};

void Administrator::updateCustomerOverdraft(Customer & customer)
{
    if (customer.overDraftNow > 0)
    {
        customer.overDraftNow += ((this -> overdraftCharge) / 100) * (customer.overDraftNow);
        customer.moneyDeposit=(customer.overDraftNow)*(-1);

    }
}

void Administrator::updateCustomerInterest(Customer & customer,Bank& bank,ofstream& BankFile)
{
    float dailyInterest = (this -> annual_saving_interest) / 365;
    if (customer.moneyDeposit > 0)
    {
        cout<<"line-1"<<endl;
        customer.moneyDeposit += ((dailyInterest) / 100) * (customer.moneyDeposit);
        bank.bank_balance-=((dailyInterest) / 100) * (customer.moneyDeposit);
        BankFile<<this->dateTay<<" - "<<customer.CustomerName<<" : Interest debited to bank account : "<<to_string(((dailyInterest) / 100) * (customer.moneyDeposit))<<" Rs."<<endl;
        BankFile<<this->dateTay<<" - "<<"Bank balance : "<<bank.bank_balance<<endl;
        Transaction transaction_now;
        transaction_now.setTransactionDate(this->dateTay);
        transaction_now.setDescription(customer.CustomerName,"Interest credited to customer account",((dailyInterest) / 100) * (customer.moneyDeposit));
        bank.transactions.push_back(transaction_now);
        customer.transactions.push_back(transaction_now);

    }
}

string Administrator::getEmployeeName(Bank& bank)
{
    return bank.employeeArray.back().getEmployeeName();
}

string Administrator::getDate()
{
    return this->dateTay;
}

float Administrator::getAnnualSavingInterest()
{
    return (this->annual_saving_interest);
};

float Administrator::getOverdraftCharge()
{
    return (this->overdraftCharge);
};


void Customer::viewTransaction()
{
    int numberOfTransactions=this->transactions.size();
    for(int i=0; i<numberOfTransactions; i++)
    {
        cout<<this->transactions[i].getDescription()<<endl;
    }
};

void Customer::depositMoneyFun(Bank& bank,ofstream& MyFile,ofstream& BankFile)
{
    int moneyAdded;
    cout << "Enter the amount of money to deposit: ";
    cin >> moneyAdded;
    this -> moneyDeposit += moneyAdded;
    MyFile<<this->dateTay<<" - "<<this->CustomerName<<" : deposited "<<to_string(moneyAdded)<<endl;
    Transaction transaction_now_1;
    transaction_now_1.setTransactionDate(this->dateTay);
    transaction_now_1.setDescription(this->CustomerName,"Money credited to customer account",moneyAdded);
    this->transactions.push_back(transaction_now_1);

    if (this -> moneyDeposit >= 0)
    {
        bank.bank_balance+=overDraftNow;
        BankFile<<this->dateTay<<" - "<<this->CustomerName<<" : overdraft credited to bank account : "<<to_string(overDraftNow)<<" Rs."<<endl;
        BankFile<<this->dateTay<<" - "<<"Bank balance : "<<bank.bank_balance<<endl;
        Transaction transaction_now_2;
        transaction_now_2.setTransactionDate(this->dateTay);
        transaction_now_2.setDescription(this->CustomerName,"overdraft credited to bank account",overDraftNow);
        bank.transactions.push_back(transaction_now_2);
        this -> overDraftNow = 0;
    }
    else
    {
        bank.bank_balance+=moneyAdded;
        BankFile<<this->dateTay<<" - "<<this->CustomerName<<" : overdraft credited to bank account : "<<to_string(moneyAdded)<<" Rs."<<endl;
        BankFile<<this->dateTay<<" - "<<"Bank balance : "<<bank.bank_balance<<endl;
        this -> overDraftNow -= moneyAdded;
        Transaction transaction_now_2;
        transaction_now_2.setTransactionDate(this->dateTay);
        transaction_now_2.setDescription(this->CustomerName,"overdraft credited to bank account",moneyAdded);
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

void Customer::setDate(Administrator & Admin)
{
    this -> dateTay = Admin.getDate();
};

void Customer::withdrawMoney(Bank& bank,ofstream& MyFile,ofstream& BankFile)
{
    int moneyToWithdraw;
    cout << "Enter the amount of money to withdraw: ";
    cin >> moneyToWithdraw;
    if (this -> isCurrentAccount == false && moneyToWithdraw > this -> moneyDeposit)
    {
        cout << "Sorry insufficient balance" << endl;
    }
    else if (this -> isCurrentAccount == false && moneyToWithdraw <= this -> moneyDeposit)
    {
        this -> moneyDeposit -= moneyToWithdraw;
        cout << "Successful withdrawal" << endl;
        MyFile<<this->dateTay<<" - "<<this->CustomerName<<" : Withdrawn "<<to_string(moneyToWithdraw)<<endl;
        Transaction transaction_now_1;
        transaction_now_1.setTransactionDate(this->dateTay);
        transaction_now_1.setDescription(this->CustomerName,"Money withdrawn from customer account",moneyToWithdraw);
        this->transactions.push_back(transaction_now_1);
    }
    else if (this -> isCurrentAccount == true && moneyToWithdraw > this -> moneyDeposit)
    {
        if (((this -> moneyDeposit - moneyToWithdraw) * (-1)) <= this -> overdraftMaximum && ((this -> moneyDeposit - moneyToWithdraw) * (-1)) <= bank.bank_balance )
        {
            this -> overDraftNow = moneyToWithdraw - (this -> moneyDeposit);
            bank.bank_balance-=this->overDraftNow;
            this -> moneyDeposit -= moneyToWithdraw;
            cout << "Successful withdrawal" << endl;
            BankFile<<this->dateTay<<" - "<<this->CustomerName<<" : overdraft debited to bank account : "<<to_string(this->overDraftNow)<<" Rs."<<endl;
            BankFile<<this->dateTay<<" - "<<"Bank balance : "<<bank.bank_balance<<endl;
            MyFile<<this->dateTay<<" - "<<this->CustomerName<<" : Withdrawn "<<to_string(moneyToWithdraw)<<endl;
            Transaction transaction_now_2;
            Transaction transaction_now_3;
            transaction_now_2.setTransactionDate(this->dateTay);
            transaction_now_3.setTransactionDate(this->dateTay);
            transaction_now_2.setDescription(this->CustomerName,"Money withdrawn from customer account",moneyToWithdraw);
            transaction_now_3.setDescription(this->CustomerName,"overdraft debited to bank account",this->overDraftNow);
            this->transactions.push_back(transaction_now_2);
            bank.transactions.push_back(transaction_now_3);
        }
        else if(((this -> moneyDeposit - moneyToWithdraw) * (-1)) <= this -> overdraftMaximum && ((this -> moneyDeposit - moneyToWithdraw) * (-1)) > bank.bank_balance)
        {
            cout << "Sorry. Cannot withdraw amount greater than balance in the bank.";
        }
        else
        {
            cout << "Sorry. Cannot withdraw amount greater than maximum overdraft.";
        }
    }
    else if (this -> isCurrentAccount == true && moneyToWithdraw <= this -> moneyDeposit)
    {
        this -> moneyDeposit -= moneyToWithdraw;
        cout << "Successful withdrawal" << endl;
        MyFile<<this->dateTay<<" - "<<this->CustomerName<<" : Withdrawn "<<to_string(moneyToWithdraw)<<endl;
        Transaction transaction_now_4;
        transaction_now_4.setTransactionDate(this->dateTay);
        transaction_now_4.setDescription(this->CustomerName,"Money withdrawn from customer account",moneyToWithdraw);
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

void Employee::setDate(Administrator & Admin)
{
    this -> dateTay = Admin.getDate();
};

void Employee::setEmployeeName(string employeeName)
{
    this->employeeName = employeeName;
};

void Employee::createCustomer(Customer & customerCreated,Bank& bank,ofstream& MyFile)
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
    customerCreated.dateCreated = dateTay;
    string customerName= "customer" +startLetter+to_string((bank.customerArray.size()) + 1);
    cout << "Customer name : " << "customer" +startLetter+to_string((bank.customerArray.size()) + 1)<<endl;
    cout << "Enter mobile number : ";
    cin >> customer_contact;
    customerCreated.CustomerName=customerName;
    customerCreated.customerContact = customer_contact;
    cout << "Select the account type : " << endl;
    cout << "1. Current account" << endl;
    cout << "2. Saving account" << endl;
    cin >> account_type;

    while (account_type < 0 || account_type > 3)
    {
        cout << "Incorrect input! Select a proper account";
        cout << "Select the account type : " << endl;
        cout << "1. Current account" << endl;
        cout << "2. Saving account" << endl;
        cin >> account_type;
    }

    if (account_type == 1)
    {
        customerCreated.accountType = "Current";
        customerCreated.isCurrentAccount = true;
        cout << "Enter the overdraft limit : ";
        cin >> customerCreated.overdraftMaximum;
    }
    else if (account_type == 2)
    {
        customerCreated.accountType = "Saving";
    }

    long moneyDepositNow;
    cout << "Enter the initial amount deposited : ";
    cin>>moneyDepositNow;
    customerCreated.moneyDeposit=moneyDepositNow;
    bank.customerNameArray.push_back(customerName);
    bank.customerArray.push_back(customerCreated);
    MyFile<<customerCreated.dateTay<<" - "<<customerCreated.CustomerName<<" : Account created."<<endl;
    MyFile<<customerCreated.dateTay<<" - "<<customerCreated.CustomerName<<" : Deposited "<<moneyDepositNow<<endl;
    Transaction transaction_now_1;
    transaction_now_1.setTransactionDate(customerCreated.dateTay);
    transaction_now_1.setDescription(customerCreated.CustomerName,"Money credited to customer account",moneyDepositNow);
    customerCreated.transactions.push_back(transaction_now_1);

};

void Employee::depositMoney(Customer& customer,Bank& bank,ofstream& MyFile,ofstream& BankFile)
{
    if (customer.isClosed == false)
    {
        long account_balance = customer.moneyDeposit;
        long now_deposited;
        cout << "Enter the amount to be deposited: ";
        cin >> now_deposited;
        customer.moneyDeposit = account_balance + now_deposited;
        MyFile<<customer.dateTay<<" - "<<customer.CustomerName<<" : Deposited "<<now_deposited<<endl;
        Transaction transaction_now_1;
        transaction_now_1.setTransactionDate(customer.dateTay);
        transaction_now_1.setDescription(customer.CustomerName,"Money credited to customer account",now_deposited);
        customer.transactions.push_back(transaction_now_1);

        if (customer.moneyDeposit >= 0)
        {
            bank.bank_balance+=customer.overDraftNow;
            BankFile<<customer.dateTay<<" - "<<customer.CustomerName<<" : overdraft credited to bank account : "<<to_string(customer.overDraftNow)<<" Rs."<<endl;
            BankFile<<customer.dateTay<<" - "<<"Bank balance : "<<bank.bank_balance<<endl;
            Transaction transaction_now_2;
            transaction_now_2.setTransactionDate(customer.dateTay);
            transaction_now_2.setDescription(customer.CustomerName,"overdraft credited to bank account",customer.overDraftNow);
            bank.transactions.push_back(transaction_now_2);
            customer.overDraftNow = 0;

        }
        else
        {
            bank.bank_balance+=now_deposited;
            customer.overDraftNow -= now_deposited;
            BankFile<<customer.dateTay<<" - "<<customer.CustomerName<<" : overdraft credited to bank account : "<<to_string(now_deposited)<<" Rs."<<endl;
            BankFile<<customer.dateTay<<" - "<<"Bank balance : "<<bank.bank_balance<<endl;
            Transaction transaction_now_2;
            transaction_now_2.setTransactionDate(customer.dateTay);
            transaction_now_2.setDescription(customer.CustomerName,"overdraft credited to bank account",now_deposited);
            bank.transactions.push_back(transaction_now_2);
        }
    }
    else
    {
        cout << "Sorry. Account is already closed" << endl;
    }
};

void Employee::withDrawMoney(Customer& customer,Bank& bank,ofstream& MyFile,ofstream& BankFile)
{
    if (customer.isClosed == false)
    {
        //long account_balance=customer.moneyDeposit;
        long now_withdraw;
        cout << "Enter the amount to be withdrawed: ";
        cin >> now_withdraw;
        //customer.moneyDeposit=account_balance-now_withdraw;

        if (customer.isCurrentAccount == false && now_withdraw > customer.moneyDeposit)
        {
            cout << "Sorry insufficient balance" << endl;
        }
        else if (customer.isCurrentAccount == false && now_withdraw <= customer.moneyDeposit)
        {
            customer.moneyDeposit -= now_withdraw;
            cout << "Successful withdrawal" << endl;
            MyFile<<customer.dateTay<<" - "<<customer.CustomerName<<" : Withdrawn "<<now_withdraw<<endl;
            Transaction transaction_now_1;
            transaction_now_1.setTransactionDate(customer.dateTay);
            transaction_now_1.setDescription(customer.CustomerName,"Money debited from customer account",now_withdraw);
            bank.transactions.push_back(transaction_now_1);
        }
        else if (customer.isCurrentAccount == true && now_withdraw > customer.moneyDeposit)
        {
            if (((customer.moneyDeposit - now_withdraw) * (-1)) <= customer.overdraftMaximum && ((customer.moneyDeposit - now_withdraw) * (-1)) <= bank.bank_balance )
            {
                customer.overDraftNow = now_withdraw - (customer.moneyDeposit);
                bank.bank_balance-=customer.overDraftNow;
                customer.moneyDeposit -= now_withdraw;
                cout << "Successful withdrawal" << endl;
                BankFile<<customer.dateTay<<" - "<<customer.CustomerName<<" : overdraft debited to bank account : "<<to_string(customer.overDraftNow)<<" Rs."<<endl;
                BankFile<<customer.dateTay<<" - "<<"Bank balance : "<<bank.bank_balance<<endl;
                MyFile<<customer.dateTay<<" - "<<customer.CustomerName<<" : Withdrawn "<<now_withdraw<<endl;
                Transaction transaction_now_2;
                Transaction transaction_now_3;
                transaction_now_2.setTransactionDate(customer.dateTay);
                transaction_now_3.setTransactionDate(customer.dateTay);
                transaction_now_2.setDescription(customer.CustomerName,"Money withdrawn from customer account",now_withdraw);
                transaction_now_3.setDescription(customer.CustomerName,"overdraft debited to bank account",customer.overDraftNow);
                customer.transactions.push_back(transaction_now_2);
                bank.transactions.push_back(transaction_now_3);
            }
            else if(((customer.moneyDeposit - now_withdraw) * (-1)) <= customer.overdraftMaximum && ((customer.moneyDeposit - now_withdraw) * (-1)) > bank.bank_balance)
            {
                cout << "Sorry. Cannot withdraw amount greater than balance in bank.";
            }
            else
            {
                cout << "Sorry. Cannot withdraw amount greater than maximum overdraft.";
            }
        }
        else if (customer.isCurrentAccount == true && now_withdraw <= customer.moneyDeposit)
        {
            customer.moneyDeposit -= now_withdraw;
            cout << "Successful withdrawal" << endl;
            MyFile<<customer.dateTay<<" - "<<customer.CustomerName<<" : Withdrawn "<<now_withdraw<<endl;
            Transaction transaction_now_4;
            transaction_now_4.setTransactionDate(customer.dateTay);
            transaction_now_4.setDescription(customer.CustomerName,"Money debited from customer account",now_withdraw);
            customer.transactions.push_back(transaction_now_4);
        }
    }
    else
    {
        cout << "Sorry. Account is already closed" << endl;
    }
};

void Employee::viewTransactions(Bank& bank)
{
    int numberOfBankTransactions=bank.transactions.size();
    cout<<"Transaction related with money owned by bank"<<endl<<endl;
    for(int i=0; i<numberOfBankTransactions; i++)
    {
        cout<<bank.transactions[i].getDescription()<<endl;
    }
    for(int i=0; i<bank.customerArray.size(); i++)
    {
        cout<<"Transaction related with "+bank.customerArray[i].CustomerName<<endl;
        for(int j=0; j<bank.customerArray[i].transactions.size(); j++)
        {
            cout<<bank.customerArray[i].transactions[j].getDescription()<<endl;
        }
    }
};

string Employee::getEmployeeName()
{
    return this->employeeName;
};

void Employee::closeCustomerAccount(Customer& customer,ofstream& MyFile)
{
    if (customer.moneyDeposit == 0)
    {
        customer.isClosed = true;
        cout << "Account closed successfully." << endl;
        MyFile<<customer.dateTay<<" - "<<customer.CustomerName<<" : Account closed."<<endl;
    }
    else if (customer.moneyDeposit > 0)
    {
        string userInputCloseAcc;
        cout << "Do you want to withdraw the remaining amount of " << customer.moneyDeposit << " and close the account." << endl;
        cout << "press 1 for yes \nPress 2 for no" << endl; ////////////////////////////////////////////////////////////////
        cin >> userInputCloseAcc;
        if (userInputCloseAcc == "1")
        {
            customer.moneyDeposit == 0;
            customer.isClosed = true;
            cout << "Account closed successfully." << endl;
            MyFile<<customer.dateTay<<" - "<<customer.CustomerName<<" : remaining amount of"<<customer.moneyDeposit <<" is taken and Account is closed."<<endl;
            Transaction transaction_now_2;
            transaction_now_2.setTransactionDate(customer.dateTay);
            transaction_now_2.setDescription(customer.CustomerName,"Money debited from customer account",customer.moneyDeposit);
            customer.transactions.push_back(transaction_now_2);

        }
        else if (userInputCloseAcc == "2")
        {
            cout << "Account not closed." << endl;
        }
    }
    else if (customer.moneyDeposit < 0)
    {
        cout << "There is an overdraft in this account. So account cannot be closed." << endl;
    }

};

void depositFromOutside(Bank& bank,Customer& customer,ofstream& MyFile,ofstream& BankFile)
{
    if (customer.isClosed == false)
    {
        long account_balance = customer.moneyDeposit;
        long now_deposited;
        cout << "Enter the amount to be deposited: ";
        cin >> now_deposited;
        customer.moneyDeposit = account_balance + now_deposited;
        Transaction transaction_now_1;
        transaction_now_1.setTransactionDate(customer.dateTay);
        transaction_now_1.setDescription(customer.CustomerName,"Money credited to customer account",now_deposited);
        customer.transactions.push_back(transaction_now_1);

        if (customer.moneyDeposit >= 0)
        {
            bank.bank_balance+=customer.overDraftNow;
            BankFile<<customer.dateTay<<" - "<<customer.CustomerName<<" : overdraft credited to bank account : "<<to_string(customer.overDraftNow)<<" Rs."<<endl;
            BankFile<<customer.dateTay<<" - "<<"Bank balance : "<<bank.bank_balance<<endl;
            Transaction transaction_now_2;
            transaction_now_2.setTransactionDate(customer.dateTay);
            transaction_now_2.setDescription(customer.CustomerName,"overdraft credited to bank account",customer.overDraftNow);
            bank.transactions.push_back(transaction_now_2);
            customer.overDraftNow = 0;
        }
        else
        {
            bank.bank_balance+=now_deposited;
            customer.overDraftNow -= now_deposited;
            BankFile<<customer.dateTay<<" - "<<customer.CustomerName<<" : overdraft credited to bank account : "<<to_string(now_deposited)<<" Rs."<<endl;
            BankFile<<customer.dateTay<<" - "<<"Bank balance : "<<bank.bank_balance<<endl;
            Transaction transaction_now_2;
            transaction_now_2.setTransactionDate(customer.dateTay);
            transaction_now_2.setDescription(customer.CustomerName,"overdraft credited to bank account",now_deposited);
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
        cout<<"Money successfully deposited"<<endl;
        MyFile<<customer.dateTay<<" - "<<customer.CustomerName<<" : Deposited "<<now_deposited<<endl;
    }
    else
    {
        cout << "Sorry. Account is already closed" << endl;
    }
}

string getPassword()
{
    cout << "Enter password: ";

    string password;
    char ch;
    while (true)
    {
        ch = _getch(); // Read a single character without echoing
        if (ch == '\r') // '\r' corresponds to Enter key
        {
            cout << endl;
            break;
        }
        password += ch;
        cout << '*'; // Show asterisk instead of the actual character
    }

    return password;
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
        cout << "Bank File did not open!";
        exit(1);
    }

    if (!CustomerFile_handler)
    {
        cout << "Customer details File did not open!";
        exit(1);
    }

    int input=1,user_input_1, user_input_2,user_input_3;
    Administrator Admin;
    Admin.setDate();
    string username, password;
    while(input!=0)
    {
        cout << "Select an option below : " << endl;
        cout << "1. Deposit as an outside source." << endl;
        cout << "2. Login." << endl;
        cin >> user_input_1;
        if (user_input_1 == 2)
        {
            int login_checker=0;
            while(login_checker!=1)
            {
                cout << "Select an option below : " << endl;
                cout << "1. Enter as administrator ." << endl;
                cout << "2. Enter as customer ." << endl;
                cout << "3. Enter as employee ." << endl;
                cout<<"4. Back"<<endl;

                cin >> user_input_2;
                if (user_input_2 == 1)
                {
                    cout << "Enter the user name : ";
                    cin >> username;
                    cout << endl;
                    password = getPassword();

                    while (username != "Admin" || password != "Password@1234")
                    {
                        cout << "Incorrect credentials. Please try again.." << endl;
                        cout << "Enter the user name : ";
                        cin >> username;
                        cout << endl;
                        password = getPassword();
                    }
                    cout << "Successful login" << endl<<endl;
                    int checker_admin=0;
                    while(checker_admin!=1)
                    {
                        cout<<"\ndate : "<<Admin.getDate()<<endl;
                        float annualInterest=Admin.getAnnualSavingInterest();
                        float overdraftCharge=Admin.getOverdraftCharge();
                        cout<<"Annual interest rate is "<<annualInterest<<endl;
                        cout<<"Overdraft charge is "<<overdraftCharge<<endl;
                        cout<<"Bank balance now :  "<<bank.bank_balance<<endl<<endl;
                        // cout<<"Overdraft charge per day is " <<" % "<<end;
                        cout<<"1. Create an employee"<<endl;
                        cout<<"2. Increase the date"<<endl;
                        cout<<"3. Change overdraft charge"<<endl;
                        cout<<"4. Change annual savings interest"<<endl;
                        cout<<"5. Logout"<<endl;
                        cout <<"Select an option above"<<endl;
                        cin>>user_input_3;
                        switch(user_input_3)
                        {
                        case 1:
                        {
                            Admin.createEmployee(bank);
                            Employee employee;
                            break;
                        }
                        case 2:
                        {
                            Admin.increaseDate();
                            if(bank.customerArray.size()>0)
                            {
                                for(int i=0; i<bank.customerArray.size(); i++)
                                {
                                    Admin.updateCustomerInterest(bank.customerArray[i],bank,BankFile);
                                    Admin.updateCustomerOverdraft(bank.customerArray[i]);
                                }
                            }
                            break;
                        }
                        case 3:
                        {
                            Admin.setOverdraftCharge();
                            break;
                        }
                        case 4:
                        {
                            Admin.setAnnualSavingInterest();
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
                    cout << "Enter the employee user name : ";
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
                        cout<<"Login unsuccessful";
                        login_checker=0;
                    }
                    else
                    {
                        cout << "Successful login" << endl<<endl;
                        int checker_employee=0;
                        while(checker_employee!=1)
                        {
                            cout<<"\ndate : "<<Admin.getDate()<<endl;
                            float annualInterest=Admin.getAnnualSavingInterest();
                            float overdraftCharge=Admin.getOverdraftCharge();
                            cout<<"Annual interest rate is "<<annualInterest<<endl;
                            cout<<"Overdraft charge is "<<overdraftCharge<<endl;
                            // cout<<"Overdraft charge per day is " <<" % "<<end;
                            cout<<"1. Create a customer account"<<endl;
                            cout<<"2. Close the customer account"<<endl;
                            cout<<"3. Deposit money in account"<<endl;
                            cout<<"4. Withdraw money from account"<<endl;
                            cout<<"5. View transactions"<<endl;
                            cout<<"6. Logout"<<endl;
                            cout <<"Select an option above"<<endl;
                            cin>>user_input_3;
                            switch(user_input_3)
                            {
                            case 1:
                            {
                                Customer customerCreated;
                                customerCreated.setDate(Admin);
                                employeeNow.createCustomer(customerCreated,bank,CustomerFile);
                                break;
                            }
                            case 2:
                            {
                                string customerName;
                                cout<<"Enter the name of the customer: ";
                                cin>>customerName;
                                for(int i=0; i<bank.customerArray.size(); i++)
                                {
                                    if(customerName==bank.customerArray[i].CustomerName)
                                    {
                                        bank.customerArray[i].setDate(Admin);
                                        employeeNow.closeCustomerAccount(bank.customerArray[i],CustomerFile);
                                        break;
                                    }
                                }
                                break;
                            }
                            case 3:
                            {
                                string customerName;
                                cout<<"Enter the name of the customer: ";
                                cin>>customerName;
                                for(int i=0; i<bank.customerArray.size(); i++)
                                {
                                    if(customerName==bank.customerArray[i].CustomerName)
                                    {
                                        bank.customerArray[i].setDate(Admin);
                                        employeeNow.depositMoney(bank.customerArray[i],bank,CustomerFile,BankFile);
                                        break;
                                    }
                                }
                                break;
                            }
                            case 4:
                            {
                                string customerName;
                                cout<<"Enter the name of the customer: ";
                                cin>>customerName;
                                for(int i=0; i<bank.customerArray.size(); i++)
                                {
                                    if(customerName==bank.customerArray[i].CustomerName)
                                    {
                                        bank.customerArray[i].setDate(Admin);
                                        employeeNow.withDrawMoney(bank.customerArray[i],bank,CustomerFile,BankFile);
                                        break;
                                    }
                                }
                                break;
                            }
                            case 5:
                            {
                                employeeNow.viewTransactions(bank);
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
                    cout << "Enter the customer user name : ";
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
                        cout<<"Login unsuccessful";
                        login_checker=0;
                    }
                    else
                    {
                        cout << "Successful login" << endl<<endl;
                        int checker_customer=0;
                        while(checker_customer!=1)
                        {
                            cout<<"\ndate : "<<Admin.getDate()<<endl;
                            float annualInterest=Admin.getAnnualSavingInterest();
                            float overdraftCharge=Admin.getOverdraftCharge();
                            cout<<"Annual interest rate is "<<annualInterest<<endl;
                            cout<<"Overdraft charge is "<<overdraftCharge<<endl<<endl;
                            cout<<"Account name : "<<customer.CustomerName<<endl;
                            cout<<"Account type : "<<customer.accountType<<endl;
                            cout<<"Account Contact details : "<<customer.customerContact<<endl<<endl;
                            cout<<"Account balance : "<<customer.moneyDeposit<<endl<<endl;
                            cout<<"Account overdraft : "<<customer.overDraftNow<<endl<<endl;
                            // cout<<"Overdraft charge per day is " <<" % "<<end;
                            cout<<"1. Deposit money in account"<<endl;
                            cout<<"2. Withdraw money from account"<<endl;
                            cout<<"3. View transactions"<<endl;
                            cout<<"4. Logout"<<endl;
                            cout <<"Select an option above"<<endl;
                            cin>>user_input_3;
                            switch(user_input_3)
                            {
                            case 1:
                            {
                                if(customer.isClosed==false)
                                {
                                    customer.setDate(Admin);
                                    customer.depositMoneyFun(bank,CustomerFile,BankFile);
                                    bank.customerArray[customerArrayElement]=customer;
                                }
                                else
                                {
                                    cout<<"Account closed already."<<endl;
                                }
                                break;
                            }
                            case 2:
                            {
                                if(customer.isClosed==false)
                                {
                                    customer.setDate(Admin);
                                    customer.withdrawMoney(bank,CustomerFile,BankFile);
                                    bank.customerArray[customerArrayElement]=customer;
                                }
                                else
                                {
                                    cout<<"Account closed already."<<endl;
                                }
                                break;
                            }
                            case 3:
                            {
                                customer.viewTransaction();
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
            cout<<"Enter the customer name : ";
            cin>>customerUserName;
            for(int i=0; i<bank.customerArray.size(); i++)
            {
                if(customerUserName==bank.customerNameArray[i])
                {
                    bank.customerArray[i].setDate(Admin);
                    depositFromOutside(bank,bank.customerArray[i],CustomerFile,BankFile);
                    isSuccess=1;
                    break;
                }
            }
            if(isSuccess==0)
            {
                cout<<"Transaction unsuccessful"<<endl;
            }
        }
    }
    return 0;
}