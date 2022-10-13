#include <VirtualWire.h>

int valor_recebido_RF;
char recebido_RF_char[4]; 

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);

  vw_set_rx_pin(7);
  vw_setup(5000); 
  //Inicia a recepcao  
  vw_rx_start();
  Serial.println("Recepcao modulo RF - Aguardando...");
}

// the loop function runs over and over again forever
void loop() {


  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
    
    if (vw_get_message(buf, &buflen)) 
    {
    int i;
        for (i = 0; i < buflen; i++)
       {            
          //Armazena os caracteres recebidos  
          recebido_RF_char[i] = char(buf[i]);
       }
       recebido_RF_char[buflen] = '\0';
       
       //Converte o valor recebido para integer
       valor_recebido_RF = atoi(recebido_RF_char);
         
       //Mostra no serial monitor o valor recebido
       Serial.print("Recebido: ");
       Serial.print(valor_recebido_RF);
    }
}






