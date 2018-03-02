#!/usr/bin/env python3

import fileinput

words = {}
for line in fileinput.input():
	try:
		words[line.rstrip()]=1
	except:
		pass
    #words.append(line.rstrip())

palindroms = {w:1 for w in set(words) if w == w[::-1]}
reverse = {w:w[::-1] for w in set(words)}

result = []
for w in words:
	try:
		if (palindroms[w])==1:
			pass
	except:
		try:
			result.append(reverse[w[::-1]])
		except: 
			pass 


#result = [w for w in words if w not in palindroms and w[::-1] in words]


print(sorted(result))
