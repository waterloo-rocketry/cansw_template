#include <xc.h>
#include "canlib.h"

#include "device_config.h"
#include "board.h"

static void can_msg_handler(const can_msg_t *msg);

//memory pool for the CAN tx buffer
uint8_t tx_pool[100];

int main(void) {
    // intiialize the external oscillator
    OSCILLATOR_Initialize();

    // init our millis() function
    timer0_init();

    // set up pins
    gpio_init();

    // Enable global interrupts
    INTCON0bits.GIE = 1;

    // Set up CAN TX
    TRISC1 = 0; // set as output
    RC1PPS = 0x33; // make C1 transmit CAN TX (page 267)

    // Set up CAN RX
    TRISC0 = 1; // set as input
    ANSELC0 = 0; // not analog
    CANRXPPS = 0x10; // make CAN read from C0 (page 264-265)

    // set up CAN module
    can_timing_t can_setup;
    can_generate_timing_params(_XTAL_FREQ, &can_setup);
    can_init(&can_setup, can_msg_handler);
    // set up CAN tx buffer
    txb_init(tx_pool, sizeof(tx_pool), can_send, can_send_rdy);

    // loop timer
    uint32_t last_millis = millis();
    
    bool heartbeat = false;
    while (1) {
        if (millis() - last_millis > MAX_LOOP_TIME_DIFF_ms) {
            // update our loop counter
            last_millis = millis();

            // visual heartbeat indicator
            BLUE_LED_SET(heartbeat);
            heartbeat = !heartbeat;
            
            // We're alive, let's tell the world!
            can_msg_t board_stat_msg;
            build_board_stat_msg(millis(), E_NOMINAL, NULL, 0, &board_stat_msg);
            txb_enqueue(&board_stat_msg);
        }
        //send any queued CAN messages
        txb_heartbeat();
    }
}

static void can_msg_handler(const can_msg_t *msg) {
    uint16_t msg_type = get_message_type(msg);

    // ignore messages that were sent from this board
    if (get_board_unique_id(msg) == BOARD_UNIQUE_ID) {
        return;
    }

    switch (msg_type) {
        case MSG_LEDS_ON:
            RED_LED_SET(1);
            BLUE_LED_SET(1);
            WHITE_LED_SET(1);
            break;

        case MSG_LEDS_OFF:
            RED_LED_SET(0);
            BLUE_LED_SET(0);
            WHITE_LED_SET(0);
            break;

        // all the other ones - do nothing
        default:
            break;
    }
}

static void __interrupt() interrupt_handler(void) {
    if (PIR5) {
        can_handle_interrupt();
    }

    // Timer0 has overflowed - update millis() function
    // This happens approximately every 500us
    if (PIE3bits.TMR0IE == 1 && PIR3bits.TMR0IF == 1) {
        timer0_handle_interrupt();
        PIR3bits.TMR0IF = 0;
    }
}
