__author__ = 'tasnimab'
import re
text = "There is a large variety of formats for dates in use, which differ in the order of\
date components (e.g. 31/05/1999, 05/31/1999), component separators (e.g.\
31.05.1999, 31/05/1999, 31-05-1999), whether leading zeros are included\
(e.g. 31/5/1999 vs. 31/05/1999) and whether all four digits of the year are\
written (e.g., 31.05.1999 vs. 31.05.99). "

domain_type = re.findall(r"[0-9]{1,2}[\.-/][0-9]{1,2}[\.-/][0-9]{2,4}", text)
for domain in domain_type:
   print(domain)




