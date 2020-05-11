Number1 = int(input("Enter your first number: "))
Number2 = int(input("Enter your second number: "))
print("‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐")
print("CALCULATIONS")
print("‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐")
print("1. Addition")
print("2. Subtraction")
print("3. Multiplication")
print("4. Division")
print("5. Floor Division")
print("6. Modulus")
print("7. Exponent")
print("‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐‐")

command = int(input("Enter your choice [1‐7] : "))

if command == 1:
    result = Number1+Number2
    print("The result is:", result)
elif command == 2:
    result = Number1-Number2
    print("The result is:", result)
elif command == 3:
    result = Number1*Number2
    print("The result is:", result)
elif command == 4:
    result = Number1/Number2
    print("The result is:", result)
elif command == 5:
    result = Number1//Number2
    print("The result is:", result)
elif command == 6:
    result = Number1%Number2
    print("The result is:", result)
elif command == 7:
    result = Number1**Number2
    print("The result is:", result)
else:
    print("Error")

