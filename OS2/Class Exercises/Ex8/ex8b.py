__author__ = 'tasnimab'


def my_enumerate(list):
    i = 0
    for i, a in enumerate(list):
        yield i, a
        i += 1


List = ['a', 'b', 'c']
x = my_enumerate(List)
print(next(x))
print(next(x))
print(next(x))
