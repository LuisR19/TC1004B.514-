
#include <ESP8266WiFi.h>
#include "FirebaseESP8266.h"

#define WIFI_SSID "INFINITUM669F_2.4"
#define WIFI_PASSWORD "MGPqN9E4tY"
#define FIREBASE_HOST "https://li-tec-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "1JuJwCYldFn5OW1Iplng7cxC8Qsu8rCg0ZX5uG6g"

#define LED 16 // D0
#define SENSOR_LUZ_LED 5 // D1
#define SENSOR_LUZ_SOLAR 4 // D2
#define INDICADOR_CONECTANDO 2 // D4

volatile bool controlManual;
volatile bool ledEncendido;
volatile bool ledFundido;

String path = "farolEste/";

FirebaseData firebaseData;

void setup()
{
    Serial.begin(9600);

    pinMode(LED, OUTPUT);
    pinMode(SENSOR_LUZ_LED, INPUT);
    pinMode(SENSOR_LUZ_SOLAR, INPUT);
    pinMode(INDICADOR_CONECTANDO, OUTPUT);

    digitalWrite(INDICADOR_CONECTANDO, HIGH);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("connecting");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }
    Serial.println();
    Serial.print("connected: ");
    Serial.println(WiFi.localIP());

    digitalWrite(INDICADOR_CONECTANDO, LOW);

    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);
}

void loop()
{
    if (ledFundido)
    {
        ledEncendido = true;
        escribir_led_encendido();

        digitalWrite(LED, HIGH);

        delay(5000);
        
        bool hayQueSuprimirAlerta = true;
        do
        {
            hayQueSuprimirAlerta = true;
            for (int i = 0; i < 10; ++i)
            {
                delay(500);
                Serial.println(luzLed());
                if (!luzLed())
                {
                    hayQueSuprimirAlerta = false;
                    break;
                }
                Serial.println(i);
            }
        }
        while(!hayQueSuprimirAlerta);

        ledFundido = false;
        escribir_led_fundido();
    }

    leer_control_manual();

    if (controlManual)
    {
        ledEncendido = !luzSolar();
        escribir_led_encendido();
    }
    else
    {
        leer_led_encendido();
    }

    digitalWrite(LED, ledEncendido);

    if (ledEncendido && !luzLed())
    {
        bool hayQueEnviarAlerta = true;
        for (int i = 0; i < 10; ++i)
        {
            if (luzLed())
            {
                hayQueEnviarAlerta = false;
                break;
            }
            delay(500);
        }
        if (hayQueEnviarAlerta)
        {
            ledFundido = true;
            escribir_led_fundido();
        }
    }   

    delay(50);
}

void leer_control_manual()
{
    if (Firebase.getBool(firebaseData, path + "controlManual"))
    {
        Serial.println("PASSED");
        Serial.println("PATH: " + firebaseData.dataPath());
        Serial.println("TYPE: " + firebaseData.dataType());
        Serial.println("ETag: " + firebaseData.ETag());
        Serial.print("VALUE: ");
        controlManual = firebaseData.boolData();
        Serial.println(controlManual);
        Serial.println("------------------------------------");
        Serial.println();
    }
    else
    {
        Serial.println("FAILED");
        Serial.println("REASON: " + firebaseData.errorReason());
        Serial.println("------------------------------------");
        Serial.println();
    }
}

void leer_led_encendido()
{
    if (Firebase.getBool(firebaseData, path + "ledEncendido"))
    {
        Serial.println("PASSED");
        Serial.println("PATH: " + firebaseData.dataPath());
        Serial.println("TYPE: " + firebaseData.dataType());
        Serial.println("ETag: " + firebaseData.ETag());
        Serial.print("VALUE: ");
        ledEncendido = firebaseData.boolData();
        Serial.println(ledEncendido);
        Serial.println("------------------------------------");
        Serial.println();
    }
    else
    {
        Serial.println("FAILED");
        Serial.println("REASON: " + firebaseData.errorReason());
        Serial.println("------------------------------------");
        Serial.println();
    }
}

void escribir_led_encendido()
{
    if (Firebase.setBool(firebaseData, path + "ledEncendido", ledEncendido))
    {
        Serial.println("PASSED");
        Serial.println("PATH: " + firebaseData.dataPath());
        Serial.println("TYPE: " + firebaseData.dataType());
        Serial.println("ETag: " + firebaseData.ETag());
        Serial.print("VALUE: ");
        ledEncendido = firebaseData.boolData();
        Serial.println(ledEncendido);
        Serial.println("------------------------------------");
        Serial.println();
    }
    else
    {
        Serial.println("FAILED");
        Serial.println("REASON: " + firebaseData.errorReason());
        Serial.println("------------------------------------");
        Serial.println();
    }
}

void escribir_led_fundido()
{
    if (Firebase.setBool(firebaseData, path + "ledFundido", ledFundido))
    {
        Serial.println("PASSED");
        Serial.println("PATH: " + firebaseData.dataPath());
        Serial.println("TYPE: " + firebaseData.dataType());
        Serial.println("ETag: " + firebaseData.ETag());
        Serial.print("VALUE: ");
        ledFundido = firebaseData.boolData();
        Serial.println(ledFundido);
        Serial.println("------------------------------------");
        Serial.println();
    }
    else
    {
        Serial.println("FAILED");
        Serial.println("REASON: " + firebaseData.errorReason());
        Serial.println("------------------------------------");
        Serial.println();
    }
}

bool luzSolar()
{
    return !digitalRead(SENSOR_LUZ_SOLAR);
}

bool luzLed()
{
    return !digitalRead(SENSOR_LUZ_LED);
}
