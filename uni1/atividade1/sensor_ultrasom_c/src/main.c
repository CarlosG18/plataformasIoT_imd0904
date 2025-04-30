#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
//#include "esp_spi_flash.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "rom/ets_sys.h"
#include "esp_timer.h"
#include "spi_flash_mmap.h"

// CODIGO EM C PARA O SENSOR ULTRASONICO
#define MAX_DISTANCE 300

unsigned long tempo1, tempo2, totalTime, maxTime = MAX_DISTANCE*1000000*2/34300;

// configuração da comunicação serial UART para log
uart_config_t config = {
  .baud_rate = 115200,
  .data_bits = UART_DATA_8_BITS,
  .parity = UART_PARITY_DISABLE,
  .stop_bits = UART_STOP_BITS_1,
  .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
};

long ping_cm(){
  gpio_set_level(GPIO_NUM_27, 1); // SET TRIGGER
  ets_delay_us(10); // pulso do trigger para iniciar o sensor
  gpio_set_level(GPIO_NUM_27, 0); // RETIRADA DO PULSO DO TRIGGER

  // lendo o valor do echo
  long valor = gpio_get_level(GPIO_NUM_26);
  while(!valor){
    tempo1 = esp_timer_get_time(); // tempo inicial
    valor = gpio_get_level(GPIO_NUM_26); // le o valor do echo
    do{
        tempo2 = esp_timer_get_time(); // tempo final
      }while(valor && tempo2-tempo1 <= maxTime);
      totalTime = tempo2-tempo1; // calcula o tempo total
      if(totalTime <= maxTime){
        long dist = 34300 * totalTime / 2000000; // calcula a distancia
        return dist;
      }
      else{
        return 0;
      }
  }
   return -1;
}

void app_main(void){ //setup do codigo
  // definindo os pinos de entrada e saida
  gpio_set_direction(GPIO_NUM_27, GPIO_MODE_OUTPUT); // TRIGGER
  gpio_set_direction(GPIO_NUM_26, GPIO_MODE_INPUT); // ECHO

  // definindo as configurações base da comunicação UART
  uart_param_config(UART_NUM_0, &config);
  uart_driver_install(UART_NUM_0, 1024, 0, 0, NULL, 0);

  while(1){ //loop do codigo
    vTaskDelay(50 / portTICK_PERIOD_MS); // delay de 50ms
    long dist = ping_cm();
    char msg[64];
    snprintf(msg, sizeof(msg), "Distância: %ld cm\n", dist); // formata a string
    uart_write_bytes(UART_NUM_0, msg, strlen(msg)); // envia a string pela UART
  }
};
