#! /usr/bin/python3

import re

def map_digit(input):
    if input == '1' or input == 'one':
        return 1
    if input == '2' or input == 'two':
        return 2
    if input == '3' or input == 'three':
        return 3
    if input == '4' or input == 'four':
        return 4
    if input == '5' or input == 'five':
        return 5
    if input == '6' or input == 'six':
        return 6
    if input == '7' or input == 'seven':
        return 7
    if input == '8' or input == 'eight':
        return 8
    if input == '9' or input == 'nine':
        return 9

with open('long_input', 'rt') as fin:
    acc = 0
    while (line := fin.readline()):
        fdigit = (-1, -1)
        ldigit = (-1, -1)
        for d in ['one', 'two', 'three', 'four', 'five', 'six', 'seven', 'eight', 'nine', '1', '2', '3', '4', '5', '6', '7', '8', '9']:
            index = line.find(d)
            if index >= 0:
                if fdigit[0] == -1 or fdigit[0] > index:
                    fdigit = (index, map_digit(d))
            index = line.rfind(d)
            if index >= 0:
                if ldigit[0] == -1 or ldigit[0] < index:
                    ldigit = (index, map_digit(d))
        acc += fdigit[1] * 10 + ldigit[1]
    print(acc)
