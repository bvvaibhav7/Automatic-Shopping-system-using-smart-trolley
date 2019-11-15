//********************************************* AUTOMATIC SHOPPING CART*****************************************************
//******************** created by Venkata Vaibhav Bhagavatula, Varad Anant Pimpalkhute, Nilimay Nirbhik*********************
//**************************************************************************************************************************

#include <SPI.h>      //include the header file for SPI communication
#include <MFRC522.h>  //include the RFID module model 

#define SW 2          //Conncting the switch button 1 for providing an option to remove items
#define SW1 5         //Connecting switch button 2 for creating a customer id
#define SS_PIN 10     //SDA pin
#define RST_PIN 9     //reset pin of RFID card reader

//***********************************Global variables*****************************************************
int sum,i; 
int code_len = 7;     //custemer id length
int k = 0;
int flag = 0;         //Flag to check whether the person has got a customer id or not
int quantity[7];      //Array of available items

String letters[]= {"a", "b", "c", "d", "e", "f","g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};
String randString = "";

String str[7]={"87 C1 D6 73","D3 2A 3D D9","95 5E 7D B0","E8 C0 B2 79","CB D1 65 D9","EB A9 F1 D9","98 60 6C D9"};
String str1[7]={"PEN      ","NOTE BOOK","DOSA     ","AGARBATI ", "CASHEWNUT","PERFUME  ", "BUTTER   "};
int price[7]={20,40,60,35,99,64,150};

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

 
void setup() 
{
  pinMode(SW, INPUT_PULLUP);  
  pinMode(SW1, INPUT_PULLUP);  
  Serial.begin(9600);                   // Initiate a serial communication
  SPI.begin();                          // Initiate  SPI bus
  mfrc522.PCD_Init();                   // Initiate MFRC522
  mfrc522.PCD_DumpVersionToSerial();    // Show details of PCD - MFRC522 Card Reader details
}
void loop() 
{
  //creating a random customer id
      if (digitalRead(SW1)==0)          //create a customer id when the switch button 2 is pressed
      {
        sum=0;
        flag = 1;                       //Flag to tell that the customer has got an id and can proceed for shopping
        
        for(i=0;i<7;i++)                //initially making quantities to 0
        {
          quantity[i]=0;
        }
        
        if(code_len > 0)
        {
          for(i = 0; i<code_len; i++)
          {
           randString = randString + letters[random(0, 36)];    
          }

   
        Serial.println("Welcome dear customer");
        Serial.print("This is your customer id:");
        Serial.print("  ");
        Serial.print(randString);
        Serial.println();
        Serial.println("You may start the shopping now......");
        Serial.println();
        delay(1000);
        Serial.println("Please place your RFID tag/card near the reader:");
        Serial.println();
        randString = "";
        }
       }
       if (flag == 1)               //Allow shopping only when the customer has got a unique customer id
       {
          if ( ! mfrc522.PICC_IsNewCardPresent()) 
          {
            return;
          }
          // Select one of the cards
          if ( ! mfrc522.PICC_ReadCardSerial()) 
          {
            return;
          }

          //Detect the tag id and printing it
          String content= "";
          byte letter;
          for (byte i = 0; i < mfrc522.uid.size; i++) 
          {
             //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
             //Serial.print(mfrc522.uid.uidByte[i], HEX);
             content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
             content.concat(String(mfrc522.uid.uidByte[i], HEX));
          }
          Serial.println();
          content.toUpperCase();

          //Check for the accesed rfid tag id so as to fetch the item related to it 
          for (i=0;i<7;i++)
          {
            if (content.substring(1) == str[i])           //If matched with itms present in the shop then add it to bill and update the billing amount
            {
              Serial.println(str1[i]);                
              if(digitalRead(SW)!=0)
              {
                quantity[i]++;                            //increment the items quantity
                Serial.println(quantity[i]);
                sum=sum+price[i];                         //update the price
                Serial.println(sum);
              }
              else
              {
                quantity[i]--;                            //If individual want to remove any item then it will be removed from the billing list also
                if(quantity[i]<0)
                {
                  Serial.println("Sorry no such item");   
                }
                else
                {
                  sum=sum-price[i];                       //Update the total bill amount
                  Serial.println(sum);
                } 
              }
            }
          }
    
            //Printing the final list
            Serial.println("********************************************************************************************");
            Serial.println("                             BILL OF ITEMS                                                  ");
            Serial.println("********************************************************************************************");
            for (i=0;i<7;i++)
            {
              if (quantity[i] > 0)
              {
                Serial.print(str1[i]);
                Serial.print("                              ");
                Serial.print(quantity[i]);
                Serial.print("                              ");
                Serial.print(quantity[i]*price[i]);
                Serial.print("                              ");
                Serial.println();
              }
              else
              {
                if(quantity[i]==-1)
                  quantity[i]++;
              }
            }
            Serial.println();
            Serial.println("********************************************************************************************");
            Serial.print("                                 TOTAL = ");
            Serial.print(sum);
            Serial.println();
            Serial.println("********************************************************************************************");
            
            
          delay(1000); 
       }
        
}
