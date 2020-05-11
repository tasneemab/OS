 #finds if there is a common number between two lists
list1 = [int(x) for x in input().split()]
list2 = [int(x) for x in input().split()]

result = False
for x in list1:
    for y in list2:
        if x == y:
            result = True
            break

print(result)
