##########################################################################
#																		 #
# 	This program reads from "graph.csv", produced by modwalksat.cpp		 #
#	Intermediate data manipulation is done using pandas.				 #
#	After that, the graph is plotted using matplotlib.					 #
#	A popup will open with the graph									 #
#																		 #
##########################################################################

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

df = pd.read_csv('graph.csv');

x_series = df.iloc[:,0]
y_series = df.iloc[:,1]
frame = { 'X': x_series, 'Y': y_series }
result = pd.DataFrame(frame)

no_of_bins = 30

store = result.assign(Bin=lambda x: pd.cut(x.X, bins=no_of_bins)).groupby(['Bin']).agg({'Y': ['mean', 'count']})

answer = pd.DataFrame()

#take log of first column in store, which represents the mean values in a bucket
answer['Y(log)'] = np.log(store.iloc[:,0])
rows = len(store.index)
indices = [1]

num = 1
max_range = result['X'].max()
min_range = result['X'].min()
increment = (max_range - min_range) / no_of_bins
for i in range(rows-1):
    num+=increment
    indices.append(num)

indices = np.asarray(indices)
answer['index'] = indices

x = answer['index']
y = answer['Y(log)']

plt.plot(x,y)
plt.xlabel('Clause symbol ratio m/n')
plt.ylabel('log of mean models tried')
plt.show()