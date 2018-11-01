#!/usr/bin/python3
# -*- coding: utf-8 -*-

import numpy as np
import cv2

default_filename = '131206N797_1315_1195_01.vrc'

if __name__ == '__main__':
    filename = default_filename
    with open(filename, 'rb') as file:
        header = file.read(144)
        # todo: parse header!
        data = file.read(768*576*999)
    frame = np.array(bytearray(data), np.uint8).reshape((999, 576, 768))
    for i in range(999):
        cv2.imshow('im', frame[i])
        if cv2.waitKey(40) == ord('q'):
            break

