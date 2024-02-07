# -*- coding: utf-8 -*-
"""
Created on Wed Nov 22 14:11:49 2023

@author: beshr
"""

import cv2
import numpy as np

#############################################exo 1###########################

# img=cv2.imread("opencv.png")
# for j in range (len(img)):

#     for i in range (len(img[0])):
#         l=img[j][i].tolist()
#         if l==[255,255,255]:
#             print("-",end="")
#         else:
#             print("+",end="")
#     print("\n")
# cv2.imshow("image",img)
# k=cv2.waitKey(0)
# if k=='q':
#     cv2.destroyAllWindows()
    
#########################################exo 2################################

# img=cv2.imread("bgr.png") 

# b,g,r=cv2.split(img)

# cv2.imshow("image",b)
# k=cv2.waitKey(0)
# if k=='q':
#     cv2.destroyAllWindows()  

# cv2.imshow("image",g)
# k=cv2.waitKey(0)
# if k=='q':
#     cv2.destroyAllWindows()

# cv2.imshow("image",r)
# k=cv2.waitKey(0)
# if k=='q':
#     cv2.destroyAllWindows()  
# empty=np.zeros((240,240),dtype=np.uint8)


# img=cv2.merge((empty,empty,r))
# cv2.imshow("image",img)
# k=cv2.waitKey(0)
# if k=='q':
#     cv2.destroyAllWindows()    

# img=cv2.merge((empty,g,empty))
# cv2.imshow("image",img)
# k=cv2.waitKey(0)
# if k=='q':
#     cv2.destroyAllWindows() 
    
# img=cv2.merge((b,empty,empty))
# cv2.imshow("image",img)
# k=cv2.waitKey(0)
# if k=='q':
#     cv2.destroyAllWindows() 


###################################exo4##############################


# img=cv2.imread("circles.jpg")

# imghsv=cv2.cvtColor(img,cv2.COLOR_BGR2HSV) 

# cv2.imshow("imag",img)

# cv2.imshow("ima",imghsv)


# # define range of blue color in HSV
# lower_red = np.array([0, 100, 20])
# upper_red = np.array([10, 255, 255])

# lower2 = np.array([160,100,20])
# upper2 = np.array([179,255,255])
# # Threshold the HSV image to get only blue colors
# mask = cv2.inRange(imghsv, lower_red, upper_red)
# mask2 = cv2.inRange(imghsv, lower2, upper2)
# cv2.imshow("image",mask) 
# cv2.imshow("image2",mask2)  
# res = cv2.bitwise_or(mask,mask2)
# cv2.imshow("image3",res)
# edges = cv2.Canny(res,100,200)
# cv2.imshow("edge",edges)

# k=cv2.waitKey(0)
# if k=='q':
#     cv2.destroyAllWindows() 


###################################exo5##############################


#img=cv2.imread("C:/rail4.png",cv2.IMREAD_GRAYSCALE)
# imghsv=cv2.cvtColor(img,cv2.COLOR_BGR2HSV) 
#edges = cv2.Canny(img,10,150)
#cv2.imshow("edge",img)
# lines = cv2.HoughLinesP(edges,1,np.pi/180,100,minLineLength=1,maxLineGap=10)
# for line in lines:
#     x1,y1,x2,y2 = line[0]
#     cv2.line(img,(x1,y1),(x2,y2),(0,255,0),2)

# cv2.imshow("ima",imghsv)

# # define range of blue color in HSV
# lower_red = np.array([0, 100, 20])
# upper_red = np.array([10, 255, 255])

# lower2 = np.array([160,100,20])
# upper2 = np.array([179,255,255])
# # Threshold the HSV image to get only blue colors
# mask = cv2.inRange(imghsv, lower_red, upper_red)
# mask2 = cv2.inRange(imghsv, lower2, upper2)
# res = cv2.bitwise_or(mask,mask2)
# edges = cv2.Canny(res,100,200)
# contours,hier = cv2.findContours(res, 1, 2)
# cv2.drawContours(img,contours[0],-1,[200,0,0],5)
# cv2.drawContours(img,contours[1],-1,[0,200,0],5)
# cv2.imshow("imag",img)
# k=cv2.waitKey(0)
# if k=='q':
#     cv2.destroyAllWindows() 

###################################exo6##############################

