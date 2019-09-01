/*
 ============================================================================
 Name        : mezza_tests.c
 Author      : Danilo Almeida
 Version     :
 Copyright   : Copyright 2018 Danilo Marcos Pontes de Almeida. All rights reserved.
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

#include <mini/test.h>
#include <mini/base/error.h>

TEST_MAIN

TEST_USE_TEST(base_error_c__tests)

int main(int argc, char **argv) {

    ErrorInit(argc, argv);

    TEST_INIT(Mini Library)

    TEST_SWITH_SHOW_TOTAL

    TEST_CALL_TEST_MODULE(base_error_c__tests)

    TEST_SUMMARY
    
    return 0;
}

