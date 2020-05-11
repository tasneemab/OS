

def dividedBy5Or7(x):
    if x % 5 == 0 and x % 7 == 0:
        return True
    return False


numbers = list(range(1,200))
result = (list(filter(dividedBy5Or7, numbers)))
print(result)