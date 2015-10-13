int pinCom[20]; 
int cycle=0; 
int buff[64]; 
int end_loop=0;
int minor=0; 
int middle=0;
int major=0;
float a_old_misT[6];
float a_misT[6]; 
float a_old_misL[6];
float a_misL[6]; 
int array_start=1; 
const float voltage=5.0; 
int sensor_voltageL=0; 
int sensor_voltageT=0;
float f_voltage=0;
#include <Servo.h>
Servo servo; 
int disp_type[20]; //se 1-->sensore di temperatura,2-->luminosità,3-->ping, se 4-->attuatore, se 5-->servoM, 6-->bottone
//int pin_array[20];                                     
int server_pin=-1; 
int grades=0; 
//int temperature[6]; 
int lum[6];
int led=0;
int led_pin=-1;
int d_att=0;
int pin_count=0; 
int stp=0;
int lum_tolerance=0;
int tmp_tolerance=0;
bool endMessage = false;
int ping=0;
int ping_pin=-1;
int distance=-1;
int duration=-1;
int dist=-1;
int rgb_pin=-1;
byte digitalValues[14];
byte actualValue=-1;

void setup() {
  Serial.begin(9600); 
}

void loop() {

  endMessage=false;
  end_loop=0;

  if(array_start==1){
    for(int i=0;i<6;i++){
      a_old_misT[i]=-1.0;
      a_old_misL[i]=-1.0;
      a_misT[i]=0.0;
      a_misL[i]=0.0;
      //temperature[i]=0;
      lum[i]=0;
    }
  }

  while( Serial.available()>0 && !endMessage){
    buff[cycle]=Serial.read(); 
    endMessage=(buff[cycle]=='#');
    cycle++;
    delay(5); 
  }

  end_loop=1; //quando vale 1 significa che è stata trasmessa una parola, perciò nel controllo degli errori si entra nell'if solo se end_loop=1
  
  if( (cycle>0)and(cycle<3)){
    minor=1;
  }else minor=0;

  if( (cycle>3)and(cycle<23) ){
    middle=1;
  }else middle=0;

  if(cycle>23){
    major=1;
  }else major=0;

  //messaggio "dimmi i pin che stai controllando"  @pinreq#
  if( (buff[0]=='@')&&(buff[1]=='p')&&(buff[2]=='i')&&(buff[3]=='n')&&(buff[4]=='r')&&(buff[5]=='e')&&(buff[6]=='q')&&(buff[7]=='#') ) {     
    for(int i=0;i<10;i++){
      if(pinCom[i]==1){
        Serial.print("@0");
        Serial.print(i);
        Serial.print("#");
        pin_count=1;
      }
    }
    for(int i=10;i<20;i++){
      if(pinCom[i]==1){
        Serial.print("@");
        Serial.print(i);
        Serial.print("#");
        pin_count=1;
      }
    }
    if(pin_count==0){
        Serial.print("@err#");
      }
    else{
      Serial.print("@ack#"); 
    }
     
    for(int i=0;i<64;i++){//ripuliamo il buffer di lettura (23)
      buff[i]=0;
    }
        
    cycle=0; //rimettiamo a zero le variabili di ciclo
    pin_count=0;
     
  }//if

  //acknowledge di port
  else if( (buff[0]=='@')&&(buff[1]=='a')&&(buff[2]=='#') ){
    Serial.print("messaggio di acknowledge da Port");
    for(int i=0;i<64;i++){ 
      buff[i]=0;
    }
        
    cycle=0; 
    
  }//if

  //messaggio "smetti di tenere d'occhio questo pin"-->@stp:(pin i-esimo su 2 byte)#
  else if( (buff[0]=='@')&&(buff[1]=='s')&&(buff[2]=='t')&&(buff[3]=='p')&&(buff[4]==':')&&(buff[7]=='#') ){
    server_pin=((buff[5]-48)*10)+(buff[6]-48); //converte in decimale
    pinCom[server_pin]=0;
    disp_type[server_pin]=0;
    a_misT[server_pin]=0.0; 
    a_misL[server_pin]=0.0;
    ping=0;
    ping_pin=0;
    dist=0;
    stp=1; //ogni volta che si riceve questo comando si setta stp a 1 (sarà poi resettato a zero)
    Serial.print("@ack#");
    server_pin=0;  
                  
    for(int i=0;i<64;i++){ 
      buff[i]=0;
    }
        
    cycle=0; 
    
  }//if

  //invio la tolleranza di luminosità @ltr:000#
  else if( (buff[0]=='@')&&(buff[1]=='l')&&(buff[2]=='t')&&(buff[3]=='r')&&(buff[4]==':')&&(buff[8]=='#') ) {                             
    server_pin=((buff[5]-48)*100)+((buff[6]-48)*10)+(buff[7]-48); 
    Serial.print("@ack#");
    lum_tolerance=server_pin;
    server_pin=0;
   
    for(int i=0;i<64;i++){ 
      buff[i]=0;
    } 
        
    cycle=0;
     
  }//if

  //tmp tolerance @ttr:00#
  else if( (buff[0]=='@')&&(buff[1]=='t')&&(buff[2]=='t')&&(buff[3]=='r')&&(buff[4]==':')&&(buff[7]=='#') ) {                             
    server_pin=((buff[5]-48)*10)+(buff[6]-48); 
    Serial.print("@ack#");
    tmp_tolerance=server_pin;
    server_pin=0;
   
    for(int i=0;i<64;i++){ 
      buff[i]=0;
    } 
        
    cycle=0;
     
  }//if

  //set up degli attuatori-può servire per avere un attuatore sempre in funzioni (es. motore "rotante")-->@att:(pin su 2 byte)#
  else if( (buff[0]=='@')&&(buff[1]=='a')&&(buff[2]=='t')&&(buff[3]=='t')&&(buff[4]==':')&&(buff[7]=='#') ){                              
    server_pin=((buff[5]-48)*10)+(buff[6]-48); 
    pinCom[server_pin]=1;
    Serial.print("@ack#");
    pinMode(server_pin, OUTPUT);
    disp_type[server_pin]=4;
    server_pin=0;
   
    for(int i=0;i<64;i++){ 
      buff[i]=0;
    } 
        
    cycle=0; 
    
  }//if

  else if( (buff[0]=='@')&&(buff[1]=='d')&&(buff[2]=='s')&&(buff[3]=='e')&&(buff[4]==':')&&(buff[7]=='#') ){                              
    server_pin=((buff[5]-48)*10)+(buff[6]-48); 
    pinCom[server_pin]=1;
    Serial.print("@ack#");
    pinMode(server_pin, INPUT);
    disp_type[server_pin]=6;
    server_pin=0;
   
    for(int i=0;i<64;i++){ 
      buff[i]=0;
    } 
        
    cycle=0; 
    
  }//if

  //può essere una specificazione di attuatore, come un servo-->@ser:(pin)#
  else if( (buff[0]=='@')&&(buff[1]=='s')&&(buff[2]=='e')&&(buff[3]=='r')&&(buff[4]==':')&&(buff[7]=='#') ){ //se si usa un servo ci vuole un mess in più per le specificità                         
    server_pin=((buff[5]-48)*10)+(buff[6]-48); 
    pinCom[server_pin]=1;
    servo.attach(server_pin);
    pinMode(server_pin, OUTPUT);
    Serial.print("@ack#");
    disp_type[server_pin]=5;
    server_pin=0;
   
    for(int i=0;i<64;i++){ 
      buff[i]=0;
    } 
        
    cycle=0;
    
  }//if

  //questo pin è un led  @led:num#
  else if( (buff[0]=='@')&&(buff[1]=='l')&&(buff[2]=='e')&&(buff[3]=='d')&&(buff[4]==':')&&(buff[7]=='#') ){                     
    led_pin=((buff[5]-48)*10)+(buff[6]-48); //converte in decimale
    pinCom[led_pin]=1;
    Serial.print("@ack#");
    //Serial.println(led_pin);
    led=1;  //serve per sapere se c'è attivo un led sulla scheda, e quale è il pin (led_pin)
    pinMode(led_pin, OUTPUT);
    
    server_pin=0;
   
    for(int i=0;i<64;i++){ 
      buff[i]=0;
    } 
        
    cycle=0;
    
  }//if

  //accendi l'attuatore digitale su questo pin  @sda:05:1/0#  1 se HIGH 0 se LOW
  else if( (buff[0]=='@')&&(buff[1]=='s')&&(buff[2]=='d')&&(buff[3]=='a')&&(buff[4]==':')&&(buff[7]==':')&&(buff[9]=='#') ){                        
    d_att=((buff[5]-48)*10)+(buff[6]-48); //converte in decimale
    //Serial.println(led_pin);
    Serial.print("@ack#");
    if((buff[8]-48)==1){
      pinMode(d_att, OUTPUT);
      delay(50);
      digitalWrite(d_att, HIGH);
    }
    else if((buff[8]-48)==0){
      pinMode(d_att, OUTPUT);
      delay(50);
      digitalWrite(d_att, LOW);
    }
    
    server_pin=0;
    d_att=0;
   
    for(int i=0;i<64;i++){ 
      buff[i]=0;
    } 
        
    cycle=0;
    
  }//if

  //@rgb:11:255# 
  else if( (buff[0]=='@')&&(buff[1]=='r')&&(buff[2]=='g')&&(buff[3]=='b')&&(buff[4]==':')&&(buff[7]==':')&&(buff[11]=='#') ){                        
    rgb_pin=((buff[5]-48)*10)+(buff[6]-48); //converte in decimale
    server_pin=((buff[8]-48)*100)+((buff[9]-48)*10)+(buff[10]-48);
    pinMode(rgb_pin,OUTPUT);
    analogWrite(rgb_pin, server_pin);   
    server_pin=0;
    rgb_pin=-1;
       
    for(int i=0;i<64;i++){ 
      buff[i]=0;
    } 
        
    cycle=0;
    
  }//if

  //accendi l'attuatore analogico su questo pin  @saa:05:999#  
  else if( (buff[0]=='@')&&(buff[1]=='s')&&(buff[2]=='a')&&(buff[3]=='a')&&(buff[4]==':')&&(buff[7]==':')&&(buff[11]=='#') ){                         
    server_pin=((buff[5]-48)*10)+(buff[6]-48); //converte in decimale
    Serial.print("@ack#");
    servo.attach(server_pin);
    pinMode(server_pin, OUTPUT);
    grades=((buff[8]-48)*100)+((buff[9]-48)*10)+(buff[10]-48); //converte in decimale
    servo.write(grades); 
    delay(1000);
    grades=0;
    server_pin=0;
    
    server_pin=0;
   
    for(int i=0;i<64;i++){ 
      buff[i]=0;
    } 
        
    cycle=0;
    
  }//if

  //se riceve @tmp:(pin i-esimo)# significa che quel pin è di un sensore di temperatura
  else if( (buff[0]=='@')&&(buff[1]=='t')&&(buff[2]=='m')&&(buff[3]=='p')&&(buff[4]==':')&&(buff[7]=='#') ){ 
              
      server_pin=((buff[5]-48)*10)+(buff[6]-48); //converte in decimale
      //Serial.println(server_pin);
      Serial.print("@ack#");
      pinCom[server_pin]=1;
      //sensor[server_pin]=1;
      //temperature[server_pin]=1;
      disp_type[server_pin]=1; //sensore
      server_pin=0;
   
    for(int i=0;i<64;i++){ 
      buff[i]=0;
    }
        
    cycle=0; 
    
  }//if

  //sensore di luminosità (termoresistenza)  @lum:pin#
  else if( (buff[0]=='@')&&(buff[1]=='l')&&(buff[2]=='u')&&(buff[3]=='m')&&(buff[4]==':')&&(buff[7]=='#') ){ 
    //Serial.println("Arduino, il pin appartiene a un sensore di luminosita'");
              
      server_pin=((buff[5]-48)*10)+(buff[6]-48); //converte in decimale
      //Serial.println(server_pin);
      Serial.print("@ack#");
      pinCom[server_pin]=1;
      //sensor[server_pin]=1;
      pinMode(server_pin, INPUT);
      //lum[server_pin]=1;
      disp_type[server_pin]=2; //sensore
      server_pin=0;
   
    for(int i=0;i<64;i++){ 
      buff[i]=0;
    }
        
    cycle=0; 
    
  }//if

  //@png:()#
  else if( (buff[0]=='@')&&(buff[1]=='p')&&(buff[2]=='n')&&(buff[3]=='g')&&(buff[4]==':')&&(buff[7]=='#') ){ 
    //Serial.println("Arduino, il pin appartiene a un sensore di luminosita'");
              
      server_pin=((buff[5]-48)*10)+(buff[6]-48); //converte in decimale
      //Serial.println(server_pin);
      Serial.print("@ack#");
      pinCom[server_pin]=1;
      //sensor[server_pin]=1;
      disp_type[server_pin]=3;
      ping_pin=server_pin;
      ping=1;
      server_pin=0;
   
    for(int i=0;i<64;i++){ 
      buff[i]=0;
    }
        
    cycle=0; 
    
  }//if

  //@dis:(3 byte)#
  else if( (buff[0]=='@')&&(buff[1]=='d')&&(buff[2]=='i')&&(buff[3]=='s')&&(buff[4]==':')&&(buff[8]=='#') ){ 
              
      server_pin=((buff[5]-48)*100)+((buff[6]-48)*10)+(buff[7]-48); //converte in decimale
      //Serial.println(server_pin);
      Serial.print("@ack#");
      dist=server_pin;
      server_pin=0;
   
    for(int i=0;i<64;i++){ 
      buff[i]=0;
    }
        
    cycle=0; 
    
  }//if
  
  //sennò si è avuto errore
  else if( ( (end_loop==1)and(cycle==3) )or( (end_loop==1)and(cycle==23) )or(minor==1)or(middle==1)or(major==1) ){ 
    Serial.print("@err#");
    for(int i=0;i<64;i++){ 
      buff[i]=0;
    }
            
    cycle=0;
  }//if    fine fase di parsing dei messaggi seriali

  //ping
  if(ping==1){  //se arduino non gestisce da solo la conversione in centimetri è difficile specificargli una distanza di rilevazione consistente
    duration= measure_distance();
    distance=duration/29/2;  
    //Serial.println(distance);
    if(distance<dist){   //da decidere se se ne occupa il server
      if(ping_pin<10){
        Serial.print("@get:0");
      }
      else {
        Serial.print("@get:");
      }
      Serial.print(ping_pin);
      Serial.print(":0");  //aggiungere a github
      //Serial.print(duration);
      Serial.print(distance);
      Serial.print("#");
      delay(500);    
    }    
  }//if

  //lettura dei pin in esame
  for(int i=0;i<20;i++){
    if( (pinCom[i]==1)and((disp_type[i]==1)or(disp_type[i]==2))or(disp_type[i]==6) ){ //temperatura o luminosità o bottone
      /*leggi il valore sul pin corrispondente con tipo analogread() ecc, e metti il valore letto in mis[i]
      bisogna fare due routine separate per sensori digitali e analogici, perché in quelli analogici sono necessari calcoli per rendere la misurazione dipendente dal'alimentazione a 5V*/
      if( (i>=0)&&(i<6) ){ //pin analogici A0____A5 (6 pin) 
        //si può lasciare fare al server la gestione dei vari sensori, arduino si può limitare a misurare      
        if(disp_type[i]==1){
          if(analogRead(i)<=999){
            sensor_voltageT=analogRead(i);
          }
          else sensor_voltageT=999;
          /*a_misT[i]=sensor_voltageT*voltage/1024; //gestione particolare, caso specifico
          a_misT[i]=(a_misT[i]*1000-500)/10;*/
          a_misT[i]=sensor_voltageT;
        }

        if(disp_type[i]==2){
          if(analogRead(i)<=999){
            sensor_voltageL=analogRead(i);
          }
          else sensor_voltageL=999;
          a_misL[i]=sensor_voltageL;
        }
        delay(500);  //500
        
      }//if
      else if(disp_type[i]==6){
        actualValue=digitalRead(i);
        if((actualValue!=digitalValues[i])){
          digitalValues[i]=actualValue;
          Serial.print("@get:");
          if(i<10){
            Serial.print("0");
            Serial.print(i);
          }else Serial.print(i);
          Serial.print(":");
          Serial.print(actualValue);
          Serial.print("#");
        }
      }
      //qui if digitale (todo)
    }//fine if sensori

    //if attuatori digitali 
  }//for
  
  //controllo array valori vecchi/nuovi
  for(int i=0;i<6;i++){ //attenzione, con questa tolleranza, se la temperatura sale di un grado per volta, anche se passasse da tipo 20 a 25 gradi arduino non notificherebbe volta volta!
    if( (a_old_misT[i]!=-1)and( (a_old_misT[i]<a_misT[i]-tmp_tolerance)or(a_old_misT[i]>a_misT[i]+tmp_tolerance)) ){ //problematico il confronto se sono due float
      
      if(stp==0){  //se c'è un sensore attivo (altrimenti darebbe nuova misurazione una volta spento). anche se si è spento un sensore di luminosità lui non darà nuova misurazione,
                   //perché stp è una sola. trascuriamo questa cosa, tanto per 1 turno non succede niente a perdere una misurazione. Ogni volta che si spenge un sensore salta una misura a tutti 
        //Serial.print("il sensore ha rilevato un cambiamento sul pin: ");//da sostituire con l'invio di mis[i] a port 
        Serial.print("@get:0");
        Serial.print(i);
        Serial.print(':');
        if( (int)a_misT[i] < 10){
          Serial.print("00" + (int)a_misT[i]);
        }
        else if( (int)a_misT[i]<100){
          Serial.print("0" + (int)a_misT[i]);
        }
        else{
          Serial.print((int)a_misT[i]);
        }      
        Serial.print('#');
      }
    }//if

    if( (a_old_misL[i]!=-1)and( (a_old_misL[i]<a_misL[i]-lum_tolerance)or(a_old_misL[i]>a_misL[i]+lum_tolerance)) ){ //tolleranza supposta di 50
      
      if(stp==0){   
        Serial.print("@get:0");
        Serial.print(i);
        Serial.print(':');
        if( (int)a_misL[i] < 10){
          Serial.print("00" + (int)a_misL[i]);
        }
        else if( (int)a_misL[i]<100){
          Serial.print("0" + (int)a_misL[i]);
        }
        else{
          Serial.print((int)a_misL[i]);
        }
        Serial.print('#');
      }
    }//if
  }//for
  
  stp=0; //dopo aver perso una misurazione in caso di comando @stp, si resetta a zero 

  for(int i=0;i<6;i++){
    /*a_old_misT[i]=a_misT[i]; //come detto sopra, se la temperatura sale o scende "dolcemente", anche se alla lunga varia di molto il server non viene notificato! occhio
    a_old_misL[i]=a_misL[i];*/
    if( (a_old_misT[i]<a_misT[i]-tmp_tolerance)or(a_old_misT[i]>a_misT[i]+tmp_tolerance) ){
      a_old_misT[i]=a_misT[i];
    }

    if( (a_old_misL[i]<a_misL[i]-lum_tolerance)or(a_old_misL[i]>a_misL[i]+lum_tolerance) ){
      a_old_misL[i]=a_misL[i];
    }
  } //for
  
  array_start=0; //non re-inizializzare old_mis
}//loop


int measure_distance(){
  pinMode(ping_pin, OUTPUT);
  digitalWrite(ping_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(ping_pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(ping_pin, LOW);

  pinMode(ping_pin,INPUT);
  return pulseIn(ping_pin,HIGH);
}


    
