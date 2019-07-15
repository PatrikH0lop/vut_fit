#!/usr/bin/env python3

'''
File with implemented polynomial class
Author: Patrik Holop
'''

class Polynomial:
    '''Class polynomial representic polynomial as list'''
    def __init__(self, *init_arg, **kwargs):
        ''' Initialization of Polynomial
        :param init_arg: initializing list or numbers
        :param kwargs: initializing values
        '''
        #if polynomial is not set by assigning to variables
        if len(kwargs)==0:
            #if polynomial is set by list
            if type(init_arg[0]) is list:
                self.polynom = init_arg[0]
            #if polynomial is set by numbers
            elif type(init_arg) is tuple:
                self.polynom = list(init_arg)
        #if polynomial is set by assigning to variables
        else:
            max=0 #highest power
            self.polynom=[]
            #find highest power in kwargs
            for x in kwargs:
                #if number after 'x' if higher than max
                if int(x[1:])>max:
                    #set new max
                    max=int(x[1:])
            #initiliaze polynom to nulls
            for i in range(0, max+1):
                self.polynom.append(0)
            #assign values to set powers
            for x in kwargs:
                #to position after 'x' assign value
                self.polynom[int(x[1:])]=kwargs[x]
        #delete the sequence of null of highest powers of polynom
        while len(self.polynom)>0 and self.polynom[len(self.polynom) - 1] == 0:
            self.polynom.pop()
        #if whole polynom was set by zeros
        if len(self.polynom)==0:
            self.polynom=[0]

    def __str__(self):
        '''Converting polynomial to human readable string'''
        #return null if its null polynom
        if self.polynom==[0]:
            return "0"
        i=0 #index of power in polynom list
        res = "" #final string
        sign="+" #sign of numbers

        #iterate through polynom
        while i<len(self.polynom):
            #set the sign of current number
            if self.polynom[i]>0:
                sign=" + "
            else:
                sign=" - "
            #dont print power if its zero
            if self.polynom[i]==0:
                pass
            #highest power of polynom
            elif i==len(self.polynom)-1:
                #1 is special case
                if i==1:
                    #dont print number if value is 1
                    if abs(self.polynom[i]) == 1:
                        res = 'x' + res
                    else:
                        res = str(self.polynom[i]) + 'x' + res
                #if highest power is 1
                elif abs(self.polynom[i])==1:
                    res = 'x^' + str(i) + res
                else:
                    res = str(self.polynom[i]) + 'x^' + str(i) + res
            #middle power in polynom
            elif i>1:
                #dont print number if value is 1
                if abs(self.polynom[i])==1:
                    res = sign + 'x^' + str(i) + res
                else:
                    res = sign + str(abs(self.polynom[i])) + 'x^' + str(i) + res
            #power of 1, without ^
            elif i==1:
                if abs(self.polynom[i])==1:
                    res = sign + 'x' + res
                else:
                    res = sign + str(abs(self.polynom[i])) + 'x' + res
            #first power x^0, number
            else:
                res = sign + str(abs(self.polynom[i])) + res
            #increase number
            i=i+1
        return res

    def __eq__(self, other):
        '''Function to compare equality two polynomials
        :param other: second polynom
        '''
        #comparing their lists
        return self.polynom==other.polynom

    def __add__(self, other):
        '''Function to add two polynomials
        :param second polynom
        '''
        resultpolynom = [] #final polynom
        #find longer polynom
        if len(self.polynom)>len(other.polynom):
            shorter = other.polynom
            longer = self.polynom
        else:
            longer = other.polynom
            shorter = self.polynom
        #initialize result list to length of longer polynom
        for i in range(0,len(longer)):
            resultpolynom.append(longer[i]);
        #calculate the result power value
        for i in range(0, len(shorter)):
            resultpolynom[i]=resultpolynom[i]+shorter[i]
        #return new polynomial
        return Polynomial(resultpolynom)

    def srt(self,l1,l2):
        '''Function to multiply two polynomials represented by list
        :param l1: first list
        :param l2:_second list
        '''
        #calculate length of final polynom
        res = [0] * (len(l1) + len(l2) - 1)
        #values in first list
        for number1, value1 in enumerate(l1):
            #values in second list
            for number2, value2 in enumerate(l2):
                #assign result to position in final polynom
                res[number1 + number2] += value1 * value2
        #return final string
        return res

    def __pow__(self, power, modulo=None):
        '''Function to calculate powered polynomial
        :param power: polynom powered by this number
        :param modulo: modulo of pow function
        '''
        #everything powered by 0 is 1
        if power==0:
            return Polynomial([1])
        #x powered by 1 is x
        elif power==1:
            return Polynomial(self.polynom)
        #calculate powe
        else:
            #final result
            respol=self.polynom
            #copy of polynom
            s2 = self.polynom
            #keep multiplying polynom with its copy power times
            for i in range(1,power):
                #multiply polynom by itself
                respol=self.srt(respol,s2)
            #return final polynom
            return Polynomial(respol)

    def derivative(self):
        '''Function to calculate derivative from polynomial'''
        res = [] #result
        #calculate derivation for each power in polynom
        for i in range(0,len(self.polynom)):
            #f'=n*x^(n-1)
            res.append(self.polynom[i]*i)
        #return final polynom
        return Polynomial(res[1:])

    def at_value(self, *cislo):
        '''Function to calculate value of polynomial for given point
        :param cislo: value of x or value of x1 and x2
        '''
        res = 0 #final result
        #calculate basic value of polynom
        if len(cislo)==1:
            #for each power in polynom
            for i in range(0,len(self.polynom)):
                #calculate value
                res += cislo[0]**i * self.polynom[i]
            #return final result
            return res
        #calculate difference between 2 polynoms
        elif len(cislo)==2:
            res1 = 0 #result of first value
            res2 = 0 #result of second value
            #calculate final value for first argument
            for i in range(0,len(self.polynom)):
                res1 += cislo[0]**i * self.polynom[i]
            #calculate final value for second argument
            for i in range(0, len(self.polynom)):
                res2 += cislo[1] ** i * self.polynom[i]
            #return difference
            return res2 - res1

