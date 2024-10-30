#include <can2040.h>
#include <hardware/regs/intctrl.h>
#include <stdio.h>
#include <pico/stdlib.h>

static struct can2040 cbus;

static void can2040_cb(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg)
{
    printf("callback\n");
    // Put your code here....
}

static void PIOx_IRQHandler(void)
{
    can2040_pio_irq_handler(&cbus);
}

void canbus_setup(void)
{
    uint32_t pio_num = 0;
    uint32_t sys_clock = 125000000, bitrate = 500000;
    uint32_t gpio_rx = 4, gpio_tx = 5;

    // Setup canbus
    can2040_setup(&cbus, pio_num);
    can2040_callback_config(&cbus, can2040_cb);

    // Enable irqs
    irq_set_exclusive_handler(PIO0_IRQ_0, PIOx_IRQHandler);
    irq_set_priority(PIO0_IRQ_0, PICO_DEFAULT_IRQ_PRIORITY - 1);
    irq_set_enabled(PIO0_IRQ_0, 1);

    // Start canbus
    can2040_start(&cbus, sys_clock, bitrate, gpio_rx, gpio_tx);
}

void main(void){

    stdio_init_all();

     canbus_setup();

struct can2040_msg msg;
    msg.id = 0x1; // Set your desired CAN ID
    msg.dlc = 1;    // Length of the message
    msg.data[0] = 0x01; // Character to transmit
    int i;
    sleep_ms(10000);
    while (1) {
        i = can2040_check_transmit(&cbus);

        printf("check %d\n",i);
        i = can2040_transmit(&cbus,&msg);
  
        printf("transmit%d\n",i);
        sleep_ms(1000); 
    }
}