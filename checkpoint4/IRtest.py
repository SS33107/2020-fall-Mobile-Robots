#!/usr/bin/env python
import rospy
import sys
import time
import RPi.GPIO as GPIO
from std_msgs.msg import String
from std_msgs.msg import Bool

IR = 27

GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)
GPIO.setup(IR, GPIO.IN)

def talker():
    pub_IR = rospy.Publisher('IR_found', Bool, queue_size=10)
    rospy.init_node('talker', anonymous=True)
    rate = rospy.Rate(25)
    while not rospy.is_shutdown():
      found2 = False
      count = 0
      for i in range(9):
        lastms = time.time() * 1000
        one = 0
        zero = 0
        while (time.time() * 1000)-lastms <17:
          signal = GPIO.input(IR)
          if signal :
            one = one + 1
          else:
            zero = zero + 1
        IR_rate = float(zero)/(one+zero)
        print "%.3f" %IR_rate
        if IR_rate>0.27 and IR_rate <0.33:
          count = count + 1
          if count >=3:
            found2 = True
        rate.sleep()
      pub_IR.publish(found2)
                
if __name__ == '__main__':
    try:
        talker()

    except rospy.ROSInterruptException:
        pass
