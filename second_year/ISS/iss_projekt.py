'''
author: Patrik Holop
about: Signal processing using numpy and scipy
'''

import wave
from scipy.fftpack import fft
from scipy.io import wavfile
import numpy

wavname = "xholop01.wav"
outputname = "xholop01_filtered.wav"

#1. general info
with wave.open(wavname, "r") as signal:
    wav_info = signal.getparams()
    sampling_rate = wav_info[2]
    samples = wav_info[3]
    duration = sampling_rate/samples
    print("\nTask 1 (General info)\nFile: {}\nSampling rate: {} [samp/s]\nSamples: {}\nDuration: {} [s]\n".format(wavname, sampling_rate, samples, duration))

#2. spectre
import matplotlib.pyplot as plt
import soundfile as sf
data, fs = sf.read(wavname) #wavfile.read(wavname)
plt.plot(data)
plt.show()
print("Task 2 (Spectre with fft)\nSample rate: {}\n".format(fs))
spectre = fft(data)
spectre = numpy.abs(spectre)
spectre = spectre[0:int(len(spectre)/2)]
plt.xlabel("Frequency [Hz]")
plt.ylabel("Module")
plt.plot(spectre, 'b')
plt.show()

#3. Max of module
frequency = numpy.argmax(spectre)
print("Task 3 (Maximum of module)\nFrequency: {} [Hz]\n".format(frequency))

#4. Poles and zeros
plt.gcf().clear()
from control import matlab, TransferFunction
from numpy  import real, imag
num = [0.2324, -0.4112, 0.2324]
den = [1, 0.2289, 0.4662]
transfer_function = TransferFunction(num, den)
p = transfer_function.pole()
z = transfer_function.zero()
ax = plt.gca()
patch = plt.Circle((0,0),1,color='b',alpha=0.2)
ax.add_patch(patch)
if len(p) > 0:
    plt.scatter(real(p), imag(p), s=50, marker='x')
if len(z) > 0:
    plt.scatter(real(z), imag(z), s=50, marker='o',
                facecolors='none', edgecolors='g')
plt.axhline(y=0, color='black')
plt.axvline(x=0, color='black')
plt.xlabel('Re')
plt.ylabel('Im')
plt.title("Poles & zeros")
plt.show()
print("Task 4 (IIR filter)")
print(transfer_function)

#5. Kmitoctove characteristika
from scipy import signal
print("Task 5 (Kmitocet)\n")
num = numpy.asarray(num)
den = numpy.asarray(den)
w,h = numpy.abs(signal.freqz(num, den, worN=8000))
plt.plot(h, 'r')
plt.show()

#6. Apply filter to signal and generate it
print("Task 6 (Applying filter)\n")
filtered = signal.lfilter(num, den, data)
spectre = numpy.abs(fft(filtered))
spectre = spectre[0:int(len(spectre)/2)]
plt.xlabel("Frequency [Hz]")
plt.ylabel("Module")
plt.plot(spectre, 'b')
plt.show()
sf.write(outputname, filtered, fs)

#7. Max module (frequency)
frequency = numpy.argmax(spectre)
print("Task 7 (Maximum of module (filtered))\nFrequency: {} [Hz]\n".format(frequency))

#8. Finding noise
print("Task 8 (Locating noise)\n")
plt.gcf().clear()
noise = [1,1,-1,-1]*80
while (len(noise)<fs):
    noise.append(0)
new_signal = numpy.correlate(data, noise, "full")
new_signal = new_signal[int(len(new_signal)/2):]
plt.xlabel("Frequency [Hz]")
plt.ylabel("Module")
plt.plot(new_signal, 'r')
start_hz = numpy.argmax(new_signal)
print("Maximum: {} [Hz]\n".format(start_hz))
print("         {} [s]".format(start_hz/fs))
plt.show()
#RESULT: 7680 Hz

#9. Autocorrelation
k = 50
plt.gcf().clear()
print("Task 9 (Autocorrelation)\n")
new_signal = signal.correlate(data, data, mode='full')/fs
x = [x for x in range(-fs,fs-1)]
plt.plot(x[fs-k:fs+k+1], new_signal[fs-k:fs+k+1], 'b')
plt.show()

#10. R[10]
k = 10
r_10 = new_signal[fs+k-1]
print("Task 10 (R[10])\nHodnota R[10]: {}\n".format(r_10))

#11.
print("Task 11: Probability density histogram\n")
plt.gcf().clear()
k2 = 10

doubles = []
length = len(data)

for i in range(0, length):
    if k2+i > length-1:
        break
    doubles.append(data[i+k2])

for i in range(0, k2):
    doubles.append(0)

z = plt.hist2d(data, doubles, bins=40)
histogram_array = z[0]
new_histogram = []
for line in histogram_array:
    new_histogram.append([])
    for column in line:
        new_histogram[-1].append(column/fs/(1/40*1/40))
plt.gcf().clear()
plt.imshow(new_histogram)
plt.colorbar()
plt.show()

#12.
print("Task 12: Test of probability function")
summary = 0
for line in new_histogram:
    for number in line:
        summary += number*(1/40*1/40)
print("Sum:", summary)

#13. R[10]
summary = sum([data[i]*doubles[i] for i in range(0, len(data))])/len(data)
print("\nTask 13:")
print("R[10]:", summary)
