#include "lpc_puf.h"
#include <stdio.h>
#include "fsl_device_registers.h"
#include "fsl_common.h"
#include "fsl_puf.h"
#include "fsl_power.h"
// #include "board.h"

#include "mbedtls/timing.h"
#include "mbedtls/ecp.h"
#include "mbedtls/entropy.h"
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/ecdsa.h"
#include "mbedtls/sha256.h"
//
#include <authentication_prover.h>
#include <authentication_verifier.h>
#include <enrollment.h>

void lpc_puf_init()
{
    status_t status;
    __attribute__((aligned(16))) uint8_t activation_code[PUF_ACTIVATION_CODE_SIZE];

    puf_config_t pufConfig = { .dischargeTimeMsec = 400, .coreClockFrequencyHz = CLOCK_GetFreq(kCLOCK_CoreSysClk) };

    POWER_SetBodVbatLevel(kPOWER_BodVbatLevel1650mv, kPOWER_BodHystLevel50mv, false);
    return;
}
