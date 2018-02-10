#include <SPI.h>
#include <AMIS30543.h>

const uint8_t amis1DirPin = 44;
const uint8_t amis1StepPin = 46;
const uint8_t amis1SlaveSelect = 42;
const uint8_t amis2DirPin = 26;
const uint8_t amis2StepPin = 28;
const uint8_t amis2SlaveSelect = 24;
byte sp=5;
byte inputh;
int buzzer  = 7;
int pinAcc1 = A0;
int pinAcc2 = A1;
int Acc1v = 0;
int Acc2v = 0;
bool acc1ok = false;
bool acc2ok = false;
unsigned long s_time, n_time, d_time, w_time = 5000;
int fspd = 0;

AMIS30543 stepper1, stepper2;

  void amis1Step(int sp)
 { 
  digitalWrite(amis1StepPin, HIGH);
  delayMicroseconds(3);
  digitalWrite(amis1StepPin, LOW);
  delayMicroseconds(3);
  delay(sp);
 }
 
 
 void amis2Step(int sp)
 {
  digitalWrite(amis2StepPin, HIGH);
  delayMicroseconds(3);
  digitalWrite(amis2StepPin, LOW);
  delayMicroseconds(3);
  delay(sp);
 }
 
 
 int StepAZ(unsigned int steps)
 {
    int sph;
    if (steps>25600)
    {
      digitalWrite(amis1DirPin, LOW);
      steps = steps - 25600;
    }
    else
      digitalWrite(amis1DirPin, HIGH);
    if (steps>=356)
    {
      stepper1.setStepMode(128);
      while (steps%4 != 0)
      {
        amis1Step(5);
        steps--;
      } 
      stepper1.setStepMode(32);
      steps = steps / 4;
      for (int i=0; i<=steps; i++)
        amis1Step(5);
      //return 1;
    }
   if (steps >= 71 && steps < 356)
    {
      stepper1.setStepMode(128);
      while (steps%4 != 0)
      {
        amis1Step(5);
        steps--;
      } 
      stepper1.setStepMode(32);
      steps = steps / 4;
      for (int i=0; i<=steps; i++)
        amis1Step(10);
     // return 1;
    }
    if (steps >= 2 && steps <71)
    {
      stepper1.setStepMode(128);
      while (steps%2 != 0)
      {
        amis1Step(5);
        steps--;
      } 
      stepper1.setStepMode(64);
      steps = steps / 2;
      for (int i=0; i<=steps; i++)
        amis1Step(10);
     // return 1;
    }
    if (steps == 1)
    {
      stepper1.setStepMode(128);
        amis1Step(0);
      //return 1;
    }
    return 1;
 }

 int StepALT(unsigned int steps)
 {
    int sph;
    if (steps>25600)
    {
    digitalWrite(amis2DirPin, LOW);
    steps = steps - 25600;
    }
    else
    digitalWrite(amis2DirPin, HIGH);
    if (steps>=356)
    {
      stepper2.setStepMode(128);
      while (steps%4 != 0)
      {
        amis2Step(5);
        steps--;
        //Serial.print("wyrownywanie");
      } 
      stepper2.setStepMode(32);
      steps = steps / 4;
      for (int i=0; i<=steps; i++)
        amis2Step(5);
      //return 1;
    }
    if (steps >= 71 && steps < 356)
    {
      stepper2.setStepMode(128);
      while (steps%4 != 0)
      {
        amis2Step(5);
        steps--;
      } 
      stepper2.setStepMode(32);
      steps = steps / 4;
      for (int i=0; i<=steps; i++)
        amis2Step(10);
      //return 1;
    }
    if (steps >= 2 && steps <71)
    {
      stepper2.setStepMode(128);
      while (steps%2 != 0)
      {
        amis2Step(5);
        steps--;
      } 
      stepper2.setStepMode(64);
      steps = steps / 2;
      for (int i=0; i<=steps; i++)
        amis2Step(10);
      //return 1;
    }
    if (steps == 1)
    {
      stepper2.setStepMode(128);
        amis2Step(0);
     // return 1;
    }
    return 1;
}

SetSpeed(int sspd)
{
  sp = 5;
  if (sspd == 0)
  {
    stepper1.setStepMode(32); delay(1);
    stepper2.setStepMode(32);delay(1);
  }
   if (sspd == 1)
  {
    stepper1.setStepMode(64);delay(1);
    stepper2.setStepMode(64);delay(1);
  }
   if (sspd == 2)
  {
    stepper1.setStepMode(128);delay(1);
    stepper2.setStepMode(128);delay(1);
  }
  
}
 