def test():
    assert str(Polynomial(0,1,0,-1,4,-2,0,1,3,0)) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x"
    assert str(Polynomial([-5,1,0,-1,4,-2,0,1,3,0])) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x - 5"
    assert str(Polynomial(x7=1, x4=4, x8=3, x9=0, x0=0, x5=-2, x3= -1, x1=1)) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x"
    assert str(Polynomial(x2=0)) == "0"
    assert str(Polynomial(x0=0)) == "0"
    assert Polynomial(x0=2, x1=0, x3=0, x2=3) == Polynomial(2,0,3)
    assert Polynomial(x2=0) == Polynomial(x0=0)
    assert str(Polynomial(x0=1)+Polynomial(x1=1)) == "x + 1"
    assert str(Polynomial([-1,1,1,0])+Polynomial(1,-1,1)) == "2x^2"
    pol1 = Polynomial(x2=3, x0=1)
    pol2 = Polynomial(x1=1, x3=0)
    assert str(pol1+pol2) == "3x^2 + x + 1"
    assert str(pol1+pol2) == "3x^2 + x + 1"
    assert str(Polynomial(x0=-1,x1=1)**1) == "x - 1"
    assert str(Polynomial(x0=-1,x1=1)**2) == "x^2 - 2x + 1"
    pol3 = Polynomial(x0=-1,x1=1)
    assert str(pol3**4) == "x^4 - 4x^3 + 6x^2 - 4x + 1"
    assert str(pol3**4) == "x^4 - 4x^3 + 6x^2 - 4x + 1"
    assert str(Polynomial(x0=2).derivative()) == "0"
    assert str(Polynomial(x3=2,x1=3,x0=2).derivative()) == "6x^2 + 3"
    assert str(Polynomial(x3=2,x1=3,x0=2).derivative().derivative()) == "12x"
    pol4 = Polynomial(x3=2,x1=3,x0=2)
    assert str(pol4.derivative()) == "6x^2 + 3"
    assert str(pol4.derivative()) == "6x^2 + 3"
    assert Polynomial(-2,3,4,-5).at_value(0) == -2
    assert Polynomial(x2=3, x0=-1, x1=-2).at_value(3) == 20
    assert Polynomial(x2=3, x0=-1, x1=-2).at_value(3,5) == 44
    pol5 = Polynomial([1,0,-2])
    assert pol5.at_value(-2.4) == -10.52
    assert pol5.at_value(-2.4) == -10.52
    assert pol5.at_value(-1,3.6) == -23.92
    assert pol5.at_value(-1,3.6) == -23.92

if __name__ == '__main__':
    test()