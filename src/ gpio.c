#include "gpio.h"
#include "utils.h"

/*
    Asigna a pin GPIO (pinNumber) cierta funcionalidad (func).
      La funcionalidad de cada GPIO pin se implementa en los registros
      de selección de función (GPFSELn). 
*/
void gpio_pin_set_func(u8 pinNumber, GpioFunc func){
    u8 bitStart = (pinNumber*3) % 30; // Se calcula el bit de inicio 
    u8 reg = pinNumber / 10; // Se calcula el registro en el que se encuentra

    u32 selector = REGS_GPIO->func_select[reg]; // Se lee el estado actual del reg
    selector &= ~(7 << bitStart); // Se apagan los tres bits correspondientes
    selector |= (func << bitStart); // Se encienden solo los bits seleccionados 

    REGS_GPIO->func_select[reg] = selector; // Se vuelve a asignar a la memoria
}

/*
    Funcion para habilitar un pin GPIO (pinNumber).
      Se siguen las instrucciones para los registros de reloj de pull-up/down
      GPIO (GPPUDCLKn).
*/
void gpio_pin_enable(u8 pinNumber){
    REGS_GPIO->pupd_enable = 0;
    delay(150);
    REGS_GPIO->pupd_enable_clocks[pinNumber/32] = 1 << (pinNumber % 32);
    delay(150);
    REGS_GPIO->pupd_enable = 0;
    REGS_GPIO->pupd_enable_clocks[pinNumber/32] = 0;
}