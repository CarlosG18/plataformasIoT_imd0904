#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/uart.h"
#include "driver/gpio.h"

// CODIGO EM C PARA O SENSOR ULTRASONICO
#define TRIGGER_PIN GPIO_NUM_27
#define ECHO_PIN GPIO_NUM_34
#define MAX_DISTANCE 300

unsigned long tim, tim2, totalTime, maxTime = MAX_DISTANCE*1000000*2/34300;

static inline uint32_t get_ccount() {
    uint32_t ccount;
    __asm__ __volatile__("rsr %0, ccount" : "=a" (ccount));  // Acessa o registrador CCOUNT
    return ccount;
}

// Função para converter ciclos de clock em microsegundos
uint64_t micros() {
    static uint32_t last_ccount = 0;
    uint32_t current_ccount = get_ccount();
    
    // O número de ciclos desde o último valor de CCOUNT
    uint32_t delta = current_ccount - last_ccount;
    
    // Frequência da CPU (240 MHz padrão para ESP32)
    uint32_t cpu_freq_mhz = CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ;  // Frequência da CPU em MHz (geralmente 240 MHz)
    
    // Converte ciclos para microsegundos
    uint64_t microseconds = (uint64_t)delta * 1000 / cpu_freq_mhz;

    last_ccount = current_ccount;  // Atualiza o valor para o próximo cálculo
    return microseconds;
}

// configuração da comunicação serial UART para log
uart_config_t config = {
 .baud_rate = 115200,
 .data_bits = UART_DATA_8_BITS,
 .parity = UART_PARITY_DISABLE,
 .stop_bits = UART_STOP_BITS_1,
 .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
};

long ping_cm(){
  gpio_set_level(GPIO_NUM_27, 1); // pulso do trigger para iniciar o sensor
  tim = micros();
  while(micros()-tim<10);
  gpio_set_level(GPIO_NUM_27, 0); // retirada do pulso do trigger

  // lendo o valor do echo
  int valor = adc1_get_raw(ADC1_CHANNEL_6);
  
  while(!valor){
    tim = micros();
    do{
        tim2 = micros();
      }while(adc1_get_raw(ADC1_CHANNEL_6) && tim2-tim <= maxTime);
      totalTime = tim2-tim;
      if(totalTime <= maxTime){
        long dist = 34300 * totalTime / 2000000;
        return dist;
      }
      else{
        return 0;
      }
  }
   
}

void app_main(void){ //setup do codigo
  // definindo os pinos de entrada e saida
  gpio_set_direction(GPIO_NUM_27, GPIO_MODE_OUTPUT); // TRIGGER
  gpio_set_direction(GPIO_NUM_34, GPIO_MODE_INPUT); // ECHO

  // definindo as configurações base da comunicação UART
  uart_param_config(UART_NUM_1, &config);
  uart_driver_install(UART_NUM_1, 1024, 0, 0, NULL, 0);
  
  //definindo as configurações para leitura ADC
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11);

  while(1){ //loop do codigo
    vTaskDelay(50 / portTICK_PERIOD_MS); // delay de 50ms
    uart_write_bytes(UART_NUM_1, "Ping: ", strlen("Ping: "));
    uart_write_bytes(UART_NUM_1, ping_cm(), strlen(ping_cm()));
    uart_write_bytes(UART_NUM_1, "cm", strlen("cm"));
  }
};