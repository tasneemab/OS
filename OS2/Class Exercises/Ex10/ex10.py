from tkinter import *
from tkinter import ttk


window = Tk()
window.title("Welcome to Ticket Booking app")
window.geometry("400x450")

lbl = Label(window, text="Destination Countries ")
listbox = Listbox(window, height=4, bd=2)

listbox.insert(END)

for item in ["India", "UK", "Ireland", "Spain"]:
    listbox.insert(END, item)




lbl.pack()
listbox.pack()
window.mainloop()
