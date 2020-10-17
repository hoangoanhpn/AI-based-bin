
#include "string.h"
#include <stdio.h>

#include "esp_system.h"

#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "main_functions.h"

#include "tensorflow/lite/micro/kernels/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

#include "model_data.h"
#include "model_operations.h"






// Quanhh đã chữa nè 

#include "app_camera.h" 


int totalExpectedDataAmount = INPUT_IMAGE_WIDTH * INPUT_IMAGE_HEIGHT * INPUT_IMAGE_CHANNELS;

tflite::ErrorReporter *error_reporter = nullptr;
const tflite::Model *model = nullptr;
tflite::MicroInterpreter *interpreter = nullptr;
TfLiteTensor *input = nullptr;
TfLiteTensor *output = nullptr;

// Create an area of memory to use for input, output, and intermediate arrays.
// Finding the minimum value for your model may require some trial and error.
// max Arenasize 153388
//constexpr int kTensorArenaSize =  149*1024;

//constexpr int kTensorArenaSize =  322496;
constexpr int kTensorArenaSize =  420548;
//uint8_t tensor_arena[kTensorArenaSize];
uint8_t *tensor_arena;

// The name of this function is important for Arduino compatibility.
void setup()
{
    tensor_arena= new uint8_t[kTensorArenaSize];
    // Set up logging.
    static tflite::MicroErrorReporter micro_error_reporter;
    error_reporter = &micro_error_reporter;

    // Pepare the model.
    model = tflite::GetModel(fashion_mnist_model_tflite);
    if (model->version() != TFLITE_SCHEMA_VERSION)
    {
        error_reporter->Report(
            "Model provided is schema version %d not equal "
            "to supported version %d.",
            model->version(), TFLITE_SCHEMA_VERSION);
        return;
    }

    // Prepare graph model operations resolver.
    // Here I use resolver that loads all possible set of operations.
    // It will be better to use MicroOpResolver that loads
    // only really needed set of operations.
    static tflite::OpResolver &resolver = getFullMicroOpResolver();

    // Build an interpreter to run the model with.
    static tflite::MicroInterpreter static_interpreter(
        model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
    interpreter = &static_interpreter;

    // Allocate memory from the tensor_arena for the model's tensors.
    TfLiteStatus allocate_status = interpreter->AllocateTensors();
    if (allocate_status != kTfLiteOk)
    {
        error_reporter->Report("AllocateTensors() failed.");
        return;
    }

    // Obtain pointers to the model's input and output tensors.
    input = interpreter->input(0);
    output = interpreter->output(0);
}

void doInference()
{
    TfLiteStatus invoke_status = interpreter->Invoke();
    if (invoke_status != kTfLiteOk)
    {
        error_reporter->Report("Invoke failed.");
        return;
    }
}

#include "esp_timer.h"


static size_t jpg_encode_stream(void * arg, size_t index, const void* data, size_t len){
    // jpg_chunking_t *j = (jpg_chunking_t *)arg;
    // if(!index){
    //     j->len = 0;
    // }
    // if(httpd_resp_send_chunk(j->req, (const char *)data, len) != ESP_OK){
    //     return 0;
    // }
    // j->len += len;
    //return sendData((const char*)data, len);
    //char str[4*len] ="";
    char tmp[40];
    const char* ahihi = (const char*) data;
    //itoa((int)len, tmp, 10);
    
    //sendData(tmp);
    //sendData("tren tmp ne");sendData("\n");
    for (int i=0; i< len; i++ )
    {
        itoa( (int)ahihi[i], tmp,10);
        strcat( tmp, " ");
        sendData(tmp);
    }
    //sendData("dang test ne \n");
    //strcalen; str, "\n");
    return len;
}

void jpg_httpd_handler(){
    camera_fb_t * fb = NULL;
    // esp_err_t res = ESP_OK;
    size_t fb_len = 0;
    int64_t fr_start = esp_timer_get_time();
    
    // Code chup hinh ne :)
    fb = esp_camera_fb_get();
    if (!fb) {
        sendData("Camera capture failed");
       // ESP_LOGE(TAG, "Camera capture failed");
        
        return ;
    }
    else
    {
       // sendData("Camera cua Q da chup ne :D ");
         //return ;
    }
    
    

    // if(res == ESP_OK){
        if(fb->format == PIXFORMAT_JPEG){
            fb_len = fb->len;
            //sendData((const char*)fb->buf, fb->len);
            //char str[4*fb->len] ="";
            char tmp[40];
            for (int i=0; i< fb->len; i++ )
            {
                itoa( (int)fb->buf[i], tmp,10);
                strcat( tmp, " ");
                sendData(tmp);
            }
            sendData("  nen ne\n");
            sendData("\n");
            
            doInference();
            sendBackPredictions(output);
            // res = httpd_resp_send(req, (const char *)fb->buf, fb->len);
        } else {
            fb_len = fb->len;
            //sendData((const char*)fb->buf, fb->len);
            //char str[4*fb->len] ="";
            char tmp[40];
            for (int i=0; i< fb->len; i++ )
            {
                itoa( (int)fb->buf[i], tmp,10);
                strcat( tmp, " ");
                sendData(tmp);
            }
            // frame2jpg_cb(fb, 80, jpg_encode_stream, 0);
            sendData("\n");
            
            doInference();sendData("hinh chua nen ne\n");
            sendBackPredictions(output);
            
        }
    // }
    esp_camera_fb_return(fb);
    int64_t fr_end = esp_timer_get_time();
    // ESP_LOGI(TAG, "JPG: %uKB %ums", (uint32_t)(fb_len/1024), (uint32_t)((fr_end - fr_start)/1000));
    // return res;
}

#include <stdio.h>
#include <stdbool.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <ultrasonic.h>
#include <iostream>

using namespace std;

#define MAX_DISTANCE_CM 500 // 5m max
#define TRIGGER_GPIO GPIO_NUM_12
#define ECHO_GPIO GPIO_NUM_13



void ultrasonic_test(void *pvParamters)
{
    ultrasonic_sensor_t sensor = {
        .trigger_pin = TRIGGER_GPIO,
        .echo_pin = ECHO_GPIO
    };

    ultrasonic_init(&sensor);

    while (true)
    {
        uint32_t distance;
        esp_err_t res = ultrasonic_measure_cm(&sensor, MAX_DISTANCE_CM, &distance);
        // printf("ahihihiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii");
        cout<<(distance);
        if (res != ESP_OK)
        {
            printf("Error: ");
            switch (res)
            {
                case ESP_ERR_ULTRASONIC_PING:
                    cout<<("Cannot ping (device is in invalid state)\n");
                    break;
                case ESP_ERR_ULTRASONIC_PING_TIMEOUT:
                    printf("Ping timeout (no device found)\n");
                    break;
                case ESP_ERR_ULTRASONIC_ECHO_TIMEOUT:
                    printf("Echo timeout (i.e. distance too big)\n");
                    break;
                default:
                    printf("%d\n", res);
            }
        }
        else
            printf("Distance: %d cm\n", distance);

        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}
/*
MIT License

Copyright (c) 2018 ShellAddicted <github.com/ShellAddicted>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "servoControl.h"


// extern "C" void app_main(){
// 	servoControl myServo;
// 	myServo.attach(GPIO_NUM_12);
// 	//Defaults: myServo.attach(pin, 400, 2600, LEDC_CHANNEL_0, LEDC_TIMER0);
// 	// to use more servo set a valid ledc channel and timer
	
// 	myServo.write(0);
// 	vTaskDelay(1000 / portTICK_RATE_MS);
	
// 	while (1){
// 		for (int i = 0; i<180; i++){
// 			myServo.write(i);
// 			vTaskDelay(10 / portTICK_RATE_MS);
// 		}
// 		for (int i = 180; i>0; i--){
// 			myServo.write(i);
// 			vTaskDelay(10 / portTICK_RATE_MS);
// 		}
// 	}
// }

extern "C" void app_main(void)
{
     xTaskCreate(ultrasonic_test, "ultrasonic_test", configMINIMAL_STACK_SIZE * 3, NULL, 5, NULL);


//     // app_camera_init();
//     // initUart(UART_NUMBER);
//     // setup();
//     // while (true)
//     // {
//     //     uint8_t a[10]={0};
//     //     // doc chuoi python gui ("1")
//     //     uart_read_bytes(UART_NUMBER, a, 1, 1000 / portTICK_RATE_MS);
//     //     uart_flush(UART_NUMBER);
//     //     // doInference();
//     //     // sendBackPredictions(output);
        
//     //         if( a[0]==49 )
//     //         {
//     //             camera_fb_t * fb = NULL;
//     //             // esp_err_t res = ESP_OK;
//     //             size_t fb_len = 0;
//     //             int64_t fr_start = esp_timer_get_time();
                
//     //             // Code chup hinh ne :)
//     //             fb = esp_camera_fb_get();
//     //             if (!fb) {
//     //                 sendData("Camera capture failed");
//     //             // ESP_LOGE(TAG, "Camera capture failed");
                    
//     //                 return ;
//     //             }
//     //             else
//     //             {
//     //             // sendData("Camera cua Q da chup ne :D ");
//     //                 //return ;
//     //             }
//     //             // jpg_httpd_handler();
//     //             fb_len = fb->len;
//     //             //sendData((const char*)fb->buf, fb->len);
//     //             //char str[4*fb->len] ="";
//     //             char tmp[40];
//     //             for (int i=0; i< fb->len; i++ )
//     //             {
//     //                 itoa( (int)fb->buf[i], tmp,10);
//     //                 strcat( tmp, " ");
//     //                 sendData(tmp);
//     //             }
//     //             // frame2jpg_cb(fb, 80, jpg_encode_stream, 0);
//     //             sendData("\n");
//     //             for(int i = 0; i < fb->len; i++){
//     //                 input->data.f[i] = fb->buf[i]/255.0f;
//     //             }
               
//     //             doInference();
//     //             sendBackPredictions(output);
//     //             esp_camera_fb_return(fb);
//             //  sendData("ahihi \n");
            
//             // char str[250] ="";
//             // char motso[10];
//             // for (int i=0; i<10; i++ )
//             // {
//             //     strcat( str, itoa( (int)a[i], motso, 10 ));
//             // }
//             // strcat( str, "\n");
//             // sendData(str);
//         }
        

    }

// ///// TEST ẢNH TĨNH : test_image_96x96
//     // for (;;)
//     // {
//     //     char a[30]={0};
//     //     // doc chuoi python gui ("1")
       
//         // readUartBytes(input->data.f, totalExpectedDataAmount);
//     //     doInference();
//     //     sendBackPredictions(output);
//     //     uart_read_bytes(UART_NUMBER, (uint8_t*)(a) , 30, 1000 / portTICK_RATE_MS);
//     //     sendData(a);
//     //     sendData("\n");
//     // }
// }