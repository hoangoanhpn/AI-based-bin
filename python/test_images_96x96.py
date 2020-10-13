#!/usr/bin/python

from __future__ import absolute_import, division, print_function, unicode_literals

import glob
import serial
import time

import numpy as np
import tensorflow as tf

from PIL import Image
import matplotlib.pyplot as plt

# try:
#   tensorflow_version 2.x
# except:
#   pass

INPUT_IMAGE_SIZE = 96

# Path to TensorFlow Lite model file
tflite_model_file = 'model.tflite'

# Path to images folder
# path = 'test_trash/'
path = 'test_trash/'
file_format = ".jpg"

SERIAL_PORT_NAME = '/dev/ttyUSB0'

EXPECTED_PRECISION = 0.000001



def init_serial(port_name):
    # Configure and open serial port
    port = serial.Serial(
        port=port_name,
        baudrate=115200,
        bytesize=serial.EIGHTBITS,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        xonxoff=False,
        rtscts=False,
        dsrdtr=False
    )
    
    port.reset_input_buffer()
    port.reset_output_buffer()
    return port



def send_to_mcu(port, data):
    bytes_written = port.write(data)
    return bytes_written

def read_result_from_mcu(port):
    line = port.read_until()   # read a '\n' terminated line
    return line

# def infer_with_TF_lite(interpreter, input_details, output_details, raw_image):
#     # Get input size
#     input_shape = input_details[0]['shape']
#     size = input_shape[:2] if len(input_shape) == 3 else input_shape[1:3]

#     # Preprocess image
#     #raw_image = raw_image.resize(size)
#     img = np.array(raw_image, dtype=np.float32)

#     # Normalize image
#     img = img / 255.

#     # Add a batch dimension and a dimension because we use grayscale format
#     # Reshape from (INPUT_IMAGE_SIZE, INPUT_IMAGE_SIZE) to (1, INPUT_IMAGE_SIZE, INPUT_IMAGE_SIZE, 1)
#     input_data = img.reshape(1, img.shape[0], img.shape[1], 1)
    
#     # Point the data to be used for testing
#     interpreter.set_tensor(input_details[0]['index'], input_data)

#     # Run the interpreter
#     interpreter.invoke()

#     # Obtain results
#     predictions = interpreter.get_tensor(output_details[0]['index'])[0]
    
#     return predictions


def infer_with_MCU(raw_image):
    img = np.array(raw_image, dtype=np.uint8)
    bytes_sent = send_to_mcu(serial_port, img)

    print("sent to mcu ", bytes_sent)
    response_str = ""
    while response_str != "###ketquapredictne###\n":
        response_str = read_result_from_mcu(serial_port)
        response_str = response_str.decode("utf-8")
        print(response_str)

    response_str = read_result_from_mcu(serial_port)
    response_str = response_str.decode("utf-8")

    predictions = np.fromstring(response_str, dtype=np.float32, sep=',')
    return predictions

def compare_results(result_tfl, result_mcu):
    for i in range(len(result_tfl)):
        if abs(result_tfl[i] - result_mcu[i]) > EXPECTED_PRECISION :
            return False
        else:
            pass
    return True

# # Create Interpreter (Load TFLite model).
# interpreter = tf.lite.Interpreter(model_path=tflite_model_file)
# # Allocate tensors
# interpreter.allocate_tensors()

# # Get input and output tensors.
# input_details = interpreter.get_input_details()
# output_details = interpreter.get_output_details()

# Configure and open serial port
serial_port = init_serial(SERIAL_PORT_NAME)

# Check which port was really used
print("Opened serial port : {0}".format(serial_port.name))

files = [files for files in glob.glob(path + "*" + file_format, recursive=False)]
print("Found {0} {1} files".format(len(files), file_format))


label= {0: 'cardboard', 1: 'glass', 2: 'metal', 3: 'paper', 4: 'plastic', 5: 'trash'}
i=0
for f in files:
    # Read image
    raw_image= Image.open(f).resize((96,96)).convert('RGB')
    # raw_image = Image.open(f).convert('LA').resize((96,96))z
    print("file: ", f)
    result_mcu = infer_with_MCU(raw_image)
    print("result_mcu: ", result_mcu)
    Quanhh= np.argmax(result_mcu)
    # Quanhh = 0
##################################
### ten file = name ne!!!!!!!!!!!
### su dung dataset khac thi nhớ đổi số kí tự nheeeeeeeeeeee
    name = f.rjust(30,' ')
    a=send_to_mcu(serial_port, [ord(elem) for elem in name])
    
    
    confirm_name =read_result_from_mcu(serial_port)
    confirm_name = confirm_name.decode("utf-8")
    print("confirm_name", confirm_name)
   
    # status = compare_results(result_tfl, result_mcu)
    # status_str = "Ok" if status else "Reults NOT eqal!"
    print("ket qua ne ", f, label[Quanhh],result_mcu)
    # i+=1
    # if i==100:break

serial_port.close()
