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
path = 'test_images_14x14/'
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

def infer_with_TF_lite(interpreter, input_details, output_details, raw_image):
    # Get input size
    input_shape = input_details[0]['shape']
    size = input_shape[:2] if len(input_shape) == 3 else input_shape[1:3]

    # Preprocess image
    raw_image = raw_image.resize(size)
    img = np.array(raw_image, dtype=np.float32)

    # Normalize image
    img = img / 255.

    # Add a batch dimension and a dimension because we use grayscale format
    # Reshape from (INPUT_IMAGE_SIZE, INPUT_IMAGE_SIZE) to (1, INPUT_IMAGE_SIZE, INPUT_IMAGE_SIZE, 1)
    input_data = img.reshape(1, img.shape[0], img.shape[1], 1)
    
    # Point the data to be used for testing
    interpreter.set_tensor(input_details[0]['index'], input_data)

    # Run the interpreter
    interpreter.invoke()

    # Obtain results
    predictions = interpreter.get_tensor(output_details[0]['index'])[0]
    
    return predictions


def infer_with_MCU(raw_image):
    img = np.array(raw_image, dtype=np.uint8)
    bytes_sent = send_to_mcu(serial_port, img)
    print("sent to mcu ")
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

# Create Interpreter (Load TFLite model).
interpreter = tf.lite.Interpreter(model_path=tflite_model_file)
# Allocate tensors
interpreter.allocate_tensors()

# Get input and output tensors.
input_details = interpreter.get_input_details()
output_details = interpreter.get_output_details()

# Configure and open serial port
serial_port = init_serial(SERIAL_PORT_NAME)

# Check which port was really used
print("Opened serial port : {0}".format(serial_port.name))

files = [files for files in glob.glob(path + "*" + file_format, recursive=False)]
print("Found {0} {1} files".format(len(files), file_format))


import array
while True:
    ## Nhận input từ người dùng: khi nhấn enter: 
    ## 1. send "1" đến  board 
    ## 2. nhận hình và kết quả dự đoán từ board  
    ## 3. save hình vào local (cùng thư mục vs python, tên file là thời điểm nhận hình)
    ## 4. show kq dự đoán trên console và bật app show ảnh ( đã save ở 3)

    input()


    bytes_sent = send_to_mcu(serial_port, [ord(elem) for elem in "1"])
    print(bytes_sent)
    ## Nhận hình
    image = read_result_from_mcu(serial_port)
    image = image.decode("utf-8")

   
    ## save hình
    # print(image)

    # VÍ DỤ NÈ NHE MÁ 
    # Cắt chuỗi to thành nhiều chuỗi nhỏ( mỗi chuỗi là 1 số)
    # image =" 123 12 14 4"
    # >>> image.split()
    # ['123', '12', '14', '4']

    # Đổi cái chuỗi hồi nãy nè, thành số  nguyên nè má :D
    # [int(i) for i in image.split()]
    # [123, 12, 14, 4]
    
    #bytearray: công cụ ghi cái nùi ở trên vào 1 file 
    hien_hinh=bytearray([int(i) for i in image.split()])

    # CHUẨN BỊ HIỆN HÌNH NÈ MÁ :)
    import datetime

    f = open(str(datetime.datetime.now())+".jpg", 'w+b')
    f.write(hien_hinh)
    f.close()
    print('CHO HIEN HINH DOI NE')
    ## show kq dự đoán
    # print ( response_kq)

 ## Nhận kq dự đoán
    response_kq = ""
    while response_kq != "###ketquapredictne###\n":
        response_kq = read_result_from_mcu(serial_port)
        response_kq = response_kq.decode("utf-8")
        print(response_kq)
    response_kq = read_result_from_mcu(serial_port)
    response_kq = response_kq.decode("utf-8")
    print("ket qua ne ",response_kq)

# for f in files:
#     # Read image
#     raw_image = Image.open(f).convert('L')
#     print("file: ", f)
#     result_tfl = infer_with_TF_lite(interpreter, input_details, output_details, raw_image)
#     print("result_tfl: ", result_tfl)
#     result_mcu = infer_with_MCU(raw_image)
#     print("result_mcu: ", result_mcu)

#     status = compare_results(result_tfl, result_mcu)
#     status_str = "Ok" if status else "Reults NOT eqal!"
#     print("For file: {0} - {1}".format(f, status_str))

serial_port.close()
