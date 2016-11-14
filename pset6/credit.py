__author__ = 'Acheron'

import re
import argparse

class CCard:

    def __init__(self, card_number):
        self.number = str(card_number)
        self.length = len(self.number)
        self.BANK_CODES = {
            'AMEX': {'lead_numbers': (34, 37), 'length': (15, )},
            'MasterCard': {'lead_numbers': (51, 52, 53, 54, 55), 'length': (16, )},
            'Visa': {'lead_numbers': (4, ), 'length': (13, 16)},
        }

        self.patter_bank = re.compile(r'^([4]|\d{2})', re.IGNORECASE)

    def _getBankType(self):
        try:
            b_number = int(re.search(self.patter_bank, self.number).group(0))
            for bank in self.BANK_CODES.keys():
                if b_number in self.BANK_CODES[bank]['lead_numbers']:
                    if self.length in self.BANK_CODES[bank]['length']:
                        return bank
        except:
            pass
        return None

    def _checkCard(self):

        # to short for credit card; 13 is hardcoded and that's bad!
        if self.length < 13:
            return None

        # multiply by 2 every second digit starts from second-to-last
        n_list = []
        i = self.length - 2
        while i >= 0:
            n = str(int(self.number[i]) * 2)
            n_list += [num for num in n]
            i -= 2

        # summarize digits
        s_sum = 0
        for element in n_list:
            s_sum += int(element[0])

        # summarize digits that weren’t multiplied by 2
        part = 0
        i = self.length - 1
        while i >= 0:
            part += int(self.number[i])
            i -= 2

        # if last digit is zero - this is valid card
        try:
            if str(s_sum + part)[1] == '0':
                return True
        except:
            pass
        return False

    def check(self):
        # if valid card - print bank emitent
        if self._checkCard():
            return self._getBankType()
        return False

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('-n', help='number of blocks', nargs=1)
    args = parser.parse_args()

    card_number = int(args.n[0])

    checker = CCard(card_number)
    result = checker.check()
    if result:
        print(result)
    else:
        print('INVALID')