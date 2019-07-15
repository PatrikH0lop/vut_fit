'''
Bonus ISS
author: Patrik Holop
'''

import scipy.signal as signal
import numpy as np 
import soundfile
import matplotlib.pyplot as plt 

from audiolazy.lazy_lpc import *
from audiolazy import z, ZFilter

#1 - 0.204957 * z^-1 - 0.1374 * z^-2 - 0.335082 * z^-3 - 0.043341 * z^-4 + 0.100568 * z^-5
data, fs = soundfile.read('xholop01.wav')

dato = lpc(data, 5);


dat = list(dato(data))	
error = [x-y for x,y in zip(data, dat)]
error = sum([x*x for x in error])
print(error)
num = dato.numerator[1:]
num = [0]+[-x for x in num]
den = [1,0,0,0,0,0]
f2 = ZFilter(num,den)
sig2 = list(f2(data))

error2 = [x-y for x,y in zip(data, sig2)]
error2 = sum([x*x for x in error2])
print(error2)

plt.plot(data, 'r')
plt.plot(sig2,'g')
plt.show()	

print("\nFilter")
print(num)
print(den)	

