def positive(x):
    if x.isdigit() and int(x) > 0:
            return x


userInput = input("Enter an input: \n").split()


result = list(filter(positive, userInput))
print(result)
