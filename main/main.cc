
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
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "servoControl.h"


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

#define trigger_taiche GPIO_NUM_15
#define echo_taiche GPIO_NUM_16

#define trigger_khongtaiche GPIO_NUM_2
#define echo_khongtaiche GPIO_NUM_4



uint32_t distance_;
void ultrasonic_test(void *pvParamters)
{
    ultrasonic_sensor_t sensor = {
        .trigger_pin = TRIGGER_GPIO,
        .echo_pin = ECHO_GPIO
    };

    ultrasonic_init(&sensor);

    // while (true)
    {
       
        esp_err_t res = ultrasonic_measure_cm(&sensor, MAX_DISTANCE_CM, &distance_);
        // printf("ahihihiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii");
       
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
                    printf("Echo timeout (i.e. distance_ too big)\n");
                    break;
                default:
                    printf("%d\n", res);
            }
        }
        else{
           
            // printf("Distance: %d cm\n", distance_);
        }
            

        vTaskDelay(900 / portTICK_PERIOD_MS);
    }
}



uint32_t distance_taiche;
void ultrasonic_taiche()
{
    ultrasonic_sensor_t sensor = {
        .trigger_pin = trigger_taiche,
        .echo_pin = echo_taiche 
    };

    ultrasonic_init(&sensor);

    // while (true)
    {
       
        esp_err_t res = ultrasonic_measure_cm(&sensor, MAX_DISTANCE_CM, &distance_taiche);
        // printf("ahihihiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii");
       
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
                    printf("Echo timeout (i.e. distance_ too big)\n");
                    break;
                default:
                    printf("%d\n", res);
            }
        }
        else{
           
            printf("Distance: TAI CHE %d cm\n", distance_taiche);
        }
    }
}

uint32_t distance_khongtaiche;
void ultrasonic_khongtaiche()
{
    ultrasonic_sensor_t sensor = {
        .trigger_pin = trigger_khongtaiche,
        .echo_pin = echo_khongtaiche 
    };

    ultrasonic_init(&sensor);

    // while (true)
    {
       
        esp_err_t res = ultrasonic_measure_cm(&sensor, MAX_DISTANCE_CM, &distance_khongtaiche);
        // printf("ahihihiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii");
       
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
                    printf("Echo timeout (i.e. distance_ too big)\n");
                    break;
                default:
                    printf("%d\n", res);
            }
        }
        else{
           
            printf("Distance: KHONG TAI CHE %d cm\n", distance_khongtaiche);
        }
            

        // vTaskDelay(60000 / portTICK_PERIOD_MS);
    }
}

servoControl myServo;

void servo_left()
{       
    myServo.attach(GPIO_NUM_14);
    myServo.write(68);	
    vTaskDelay(1000 / portTICK_RATE_MS);
		// for (int i = 90; i>0; i--)
        // {
		// 	myServo.write(i);
        //     vTaskDelay(10 / portTICK_RATE_MS);
		// }
        // for ( int j= 0; j<=90; j++)
        // {
        //     myServo.write(j);
        //    TaskDelay(10 / portTICK_RATE_MS);
        // }
    myServo.write(0);	
    vTaskDelay(1000 / portTICK_RATE_MS); 
    myServo.write(68);
    vTaskDelay(1000 / portTICK_RATE_MS);
    cout<< " quay 0 ne"<< endl;
	// myServo.write(90);	
}

void servo_right()
{
    myServo.attach(GPIO_NUM_14);
    myServo.write(68);
    vTaskDelay(200 / portTICK_RATE_MS);
    myServo.write(180);
    vTaskDelay(1000 / portTICK_RATE_MS);      
    myServo.write(68);
    vTaskDelay(200 / portTICK_RATE_MS);
    cout<< " quay 180 ne"<< endl;
    
    //cout<<" tra ve 90"<< endl;
            
      
}

void servo()
{
    myServo.attach(GPIO_NUM_14);
    myServo.write(68);
    vTaskDelay(200 / portTICK_RATE_MS);
    cout<< " back"<< endl;
    
    //cout<<" tra ve 90"<< endl;
            
      
}

int arg_max(float *array, int sl ){
    int kq= 0;
    for( int i=1; i <sl; i++){
        if(array[kq]< array[i]){
            kq =i;
        }
    }
    return kq;
}

