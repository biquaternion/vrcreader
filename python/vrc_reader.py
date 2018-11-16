#!/usr/bin/python3
# -*- coding: utf-8 -*-

import numpy as np
import cv2
import struct
from collections import namedtuple
import sys

default_filename = '131206N797_1315_1195_01.vrc'
default_fps = 25
term_crit = (cv2.TERM_CRITERIA_EPS | cv2.TERM_CRITERIA_COUNT, 10, 1)


def on_click(event, x, y, flags, param):
    strobe = param
    if event == cv2.EVENT_LBUTTONDOWN:
        strobe[0] = x - 20
        strobe[1] = y - 20
        strobe[2] = 40
        strobe[3] = 40


def draw_strobe(im, strobe):
    if strobe[2] == 0:  # todo: invent good check condition
        return
    cv2.rectangle(im,
                  (strobe[0], strobe[1]),
                  (strobe[0] + strobe[2], strobe[1] + strobe[3]),
                  (0, 255, 255), thickness=3)
    cv2.rectangle(im,
                  (strobe[0], strobe[1]),
                  (strobe[0] + strobe[2], strobe[1] + strobe[3]),
                  (255, 0, 0),
                  thickness=1)


def play_all(frames, frame_period=40, paused=False):
    strobe = [0, 0, 0, 0]
    cv2.namedWindow('im')
    cv2.setMouseCallback('im', on_click, strobe)
    for i in range(frames):
        fr = np.copy(frame[i])
        img = cv2.merge(np.array([fr, fr, fr]))
        # img = cv2.equalizeHist(img)
        if strobe[2] != 0:
            ret, s = cv2.meanShift(img[:, :, 0], (strobe[0], strobe[1], strobe[2], strobe[3]), term_crit)
            strobe = [s[0], s[1], s[2], s[3]]
        draw_strobe(img, strobe)
        cv2.imshow('im', img)
        if paused:
            period = 0
        else:
            period = frame_period
        key = cv2.waitKey(period)
        if key == ord('q'):
            return False
        if key == ord(' '):
            paused = not paused


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
    paused = False
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
    # cv2.namedWindow('im')
    # cv2.setMouseCallback('im', on_click, (img, strobe))
    play_all(frames)
    if cv2.waitKey() == ord('r'):
        play_all(frames)
