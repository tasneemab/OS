from builtins import ValueError, round, float
from tkinter import *
from tkinter import ttk


def convert(*args):
    try:
        value = float(num_entry.get())
        C = round((value - 32) * 5 / 9, 1)
        res.set(C)

    except ValueError:
        pass


window = Tk()
window.title(" Calculator ")
mainframe = ttk.Frame(window, padding="12")
mainframe.grid(sticky=(N, W, E, S))
num = StringVar()
res = StringVar()
num_entry = ttk.Entry(mainframe, width=7, textvariable=num)
num_entry.grid(column=2, row=1, sticky=(W, E))
ttk.Label(mainframe, textvariable=res).grid(column=2, row=2, sticky=(W, E))
ttk.Button(mainframe, text="convert", command=convert).grid(column=4, row=3, sticky=W)
ttk.Label(mainframe, text="Fahrenheit Degree").grid(column=1, row=1, sticky=W)
ttk.Label(mainframe, text="Celsius Degree").grid(column=1, row=2, sticky=W)

for child in mainframe.winfo_children(): child.grid_configure(padx=5, pady=5)

num_entry.focus()

window.bind('<Return>', convert)
window.mainloop()
