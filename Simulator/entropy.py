import sys
import numpy as np
import math

def calculateEntropy(data, numBins) :
    hist = np.histogram(data,bins=numBins,range=(-2*math.pi,2*math.pi))[0]
    normed_hist = [x/float(len(data)) for x in hist]

    return -1.0 * sum([h*math.log(h)/math.log(2) for h in normed_hist if h>0])


for line in open(sys.argv[1],'r') :
	entropy = calculateEntropy([float(a) for a in line.split(":")[1].strip().split(",")], 40)
	if(len(sys.argv) > 2) :
		f = open(sys.argv[2], 'w')
		f.write(str(entropy))
		f.write("\n")
	else :
		print entropy
