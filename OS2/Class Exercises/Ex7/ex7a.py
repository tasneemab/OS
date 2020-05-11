__author__ = 'tasnimab'


def array(List, val, i, j):
    if i > j:
        raise ValueError('ok')
    if List[i] + List[j - 1] != val:
        raise ValueError('error')
    else:
        array(List, val, i + 1, j - 1)


try:
    x = [1, 2, 3, 4, 5, 6]
    y = 7

    array(x, y, 0, len(x))
except ValueError as e:
    print(e)
