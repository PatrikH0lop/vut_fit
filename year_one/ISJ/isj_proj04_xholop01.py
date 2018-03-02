#!/usr/bin/env python3

"""
Module providing functions to encrypt words with
Caesar cipher and check bracket validity
"""

import itertools

def balanced_paren(parenstr):
    '''Function to check brackets validity'''
    bracketsf=['{','(','[','<'] #left brackets
    bracketsb=['}',')', ']','>'] #right brackets
    stackf=[] #stack of left brackets in parenstr
    for char in parenstr:
        #if character is left bracket, add it to stack
        if char in bracketsf:
            stackf.append(char)
        #if character is right bracket,remove the left bracket from stack
        elif char in bracketsb:
            #get index of bracket to remove
            ind = bracketsb.index(char)
            #remove the left bracket from stack
            try:
                c=stackf.pop()
            except:
                #more right than left brackets in string
                return False
            #if popped brackets index is different than index of bracket to remove
            #brackets are not correct
            if bracketsf.index(c)!=ind:
                return False
    #if nothing left in the stack, bracket are correct
    if len(stackf)==0:
        return True
    return False


def caesar_list(word, key=[1,2,3]):
    '''Function to encrypt word with Caesars cipher: key is list'''
    #final encrypted string
    s = ""
    alphabet="abcdefghijklmnopqrstuvwxyz"
    #for each char in word
    for char in word:
        #if character is not 'a-z', throw exception
        if not char in alphabet:
            #throw exception
            raise ValueError('Word contains non-lower characters')
    #encrypting char in word with number x from key list
    for char,x in zip(word, itertools.cycle(key)):
        #ascii position of encrypted letter
        pos=ord(char)+x
        #if number is key list is too high, overflow it
        while pos>122:
            #decrease position
            pos=pos-26
        #adding encrypted letter to final string
        s=s+chr(pos)
    #return final string
    return s


def caesar_varnumkey(word, *key):
    '''Function to encrypt word with Caesars cipher: keys are numbers'''
    key = list(key) #list from number arguments
    #if no number is passed, set default list as key
    if len(key)==0:
        key=[1,2,3]
    s="" #final encrypted string
    alphabet = "abcdefghijklmnopqrstuvwxyz"
    for char in word:
        #if character is not 'a-z', throw exception
        if not char in alphabet:
            raise ValueError('Word contains non-lower characters')
    #encrypting char in word with number x from key list
    for char,x in zip(word, itertools.cycle(key)):
        #ascii position of encrypted letter
        pos=(ord(char)+x)
        #if number is key list is too high, overflow it
        while pos>122:
            pos=pos-26
        #adding encrypted letter to final string
        s=s+chr(pos)
    #return final string
    return s
