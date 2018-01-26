'''
useful functions
'''
import os, cv2
import numpy as np

def convertNPZToGrayImage(npzFile, score, img_size):
    dir_name = os.path.dirname(npzFile)
    n = np.load(npzFile)
    for key in n:
        mask_image = os.path.join(dir_name, key + ".jpg")
        if os.path.isfile(mask_image):
            print("{} already exists. Skipping!".format(mask_image))
        else:
            print("creating {}".format(os.path.split(mask_image)[1]))
            segmentation = (n[key] > score).reshape(img_size,img_size, 1) * 255
            cv2.imwrite(mask_image, segmentation)

if __name__ == '__main__':
    #convertNPZToGrayImage("small.npz", .5, 5000)
    im_gray = cv2.imread('25063_56256_200_DB_REF_20141010.jpg', cv2.IMREAD_GRAYSCALE)
    (h,w) = im_gray.shape
    count = 0
    for y in range(h):
        for x in range(w):
            if im_gray[y,x] == 255:
                count += 1

    print(count)
