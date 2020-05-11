#print the odd numbers in a list
print("The Odd Numbers are:")
list1 = []
for i in range(0,11):
    list1.append(i)
    if(i%2==0):
        print(list1[i], end =" ")