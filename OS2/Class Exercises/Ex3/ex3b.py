# This function builds a list from the users data.

def createList():
    tempInput = [int(x) for x in input("Enter a number").split()]
    return tempInput


List1 = createList()
List2 = createList()
result = []

for a, b in zip(List1, List2):
    if a ** 2 == b:
        result.append(a + b)
    else:
        exit(0)

print(result)
