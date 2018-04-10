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
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>

/* TI-RTOS header files */
#include <ti/drivers/PIN.h>
#include <ti/drivers/SPI.h>
#include <ti/drivers/UART.h>

/* LoRaBug Firmware header files */
#include "Board.h"

/* LoRaMac-node header files */
#include "LoRaMac-node/src/boards/LoRaBug/board.h"

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

/* Transmit power (dBm) */
#define TX_POWER 20

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
#define BUFFER_SIZE 64

/* Reference time of one millisecond */
#define TIME_MS (1000/Clock_tickPeriod)

/* */
uint16_t BufferSize = BUFFER_SIZE;
uint8_t Buffer[BUFFER_SIZE];

/* Value for received signal strength indicator (RSSI) */
int8_t rssiValue = 0;

/* Value for signal-to-noise ratio (SNR) */
int8_t snrValue = 0;

/* Buffer for snprintf() */
static char sbuf[128];

/* Radio events */
#define EVENT_TRANSMIT                      Event_Id_00
#define EVENT_RECEIVE                       Event_Id_01
#define EVENT_TRANSMIT_TIMEOUT              Event_Id_02
#define EVENT_RECEIVE_TIMEOUT               Event_Id_03
#define EVENT_RECEIVE_ERROR                 Event_Id_04
#define EVENT_CHANNEL_ACTIVITY_DETECT_TRUE  Event_Id_05
#define EVENT_CHANNEL_ACTIVITY_DETECT_FALSE Event_Id_06

/* Radio events function pointer */
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
    
}

void onReceive(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr) {

}

void onTransmitTimeout(void) {

}

void onReceiveTimeout(void) {

}

void onReceiveError(void) {

}

void onChannelActivityDetection(bool channelActivityDetected) {

}

int main(void) {

}
