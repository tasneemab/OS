#File : ex1
# —————————————————————————————————————————————————————————————————————————————————————————————
# written by: Tasneem Abu Odeh, username: tasnimab, id:314902685
#             Hanan Himouni, username: hananhi, id: 318314663
# —————————————————————————————————————————————————————————————————————————————————————————————
#
# This Program runs four functions in infinity loop according to the
# user’s choice from 1 to 4:
# 1: Returns the number of common digits between two numbers.
# input: two numbers.
# output: the number of common digits.
# 2: Edits a text given by the user, it removes, adds and writes every letter and it’s place.
# input: a string.
# output: the string after the editing.
# 3: Finds a solution for the king and his servants riddle.
# input:
# output: the number of the only servant who survives
# 4: returns a coded text for the given one.
# input: a string.
# output: coded string according to caesar coding.
#
# —————————————————————————————————————————————————————————————————————————————————————————————

# This function returns the number of the digits in the first number
# which are exist in the second number.
def getNumberOfDigits():
    # gets the numbers from the user
    number1 = int(input("Enter your first number:\n"))
    number2 = int(input("Enter your second number:\n"))

    # puts the numbers in a lists
    list1 = list(str(number1))
    list2 = list(str(number2))

    # find the length of every number
    len1 = len(str(number1))
    len2 = len(str(number2))
    counter = 0
    for i in range(len1):
        for j in range(len2):
            if list1[i] == list2[j]:
                counter += 1

    # prints the final counter
    if i != len1 and j != len2:
        print("The number of common digits is:", counter)
    print("\n")

# This function edits the string which it get it from the user 
# according to what he wants, he choose i, e or p:
# i: add.
# e: remove.
# p: writes all the letters and it's place.

def stringEditting():
    list1 = input("enter your string:")
    while True:
        pass
        command = input("Now, Enter a char i, e or p to do a command:\n")
        if command == "i":
            string = input("Enter the string you want to add:\n")
            list1 = string[1:] + list1
            print(list1)
        elif command == "e":
            list1 = list1[1::]
            print(list1)

        elif command == "p":
            for i in range(len(list1)):
                print(i, " ", list1[i])
        print("\n")

# This function finds the wanted servant.

def KillTheServants():
    SIZE = 100
    array = []
    for i in range(SIZE):
        array.append(i + 1)

    current = 1
    while len(array) != 1:
        if current == len(array) + 1:
            current = 1
        if current == len(array):
            current = 0
        array.pop(current)
        current = (current % len(array)) + 1
        print(array)
    print("")

# This function codes a text according to caesar coding.

def codedText():
    print("Enter Your sentence:\n")
    sentence = [str(x) for x in input()]
    for x in range(len(sentence)):
        if sentence[x].isalpha():
            sentence[x] = chr(ord(sentence[x]) + 13)
        print(sentence[x], end="")
    print("\n")

# The main part of the program which runs all the sub-missions according to what the user choose between 1-4.
while True:
    choose = int(input("Pick one number between 1 and 4 to run a program:\n"))

    if choose == 1:
        getNumberOfDigits()
    elif choose == 2:
        stringEditting()
    elif choose == 3:
        KillTheServants()
    elif choose == 4:
        codedText()
