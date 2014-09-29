

void intro(void){
  dvdLED.setCursor(0);
  dvdLED.print("    H");
  delay(200);
  dvdLED.setCursor(0);
  dvdLED.print("   HO");
  delay(200);
  dvdLED.setCursor(0);   
  dvdLED.print("  HOL");
  delay(200);
  dvdLED.setCursor(0);   
  dvdLED.print(" HOLA");
  delay(200); 
  dvdLED.setCursor(0);   
  dvdLED.print("HOLA ");
  delay(800);
  dvdLED.setCursor(0);
  
}



void cargaCD(void){
  for(int i=0;i<14;i++){
    dvdLED.setLEDon(i);
    delay(20);    
  }
}


void vueltaCD(void){

  for(int i=0;i<14;i++){
    dvdLED.setLEDoff(i);
  }
  
  for(int i=0;i<14;i++){
    dvdLED.setLEDon(i);
    
    if(i!=0){
    dvdLED.setLEDoff(i-1);      
    }
 
    delay(20);
 
    if(i>=7){
    dvdLED.setLEDoff(i);      
    }    
  }   
}

void DVDsistema(void){
  dvdLED.setCursor(0);
  
  if(sistema == 0){
    dvdLED.print("XP     ");
  }
  else{    
    dvdLED.print("LINUX  ");    
  }
}

void DVDprograma(void){
  QuitaPuntos ();  
  dvdLED.setCursor(0);  
  switch (programa) {
    case WMP:
      dvdLED.print("WMP    ");
      break;
    case VLC:
      dvdLED.print("ULC    ");
      break;
    case FOTOS:
      dvdLED.print("FOTOS  ");
      break;  
    case TIEMPO:
      dvdLED.print("TIEMPO ");
      break;
    case TEMP:
      dvdLED.print("TEMP   ");
      break;      
    case ONLINE:
      dvdLED.print("ONLINE ");
      break;    
  }
  
  
        
}

void DVDvol_menos(void){
  QuitaPuntos ();  
  dvdLED.setCursor(0);  
  dvdLED.print("UOL--");    
}

void DVDvol_mas(void){
  QuitaPuntos ();  
  dvdLED.setCursor(0);
  dvdLED.print("UOL+-");    
}  

void vueltaCDf(void){
/*
  for(int i=0;i<14;i++){
    dvdLED.setLEDoff(i);
  }
  
  for(int i=0;i<14;i++){
    dvdLED.setLEDon(i);
    
    if(i!=0){
    dvdLED.setLEDoff(i-1);      
    }
 
//    delay(20);
     
  }
    dvdLED.setLEDoff(13);   
*/

  for(int i=0;i<14;i=i+2){
    dvdLED.setLEDon(i);
    dvdLED.setLEDoff(i-2);    
  }
    dvdLED.setLEDoff(12);  
  
}


void Funcmenu(void){
  infra.resume();  
VFuncmenu:



  // Recibe el siguiente valor 
  infra.resume();
  goto VFuncmenu;
}

void QuitaPP (void){
  dvdLED.setLEDoff(LED_PAU);
  dvdLED.setLEDoff(LED_PLY);
}

void QuitaPuntos (void){
  dvdLED.setLEDoff(LED_CL1);
  dvdLED.setLEDoff(LED_CL2);
}

void Tiempo (void){

/*
Un dia tiene 1000 * 60 * 60 * 24 = 86400000 milisegundos.
Una hora tiene 1000 * 60 * 60 = 3600000 milisegundos.
Un minuto tiene 1000 * 60 = 60000 milisegundos.
Un segundo tiene 1000 milisegundos.
*/  

int dias = 0;
int horas = 0;
int minutos = 0;
int segundos = 0;
unsigned long tiempo;
unsigned long auxiliar;

  tiempo = millis ();
    
  auxiliar = tiempo / 86400000;
  dias = (int) auxiliar;
  tiempo = tiempo - (dias * 86400000);
  //ya están calculados los dias y restados al total.
  auxiliar = tiempo / 3600000;
  horas = (int) auxiliar;
  tiempo = tiempo - (horas * 3600000);
  //ya están calculadas las horas y restadas al total.
  auxiliar = tiempo / 60000;
  minutos = (int) auxiliar;
  tiempo = tiempo - (minutos * 60000);
  //ya están calculados los minutos y restados al total.  
  auxiliar = tiempo / 1000;
  segundos = (int) auxiliar;
  tiempo = tiempo - (segundos * 1000);
  //ya están calculados los segundos y restados al total. Solo quedan milisegundos.
  
  dvdLED.setCursor(0);  
 
  if(dias>0){  
    if(dias<10){
    dvdLED.print(dias);
    }
  }
  else {
    dvdLED.print(" ");    
  }
  
  
  if(horas < 10){
    dvdLED.print(" ");  
  }            
  
  dvdLED.print(horas);

  if(minutos < 10){
    dvdLED.print("0");    
  } 

  dvdLED.print(minutos);
  
  dvdLED.setLEDon(LED_CL2); 
   
}

void Temperatura(void){  
  float temperatura;  // variable para almacenar el valor de temperatura en ºC
 
  dvdLED.setCursor(0);  

  temperatura = LM35(); 
  
  dvdLED.print(" ");   
  dvdLED.print(temperatura,0);  
  dvdLED.print("^C"); 
  dvdLED.print(" ");
  dvdLED.print(" ");  
}


float LM35 (void){
  int  cad;
  float temperatura;
  
//  analogReference(INTERNAL); //Valor para Leonardo de 2,56V (2.56/1023 = 0,0025024437927664)
  cad  = analogRead (A0);  
//  analogReference(DEFAULT);  //Dejamos la referencia como estaba por defecto
  
  temperatura = (float) cad;
  temperatura = temperatura * 0.25024437927664;
  temperatura = temperatura - 3.0;//*********************************para ajustar la medida 
  return temperatura;  
}

