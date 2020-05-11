# globals
global firstname
global lastname
global address
global ID
global BALANCE
global accountType

firstname = input("Enter Your First Name, Please!")
lastname = input("Enter Your Last Name, Please! ")
address = input("Enter Your Address, Please! ")
ID = input("Enter Your ID, Please! ")
BALANCE = input("Enter Your Balance, Please! ")
accountType = input("which type you are: r for Regular / s for Student ")


class person(object):
    def __init__(self, first_name, last_name, adress, id_no):
        self.first_name = first_name
        self.last_name = last_name
        self.adress = adress
        self.id_no = id_no


class BankAccount(object):
    def __init__(self, account_no, first_name, last_name, adress, id_no, account_balance):
        self.client = person(first_name, last_name, adress, id_no)
        self.account_no = account_no
        self.account_balance = account_balance

    def __str__(self):
        return "Account:", self.account_no,  "Name:", self.client.first_name, self.client.last_name,\
               "\n", "Balance:", self.account_balance


class StudentAccount(BankAccount):
    def __init__(self):
        BankAccount.__init__(self)

    def __str__(self):
            return "Student account:", self.account_no, "\n " "Name:", self.client.first_name, self.client.last_name,\
                   "\n", "Balance:<", self.account_balance, ""


from random import *

account_no = randint(1, 100)  # Pick a random number between 1 and 100.
# create the file
file = open('history.txt', 'w')
file.write('New account, balance: ')
file.write(str(BALANCE))
file.write("\n")

while True:
    # adding money
    add_to_account = input("Enter the amount you want to add")
    BALANCE = int(BALANCE) + int(add_to_account)
    # writing on the file
    file.write('Deposit: ')
    file.write(str(add_to_account))
    file.write(" ")
    if accountType == 'r':
        BALANCE = int(BALANCE) - int(5)
        file.write(',balance: ')
        file.write(str(BALANCE))
        file.write("\n")
    elif accountType == 's':
        BALANCE = int(BALANCE) - int(3)
        file.write(',balance: ')
        file.write(str(BALANCE))
        file.write("\n ")
    # removing money
    withdraw = input("Enter the amount you want to withdraw")
    if int(withdraw) < BALANCE:
        BALANCE = int(BALANCE) - int(withdraw)
        # writing on the file
        file.write('Withdraw:')
        file.write(str(withdraw))
        file.write(" ")
        if accountType == 'r':
            BALANCE = int(BALANCE) - int(5)
            file.write(',balance: ')
            file.write(str(BALANCE))
            file.write("\n")
        elif accountType == 's':
            BALANCE = int(BALANCE) - int(3)
            file.write('balance: ')
            file.write(str(BALANCE))
            file.write(" ")
        print("You can get the money")
    # checking
    elif int(withdraw) > BALANCE:
        print("you cant get the money- there is no enough money")
    # printing
    if accountType == 's':
        person = BankAccount(account_no, firstname, lastname, address, ID, BALANCE)

    # printing
    elif accountType == 'r':
        person = BankAccount(account_no, firstname, lastname, address, ID, BALANCE)
        print(person.__str__())
    # if the user wants to see the history file
    his_tory = input("you wanna see your history file? Y for yes/ N for no ")
    if his_tory == 'y':
        file = open('history.txt', 'r')
        content = file.read()
        print(content)
    elif his_tory == 'n':
        continue
