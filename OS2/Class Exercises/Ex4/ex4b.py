def add(x, y):
    return x + y


def subtract(x, y):
    return x - y


def multiply(x, y):
    return x * y


def divide(x, y):
    return x / y


def floordiv(x, y):
    return x // y


def modulus(x, y):
    return x % y


def exponent(x, y):
    return x ** y


funcs = [add,subtract,multiply, divide, floordiv, modulus, exponent]

num1 = int(input("Enter first number: "))
num2 = int(input("Enter second number: "))

item = map(lambda f:f(num1,num2),funcs)

print(list(item))

