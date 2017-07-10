#!/usr/bin/env python
# -*- coding: UTF-8 -*-

import cv2

def camera():
	capture = cv2.VideoCapture(0)
	index = 1
	while capture.isOpened():
		# 返回一个元组，value和image，value用来检查是否读入成功
		ok, frame = capture.read()
		if not ok:
			break;
		else:
			cv2.imshow("MyPicture", frame)
			gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
			# cv2.imshow("MyPicture", gray)
			dst = cv2.resize(gray, (92, 112));
			c = cv2.waitKey(100)
			if c & 0xFF == ord('q'):				# q退出
				break;
			elif c & 0xFF == ord('a'):				# a拍照
				filename = '%d.pgm' % index
				index += 1
				cv2.imwrite(filename, dst)
			else:
				continue
	capture.release()
	cv2.destroyAllWindows()

if __name__ == '__main__':
	camera()
