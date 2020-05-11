# ---------------------------------------------------------------------
# This class has all the information about every product
import self as self


class Product(object):
    def __init__(self, code, name, amount, minAmount, price):
        self.__code = code
        self.__name = name
        self.__amount = amount
        self.__minAmount = minAmount
        self.__price = price

    @property
    def code(self):
        return self.__code

    @property
    def name(self):
        return self.__name

    @property
    def amount(self):
        return self.__amount

    @property
    def minAmount(self):
        return self.__minAmount

    @property
    def price(self):
        return self.__price

    @price.setter
    def price(self, value):
        self.__price = value

    @minAmount.setter
    def minAmount(self, value):
        self.__minAmount = value

    @amount.setter
    def amount(self, value):
        self.__amount = value

    @name.setter
    def name(self, value):
        self.__name = value

    @code.setter
    def code(self, value):
        self.__code = value

    @property
    def __str__(self):
        return self.name


# ---------------------------------------------------------------------
# This class has a lists of items in the store


class ProductsList(object):

    def __init__(self):
        self.items = []

    @property
    def __str__(self):
        return str(self.items.pop(0))

    # add items to the store list
    def add(self, item):
        self.items.append(item)

    # checks which items are less that the minimum amount
    def checkAmount(self):
        for x in self.items:
            if x.minAmount > x.amount:
                print("Please Order", x.minAmount - x.amount, "From the Product:", x.code)

    # gives a report about each item
    def report(self, name):
        for x in self.items:
            if x.name == name:
                print(x.name, ": ", x.code, ", ", x.amount, ", ", x.price)

    # returns the price of a given code
    def prices(self, code):
        for x in self.items:
            if x.code == code:
                print("Item Price:", x.price)

    # returns the number of items which the amount of them is less than the given amount
    def amounts(self, amount):
        counter = 0
        for x in self.items:
            if x.amount < amount:
                counter += 1

        return counter


# The main function that runs the whole script

def main():
    products = ProductsList()
    product1 = Product(4455, "Snacks & Candies", 10, 40, 15)
    product2 = Product(2390, "Fruits & Vegetables", 60, 50, 25)
    product3 = Product(6582, "Milk Dairies", 200, 100, 35)

    products.add(product1)
    products.add(product2)
    products.add(product3)

    print("------------------------")
    products.checkAmount()
    print()

    print("------------------------")
    products.report("Fruits & Vegetables")
    print()

    print("------------------------")
    products.prices(4455)
    print()

    print("------------------------")
    print(products.amounts(80))


if __name__ == '__main__':
    main()
