/* #######################################################################
pH метр на базе Arduino Uno ver: 2.0
Разработчики: 
Иванов Р.Н. 
https://github.com/Ushuiski/pH-meter
r.ivanov@avgust.com
Семеняченко А.А. 
https://github.com/sechanius/pH-meter
a.semenyachenko@avgust.com
#########################################################################*/

#include <LiquidCrystal.h>     // библиотека для работы с LCD 1602
#include <OneWire.h>           // библиотека для работы с протоколом 1-Wire
#include <DallasTemperature.h> // библиотека для работы с датчиком DS18B20
#include <EEPROM.h>            // библиотека для EEPROM памяти
#include "pitches.h"           // библиотека нот 
//#include <themes.h>            //add Note vale and duration 


#define BUZZER_PIN 13          // пин с пищалкой
#define NUM_ELT 3              // количество элементов меню
#define OPERATING_VOLTAGE 5.0  // опорное напряжения для АЦП
#define SENSOR 2               // пин АЦП для pH электрода
#define ONE_WIRE_BUS 2         // пин датчика температуры
#define REST 0

OneWire oneWire(ONE_WIRE_BUS); // создаём объект для работы с библиотекой OneWire
 
DallasTemperature sensor(&oneWire); // создадим объект для работы с библиотекой DallasTemperature
 
// инициализируем объект-экран, передаём использованные 
// для подключения контакты на Arduino в порядке:
// RS, E, DB4, DB5, DB6, DB7
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

//Пример создание символа
byte hui[8] = {
    0b00100,
    0b01110,
    0b01110,
    0b01110,
    0b01110,
    0b11111,
    0b11111,
    0b11111
};

// Мелодия Марио
int notes1[] = {
    // 1318, 1318, 1318, 1046, 1318, 1568, 784,
     1046, 784, 659, 880, 987, 932, 880, 784,
     1318, 1568, 1750, 1396, 1568, 1318, 1046, 1174, 987,
     /* 1046, 784, 659, 880, 987, 932, 880,
     784, 1318, 1568, 1750, 1396, 1568, 1318, 1046, 1174, 987,
     1568, 1480, 1396, 1244, 1318, 830, 880, 1046, 880, 1046, 1174,
     0, 1568, 1480, 1396, 1244, 1318, 2093, 2093, 2093,
     1568, 1480, 1396, 1244, 1318, 830, 880, 1046, 880, 1046, 1174, 1244, 1174, 1046, */
};

int times1[] = {
    // 150, 300, 150, 150, 300, 600, 600,
    450, 150, 300, 300, 150, 150, 300, 210,
    210, 150, 300, 150, 150, 300, 150, 150, 450,
    /* 450, 150, 300, 300, 150, 150, 300,
    210, 210, 150, 300, 150, 150, 300, 150, 150, 450,
    150, 150, 150, 300, 150, 150, 150, 150, 150, 150, 150,
    0, 150, 150, 150, 300, 150, 300, 150, 600,
    150, 150, 150, 300, 150, 150, 150, 150, 150, 150, 150, 300, 450, 600, */
};

int delays1[] = {
    // 150, 300, 300, 150, 300, 600, 600,
    450, 450, 450, 300, 300, 150, 300, 210,
    210, 150, 300, 150, 300, 300, 150, 150, 450,
    /* 450, 450, 450, 300, 300, 150, 300,
    210, 210, 150, 300, 150, 300, 300, 150, 150, 600,
    150, 150, 150, 300, 300, 150, 150, 300, 150, 150, 150,
    300, 150, 150, 150, 300, 300, 300, 150, 600,
    150, 150, 150, 300, 300, 150, 150, 300, 150, 150, 450, 450, 450, 1200, */
};

// Мелодия Имперский марш
int notesIM[] = {
    392, 392, 392, 311, 466, 392, 311, 466, 392 /*,
    587, 587, 587, 622, 466, 369, 311, 466, 392,
    784, 392, 392, 784, 739, 698, 659, 622, 659,
    415, 554, 523, 493, 466, 440, 466,
    311, 369, 311, 466, 392 */
};

int timesIM[] = {
    350, 350, 350, 250, 100, 350, 250, 100, 700 /*,
    350, 350, 350, 250, 100, 350, 250, 100, 700,
    350, 250, 100, 350, 250, 100, 100, 100, 450,
    150, 350, 250, 100, 100, 100, 450,
    150, 350, 250, 100, 750 */
};

// change this to make the song slower or faster
int tempo=114; 

