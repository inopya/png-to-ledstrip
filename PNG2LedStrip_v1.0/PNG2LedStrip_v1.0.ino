
/*
#       _\|/_   A ver..., ¿que tenemos por aqui?
#       (O-O)        
# ---oOO-(_)-OOo---------------------------------
 
 
##########################################################
# ****************************************************** #
# *           DOMOTICA PARA PRINCIPIANTES              * #
# *   Pantalla LED basada en leds direccionables       * #
# *       Autor:  https://github.com/inopya/           * #
# *                                                    * #
# *       Versión 1.0       Fecha: 25/06/2022          * #
# ****************************************************** #
##########################################################
*/

#define __VERSION__ "\nPantalla LED basada en leds direccionables v1.0\n"


/*
      ===== NOTAS DE LA VERSION ===== 

      Lectura de imagenes 16x16 desde memoria FLASH 
	  y representadas en pantalla basada en leds direccionables 
	  
      uso de memoria aproximado (compilado con visualStudio + platformIO):
        ROM 29578 bytes (91.7%) con 35 fotogramas
        RAM 41 bytes (2%)

*/



/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//        SECCIONPARA IMPORTACION DE LIBRERIAS
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

#include <Arduino.h>                //necesaria para visualStudio + platformIO

#include <Adafruit_NeoPixel.h>      // control de leds RGB direccionables

#include "fotogramas.h"             //contiene la descripcion de pixeles de los
                                    //fotogramas en fullcolor, 16M colores



/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//        ALGUNAS DEFINICIONES PERSONALES PARA MI COMODIDAD AL ESCRIBIR CODIGO
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

#define SERIAL_BEGIN          //Serial.begin
#define PRINT                 //Serial.print
#define PRINTLN               //Serial.println
#define PRINT_DEBUG           //Serial.print
#define PRINTLN_DEBUG         //Serial.println

#define SERIAL_BAUD_RATE      115200



/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//        SECCION DE DEFINICION DE PINES
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

#define PIN_LED_OnBoard             13   // Led on Board
#define PIN_TIRA_PANTALLA            7 	 // tira de leds que hace de pantalla



/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//        SECCION DE DEFINICION DE CONSTANTES DEL PROGRAMA
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

#define TIPO_LED          		   NEO_GRB + NEO_KHZ800 //tipo controlador de los led  
#define SCREEN_TOTAL_PIXELS        256 

#define BRILLO                      35 
#define PAUSA_FOTOGRAMA           3000 



/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//        SECCION DE DEFINICION DE VARIABLES GLOBALES
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/



/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//        SECCION DE CREACION DE OBJETOS
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

Adafruit_NeoPixel pantalla_led = Adafruit_NeoPixel(SCREEN_TOTAL_PIXELS, PIN_TIRA_PANTALLA, TIPO_LED);





/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//        PROTOTIPADO DE FUNCIONES
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

void dibuja_fotograma_24bits( uint8_t );




/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
   ****************************************************************************************************** 
                                    FUNCION DE CONFIGURACION
   ****************************************************************************************************** 
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

void setup()
{
  SERIAL_BEGIN( SERIAL_BAUD_RATE );         // Iniciar puerto serie para DEBUG
  PRINTLN (F(__VERSION__));                 // mostramos la version por consola, DEBUG

  pantalla_led.begin();

  
  pinMode(PIN_LED_OnBoard, OUTPUT);         // declarar el pin 13 como salida
  digitalWrite(PIN_LED_OnBoard, LOW);       // apagar el led del pin 13
  
 
  pantalla_led.begin();                 	// inicializacion de la tira
  pantalla_led.setBrightness(BRILLO);   	// Brillo de los LEDS , mas brillo, mas consumo electrico
  pantalla_led.show();                  	// muestra datos en pixel


  PRINTLN(F(" INICIANDO PROYECCION...\n\n"));

}



 
/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
   ****************************************************************************************************** 
                                  BUCLE PRINCIPAL DEL PROGRAMA
   ****************************************************************************************************** 
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/


void loop()
{
  
  for(uint8_t f=0; f<(sizeof(fotogramas)/sizeof(fotogramas[0])); f++){

    PRINT_DEBUG(F(" == FOTOGRAMA ")); PRINTLN_DEBUG(f);  
    
    dibuja_fotograma_24bits(f);
    delay(PAUSA_FOTOGRAMA);
    
  }

}



/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
   xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx 
        BLOQUE DE FUNCIONES: LECTURAS DE SENSORES, COMUNICACION SERIE, INTERRUPCIONES...
   xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx 
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/


/*mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm
//    CONTROL DE LA TIRA DE LEDS PARA USO COMO PANTALLA
//mmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmmm*/

//========================================================
//  MOSTRAR FOTOGRAMAS FULL COLOR 24 bits
//========================================================

void dibuja_fotograma_24bits(uint8_t _frame)
{

  uint16_t frame_addr = pgm_read_word(&(fotogramas[_frame]));

  for (uint16_t x=0;x<SCREEN_TOTAL_PIXELS;x++){
    //lectura de las 3 componentes de 1 fotograma
    uint8_t r = pgm_read_byte(frame_addr+3*x);
    uint8_t g = pgm_read_byte(frame_addr+3*x+1);
    uint8_t b = pgm_read_byte(frame_addr+3*x+2);


    pantalla_led.setPixelColor(x, pantalla_led.Color(r,g,b));
  }
  pantalla_led.show();  
}




//*******************************************************
//                    FIN DE PROGRAMA
//*******************************************************
