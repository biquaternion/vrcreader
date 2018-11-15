#!/usr/bin/python3
# -*- coding: utf-8 -*-

import numpy as np
import cv2
import struct
from collections import namedtuple
import sys

default_filename = '131206N797_1315_1195_01.vrc'
default_fps = 25


def parse_header(header_data):
    struct_fmt = '4s14i68s'
    Header = namedtuple('Header', ['fsign',
                                   'f_version',
                                   'f_frames',
                                   'f_width',
                                   'f_height',
                                   'f_imagetype',
                                   'f_bits',
                                   'f_ofsR',
                                   'f_bitsR',
                                   'f_ofsG',
                                   'f_bitsG',
                                   'f_ofsB',
                                   'f_bitsB',
                                   'f_interval',
                                   'f_strorder',
                                   'f_reserved'])
    header = Header._make(struct.unpack(struct_fmt, header_data))
    return header


if __name__ == '__main__':
    filename = default_filename
    fps = default_fps
    if len(sys.argv) > 1:
        filename = sys.argv[1]
    if len(sys.argv) > 2:
        fps = int(sys.argv[2])
    frame_period = max(1, int(1000 / fps))
    with open(filename, 'rb') as file:
        header_data = file.read(128)
        header = parse_header(header_data)
        frames = header.f_frames
        width = header.f_width
        height = header.f_height
        data = file.read(width * height * frames)
    frame = np.array(bytearray(data), np.uint8).reshape((frames, height, width))
    for i in range(frames):
        cv2.imshow('im', frame[i])
        if cv2.waitKey(frame_period) == ord('q'):
            break
