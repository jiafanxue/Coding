#!/usr/bin/env python
# -*- coding: UTF-8 -*-

def createFile():
	# Information
	info = 'E:/WorkFor2017/ML/test_image/att_faces/s{0}/{1}.pgm;{2}\n'

	# Input filename
	filename = input('Enter file name: ')

	fp = open(filename, 'w')

	for i in range(1, 42):
		for j in range(1, 11):
			fp.write(info.format(i, j, i-1))

	fp.close()

if __name__ == '__main__':
	createFile()
