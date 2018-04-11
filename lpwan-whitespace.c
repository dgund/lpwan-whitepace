/*
    lpwan-whitespace.c

    MIT License

    Copyright (c) 2018 Devin Gund (https://dgund.com).

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
    deal in the Software without restriction, including without limitation the
    rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
    sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
    IN THE SOFTWARE.
*/

/* XDCtools header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

/* BIOS header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Event.h>
#include <ti/sysbios/knl/Task.h>

/* TI-RTOS header files */
#include <ti/drivers/PIN.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/UART.h>

/* LoRaBug Firmware header files */
#include "Board.h"

/* LoRaMac-node header files */
#include "LoRaMac-node/src/boards/LoRaBug/board.h"

/* Communication mode */
#define MODE_TRANSMIT
/* #define MODE_RECEIVE */

/*
    Frequencies (Hz)
    White space frequencies obtained from a white space spectrum database for
    the Carnegie Mellon University campus (Pittsburgh, PA, USA). These
    frequencies may not necessarily be cleared for LPWAN network use. Before
    transmitting on any frequency, consult a white space database for your area
    to determine available channels and any pertinent Federal Communications
    Commission regulations for transmission.
*/
#define FREQUENCY_WHITESPACE_2   57000000
#define FREQUENCY_WHITESPACE_5   79000000
#define FREQUENCY_WHITESPACE_6   85000000
#define FREQUENCY_LORA_LF       434000000
#define FREQUENCY_WHITESPACE_16 485000000
#define FREQUENCY_WHITESPACE_21 515000000
#define FREQUENCY_WHITESPACE_27 551000000
#define FREQUENCY_WHITESPACE_28 557000000
#define FREQUENCY_WHITESPACE_32 581000000
#define FREQUENCY_WHITESPACE_35 599000000
#define FREQUENCY_WHITESPACE_44 653000000
#define FREQUENCY_WHITESPACE_45 659000000
#define FREQUENCY_WHITESPACE_46 665000000
#define FREQUENCY_WHITESPACE_47 671000000
#define FREQUENCY_LORA_HF       915000000

/* Transmit/receive frequency (Hz) */
#define FREQUENCY FREQUENCY_LORA_HF

/* Uncomment for debug printing */
/* #define DEBUG_PRINTING */

/* Transmit power (dBm) */
#define TX_POWER 14

/* Timeout time (0 for no timeout) */
#define RX_TIMEOUT 0

/* LoRa bandwidth [0: 125kHz, 1: 250kHz, 2: 500kHz, 3: reserved] */
#define LORA_BANDWIDTH 0

/* LoRa datarate [6: 64, 7: 128, 8: 256, 9: 512, 10: 1024, 11: 2048, 12: 4096] */
#define LORA_DATARATE 7

/* LoRa coding rate [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8] */
#define LORA_CODINGRATE 1

/* LoRa preamble length (bytes) */
#define LORA_PREAMBLE_LENGTH 8

/* LoRa symbol timeout (bytes) */
#define LORA_SYMBOL_TIMEOUT 5

/* LoRa fixed length payload */
#define LORA_FIXED_LENGTH_PAYLOAD false

/* LoRa IQ inversion */
#define LORA_IQ_INVERSION false

/* Payload size (bytes) */
#define PAYLOAD_SIZE 64

/* Reference time of one millisecond */
#define TIME_MS (1000/Clock_tickPeriod)

/* Payload buffer */
uint16_t payloadBufferSize = PAYLOAD_SIZE;
uint8_t payloadBuffer[PAYLOAD_SIZE];

/* Buffer for snprintf() */
#define SBUF_SIZE 128
static char sbuf[SBUF_SIZE];

/* Radio events */
#define EVENT_TRANSMIT                      Event_Id_00
#define EVENT_RECEIVE                       Event_Id_01
#define EVENT_TRANSMIT_TIMEOUT              Event_Id_02
#define EVENT_RECEIVE_TIMEOUT               Event_Id_03
#define EVENT_RECEIVE_ERROR                 Event_Id_04
#define EVENT_CHANNEL_ACTIVITY_DETECT_TRUE  Event_Id_05
#define EVENT_CHANNEL_ACTIVITY_DETECT_FALSE Event_Id_06

/* Radio events data structures */
static RadioEvents_t RadioEvents;
static Event_Struct radioEventsStruct;
static Event_Handle radioEvents;

/* Called after radio transmits packet */
void onTransmit(void);

/* Called after radio receives packet */
void onReceive(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr);

/* Called after radio timeout in transmitting packet */
void onTransmitTimeout(void);

/* Called after radio timeout in receiving packet */
void onReceiveTimeout(void);

/* Called after radio error in receiving packet */
void onReceiveError(void);

/* Called after channel activity detection */
void onChannelActivityDetection(bool channelActivityDetected);

void onTransmit(void) {
    Radio.Sleep();
    Event_post(radioEvents, EVENT_TRANSMIT);
}

void onReceive(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr) {
    /* Save payload */
    payloadBufferSize = size;
    memcpy(payloadBuffer, payload, payloadBufferSize);
#if defined(DEBUG_PRINTING)
    /* Print data */
    printf("# Received packet. Size=%u, RSSI=%d, SNR=%d.\n", size, rssi, snr);
    snprintf(sbuf, sizeof(sbuf), "# Received packet. Size=%u, RSSI=%d, SNR=%d.\r\n", size, rssi, snr);
    uartwrite(sbuf, strlen(sbuf));

    /* Print payload */
    uarthexdump(payload, size);
    uartputs("\r");
#else
    /* Print RSSI and SNR, comma-separated */
    snprintf(sbuf, sizeof(sbuf), "%d,%d,\r\n", rssi, snr);
    uartwrite(sbuf, strlen(sbuf));
#endif
}

