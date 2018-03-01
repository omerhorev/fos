#pragma once
/**
 * This file contains all the tracing and printing funcs used in the project
 */

#include <stdio.h>

#define trace_printf(format, ...) fprintf (stdout, format, ##__VA_ARGS__)
#define trace_err_printf(format, ...) fprintf (stderr, format, ##__VA_ARGS__)
