# Bachelor Project: A digital microscopy system to classify sediment samples of foraminifera
# Stitching & segmentation software


import cv2
from cv2 import bitwise_and
from cv2 import imwrite
import numpy as np
from matplotlib import pyplot as plt
import os
from skimage.io import imread_collection, imshow_collection, imshow, imread, imsave
from skimage import data, io
from skimage import filters, morphology
from skimage.color import rgb2gray
from skimage.morphology import disk
from skimage.morphology import dilation


class ImageProcessor:
    def __init__(self, input_path, output_path, canvasImg = None):
        # The input and output paths are in relation to the location of this file.
        self.input_path = input_path
        self.output_path = output_path

        self.imageLst = imread_collection(input_path + '/*') # Adding '/*' so that we get all pictures from the path
        self.elementLst = [] # List to contaion all the final objects
        self.canvasImg = canvasImg



    # Stitching the pictures into a single canvas
    def stitchPictures(self):

        # The images are resized to reduce resource use
        resizedImgLst = []
        for i in range(len(self.imageLst)):
            resize_scale = 0.6
            resizedImgLst.append(self.imageLst[i])

            resizedImgLst[i]=cv2.resize(resizedImgLst[i], 
                                        (0,0), 
                                        fx=resize_scale,
                                        fy=resize_scale)


        print("Loaded pictures")
        stitcher = cv2.Stitcher.create(mode=0)


        # DEN HER VIRKER UNDERLIGT!!!!!!!!!!!!!!!!!!!!!! JEG BRUGER IKKE RESIZE TIL NOGET
        # (exit_code, output)=stitcher.stitch(self.imageLst)
        (exit_code, output)=stitcher.stitch(resizedImgLst)
        print("Stitched Canvas")


        # checking if the stitching procedure is successful
        if exit_code != cv2.STITCHER_OK:
        
            # exit_code is set to TRUE if stitching is done successfully
            print("stitching was not successful")
        else:
            print('The canvas has been created')
            self.canvasImg = output


    # Function to compress the picture
    def preprocess(self, img, resize_scale):
        return cv2.resize(img, (0,0), fx=resize_scale, fy=resize_scale)



    def createBinaryImg(self, img):
        grayScaleImg = rgb2gray(img)

        # The threshold for foreground pixels is set to 0.17
        threshold_ = 0.17

        binaryCanvas = grayScaleImg > threshold_

        return binaryCanvas


    # Filter the noise by using morphological opening (erosion -> dilation)
    def reduceNoiseOpening(self, img, disk_size):
        disk_ = morphology.disk(disk_size) #create structure element
        print("Starting erosion")
        erodedImage = morphology.erosion(image=img, footprint=disk_) # erode image
        print("Starting dilation")
        openedImage = morphology.dilation(image=erodedImage, footprint=disk_)  # dilate image
        print("DONE filtering noise")
        return openedImage



    # This function will find each region of foreground elements and save them to a list
    def isolateSegments(self, img, structuredElement):

        #keep looking for element. 
        # As long as there are any pixels in the image 
        # different from 0 find index of foreground pixels
        z = np.where(img>0)

        binaryElementLst = []

        while(img.any() > 0): #keep looping while there are still objects left

            X_0 = np.zeros(img.shape) #reset X0 and X1
            X1 = np.zeros(img.shape)
            
            X_0[z[0][0], z[1][0]] = 1    #set a foreground pixel point in the first element
            
            while(True):
                X1 = np.logical_and(dilation(X_0, structuredElement), img)
                
                if (X_0==X1).all(): #if output is identical to previous output, we have found all foreground pixels
                    break

                X_0=X1.copy() # save the current object for next iteration
            
            binaryElementLst.append(X1)
            print("Appended binary element: " + str(len(binaryElementLst)))

            # remove the object (X1) that we just fround from img
            img = img^X1 # Use XOR as this removes pixels where both are true 
            z = np.where(img>0)



        return binaryElementLst


    def cropElement(self, img):

        # Removing all rows and columns where all channel values are 0
        horizontalCrop = np.delete(img, np.where(~img.any(axis=1))[0], axis=0)
        fullCrop = np.delete(horizontalCrop, np.where(~horizontalCrop.any(axis=0))[0], axis=1)


        # Adding back some padding to get the pictures to be square for consistency
        old_height, old_width, channels = np.shape(fullCrop)

        new_size = max(old_height, old_width)

        output = np.full((new_size,new_size, channels), (0, 0, 0), dtype=np.uint8)

        # We center the element on the axis where the padding was added
        width_center = (new_size - old_width) // 2
        height_center = (new_size - old_height) // 2

        # copy img image into center of result image
        output[height_center :height_center + old_height, 
               width_center  :width_center  + old_width  ] = fullCrop

        return output




    def findElements(self):

        compressedCanvas = self.preprocess(self.canvasImg, 0.6)


        binaryCanvas = self.createBinaryImg(compressedCanvas)

        print("Starting to reduce noise")
        # 20 for disk_size seems good for removing background noise(dust)
        binaryCanvasOpening = self.reduceNoiseOpening(binaryCanvas, 20) 

        
        # Create a simple structure element
        structElement = np.array([[0, 1 ,0],
                                  [1, 1, 1],
                                  [0, 1, 0]])


        print("Starting to isolate segments")
        binaryElementLst = self.isolateSegments(binaryCanvasOpening, structElement)


        # ____________Mapping binary image onto the original RGB image _________
        for e in binaryElementLst:
            rgbElement = self.preprocess(self.canvasImg, 0.6)

            for i in range(len(rgbElement[0, :])):
                for j in range(len(rgbElement[:, 0])):
                    rgbElement[j, i] = rgbElement[j, i] * e[j, i]
                    

            croppedElement = self.cropElement(rgbElement)

            self.elementLst.append(croppedElement)
            print("Appended rgb element: " + str(len(self.elementLst)))

            

    def saveElements(self):
        # If directory exists we dont want to save samples as this might overwrite previous data samples
        if  os.path.exists(self.output_path):
            print("The output directory already exists.")
            command_ = input("Do you wish to continue (WARNING: this might overwrite existing files) [y/n]")

            while command_.lower() not in ("y", "n"):
                command_ = input("Do you wish to continue [y/n]")

            if command_.lower() == "n":
                return

        else:
            # # Create a new directory as the path does not exist
            os.makedirs(self.output_path)
            print("The new directory is created!")


        for i in range(len(self.elementLst)):
            elementName = "element_" + str(i) + ".png"
            # try:
            cv2.imwrite(self.output_path+"/"+elementName, cv2.cvtColor(self.elementLst[i], cv2.COLOR_RGB2BGR))
            # except:
            #     print("Could not save element: ", elementName)
            #     break
            

        

# Main function to run the code
def main():


    # _______ SAMPLE IMPLEMENTATION _______

    # Defining the input and output path
    input_directory  = 'raport_test/test_7'
    output_directory = 'raport_test/comment_test_5'

    # Creating an instance of the ImageProcessor class
    sample = ImageProcessor(input_directory, output_directory)
    

    # Stitching the provided images
    sample.stitchPictures()

    # Segmenting elements
    sample.findElements()


    # Saving the images to the output directory
    sample.saveElements()

    print("Stitching and segmentation has been completed")

if __name__ == '__main__':
    main()