void setup()
{
  
  SPI.begin();
  Serial.begin(9600);
  Serial.setTimeout(10);
  delay(5);
  stepper1.init(amis1SlaveSelect);
  delay(5);
  stepper2.init(amis2SlaveSelect);
  delay(5);
 /////slave
  pinMode(amis1SlaveSelect, OUTPUT);
  digitalWrite(amis1SlaveSelect, LOW);
  delay(1);
  pinMode(amis2SlaveSelect, OUTPUT);
  digitalWrite(amis2SlaveSelect, HIGH);
  delay(1);
  // Drive the NXT/STEP and DIR pins low initially.
  digitalWrite(amis1StepPin, LOW);
  pinMode(amis1StepPin, OUTPUT);
  digitalWrite(amis1DirPin, HIGH);
  pinMode(amis1DirPin, OUTPUT);
  digitalWrite(amis2StepPin, LOW);
  pinMode(amis2StepPin, OUTPUT);
  digitalWrite(amis2DirPin, LOW);
  pinMode(amis2DirPin, OUTPUT);


  // Give the driver some time to power up.
  delay(1);

  stepper1.resetSettings();
  delay(1);
  stepper2.resetSettings();
  delay(1);
 
  stepper1.setCurrentMilliamps(400);
   delay(1);
  stepper2.setCurrentMilliamps(400);
   delay(1);

  stepper1.enableDriver();
   delay(1);
  stepper2.enableDriver();
   delay(1);

  stepper1.setStepMode(32); delay(1);
  stepper2.setStepMode(32); delay(1);

}

void loop() 
{
  do{
  Serial.print("j");
  delay(1000);
  }while(Serial.read() != 105);//co sekunde sprawdza czy serwer odpowiada
  //Serial.print("polaczono");
  do
  {
    if(Serial.available() > 0)
    {
      do
      {
         int choice = Serial.read();
         if (choice == 98) //BAZOWANIE
        {
          //Serial.print("bazowanie");
          stepper1.setStepMode(32);
           delay(1);
          stepper2.setStepMode(32);
           delay(1);
          do
          {
            int input = Serial.read();
            
            //LEFT   
             if(input==113)
             {
              digitalWrite(amis1DirPin, LOW);
              do
                {
                inputh = Serial.read();
                amis1Step(sp);
                if(inputh==97)
                  break;
                }while(true);
             }
            
            //RIGHT    
              if(input==119)
              {
              digitalWrite(amis1DirPin, HIGH);
               do
                {
                inputh = Serial.read();
                amis1Step(sp);
                if(inputh==115)
                  break;
                }while(true);
              }
            
            //UP    
              if(input==101)
             {
              digitalWrite(amis2DirPin, HIGH);
              do
                {
                 inputh = Serial.read();
                 amis2Step(sp);
                if(inputh==100)
                  break;
                }while(true);
             }
            
            //DOWN  
              if(input==114)
              {
              digitalWrite(amis2DirPin, LOW);
               do
                {
                inputh = Serial.read();
                amis2Step(sp);
                if(inputh==102)
                  break;
                }while(true);
            }
            if(input == 108)
            {
              if (fspd > 0)
                fspd--;
              SetSpeed(fspd);
            }
             if(input == 117)
            {
              if (fspd < 2)
                fspd++;
              SetSpeed(fspd);
            }
            }
            //EXIT
              if(input==120)
                break;
                
          }while(true);
            
        }
        if(choice == 99) //SZUKAJ I SLEDZ
        {
         // Serial.print("szukajisledz");
          do
          {
                int axis = Serial.read();
                if (axis == 116)
                  {
                   // Serial.print("osalt:");
                    unsigned int steps = Serial.parseInt();
                    Serial.print(steps);
                    StepALT(steps);
                    Serial.print("k");
                  }
                if (axis == 122)
                  {
                    //Serial.print("osaz:");
                    unsigned int steps = Serial.parseInt();
                    Serial.print(steps);
                    StepAZ(steps);
                    Serial.print("k");
                  }
                if (axis == 120)
                  break;
              }while(true);
            }
            if (choice == 112) //POZIOMOWANIE
            {
              do
              {
                  Acc1v = analogRead(pinAcc1);
                  if (Acc1v < 500 || Acc1v > 523)
                      tone(buzzer, 100);
     
                  else
                  {
                    s_time = millis();
                    //Serial.println(s_time);
                    noTone(buzzer);
                    do
                      {
                        n_time = millis();
                        d_time = n_time - s_time;
                        Acc1v = analogRead(pinAcc1);
                        if (Acc1v < 500 || Acc1v > 523)
                          break;
                        if (d_time >=w_time)
                          acc1ok = true;
                      }while(d_time!=w_time);
                  }
                    if (acc1ok == true)
                    {
                      do
                      {
                        Acc2v = analogRead(pinAcc2);
                        if (Acc2v < 500 || Acc2v > 523)
                          tone(buzzer, 200);
                        else
                          {
                              s_time = millis();
                              //Serial.println(s_time);
                              noTone(buzzer);
                              do
                              {
                                  n_time = millis();
                                  d_time = n_time - s_time;
                                  Acc2v = analogRead(pinAcc2);
                                  if (Acc2v < 500 || Acc2v > 523)
                                      break;
                                  if (d_time >=w_time)
                                      acc2ok = true;
                              }while(d_time!=w_time);
                        }
                      }while(acc2ok == false);
                    } 
                        if (acc1ok == true && acc2ok == true)
                            break;
                        }while(true);
                        Serial.print("p");
          
              }
      }while(true);
    }
  }while(true);
} 

        
  


