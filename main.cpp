#include "mbed.h"
#include <RF22.h>
#include <RF22ReliableDatagram.h>
#include <GPS.h>
#include "MPU6050.h"

// Sample programm for ReliableDatagramm Sending
// Uses address 1 and sends to RF22 with address 2
// See notebook http://mbed.org/users/charly/notebook/rfm22/ for connecting RFM22 to mbed
int beh=0;
DigitalOut myled(LED1); 
Serial pc(USBTX, USBRX);
GPS af(PB_10, PB_11);
//RF22ReliableDatagram (uint8_t thisAddress, PinName slaveSelectPin, PinName mosi, PinName miso, PinName sclk, PinName interrupt)
RF22ReliableDatagram rf22(0,PB_6,PA_7,PA_6,PA_5,PA_10);
float frequency = 433;           // frequency 
int af1=0;
const uint8_t sender_adress = 1;        // address of sender
const uint8_t receiver_adress =2;       // address of receiver 
float b;
float gy[100]={0};
//setting LED1 to give digital output
//creating onject of MPU6050 class
MPU6050 mpu(PB_9,PB_8);
void gyro()
{
    int i=0;
    b=0;
    
    while(i<100)
    {                    
        float gyro[3];
        mpu.getGyro(gyro);
        
        float a=gyro[0]*gyro[0]+gyro[1]*gyro[1]+gyro[2]*gyro[2];
        gy[i]=sqrt(a);
        pc.printf("Total Acc = %f \r\n",gy[i]);
        b=b+gy[i];
        //pc.printf("\r\n Add = %f\r\n",b);
        wait(.1); 
        i++;
    }
               
    b=b/100;
    //pc.printf("\r\n B = %f \r\n",b);
 
} 
 
// send messages forever
void send_loop() {
    uint8_t data[32] = "";
    af1=0;
 
    while (af1==0) {
        
        if(af.sample()==1)
        {
            float latitude=af.latitude;
            float longitude=af.longitude;
        sprintf((char*)data,"%f,%f,%d",latitude,longitude,beh);
        pc.printf("%f, %f,%d",latitude,longitude,beh);
        //sendtoWait(uint8_t* buf, uint8_t len, uint8_t address);
        pc.printf("\n\rStart sending ... ");
        if (rf22.sendtoWait(data, sizeof(data), receiver_adress)) {
            pc.printf("Send to %i ACK: >>%s<< ", receiver_adress,(char*)data);
        } else {
            pc.printf("Send to %i NOTACK: >>%s<< ", receiver_adress,(char*)data);
        }
        pc.printf("sleeping 2 seconds...  ");
        af1=1;
        wait(1); 
        }
        else
        {
            //myled=1;
          //  wait(1);
            //myled=0;
            //wait(1);
        } // Wait 2 Seconds
    }
}
 
int main() {
 
    pc.baud(9600);
    pc.printf("\n\rConnected to mbed\n\r");
 
    pc.printf ("RF22-Test-Reliable-Send V1.0\n\r");
 
    // initialize the device
    if (!rf22.init())
        pc.printf("RF22 init failed\n\r");
 
    // set to 19.2 KB
    if (!rf22.setModemConfig(RF22::GFSK_Rb19_2Fd9_6))
        pc.printf("setModemConfig failed");
 
    if (!rf22.setFrequency(frequency))
        pc.printf("setFrequency failed");
 
    // Code for sending
    pc.printf("I am sending with address %i to adress %i ...\n\r",sender_adress,receiver_adress  );
    
    int k=2;

    gyro();
    
    while(1)
    {
      //  gyro();
    //if(b>1.5)
    //{
     //   send_loop();
    //} 
    
    //pc.printf("\r\n %f \r\n",b);
        while(b<2.5)
        {
            //if(b<2.5)
            //{
              //flag=1;
            //}
              
            //if(flag==1)
            {
                 beh=1;//standstill
                 k=k+k;
                 for(int l=0;l<k;l++)
                 {
                      //myled=1;
                      wait(1);
                      //myled=0;
                      gyro();
                      pc.printf("\r\nSlooooooooooooooooowwwwwwwwwwww\r\n %f \r\n",b);
                      if(b>2.5)
                      {
                          k=2;
                          if(b>5)
                          {
                              beh=3;
                          }
                          else
                          beh=2;
                      }
                      if(b>2.5)
                      break;
                  }
                  //flag=0;
                  
                  
                  pc.printf("\r\nSlow man :p\r\n");
                  send_loop();
                  //gyro();
            }
            //else
            //{
              //   gyro();
                // k=2;
            //}
        }
        
        while(b<5 && b>2.5)
        {
            if(b<5 && b>2.5)
            //{
              //flag=1;
            //}
              
            //if(flag==1)
            {
                 //k=k+k;
                 int j=6;
                 for(int l=0;l<j;l++)
                 {
                     // myled=1;
                      wait(1);
                      //myled=0;
                      gyro();
                      pc.printf("\r\n FAAAAAAAAAASSSSSSSSSSSSSSTTTTTTTTTTTT \r\n %f \r\n",b);
                      if(b>5 || b<2.5)
                      {
                       if(b>5)
                          {
                              beh=3;
                          }
                          else
                          {
                              beh=1;
                          }
                      }
                      
                      if(b>5 || b<2.5)
                      break;
                  }
                  //flag=0;
                  pc.printf("\r\n Iam fast Iam fast :p \r\n");
                  //gyro();
                  send_loop();
            }
        }
        
        while(b>5)
        {
            if(b>5)
            {
                
                gyro();
                wait(1);
                
                pc.printf("\r\n I Am High \r\n %f \r\n",b);
            }
        if(b<5)
         {             
            if(b<2.5)
            {
                beh=1;
            }
            else
            {
                beh=2;
            }
              
          }
          send_loop();
    }
            //flag=0;
            
        }
        
    
}

      
    