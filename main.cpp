/*
 * main.cpp
 *
 *  Created on: 22.04.2013
 *      Author: igor
 */

#include <iostream>
#include <cstdlib>
#include <exception>
#include <stdexcept>
#include <vector>
#include <signal.h>
#include <bcm2835.h>

#define GREEN1 RPI_V2_GPIO_P1_11
#define YELLOW1 RPI_V2_GPIO_P1_12
#define BLUE1 RPI_V2_GPIO_P1_13
#define WHITE1 RPI_V2_GPIO_P1_15
#define RED1 RPI_V2_GPIO_P1_16
#define BUTTON RPI_V2_GPIO_P1_18
#define GREEN2 RPI_V2_GPIO_P1_19
#define YELLOW2 RPI_V2_GPIO_P1_21
#define BLUE2 RPI_V2_GPIO_P1_23
#define WHITE2 RPI_V2_GPIO_P1_24
#define RED2 RPI_V2_GPIO_P1_26
#define GREEN3 RPI_V2_GPIO_P1_03
#define YELLOW3 RPI_V2_GPIO_P1_05
#define BLUE3 RPI_V2_GPIO_P1_07
#define WHITE3 RPI_V2_GPIO_P1_08
#define RED3 RPI_V2_GPIO_P1_10
#define GREEN4 RPI_V2_GPIO_P1_22



volatile bool work = true;

void
sig_handler(int signo)
{
	work = false;
}

void
set_all(std::vector<uint8_t>::const_iterator it, std::vector<uint8_t>::const_iterator it_end, uint8_t value)
{
	for (; it != it_end; ++it)
	{
		bcm2835_gpio_write(*it, value);
	}
}

enum Mode {SINGLE, IGNITION, SHUTDOWN};

int
main(int argc, char *argv[])
try
{
	struct sigaction act;
	act.sa_handler = sig_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if (sigaction(SIGINT, &act, NULL) < 0)
	{
		throw std::runtime_error("Error set signal handler");
	}
	if (!bcm2835_init())
	{
		throw std::runtime_error("Error bcm2835_init");
	}
	std::vector<uint8_t> leds;
	bcm2835_gpio_fsel(GREEN1, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(GREEN1, LOW);
	leds.push_back(GREEN1);
	bcm2835_gpio_fsel(YELLOW1, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(YELLOW1, HIGH);
	leds.push_back(YELLOW1);
	bcm2835_gpio_fsel(BLUE1, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(BLUE1, HIGH);
	leds.push_back(BLUE1);
	bcm2835_gpio_fsel(WHITE1, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(WHITE1, HIGH);
	leds.push_back(WHITE1);
	bcm2835_gpio_fsel(RED1, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(RED1, HIGH);
	leds.push_back(RED1);
	bcm2835_gpio_fsel(GREEN2, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(GREEN2, HIGH);
	leds.push_back(GREEN2);
	bcm2835_gpio_fsel(YELLOW2, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(YELLOW2, HIGH);
	leds.push_back(YELLOW2);
	bcm2835_gpio_fsel(BLUE2, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(BLUE2, HIGH);
	leds.push_back(BLUE2);
	bcm2835_gpio_fsel(WHITE2, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(WHITE2, HIGH);
	leds.push_back(WHITE2);
	bcm2835_gpio_fsel(RED2, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(RED2, HIGH);
	leds.push_back(RED2);
	bcm2835_gpio_fsel(GREEN3, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(GREEN3, HIGH);
	leds.push_back(GREEN3);
	bcm2835_gpio_fsel(YELLOW3, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(YELLOW3, HIGH);
	leds.push_back(YELLOW3);
	bcm2835_gpio_fsel(BLUE3, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(BLUE3, HIGH);
	leds.push_back(BLUE3);
	bcm2835_gpio_fsel(WHITE3, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(WHITE3, HIGH);
	leds.push_back(WHITE3);
	bcm2835_gpio_fsel(RED3, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(RED3, HIGH);
	leds.push_back(RED3);
	bcm2835_gpio_fsel(GREEN4, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_write(GREEN4, HIGH);
	leds.push_back(GREEN4);
	bcm2835_gpio_fsel(BUTTON, BCM2835_GPIO_FSEL_INPT);
	uint8_t old_button_stat = HIGH;
	Mode mode = SINGLE;
	std::vector<uint8_t>::iterator led_on = leds.begin();
	while(work)
	{
		uint8_t new_button_stat = bcm2835_gpio_lev(BUTTON);
		if (old_button_stat == HIGH && new_button_stat == LOW)
		{
			if (mode == SINGLE)
			{
				mode = IGNITION;
			}
			else
			{
				mode = SINGLE;
			}
			set_all(leds.begin(), leds.end(), HIGH);
			led_on = leds.begin();
			bcm2835_gpio_write(*led_on, LOW);
		}
		if (mode == SHUTDOWN || mode == SINGLE)
		{
			bcm2835_gpio_write(*led_on, HIGH);
		}
		led_on++;
		if (led_on == leds.end())
		{
			led_on = leds.begin();
			if (mode != SINGLE )
			{
				if (mode == IGNITION)
				{
					mode = SHUTDOWN;
				}
				else
				{
					mode = IGNITION;
				}
			}
		}
		if (mode == IGNITION || mode == SINGLE)
		{
			bcm2835_gpio_write(*led_on, LOW);
		}
		else
		{
			bcm2835_gpio_write(*led_on, HIGH);
		}
		old_button_stat = new_button_stat;
		bcm2835_delay(100);
	}
	set_all(leds.begin(), leds.end(), HIGH);
	bcm2835_close();
	return EXIT_SUCCESS;
}
catch(const std::exception& er)
{
	std::cerr << argv[0] << " : " << er.what() << std::endl;
	return EXIT_FAILURE;
}
catch(...)
{
	std::cerr << argv[0] << " : " << "Unknown error" << std::endl;
	return EXIT_FAILURE;
}

