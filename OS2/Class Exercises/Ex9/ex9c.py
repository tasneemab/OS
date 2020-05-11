__author__ = 'tasnimab'
import re

n = input("enter your name: ")

gro = re.search(r"(\w)(\1)",n)

if gro:
    d = gro.group().upper()
    mo = re.sub(r"(\w)(\1)",d,n)
    print (mo)
