__author__ = 'tasnimab'


def sum(x, d, e, n, t):
    if (x ** n) / t < d:
        raise ValueError('ok {} {}'.format(e, n))

    e += (x ** n) / t
    sum(x, d, e, n + 1, t * (t + 1))


try:
    x = 0.5
    d = 0.0001
    sum(x, d, 1, 1, 1)
except ValueError as e:
    print(e)
