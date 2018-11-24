#!/usr/bin/python3
# -*- coding: utf-8 -*-

import numpy as np
import cv2
import struct
from collections import namedtuple
import sys

default_filename = '131206N797_1315_1195_01.vrc'
default_fps = 25


class Strobe:
    def __init__(self, left=0, top=0, width=0, height=0):
        self.rect = (left, top, width, height)
        self.ready = False
        self.term_crit = (cv2.TERM_CRITERIA_EPS | cv2.TERM_CRITERIA_COUNT, 10, 1)

    def modify(self, rect, make_ready=True):
        if make_ready:
            self.ready = True
        self.rect = rect

    def center(self):
        return self.rect[0] + self.rect[2] // 2, self.rect[1] + self.rect[3] // 2

    def calc_new_position(self, im):
        ret, new_pos = cv2.meanShift(im[:, :, 0], self.rect, self.term_crit)
        if ret:
            # todo: check SNR inside strobe to move it
            self.rect = new_pos

    def draw(self, im):
        if self.ready:
            cv2.rectangle(im,
                          (self.rect[0], self.rect[1]),
                          (self.rect[0] + self.rect[2], self.rect[1] + self.rect[3]),
                          (0, 255, 255), thickness=3)
            cv2.rectangle(im,
                          (self.rect[0], self.rect[1]),
                          (self.rect[0] + self.rect[2], self.rect[1] + self.rect[3]),
                          (255, 0, 0), thickness=1)


def on_click(event, x, y, flags, param):
    strobe = param
    if event == cv2.EVENT_LBUTTONDOWN:
        print('new strobe position', x, y)
        tb_pos = cv2.getTrackbarPos("strobe size", "im")
        strobe.modify((x - tb_pos // 2, y - tb_pos // 2, tb_pos, tb_pos))


def play_all(frames, frame_period=40, paused=False):
    strobe = Strobe(0, 0, 0, 0)
    cv2.namedWindow('im')

    def on_trackbar(param):
        tb_pos = param
        x, y = strobe.center()
        strobe.modify((x - tb_pos // 2, y - tb_pos // 2, tb_pos, tb_pos), False)

    cv2.createTrackbar('strobe size', 'im', 40, 255, on_trackbar)
    cv2.setMouseCallback('im', on_click, strobe)
    for i in range(frames):
        fr = np.copy(frame[i])
        img = cv2.merge(np.array([fr, fr, fr]))
        # img = cv2.equalizeHist(img)
        if strobe.ready:
            strobe.calc_new_position(img)
            strobe.draw(img)
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
