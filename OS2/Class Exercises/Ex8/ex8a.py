__author__ = 'tasnimab'


class reverse:
    def reverse_i(list):
        i = len(list)
        while i >= 0:
            yield i
            i -= 1


List = [1, 2, 3, 4]
x = reverse.reverse_i(List)
print(next(x))
print(next(x))
print(next(x))
print(next(x))
