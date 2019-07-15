#!/usr/bin/env python3
import itertools

'''
Author: Patrik Holop
Module with functions to find math operations over numbers to get certain result
and find first nonrepeating character in string
'''

def first_nonrepeating(string):
    '''
    Function to find first nonrepeating char in string
    :param string: String to process
    :return: First nonrepeating character, or None
    '''
    #for each character in string
    for x in string:
        #if it's count is only 1 in string, it's result
        if string.count(x)==1:
            return x
    #if none was found, return None
    return None

def combine4(numbers, result):
    '''
    Function to find math operations with numbers to get result
    :param numbers: List of numbers
    :param result: Expected result
    :return: list of operations
    '''
    results = [] #final result
    operations = ['+','-','*','/']

    #test every permutation of numbers
    for number in itertools.permutations(numbers):
        #test every variation of math operations between numbers
        op = itertools.product(operations, repeat=4)
        #for each posibility
        for sign in op:
            #string contains list of all combinations of brackets
            #for given combination of numbers and signs
            string = []
            string.append(str(number[0])+sign[0]+str(number[1])+sign[1]+str(number[2])+sign[2]+str(number[3]))
            string.append('(' + str(number[0])+sign[0]+str(number[1])+')'+sign[1]+str(number[2])+sign[2]+str(number[3]))
            string.append('(' + str(number[0]) + sign[0] + str(number[1]) + sign[1] + str(number[2]) + ')' + sign[2] + str(number[3]))
            string.append(str(number[0])+ sign[0]+ '(' + str(number[1])+sign[1]+str(number[2])+')' +sign[2]+str(number[3]))
            string.append(str(number[0]) + sign[0] + '(' + str(number[1]) + sign[1] + str(number[2]) + sign[2] + str(number[3]) +')')
            string.append(str(number[0]) + sign[0] + str(number[1]) + sign[1] + '(' + str(number[2]) + sign[2] + str(number[3]) + ')')
            #for each final math formula
            for formula in string:
                #try to calculate result, otherwise pass
                try:
                    #if result is equal to second param, add it to final list
                    if (eval(formula)==result):
                        results.append(formula)
                except:
                    pass
            #using only first 3 signs in permutations of 4 operations, only every fourth combination is needed
            next(op)
            next(op)
            next(op)
    return results