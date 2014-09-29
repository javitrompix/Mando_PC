/*Programa para un mando a distancia para windows y linux. Con caratula de un DVD. Y soporte para reproductores online.
*/

//Librerias necesarias
#include <IRremote.h>
#include <TM1628R.h>
#include "MediaKeys.h"


/*Configuraciones de las librerias.
*************************************************************************
*/
//Patilla del receptor infrarojo:
#define RECV_PIN  4

  IRrecv infra(RECV_PIN);
  decode_results results;
    
//Patillas del Display del DVD: DIO, CLK, STB
TM1628R dvdLED(1, 0, 2);

/*Costantes
*************************************************************************
*/
//sistemas operativos
#define win   0
#define linux 1

//programas
#define WMP   0
#define VLC   1
#define FOTOS 2
#define TIEMPO 3
#define TEMP  4
#define ONLINE 5    //Este define debe ser el ultimo

/*Variables
*************************************************************************
*/
int sistema = 1;
int programa = TIEMPO;
boolean player = false;
boolean mute = false;
long borrapantalla = 0;
int lecturaanterior = 0;
int registro;

int teclado;

/*Botones mando Orange
*************************************************************************
*/
#define IRprotocolo  NEC            //Protocolo a usar
#define IRarriba     0xD02F         //Devuelve el valor 1
#define IRabajo      0x30CF         //Devuelve el valor 2 
#define IRizquierda  0xD827         //Devuelve el valor 3
#define IRderecha    0x38C7         //Devuelve el valor 4
#define IRcentro     0xA857         //Devuelve el valor 5
#define Repeticion   0xFFFFFFFF     //Devuelve el valor 6

#define Power        0x48B7
#define Play         0xA45B
#define Stop         0x24DB 
#define VolumenU     0xB04F 
#define VolumenD     0x708F
#define Mute         0xF00F
#define Info         0xC837
#define Menu         0x28D7
#define mos          0x8C73
#define guide        0x04FB
#define list         0x0CF3
#define out          0x8877
#define repeat       0x50AF
#define T1           0x807F
#define T2           0x40BF
#define T3           0xC03F
#define T4           0x20DF
#define T5           0xA05F
#define T6           0x609F
#define T7           0xE01F
#define T8           0x10EF
#define T9           0x906F
#define T0           0x00FF
#define VOD          0xF807 
#define TV           0x14EB
#define Record       0x847B
#define Rojo         0x6897
#define Verde        0xE817
#define Amarillo     0x18E7
#define Azul         0x9867
#define Avance       0x649B
#define Retroceso    0xB847

void setup() {
  Serial.begin(9600);      // abre el puerto serie a 9600 bps:

  analogReference(INTERNAL); //Valor para Leonardo de 2,56V (2.56/1023 = 0,0025024437927664)
  
  dvdLED.begin(ON, 1); 
  infra.enableIRIn(); // Comienza a recibir

  cargaCD();  
  intro();  
  vueltaCD();
  dvdLED.clear();

  registro = LM35(); // para que lea bien

switch (programa) {
  case TIEMPO:
    Tiempo ();
    break;

  case TEMP:
    Temperatura();
    break;
    
  default:     
    DVDprograma(); 
}

}

