/**
 * SPDX-License-Identifier: Apache-2.0
 * Copyright (c) Bao Project and Contributors. All rights reserved.
 */

#include <core.h>
#include <util.h>
#include <stdlib.h>
#include <stdio.h>
#include <cpu.h>
#include <wfi.h>
#include <spinlock.h>
#include <plat.h>
#include <irq.h>
#include <uart.h>
#include <timer.h>
#include <string.h>

#define VM "VM0"
#define IPC_IRQ_ID 78

#define TIMER_INTERVAL      (TIME_MS(500))

#define BAO_IMAGE_START     0x10000000UL
#define BAO_HC_OFF          0x41UL
#define BAO_HC_ADDR         BAO_IMAGE_START+BAO_HC_OFF
#define BAO_HC_IPC_ID       0x1
#define VMS_IPC_BASE        0x20017000UL
#define VMS_IPC_SIZE        0x1000

/* Define shared memory layout */
#define MESSAGE0_SIZE       ((const size_t)(0x10)) //128-bit/16-byte - place for TEE Calls UUID
#define MESSAGE0_OFFSET     VMS_IPC_BASE

#define MESSAGE1_SIZE       ((const size_t)(0x4))  //32-bit/4-byte - place for TEEC_Result
#define MESSAGE1_OFFSET     MESSAGE0_OFFSET + MESSAGE0_SIZE

#define MESSAGE2_SIZE       ((const size_t)(VMS_IPC_SIZE - MESSAGE0_SIZE - MESSAGE1_SIZE)) //Ugly, WIP
#define MESSAGE2_OFFSET     MESSAGE1_OFFSET + MESSAGE1_SIZE

static char* const message[3] = {
    (const char*)(MESSAGE0_OFFSET),
    (const char*)(MESSAGE1_OFFSET),
    (const char*)(MESSAGE2_OFFSET),
};

typedef struct {
    uint32_t timeLow;
    uint16_t timeMid;
    uint16_t timeHiAndVersion;
    uint8_t  clockSeqAndNode[8];
} TEEC_UUID;

void (*bao_hypercall)(unsigned int, unsigned int, unsigned int) =
    (void (*)(unsigned int, unsigned int, unsigned int))BAO_HC_ADDR;

void print_hex(const char* label, const void* data, size_t len) {
    const uint8_t* bytes = (const uint8_t*)data;
    printf("%s: ", label);
    for (size_t i = 0; i < len; ++i) {
        printf("%02X", bytes[i]);
        if (i < len - 1) printf(":");
    }
    printf("\r\n");
}

void shmem_init(void)
{
    memset(message[0], 0, MESSAGE0_SIZE);
    memset(message[1], 0, MESSAGE1_SIZE);
}

void ipc_notify(int ipc_id, int event_id)
{
    bao_hypercall(BAO_HC_IPC_ID, ipc_id, event_id);
}

void ipc_irq_handler(void)
{
    printf(VM": IPC Handler\n");
    print_hex("Return Code:", message[1], MESSAGE1_SIZE);
}

void uart_rx_handler(void)
{
    printf(VM": UART RX Handler\n");
    uart_clear_rxirq();
}

void timer_handler(void)
{
    // Ugly but only for testing as I'm focusing on TA
    TEEC_UUID uuid_1 = {
        .timeLow         = 0x00112233,
        .timeMid         = 0x4455,
        .timeHiAndVersion = 0x6677,
        .clockSeqAndNode = { 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF }
    };


    TEEC_UUID uuid_2 = {
        .timeLow         = 0x11223344,
        .timeMid         = 0x5566,
        .timeHiAndVersion = 0x7788,
        .clockSeqAndNode = { 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00 }
    };


    TEEC_UUID uuid_3 = {
        .timeLow         = 0x22334455,
        .timeMid         = 0x6677,
        .timeHiAndVersion = 0x8899,
        .clockSeqAndNode = { 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11 }
    };

    static uint32_t counter = 0;
    if(counter==5){
        printf(VM": Calling func 1\n");
        memcpy((void*)message[0], &uuid_1, sizeof(uuid_1));
        ipc_notify(0,0);
        counter += 1;
    }
    else if(counter==10){
        printf(VM": Calling func 2\n");
        memcpy((void*)message[0], &uuid_2, sizeof(uuid_2));
        ipc_notify(0,0);
        counter += 1;
    }
    else if(counter==15){
        printf(VM": Calling func 3\n");
        memcpy((void*)message[0], &uuid_3, sizeof(uuid_3));
        ipc_notify(0,0);
        counter += 1;
    }
    else if(counter>15){
        printf(VM": Resetting counter\n");
        counter = 0;
    }
    else{
        printf(VM": Waiting\n");
        counter += 1;
    };
}

void ipc_init(void)
{
    irq_enable(IPC_IRQ_ID);
    irq_set_handler(IPC_IRQ_ID, ipc_irq_handler);
}

void main(void)
{
    printf(VM": Bao bare-metal test guest\n");

    shmem_init();

    ipc_init();

    irq_set_handler(UART_IRQ_ID, uart_rx_handler);
    irq_set_handler(TIMER_IRQ_ID, timer_handler);

    uart_enable_rxirq();

    timer_set(TIMER_INTERVAL);
    irq_enable(TIMER_IRQ_ID);
    irq_set_prio(TIMER_IRQ_ID, IRQ_MAX_PRIO);

    irq_enable(UART_IRQ_ID);
    irq_set_prio(UART_IRQ_ID, IRQ_MAX_PRIO);

    while(1) wfi();
}
