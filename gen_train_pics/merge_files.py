#coding:utf-8
import os

def mergeFiles(outFile, n):
	classPics = ["neg", "part", "pos"]
	dir = "F:/zzzzzLSJ/MTCNN-Tensorflow-master/prepare_data/multi/multi/12/"
	f = open(outFile, 'w')
	for c in classPics:
		for i in range(n):
			f0 = open(dir+"%s_12_%d.txt"%(c, i), "r")
			allLines = f0.readlines()
			for line in allLines:
				f.write(line)
			f0.close()
	f.close()

if __name__ == '__main__':
	outFile = "F:/zzzzzLSJ/MTCNN-Tensorflow-master/prepare_data/multi/multi/12/train_PNet_landmark.txt"
	n = 3
	mergeFiles(outFile, n)