int chup_hinh()
{
// uint8_t a[10]={0};
// // doc chuoi python gui ("1")
// uart_read_bytes(UART_NUMBER, a, 1, 1000 / portTICK_RATE_MS);
// uart_flush(UART_NUMBER);
// doInference();
// sendBackPredictions(output);
    // servo();
    // if( a[0]==49 )
    // {
    camera_fb_t * fb = NULL;
    // esp_err_t res = ESP_OK;

    int64_t fr_start = esp_timer_get_time();
    
    // Code chup hinh ne :)
    fb = esp_camera_fb_get();
    if (!fb) {
        cout<<"Camera capture failed"<<endl;
    // ESP_LOGE(TAG, "Camera capture failed");
        
        //  return -1;
    }
    else
    {
    // sendData("Camera cua Q da chup ne :D ");
        //return ;
    }
    // jpg_httpd_handler();

    //sendData((const char*)fb->buf, fb->len);
    //char str[4*fb->len] ="";
    // char tmp[40];
    // for (int i=0; i< fb->len; i++ )
    // {
    //     itoa( (int)fb->buf[i], tmp,10);
    //     strcat( tmp, " ");
    //     sendData(tmp);
    // }
    // // frame2jpg_cb(fb, 80, jpg_encode_stream, 0);
    // sendData("\n");

    for(int i = 0; i < fb->len; i++){
        input->data.f[i] = fb->buf[i]/255.0f;
    }
    // label= {0: 'cardboard', 1: 'glass', 2: 'metal', 3: 'paper', 4: 'plastic', 5: 'trash'};
    // Quanhh= np.argmax(predictions)

    doInference();
    sendBackPredictions(output);
    esp_camera_fb_return(fb);
    int kq_predict = arg_max(output->data.f, output->dims->data[1]);
    // return kq_predict;
    // if( distance_ <20 && distance_ >=0 ){
    //         kq_predict = chup_hinh();
    //         if( kq_predict == -1){
    //             continue;
    //         }
            char *label[]={"cardboard", "glass", "metal", "paper", "plastic", "trash"};
            cout<< label[kq_predict]<< endl;
            
            if( kq_predict== 5|| kq_predict== 3 ){
               return 0;
            }
            else
            {
                return 1;
            }
    // esp_camera_fb_return(fb);
    // vTaskDelay(5000 / portTICK_PERIOD_MS);
		
    //        
    //     }

}

void classification(){
 while (true)
    {   
        
        // cout<<"tai che ne"<< distance_taiche<< "\n";
        // vTaskDelay(500 / portTICK_RATE_MS); 

        ultrasonic_test(0);
        cout<< "distance:"<<distance_<< "\n";
    //     int kq_predict;
       if( distance_ <14 && distance_ >=4 )
        {
            // cout<< "distance:"<<distance_<< "\n";
            int check = chup_hinh();
            if( check ==1 )
            {
                cout<< "bat dau quay TAI CHE"<< endl;
                servo_right();
                ultrasonic_taiche();
            }
            else if (check ==0 )
            {
                cout<< "bat dau quay KHONG TAI CHE"<< endl;
                servo_left();
                ultrasonic_khongtaiche();
            }
            // else
            // {
            //     int check = chup_hinh();
            //     if( check ==1 )
            //     {
            //         cout<< "bat dau quay TAI CHE"<< endl;
            //         servo_right();
            //         ultrasonic_taiche();
            //     }
            //     else if (check ==0 )
            //     {
            //         cout<< "bat dau quay KHONG TAI CHE"<< endl;
            //         servo_left();
            //         ultrasonic_khongtaiche();
            //     }
                    
            //     }
            
            
        }
        
            

    }
}

extern "C" void app_main(void)
{
	
    
    app_camera_init();
    initUart(UART_NUMBER);
    setup();
    classification();
    
    
    // myServo.attach(GPIO_NUM_14);
    // myServo.write(0);	
    // /vTaskDelay(1000 / portTICK_RATE_MS); 
	//Defaults: myServo.attach(pin, 400, 2600, LEDC_CHANNEL_0, LEDC_TIMER0);
	// to use more servo set a valid ledc channel and timer
	  
    // while (true)
    // {   
        
    //     // cout<<"tai che ne"<< distance_taiche<< "\n";
    //     // vTaskDelay(500 / portTICK_RATE_MS); 

    //     ultrasonic_test(0);
    //     cout<< "distance_servo"<<distance_<< "\n";
    // //     int kq_predict;
    //    if( distance_ <20 && distance_ >=0 )
    //     {
    //         // chup_hinh();

    //         int check = chup_hinh();
    //         if( check ==1 )
    //         {
    //             cout<< "bat dau quay TAI CHE"<< endl;
    //             servo_right();

    //         }
    //         else
    //         {
    //             cout<< "bat dau quay KHONG TAI CHE"<< endl;
    //             servo_left();
    //         }
            
    //     }
    //     else if( distance_ >= 25 && distance_ <=40 )
    //     {
           
    //         cout<< "ahihi"<< endl;
    //         servo_left();
    //     }
   
    // }
    
    

    
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