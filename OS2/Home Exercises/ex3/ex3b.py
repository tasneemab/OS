import re

log = open("atoms2.log")
out = open("out.txt", "w")
for line in log:
    name = re.search(r"RUN.([\d]+).COMPLETED. OUTPUT IN FILE ([a-z]+).dat. (\d+\.)",line)
    if name:
        if name.group(3) is not None:
            out.write(name.group(2,1) + "\n")

out.close()
log.close()

