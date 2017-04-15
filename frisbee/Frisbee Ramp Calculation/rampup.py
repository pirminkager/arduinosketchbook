import math
def calcfunc(x):
	f=1*(1/(1+math.e**(-0.2*1*x)*((1/0.5)-1)))
	return f
i=-128
while i <= 32:
	value=calcfunc(i)
	value=int(value*255)
	print str(value)+";"
	i=i+1