// change this to whichever pin you want to use
int buzzer = 13;

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int melody[] = {
    NOTE_E4,4,  NOTE_E4,4,  NOTE_F4,4,  NOTE_G4,4,//1
    NOTE_G4,4,  NOTE_F4,4,  NOTE_E4,4,  NOTE_D4,4,
    NOTE_C4,4,  NOTE_C4,4,  NOTE_D4,4,  NOTE_E4,4,
    NOTE_E4,-4, NOTE_D4,8,  NOTE_D4,2,

    NOTE_E4,4,  NOTE_E4,4,  NOTE_F4,4,  NOTE_G4,4,//4
    NOTE_G4,4,  NOTE_F4,4,  NOTE_E4,4,  NOTE_D4,4,
    NOTE_C4,4,  NOTE_C4,4,  NOTE_D4,4,  NOTE_E4,4,
    NOTE_D4,-4,  NOTE_C4,8,  NOTE_C4,2,

    NOTE_D4,4,  NOTE_D4,4,  NOTE_E4,4,  NOTE_C4,4,//8
    NOTE_D4,4,  NOTE_E4,8,  NOTE_F4,8,  NOTE_E4,4, NOTE_C4,4,
    NOTE_D4,4,  NOTE_E4,8,  NOTE_F4,8,  NOTE_E4,4, NOTE_D4,4,
    NOTE_C4,4,  NOTE_D4,4,  NOTE_G3,2,

    NOTE_E4,4,  NOTE_E4,4,  NOTE_F4,4,  NOTE_G4,4,//12
    NOTE_G4,4,  NOTE_F4,4,  NOTE_E4,4,  NOTE_D4,4,
    NOTE_C4,4,  NOTE_C4,4,  NOTE_D4,4,  NOTE_E4,4,
    NOTE_D4,-4,  NOTE_C4,8,  NOTE_C4,2
};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes=sizeof(melody)/sizeof(melody[0])/2; 

// this calculates the duration of a whole note in ms (60s/tempo)*4 beats
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;

// Бумер
int notes2[] = {
    329, 392, 392, 329, 440, 392, 440, 392, 440, 392, 440, 392, 440, 493
};

int times2[] = {
    300, 600, 300, 600, 300, 300, 300, 300, 300, 300, 300, 300, 300, 600
};

int delays2[] = {
    200, 800, 200, 800, 180, 180, 180, 180, 180, 180, 180, 180, 180, 400
};


int button; //Переменная для значения кнопок
float x;    //Переменная для хранения значений pH
float t;    //Переменная для хранения температуры
float K1 = 0.0; // Коэффициент калибровки pH метра
float K2 = 0.0; // Коэффициент калибровки pH метра

// Структура для элемента меню
struct myStruc{
    byte Select;
    char Next;
    char Previous;
    char Parent;
    char Child;
    String Text;
};

myStruc menu[NUM_ELT]; // Создаём массив структур
myStruc menuNow;       // Создаём структуру menuNow

// Функция вывода ОК! для теста
void ok(void){
    //calib();
    float a1, b1;
    lcd.clear();
    EEPROM.get(1, a1);
    EEPROM.get(5, b1);
    delay(100);
    lcd.print(a1, 4);
    lcd.setCursor(0,1);
    lcd.print(b1, 4);
    delay(3000);
    lcd.clear();
}

