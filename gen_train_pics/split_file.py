#coding:utf-8
import os

def split(n):
	f = open("wider_face_train.txt", "r")
	allPics = f.readlines()
	numPics = len(allPics)
	num = numPics//n
	i = 0

	for i in range(n):
		f = open("wider_face_train_"+"%d"%i + ".txt", "w")

		if i < n-1:
			for j in range(num):
				f.write(allPics[j + num*i])
		else:
			for j in range(numPics - num * (n-1)):
				f.write(allPics[j + num * i])
	f.close()

if __name__ == '__main__':
	n = 3
	split(3)
