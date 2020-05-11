def find_prime():
    # Python program to display all the prime numbers within an interval

    # change the values of lower and upper for a different result
    lower = 1
    upper = 200


    print("Prime numbers between", lower, "and", upper, "are:")
    prim_list = []

    for num in range(lower, upper + 1):
        # prime numbers are greater than 1
        if num > 1:
            for i in range(2, num):
                if (num % i) == 0:
                    break
            else:
                prim_list.append(num)

    for c, value in enumerate(prim_list, 1):
      print(c, value)

find_prime()