// калибровка pH метра
void calib(void){
    float f = 0;
    float f1 = 0;
    float a = 0;
    float x1 = 0;
    float x2 = 0;
    float x3 = 0;
    float y1 = 4.01;
    float y2 = 6.86;
    float y3 = 9.18;
    float k1;
    float k2;
    lcd.clear();
    lcd.print("Please 4,01");               //буфер 4,01
    lcd.setCursor (0, 1);
    lcd.print ("Press right"); 

    while(1){
        button = analogRead(0);
        if (button < 600) { 
            lcd.clear();
            delay(300);
            break;
        }
    }

    delay (20000);
 
    do {
      f = analogRead(SENSOR);
        Serial.print (f,4);
        Serial.print ("\n");
        lcd.print("Calibration...");
        lcd.setCursor (0, 1);
        lcd.print("4,01");
        delay(5000);
        f1 = analogRead(SENSOR);
        a = abs ( f1 - f );
        Serial.print (f1,4);
        Serial.print ("\n");
        Serial.print (a);
        Serial.print ("\n");
        lcd.clear();
    } while(a > 2);
 
    x1 = f1;
    //if (a > 2){
    //delay(30000);
    //}
    //f2 = analogRead(SENSOR);
    //a = f2 - f1;
    //if (a < 2);
    //  x1 = f2;
    lcd.setCursor (6, 0);
    lcd.print("OK!");
    lcd.setCursor (0, 1);
    lcd.print("Press any button");

    while(1){
      button = analogRead(0);
        if (button < 800) { 
           lcd.clear();
           delay(300);
           break;
        }
    }

    lcd.print("Please 6,86");                 //буфер 6,86
    lcd.setCursor (0, 1);
    lcd.print ("Press right"); 

    while(1){
      button = analogRead(0);
      if(button < 600) { 
        lcd.clear();
            delay(300);
            break;
        }
    }

    delay (20000);

    do {
      f = analogRead(SENSOR);
        Serial.print (f,4);
        Serial.print ("\n");
        lcd.print("Calibration...");
        lcd.setCursor (0, 1);
        lcd.print("6,86");
        delay(5000);
        f1 = analogRead(SENSOR);
        a = abs(f1 - f);
        Serial.print (f1,4);
        Serial.print ("\n");
        Serial.print (a);
        Serial.print ("\n");
        lcd.clear();
    } while( a > 2);

    x2 = f1;
    //f = analogRead(SENSOR);
    //delay(30000);
    //f1 = analogRead(SENSOR);
    //a = f1 - f;
    //if (a > 2){
    //  delay(30000);
    //  }
    //}
    //f2 = analogRead(SENSOR);
    //a = f2 - f1;
    //if (a < 2);
    //  x2 = f2;
    lcd.setCursor (6, 0);
    lcd.print("OK!");
    lcd.setCursor (0, 1);
    lcd.print("Press any button");

    while(1){
      button = analogRead(0);
      if(button < 800) { 
        lcd.clear();
            delay(300);
            break;
        }
    }

    lcd.print("Please 9,18");                  //буфер 9,18
    lcd.setCursor (0, 1);
    lcd.print ("Press right"); 

    while(1){
      button = analogRead(0);
        if(button < 600) { 
          lcd.clear();
            delay(300);
            break;
        }
    }

    delay (20000);

    do {  
      f = analogRead(SENSOR);
        Serial.print (f,4);
        Serial.print ("\n");
        lcd.print("Calibration...");
        lcd.setCursor (0, 1);
        lcd.print("9,18");
        delay(5000);
        f1 = analogRead(SENSOR);
        a = abs (f1 - f);
        Serial.print (f1,4);
        Serial.print ("\n");
        Serial.print (a);
        Serial.print ("\n");
        lcd.clear();
    } while( a > 2);

    x3 = f1;
    //f = analogRead(SENSOR);
    //for (int i = 0; i < 17; i++){
    //delay(30000);
    //}
    //f1 = analogRead(SENSOR);
    //a = f1 - f;
    //if (a > 2){
    //  delay(30000);
    //  }
    //}
    //f2 = analogRead(SENSOR);
    //a = f2 - f1;
    //if (a < 2);
    //  x3 = f2;
    lcd.setCursor (6, 0);
    lcd.print("OK!");
    lcd.setCursor (0, 1);
    lcd.print("Press any button");

    while (1){
      button = analogRead(0);
        if(button < 800) { 
          lcd.clear();
            delay(300);
            break;
        }
    }

    k1 = ( 3 * ((x1 * y1) + (x2 * y2) + (x3 * y3)) - ((x1 + x2 + x3)*(y1 + y2 + y3)))/(3*(x1 * x1 + x2 * x2 + x3* x3) - (x1 + x2 + x3)* (x1 + x2 + x3)) ;
    k2 = ((y1 + y2 + y3) - k1 * (x1 + x2 + x3))/3 ;
    Serial.print (k1, 4);
    Serial.print ("\n");
    Serial.print (k2, 4);
    Serial.print ("\n");
    EEPROM.put(1, k1);
    EEPROM.put(5, k2);
    delay(100);
    
}

// Функция чтения pH щупа
float phMeasure(){
  float pHSum = 0;
    int adcSensor = 0;
    float voltageSensor = 0;
    float pHSensor = 0;
   
    for(int i = 0; i < 3; i++){
        adcSensor = analogRead(SENSOR);                       // Считываем аналоговое значение с датчика кислотности жидкости
        //voltageSensor = adcSensor * OPERATING_VOLTAGE / 1023; // Переводим данные сенсора в напряжение
        pHSensor = K1 * adcSensor + K2;                   // Конвертируем напряжение в концентрацию pH
        pHSum += pHSensor;
    }
    return pHSum/3;
}

//Функция чтения термодатчика
float tempMeasure(){
    float tempSum = 0;
    float temperature;
    for(int i = 0; i < 3; i++){
        sensor.requestTemperatures();                       // отправляем запрос на измерение температуры
        temperature = sensor.getTempCByIndex(0);            // считываем данные из регистра датчик
        tempSum += temperature; 
    }
    return tempSum/3;
}