void onTransmitTimeout(void) {
#if defined(DEBUG_PRINTING)
    printf("# Transmit timeout.\n");
    uartputs("# Transmit timeout.");
    uartputs("\r");
#endif

    Radio.Sleep();
    Event_post(radioEvents, EVENT_TRANSMIT_TIMEOUT);
}

void onReceiveTimeout(void) {
#if defined(DEBUG_PRINTING)
    printf("# Receive timeout.\n");
    uartputs("# Receive timeout.");
    uartputs("\r");
#endif

    Radio.Sleep();
    Event_post(radioEvents, EVENT_RECEIVE_TIMEOUT);
}

void onReceiveError(void) {
#if defined(DEBUG_PRINTING)
    printf("# Receive error.\n");
    uartputs("# Receive error.");
    uartputs("\r");
#endif

    Event_post(radioEvents, EVENT_RECEIVE_ERROR);
}

void onChannelActivityDetection(bool channelActivityDetected) {
#if defined(DEBUG_PRINTING)
    printf("# Channel activity %s.\n", channelActivityDetected ? "detected" : "not detected");
    snprintf(sbuf, sizeof(sbuf), "# Channel activity %s.\n", channelActivityDetected ? "detected" : "not detected");
    uartwrite(sbuf, strlen(sbuf));
#endif

    Radio.Sleep();
    Event_post(radioEvents, channelActivityDetected ? EVENT_CHANNEL_ACTIVITY_DETECT_TRUE : EVENT_CHANNEL_ACTIVITY_DETECT_FALSE);
}

#define TASKSTACKSIZE   2048

Task_Struct task0Struct;
Char task0Stack[TASKSTACKSIZE];

/*
    The main task of the experiment:
    Either continuously transmit or receive packets.
    If receiving packets, record payload, size, RSSI, and SNR.
*/
void maintask(UArg arg0, UArg arg1) {
    unsigned int packetCount = 0;

    /* Initialize target board */
    BoardInitMcu();
    BoardInitPeriph();

    /* Initialize radio events struct */
    Event_construct(&radioEventsStruct, NULL);
    radioEvents = Event_handle(&radioEventsStruct);

    /* Initialize radio events */
    RadioEvents.TxDone    = onTransmit;
    RadioEvents.RxDone    = onReceive;
    RadioEvents.TxTimeout = onTransmitTimeout;
    RadioEvents.RxTimeout = onReceiveTimeout;
    RadioEvents.RxError   = onReceiveError;
    RadioEvents.CadDone   = onChannelActivityDetection;

    /* Initialize radio */
    Radio.Init(&RadioEvents);
    Radio.SetChannel(FREQUENCY);

    Radio.SetTxConfig(MODEM_LORA, TX_POWER, 0, LORA_BANDWIDTH, LORA_DATARATE,
                      LORA_CODINGRATE, LORA_PREAMBLE_LENGTH,
                      LORA_FIXED_LENGTH_PAYLOAD, true, 0, 0, LORA_IQ_INVERSION,
                      3000);

    Radio.SetRxConfig(MODEM_LORA, LORA_BANDWIDTH, LORA_DATARATE,
                      LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                      LORA_SYMBOL_TIMEOUT, LORA_FIXED_LENGTH_PAYLOAD, 0, true,
                      0, 0, LORA_IQ_INVERSION, true);

    Radio.Rx(RX_TIMEOUT);

    while (1) {
#if defined(MODE_TRANSMIT)
        /* Construct payload */
        char payloadToTransmit[PAYLOAD_SIZE];
        snprintf(payloadToTransmit, sizeof(payloadToTransmit), "Packet %u", packetCount++);
#if defined(DEBUG_PRINTING)
        printf("# Transmitting packet: \"%s\"\n", payloadToTransmit);
        uartprintf("# Transmitting packet: \"%s\"\r\n", payloadToTransmit);
#endif

        /* Transmit payload */
        Radio.Send(payloadToTransmit, strlen(payloadToTransmit));

        /* Process events */
        Event_pend(radioEvents, Event_Id_NONE, EVENT_TRANSMIT | EVENT_TRANSMIT_TIMEOUT, BIOS_WAIT_FOREVER);
        Task_sleep(TIME_MS * 20);

#elif defined(MODE_RECEIVE)
        Task_yield();

#else
#error "Error: Define MODE_TRANSMIT or MODE_RECEIVE."
#endif
    }
}

int main(void) {
    /* Initialize target board */
    Board_initGeneral();
    Board_initSPI();
    Board_initUART();

    /* Construct tast thread */
    Task_Params taskParams;
    Task_Params_init(&taskParams);
    taskParams.arg0 = 1000 * TIME_MS;
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &task0Stack;
    Task_construct(&task0Struct, (Task_FuncPtr)maintask, &taskParams, NULL);

    /* Open and set up pins */
    setuppins();

    /* Open UART */
    setupuart();

    /* Start BIOS */
    BIOS_start();

    return 0;
}
