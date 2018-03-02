#!/usr/bin/env python3

'''
Author: Patrik Holop
Module with decorator exceptions, generator to merge iterable values
and class Log to catch exceptions
'''

class TooManyCallsError(Exception):
    '''Custom exception for decorator'''
    def __init__(self, message):
        '''initialization of exception with message'''
        self.message = message

def limit_calls(max_calls = 2, error_message_tail='called too often'):
    '''
    Decorator
    :param max_calls: Maximum calls to be done for function
    :param error_message_tail: Message to be printed
    '''
    def limit_calls_decorator(func):
        '''
        Real decorator
        :param func: function to decorate
        '''
        def inner(*args, **kwargs):
            inner.counter += 1
            #if more calls than max_calls are done, raise error
            if (inner.counter>max_calls):
                raise TooManyCallsError('function "' + func.__name__ + '" - ' + error_message_tail)
            #else return calculated value
            return func(*args, **kwargs)
        #reset counter
        inner.counter = 0
        return inner
    return limit_calls_decorator

def ordered_merge(*iterable, selector = []):
    '''
    Function to print values of more iterative objects
    :param iterable: list of iterable object
    :param selector: selector of iterable objects
    :return: list of values
    '''

    #creating copies - iterable objects
    help_list = [iter(x) for x in iterable]
    #final list
    flist = []
    #for each object to iterate
    for index in selector:
       #append new value from selected iterable object
       flist.append(next(help_list[index]))
    #return list of values
    return flist

class Log():
    '''Class representing logging'''
    def __enter__(self):
        '''Function after calling open'''
        self.file.write('Begin\n')
        return self
    def __exit__(self, exc_type, exc_val, exc_tb):
        '''Exiting file'''
        self.file.write('End\n')
        self.file.close()
    def __init__(self, filename):
        '''Creating new file and opening it'''
        self.file = open(filename, "w")
    def logging(self, message):
        '''Writing to log file'''
        self.file.write(message + '\n')