//ГОСТ 33776-2016
void gost(void){
    int i = 0;
    float x1;
    float t1;
    float x3;
    float t3;
    lcd.print("Put your rastvor");
    lcd.setCursor(0,1);
    lcd.print("Press any button");

    while (1){
      button = analogRead(0);
        if (button < 800) { 
            lcd.clear();
            delay(300);
            break;
        }
    }

    for (i = 1; i <= 120; i++){
      if (i = 61){
        x1 = phMeasure();
            t1 = tempMeasure();
            continue;
        }

        x = phMeasure();
        lcd.print("pH=");
        lcd.print(x);
        lcd.print("\x3B");
        t = tempMeasure();
        lcd.print("T = ");
        lcd.print(t);
        lcd.setCursor(0,1);
        delay(1000);  
    }

    float x2 = phMeasure();
    float xp = phMeasure();
    float t2 = tempMeasure();
    float g = abs(x2 - x1);

    if(g > 0,1){
      for(i = 1; i <= 480; i++){
        int ost;
            ost = i % 60;
            if(ost = 0){
              x3 = phMeasure();
                t3 = tempMeasure();
                g = abs(x3 - xp);

                if(g <= 0,1){
                break;
                }
                xp = phMeasure();
                continue;
            }
            delay(1000);
        }
    }

    lcd.print(x1);
    lcd.setCursor (0,7);
    lcd.print(x2);
    lcd.setCursor (0,12);
    lcd.print(x3);
    lcd.setCursor (1,0);
    lcd.print(t1);
    lcd.setCursor (1,7);
    lcd.print(t2);
    lcd.setCursor (1,12);
    lcd.print(t3);

    while(1){
      button = analogRead(0);
      if(button < 800) { 
            lcd.clear();
            delay(300);
            break;
        }
    }
  
}

void setup() {
    // Инициализация перефирии 
    pinMode(BUZZER_PIN, OUTPUT);                        

    lcd.begin(16, 2);
    lcd.createChar(0, hui);                             // create a new character
    lcd.setCursor(0, 0);

    Serial.begin(9600);                                 // инициализируем работу Serial-порта
    
    sensor.begin();                                     // начинаем работу с датчиком
    sensor.setResolution(12);                           // устанавливаем разрешение датчика от 9 до 12 бит                                   //Устанавливаем размер (количество столбцов и строк) экрана

    EEPROM.get(1, K1);
    EEPROM.get(5, K2);
    delay(100);
    Serial.print(K1, 4);
    Serial.print ("\n");
    Serial.print(K2, 4);
    Serial.print ("\n");

    lcd.setCursor(3, 0);
    lcd.print("Avgust Crop");
    lcd.setCursor(3, 1);  
    lcd.print("Protection");                           
    delay(3000);
    lcd.clear();
    
    menu[0] = {0x00, 1, 2, 0, 0, "pH measurment"};
    menu[1] = {0x01, 2, 0, 0, 0, "Calibration"};
    menu[2] = {0x02, 0, 1, 0, 0, "INFO"};
    
    menuNow = menu[0];
}

void loop() {
    lcd.setCursor(2, 0);   
    lcd.print(menuNow.Text);
    delay(100);
    button = analogRead (0);

    if(button < 60) { 
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print ("Right");
        delay(100);
        for(int i = 0; i < 17; i++){
            tone(BUZZER_PIN, notes1[i], times1[i]);
            delay(delays1[i]);
        }
    }

    else if(button < 200){
        menuNow = menu[menuNow.Previous];
        lcd.clear();
        // lcd.print ("Up");
        delay(100);
    }

    else if(button < 400){
        menuNow = menu[menuNow.Next];
        lcd.clear();
        // lcd.print ("Down");
        delay(100);
    }
    else if(button < 600){ 
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print ("Left");
        delay(100);

        for (int i = 0; i < 9; i++){
            tone(BUZZER_PIN, notesIM[i], timesIM[i]*2);
            delay(timesIM[i]*2);
            noTone(BUZZER_PIN);
        }
    }
    else if(button < 800){
        switch(menuNow.Select){
            case 0x00:
               lcd.clear();
               lcd.print("loading...");
             
               while(1){
                    button = analogRead(0);
                    x = phMeasure();
                    t = tempMeasure();
                    lcd.clear();
                 
                    lcd.print("pH = ");
                    lcd.print(x);
                    lcd.print("\x3B");
                 
                    lcd.setCursor(0, 1);

                    lcd.print("T = ");
                    lcd.print(t);
                    lcd.print(" \xDF\x43\x3B");
                 
                    if(button < 600) { 
                        lcd.clear();
                        delay(300);
                        break;
                    }
                 //char lcd_buffer[16]; // Массив для вывода
                 //sprintf(lcd_buffer,"DSdebug=%u",ds1820_devices); //запись в буфер текста и значений
                 //lcd.print(lcd_buffer); // Вывод
                }
                break;
            case 0x01:
                calib();
                break;
            case 0x02:
                lcd.clear();
                lcd.setCursor(0, 1);
                lcd.print("Ty chto durak?");
                lcd.write(byte(0));
                delay(1000);
                lcd.clear();
                ok();
        }
        delay(100);
    } 
       
}