void loop() {
  
  borrapantalla ++;

//  Serial.println(borrapantalla);
  
  if(borrapantalla > 1000){
//  Serial.println("Entró");    
    switch (programa) {
      case TIEMPO:
        Tiempo ();
        break;

      case TEMP:
        Temperatura();
        break;
    
      default:     
        DVDprograma(); 
    }
    
    borrapantalla = 0;
  }
  
/*Comprobamos el teclado
K5 4
K2 16
K3 32
SI NO HAY NADA PULSADO DEVUELVE 0
*/
  teclado = dvdLED.getButtons();    
//  Serial.println(teclado);



  //Enpezamos la recepcion de codigos  
  if (infra.decode(&results)) {
  int lectura = results.value;
  int tipo  = results.decode_type;

  borrapantalla = 0;
  
  if (tipo == IRprotocolo){

  //Comprobamos si es repeticion
  if(lectura == Repeticion){  
  //Repetimos con subir volumen
    if(lecturaanterior == VolumenU){
      lectura = lecturaanterior;
    } 
  //Repetimos con bajar volumen 
    if(lecturaanterior == VolumenD){
      lectura = lecturaanterior;
    }
  //Repetimos con Avance 
    if(lecturaanterior == Avance){
      lectura = lecturaanterior;
    }   
  //Repetimos con Retroceso 
    if(lecturaanterior == Retroceso){
      lectura = lecturaanterior;
    }  
  }   

  vueltaCDf();
  
  switch (lectura) {
    //------------------------------
    case Play:
      
      if (programa != FOTOS){        //En modo FOTOS no encendemos las luces Play/Pausa
        if(player){
          dvdLED.setLEDon(LED_PAU);
          dvdLED.setLEDoff(LED_PLY);
          player = false;
        }
        else{
          dvdLED.setLEDoff(LED_PAU);
          dvdLED.setLEDon(LED_PLY);
          player = true;      
        }
      }
      
      if (programa == ONLINE){
      //Estamos en modo Online, tenemos que usar el raton
        Mouse.begin();

        for(int i=0;i<14;i++){
          Mouse.move(-100, -100, 0);      //Me pongo en la esquina superior izquierda 
        }
        
        Mouse.move(100, 2, 0);            //Un poco abajo y a la derecha       
        Mouse.move(100, 0, 0);            //A la derecha  
        
        Mouse.click();
        Mouse.end();
      }      

      if (programa == WMP){
        Media.write(REMOTE_PLAY_PAUSE); //press + release   
      }
      
      if (programa == VLC){
        Keyboard.begin();
        Keyboard.press(' '); 
        delay(100);
        Keyboard.releaseAll();  
      }  
      
      break;
    //------------------------------
    
    //------------------------------    
    case Avance:

      if (programa == VLC){
        Keyboard.begin();
        Keyboard.press(KEY_LEFT_ALT);
        Keyboard.press(KEY_RIGHT_ARROW );       
//         delay(100);
        Keyboard.releaseAll();  
      }
      
      break;
    //------------------------------

    //------------------------------    
    case Retroceso:

      if (programa == VLC){
        Keyboard.begin();
        Keyboard.press(KEY_LEFT_ALT);
        Keyboard.press(KEY_LEFT_ARROW );         
//        delay(100);
        Keyboard.releaseAll();  
      }
      
      break;
    //------------------------------

    //------------------------------
    case VolumenU:

      Media.write(VOLUME_UP); //press + release       
      
      DVDvol_mas();

      if(mute){
        dvdLED.setLEDoff(LED_DDD);
        mute = false;        
      }
      
      break;
    //------------------------------

    //------------------------------
    case VolumenD:
    
      Media.write(VOLUME_DOWN); //press + release     
    
      DVDvol_menos();
      
      if(mute){
        dvdLED.setLEDoff(LED_DDD);
        mute = false;        
      }
      
      break;
    //------------------------------
 
    //------------------------------
    case Mute:
    
      Media.write(VOLUME_MUTE); //press + release
      
      QuitaPuntos ();      
      dvdLED.setCursor(0);
      dvdLED.print("Mute   ");
      
      if(mute){
      dvdLED.setLEDoff(LED_DDD);
      mute = false;
      }
      else{
      dvdLED.setLEDon(LED_DDD);
      mute = true;
      
      }
      
      break;
    //------------------------------

    //------------------------------
    case Info:
      DVDsistema();    
      break;
    //------------------------------

    //------------------------------
    case mos:
      Temperatura();     
      break;
    //------------------------------
    
    
    //------------------------------
    case IRarriba:
      programa = programa + 1;
      if(programa > ONLINE){
        programa = 0;
      }
      DVDprograma();      
      break;
    //------------------------------    
    
    //------------------------------
    case IRabajo:
      if(programa == 0){
        programa = ONLINE;
      }
      else{
        programa = programa - 1;        
      }
      
      DVDprograma();
      break;
    //------------------------------ 
 
    //------------------------------
    case IRderecha:
      if(programa == FOTOS){
        Keyboard.begin();
        Keyboard.press(' '); 
        delay(100);
        Keyboard.releaseAll();
        QuitaPP();  
        player = false;  
      }     
      break;
    //------------------------------ 
    
    //------------------------------
    case IRizquierda:
      if(programa == FOTOS){
        Keyboard.begin();
        Keyboard.press(KEY_BACKSPACE); 
        delay(100);
        Keyboard.releaseAll();
        QuitaPP();
        player = false;        
      }     
      break;
    //------------------------------
    
/*
    //------------------------------
    case Menu:
      Funcmenu();
      break;
    //------------------------------
*/   

    //------------------------------
    case Power:
        Keyboard.begin();
        Keyboard.press(KEY_ESC); 
        delay(100);
        Keyboard.releaseAll();
      break;
    //------------------------------

    default: 
      break;    
  } 

  lecturaanterior = lectura;
  
  }  //Fin del comprobar protocolo
  // Recibe el siguiente valor     
  infra.resume();   
  } 

 
 
}



