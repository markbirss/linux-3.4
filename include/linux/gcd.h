/*
 * include/linux/gcd.h
 *
 * Copyright (c) 2016 Allwinnertech Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */
#ifndef _GCD_H
#define _GCD_H

#include <linux/compiler.h>

unsigned long gcd(unsigned long a, unsigned long b) __attribute_const__;

#endif /* _GCD_H */
