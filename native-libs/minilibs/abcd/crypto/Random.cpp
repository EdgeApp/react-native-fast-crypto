/*
 * Copyright (c) 2014, Airbitz, Inc.
 * All rights reserved.
 *
 * See the LICENSE file for more information.
 */

#include "Random.hpp"
#include "../Context.hpp"
#include "../util/U08Buf.hpp"
#include <openssl/rand.h>
#ifndef __ANDROID__
#include <sys/statvfs.h>
#endif
#include <sys/time.h>
#include <unistd.h>
#include "Scrypt.hpp"

namespace abcd {

#define DO_STARTUP_SCRYPT_TIMING 0
#define UUID_BYTE_COUNT         16
#define UUID_STR_LENGTH         (UUID_BYTE_COUNT * 2) + 4

/**
 * Sets the seed for the random number generator
 */
Status
randomInitialize(DataSlice seed)
{
    unsigned long timeVal;
    time_t timeResult;
    clock_t clockVal;
    pid_t pid;

    // create our own copy so we can add to it
    AutoU08Buf NewSeed;
    ABC_BUF_DUP(NewSeed, seed);

    // mix in some info on our file system
#ifndef __ANDROID__
    std::string rootDir = gContext->paths.rootDir();
    ABC_BUF_APPEND_PTR(NewSeed, rootDir.data(), rootDir.size());
    struct statvfs fiData;
    if ((statvfs(rootDir.c_str(), &fiData)) >= 0 )
    {
        ABC_BUF_APPEND_PTR(NewSeed, (unsigned char *)&fiData, sizeof(struct statvfs));
    }
#endif

    // add some time
    struct timeval tv;
    gettimeofday(&tv, NULL);
    timeVal = tv.tv_sec * tv.tv_usec;
    ABC_BUF_APPEND_PTR(NewSeed, &timeVal, sizeof(unsigned long));

    timeResult = time(NULL);
    ABC_BUF_APPEND_PTR(NewSeed, &timeResult, sizeof(time_t));

    clockVal = clock();
    ABC_BUF_APPEND_PTR(NewSeed, &clockVal, sizeof(clock_t));

    timeVal = CLOCKS_PER_SEC ;
    ABC_BUF_APPEND_PTR(NewSeed, &timeVal, sizeof(unsigned long));

    // add process id's
    pid = getpid();
    ABC_BUF_APPEND_PTR(NewSeed, &pid, sizeof(pid_t));

    pid = getppid();
    ABC_BUF_APPEND_PTR(NewSeed, &pid, sizeof(pid_t));

    // TODO: add more random seed data here

    // seed it
    RAND_seed(NewSeed.data(), NewSeed.size());

    // XXX Remove for production. Used to test performance of scrypt on various devices on startup
#if DO_STARTUP_SCRYPT_TIMING
    ScryptSnrp snrp;
    snrp.createSnrpFromTime(90000);
    snrp.createSnrpFromTime(60000);
    snrp.createSnrpFromTime(45000);
    snrp.createSnrpFromTime(30000);
    snrp.createSnrpFromTime(25000);
    snrp.createSnrpFromTime(20000);
    snrp.createSnrpFromTime(15000);
    snrp.createSnrpFromTime(10000);
    snrp.createSnrpFromTime(5000);
    snrp.createSnrpFromTime(2000);
    snrp.createSnrpFromTime(1000);
    snrp.createSnrpFromTime(500);
    snrp.createSnrpFromTime(250);
    snrp.createSnrpFromTime(100);
    snrp.createSnrpFromTime(50);
    snrp.createSnrpFromTime(25);
    snrp.createSnrpFromTime(10);
    snrp.createSnrpFromTime(5);
    snrp.createSnrpFromTime(1);
    snrp.create();
#endif

    return Status();
}

Status
randomData(DataChunk &result, size_t size)
{
    DataChunk out;
    out.resize(size);

    if (!RAND_bytes(out.data(), out.size()))
        return ABC_ERROR(ABC_CC_Error, "Random data generation failed");

    result = std::move(out);
    return Status();
}

/*
 * Version 4 UUIDs use a scheme relying only on random numbers.
 * This algorithm sets the version number (4 bits) as well as two reserved bits.
 * All other bits (the remaining 122 bits) are set using a random or pseudorandom data source.
 * Version 4 UUIDs have the form xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx where x is any hexadecimal
 * digit and y is one of 8, 9, A, or B (e.g., F47AC10B-58CC-4372-A567-0E02B2E3D479).
 */
Status
randomUuid(std::string &result)
{
    DataChunk data;
    ABC_CHECK(randomData(data, UUID_BYTE_COUNT));

    // put in the version
    // To put in the version, take the 7th byte and perform an and operation using 0x0f, followed by an or operation with 0x40.
    data[6] = (data[6] & 0xf) | 0x40;

    // 9th byte significant nibble is one of 8, 9, A, or B
    data[8] = (data[8] | 0x80) & 0xbf;

    char out[UUID_STR_LENGTH + 1];
    sprintf(out,
            "%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X",
            data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
            data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

    result = out;
    return Status();
}

} // namespace abcd
