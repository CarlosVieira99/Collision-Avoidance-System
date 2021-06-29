#!/usr/bin/env python
import rospy
import sys
import serial
import numpy as np
import math
import time
from sensor_msgs.msg import LaserScan

def laserDisplay():
    pub = rospy.Publisher('LASERdata', LaserScan, queue_size=30)
    rospy.init_node('publisher', anonymous=False)
    rate = rospy.Rate(1) # 1hz

    ser = serial.Serial('/dev/ttyUSB3', 115200, timeout=5)
    initStr = sys.argv[1]+','+sys.argv[2]+','+sys.argv[3]+',\n'
    #print(initStr)
    ser.write(initStr.encode("UTF-8"))

    while not rospy.is_shutdown():
        dist=[]
        stre=[]
        data = []
        #ser = serial.Serial('/dev/ttyUSB1', 115200, timeout=5)
        data = ser.readline().decode("UTF-8")[:-2]
        data = data.split(',')
        for i in range(1, len(data), 2):
            print(data)
            dist.append(float(data[i]))
            stre.append(float(data[i+1]))

        #print(data)
        if(data[0]=='lr'):
            dist = dist[::-1]
            stre = stre[::-1]    
        laser = LaserScan()

        #laser.header.seq = 0
        laser.header.stamp.secs = time.time()
        laser.header.frame_id = "laserTestID"

        laser.angle_min = float(sys.argv[1])*2*math.pi/4095
        laser.angle_max = float(sys.argv[2])*2*math.pi/4095
        laser.angle_increment = float(sys.argv[3])*0.0879*math.pi/180
        laser.time_increment = 1.0
        laser.scan_time = 1.0
        laser.range_min = 0.0
        laser.range_max = 12000.0
        laser.ranges = dist
        laser.intensities = stre
   
        laser.header.seq += 1
        laser.header.stamp.secs = time.time()
        rospy.loginfo(laser)
        pub.publish(laser)
        rate.sleep()


if __name__ == '__main__':
	try:
		laserDisplay()
	except rospy.ROSInterruptException:
		pass
