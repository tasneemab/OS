__author__ = 'tasnimab'
import re
web = open("websites.txt")
for i in web:
    emails = re.search(r"(w{3})\.(\w+)\.(\w+)", i)
    if emails:
        print(emails.group(3))