# from matplotlib import pyplot as plt
# img = cv2.imread('Billard_Balls.jpg', cv2.IMREAD_GRAYSCALE)
# assert img is not None, "file could not be read, check with os.path.exists()"
# img2 = img.copy()
# template = cv2.imread('Ball_three.png', cv2.IMREAD_GRAYSCALE)
# assert template is not None, "file could not be read, check with os.path.exists()"
# w, h = template.shape[::-1]
# # All the 6 methods for comparison in a list
# methods = ['cv2.TM_CCOEFF', 'cv2.TM_CCOEFF_NORMED', 'cv2.TM_CCORR',
#             'cv2.TM_CCORR_NORMED', 'cv2.TM_SQDIFF', 'cv2.TM_SQDIFF_NORMED']
# for meth in methods:
#     img = img2.copy()
#     method = eval(meth)
#     # Apply template Matching
#     res = cv2.matchTemplate(img,template,method)
#     min_val, max_val, min_loc, max_loc = cv2.minMaxLoc(res)
#     # If the method is TM_SQDIFF or TM_SQDIFF_NORMED, take minimum
#     if method in [cv2.TM_SQDIFF, cv2.TM_SQDIFF_NORMED]:
#         top_left = min_loc
#     else:
#         top_left = max_loc
#     bottom_right = (top_left[0] + w, top_left[1] + h)
#     cv2.rectangle(img,top_left, bottom_right, 255, 2)
#     plt.subplot(121),plt.imshow(res,cmap = 'gray')
#     plt.title('Matching Result'), plt.xticks([]), plt.yticks([])
#     plt.subplot(122),plt.imshow(img,cmap = 'gray')
#     plt.title('Detected Point'), plt.xticks([]), plt.yticks([])
#     plt.suptitle(meth)
#     plt.show()


###################################exo7##############################



# import os
# # Read the video from specified path
# cam = cv2.VideoCapture ("C:/vidurb2.mp4")

# from matplotlib import pyplot as plt

# while(True):
#     ret,frame = cam.read()
#     ret,frame = cam.read()
#     ret,frame = cam.read()
#     ret,frame = cam.read()
#     ret,frame = cam.read()
#     ret,frame = cam.read()
    
#     if ret:
#         img = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
#         assert img is not None, "file could not be read, check with os.path.exists()"
#         img2 = img.copy()
#         template = cv2.imread('C:/rail4.png', cv2.IMREAD_GRAYSCALE)
#         assert template is not None, "file could not be read, check with os.path.exists()"
#         w, h = template.shape[::-1]
#         # All the 6 methods for comparison in a list
#         methods = ['cv2.TM_CCOEFF_NORMED']#'cv2.TM_CCOEFF', 'cv2.TM_CCOEFF_NORMED', 'cv2.TM_CCORR',  'cv2.TM_CCORR_NORMED', 'cv2.TM_SQDIFF', 
#         for meth in methods:
#             img = img2.copy()
#             method = eval(meth)
#             # Apply template Matching
#             res = cv2.matchTemplate(img,template,method)
#             min_val, max_val, min_loc, max_loc = cv2.minMaxLoc(res)
#             # If the method is TM_SQDIFF or TM_SQDIFF_NORMED, take minimum
#             if method in [cv2.TM_SQDIFF, cv2.TM_SQDIFF_NORMED]:
#                 top_left = min_loc
#             else:
#                 top_left = max_loc
#             bottom_right = (top_left[0] + w, top_left[1] + h)
#             cv2.rectangle(img,top_left, bottom_right, 255, 2)
#             plt.subplot(121),plt.imshow(res,cmap = 'gray')
#             plt.title('Matching Result'), plt.xticks([]), plt.yticks([])
#             plt.subplot(122),plt.imshow(img,cmap = 'gray')
#             plt.title('Detected Point'), plt.xticks([]), plt.yticks([])
#             plt.suptitle(meth)
#             plt.show()
#         if cv2.waitKey (25) & 0xFF == ord('q'):
#             break
#     else:
#         break
# # Release all space and windows once done
# cam. release ()
# cv2.destroyAllWindows ()





import os
# Read the video from specified path
cam = cv2.VideoCapture ("C:/vidurb2.mp4")

from matplotlib import pyplot as plt
rail_cascade = cv2.CascadeClassifier()
if not rail_cascade.load("C:/cascade.xml"):
    print('--(!)Error loading face cascade')
    exit(0)
    
while(True):
    ret,frame = cam.read()
    ret,frame = cam.read()
    ret,frame = cam.read()
    ret,frame = cam.read()
    ret,frame = cam.read()
    ret,frame = cam.read()
    
    if ret:
        img = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        frame_gray = cv2.equalizeHist(img)
        assert img is not None, "file could not be read, check with os.path.exists()"
        rail = rail_cascade.detectMultiScale(frame_gray)
        for (x,y,w,h) in rail:
        # If the method is TM_SQDIFF or TM_SQDIFF_NORMED, take minimum
            top_left = [x,y]
            bottom_right = (top_left[0] + w, top_left[1] + h) 
            cv2.rectangle(img,top_left, bottom_right, 255, 2)
            
        plt.subplot(122),plt.imshow(img,cmap = 'gray')
        plt.title('Detected Points'), plt.xticks([]), plt.yticks([])
        plt.show()
        if cv2.waitKey (25) & 0xFF == ord('q'):
            break
    else:
# Release all space and windows once done
        cam. release ()
        cv2.destroyAllWindows ()
















