/*
 * Copyright (c) 2018-2019, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**
 *  @file       Random.h
 *
 *  @brief      Interface to generate pseudo-random numbers
 *
 *  @warning    The numbers generated by this module are not crpytographically-secure!
 *              Do not use this module to generate keying material or for other
 *              security-related purposes!
 *
 *  This module generates non-cryptographically-secure random numbers in an
 *  easy to use and fast way.
 *
 *  There is a single global state that must be initialised by calling
 *  Random_seedAutomatic() or Random_seedManual(). Afterwards, you can call
 *  Random_getNumber() or Random_getBytes() as desired. Both are thread-safe
 *  and protect the internal state.
 *
 *  The pseudo-random number generator used is the "xorwow" algorithm specified in
 *  Marsaglia's "Xorshift RNGs" paper. It keeps 20 bytes of state that must be
 *  seeded and has a period of 2^160 - 2^32 before a sequence wraps.
 *
 *  Generating a random number with this algorithm is quite fast. Random_getNumber()
 *  only requires 82 instructions which is 1.7us on a 48MHz Cortex M4. That includes
 *  disabling interrupts.
 *
 *  @code
 *
 *  int_fast16_t status;
 *  uint32_t randomNumber;
 *
 *  status = Random_seedAutomatic();
 *
 *  if (status != Random_STATUS_SUCCESS) {
 *       while(1);
 *  }
 *
 *  randomNumber = Random_getNumber();
 *
 *  @endcode
 *
 *
 */

#ifndef ti_drivers_utils_Random__include
#define ti_drivers_utils_Random__include

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define Random_STATUS_SUCCESS   (0)
#define Random_STATUS_ERROR     (-1)

/*! @brief Length of the seed in bytes */
#define Random_SEED_LENGTH (20)

/**
 *  @brief Seed internal state automatically
 *
 *  This function seeds or reseeds the internal state.
 *  The method for generating the seed is device dependent.
 *
 *  If a TRNG is available, it will be used to generate the seed.
 *
 *  If a TRNG is not available, information unique to the device running
 *  the code will be used. This may be a unique device identifier or other
 *  information such as a MAC address.
 *  Since the seed is constant per device for devices without a TRNG, the
 *  number sequence will restart after each call to Random_seedAutomatic().
 *  This will usually occur after rebooting the device.
 *
 *  If neither a TRNG nor a unique device identifier is available,
 *  a constant will be used.
 *
 *  @return Returns a status code
 *
 *  @sa Random_seedManual()
 *
 *  @post Random_getNumber()
 *
 *  @post Random_getBytes()
 */
extern int_fast16_t Random_seedAutomatic(void);

/**
 *  @brief Set the internal state to a specified seed
 *
 *  This function sets the internal state to the seed specified
 *  by the application.
 *
 *  @param seed     Seed to set the internal state to
 *
 *  @sa Random_seedAutomatic()
 *
 *  @post Random_getNumber()
 *
 *  @post Random_getBytes()
 */
extern void Random_seedManual(uint8_t seed[Random_SEED_LENGTH]);

/**
 *  @brief Returns a random number
 *
 *  This function returns a random number and updates the
 *  internal state.
 *
 *  @return Returns random number
 *
 *  @pre Random_seedAutomatic()
 *  @pre Random_seedManual()
 */
extern uint32_t Random_getNumber(void);

/**
 *  Returns a number of random bytes
 *
 *  This is a convenience function that fills the specified
 *  array with random bytes by repeatedly calling Random_getNumber().
 *
 *  @param buffer     Buffer to fill with random bytes
 *
 *  @param bufferSize Size of buffer. Any value is permitted, including
 *                    those that are not multiples of sizeof(uint32_t).
 *
 *  @pre Random_seedAutomatic()
 *  @pre Random_seedManual()
 */
extern void Random_getBytes(void *buffer, size_t bufferSize);


#ifdef __cplusplus
}
#endif

#endif /* ti_drivers_utils_Random__include */