#!/usr/bin/python
# coding: utf-8
'''
Created on 30 de dic. de 2018
@author: espinosa
'''

import struct
import RPi.GPIO as GPIO
import time

#Pins to use (BCM MODE)
XB_PIN = 12
XA_PIN = 13
YA_PIN = 26
YB_PIN = 19
LB_PIN = 21
RB_PIN = 20

GPIO.setmode(GPIO.BCM)

GPIO.setup(XB_PIN,GPIO.OUT)
GPIO.setup(XA_PIN,GPIO.OUT)
GPIO.setup(YA_PIN,GPIO.OUT)
GPIO.setup(YB_PIN,GPIO.OUT)
GPIO.setup(LB_PIN,GPIO.OUT)
GPIO.setup(RB_PIN,GPIO.OUT)

file = open( "/dev/input/mouse0", "rb" )

oldL = 0
oldR = 0
velocityX = 1
velocityY = 1
ourDelayX = 0.0028
ourDelayY = 0.0028
velX = 45
velY = 45

GPIO.output(LB_PIN, GPIO.HIGH)
GPIO.output(RB_PIN, GPIO.HIGH)

def buttonAction (bLeft,bRight):
  global oldL, oldR
  if (bLeft!=oldL):
    if (bLeft==1):
      GPIO.output(LB_PIN, GPIO.LOW)
    else:
      GPIO.output(LB_PIN, GPIO.HIGH)
    oldL = bLeft
  if (bRight!=oldR):
    if (bRight==1):
      GPIO.output(RB_PIN, GPIO.LOW)
    else:
      GPIO.output(RB_PIN, GPIO.HIGH)
    oldR = bRight

def moveAction (x,y):
  global velocityX,velocityY,ourDelayX,ourDelayY,velX,velY
  if abs(y)>1 :
    vel = velocityY+(abs(y)/velY)
    for i in range(vel):
      if y<0 :
        moveDown()
      else :
        moveUp()
  if abs(x)>1 :
    vel = velocityX+(abs(x)/velX)
    for i in range(vel):
      if x<0 :
        moveLeft ()
      else:
        moveRight ()

def sendMove (first,second,delay):
  #Send signals acording parameters
  GPIO.output(first,GPIO.HIGH)
  time.sleep(delay)
  GPIO.output(second,GPIO.HIGH)
  time.sleep(delay)
  GPIO.output(first,GPIO.LOW)
  time.sleep(delay)
  GPIO.output(second,GPIO.LOW)
  time.sleep(delay)

def moveLeft ():
  sendMove (XB_PIN,XA_PIN,ourDelayY)

def moveRight ():
  sendMove (XA_PIN,XB_PIN,ourDelayY)

def moveUp ():
  sendMove (YA_PIN,YB_PIN,ourDelayX)

def moveDown ():
  sendMove (YB_PIN,YA_PIN,ourDelayX)

def getMouseEvent():
  buf = file.read(3)
  button = ord( buf[0] )
  bLeft = button & 0x1
  bMiddle = ( button & 0x4 ) > 0
  bRight = ( button & 0x2 ) > 0
  x,y = struct.unpack( "bb", buf[1:] )
  #print ("L:%d, M: %d, R: %d, x: %d, y: %d\n" % (bLeft,bMiddle,bRight, x, y) )
  buttonAction (bLeft,bRight)
  moveAction(x,y)
  

while( 1 ):
  getMouseEvent()

file.close()
