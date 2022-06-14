# from pyexpat import model
import tensorflow.keras as kr
import cv2
import time
from PIL import Image
import numpy as np
from matplotlib import pyplot as plt
import glob
import os
from skimage.io import imshow, imshow_collection, imread_collection

#The classifier responsible for the classification of images, which it does with a provided model.
class Classifier:

    #Standard values, can be changed.
    def __init__(self, model_path='Models/EfficientNetB7.h5', image_size=224, input_path='CNN custom 1/', output_path='Classification'):
        self.model = kr.models.load_model(model_path)
        self.image_size = image_size
        self.input_path = input_path
        self.output_path = output_path
        os.mkdir(output_path)
        os.mkdir(output_path + '/Foreminifera')
        os.mkdir(output_path + '/Sand')
        print("Finished initializing and creating folders!")

    def classifyImages(self):
        
        #Load in all images from file, source:
        #https://stackoverflow.com/questions/51434091/python-globbing-a-directory-of-images
        #Read in RGB
        imgs = [cv2.cvtColor(cv2.imread(file), cv2.COLOR_RGB2BGR) for file in glob.glob(self.input_path + "/*.png")]
        #Read in BGR
        #imgs = [cv2.imread(file) for file in glob.glob(self.input_path + "/*.png")]
        i, j = 0, 0
        
        time_count = 0
        count = 0

        #Classify all images, and store them as the max index.
        for img in imgs:            
            start = time.time()
            img = cv2.resize(img, (self.image_size, self.image_size))
            x = np.expand_dims(img,axis=0)
            y = self.model.predict(x)
            
            max_index = np.argmax(y[0])
            print(y[0], "Max index = ", max_index)
            img = cv2.cvtColor(img, cv2.COLOR_RGB2BGR)
            if(max_index == 1):
                cv2.imwrite(self.output_path + "/Sand/sand" + str(i) + ".png", img)
                i += 1
            else:
                cv2.imwrite(self.output_path + "/Foreminifera/foraminifera" + str(j) + ".png", img)
                j += 1
            
            end= time.time()
            print("Time of resize, prediction and storing is: ", end - start)
            time_count += end - start
            count += 1

        # To show images
        # imshow_collection(imgs)
        # plt.show() 
        if(count != 0):
            avg_time = time_count / count
        print("avg time = ", avg_time)
    


def main():
    #Define the input and output path.
    input_path = 'outcome' 
    output_path= input_path + '/Result'
    
    predict = Classifier(input_path=input_path, output_path=output_path)
    predict.classifyImages()

if __name__ == "__main__":
    main()

