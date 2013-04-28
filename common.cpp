/*
 * common.cpp
 *
 *  Created on: 28.04.2013
 *      Author: igor
 */

#include "common.h"
#include <stdint.h>
#include <vector>
#include <bcm2835.h>

void mode::set_all(uint8_t value)
{
	std::vector<uint8_t>::iterator it = vec_.begin();
	std::vector<uint8_t>::iterator it_end = vec_.end();
	for (; it != it_end; ++it)
	{
		bcm2835_gpio_write(*it, value);
	}
}

void mode::next()
{
	++it_;
	if (it_ == vec_.end())
	{
		it_ = vec_.begin();
	}
}

void mode_3::next()
{
	++it_;
	if (it_ == vec_.end())
	{
		it_ = vec_.begin();
		if (cnt_ == ODD)
		{
			cnt_ = EVEN;
		}
		else
		{
			cnt_ = ODD;
		}
	}
}

void mode_1::init()
{
	set_all(HIGH);
	it_ = vec_.begin();
	bcm2835_gpio_write(*it_, LOW);
}

void mode_1::work()
{
	bcm2835_gpio_write(*it_, HIGH);
	next();
	bcm2835_gpio_write(*it_, LOW);
}

void mode_2::init()
{
	set_all(LOW);
	it_ = vec_.begin();
	bcm2835_gpio_write(*it_, HIGH);
}

void mode_2::work()
{
	bcm2835_gpio_write(*it_, LOW);
	next();
	bcm2835_gpio_write(*it_, HIGH);
}

void mode_3::init()
{
	set_all(HIGH);
	it_ = vec_.begin();
	cnt_ = ODD;
	bcm2835_gpio_write(*it_, LOW);
}

void mode_3::work()
{
	if (cnt_ == EVEN)
	{
		bcm2835_gpio_write(*it_, HIGH);
	}
	next();
	if (cnt_ == ODD)
	{
		bcm2835_gpio_write(*it_, LOW);
	}
}


