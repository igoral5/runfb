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
#include <tr1/memory>
#include "common.h"

volatile bool work = true;

void
sig_handler(int signo)
{
	work = false;
}

enum Mode {MODE_1, MODE_2, MODE_3};

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
	leds.push_back(GREEN1);
	bcm2835_gpio_fsel(YELLOW1, BCM2835_GPIO_FSEL_OUTP);
	leds.push_back(YELLOW1);
	bcm2835_gpio_fsel(BLUE1, BCM2835_GPIO_FSEL_OUTP);
	leds.push_back(BLUE1);
	bcm2835_gpio_fsel(WHITE1, BCM2835_GPIO_FSEL_OUTP);
	leds.push_back(WHITE1);
	bcm2835_gpio_fsel(RED1, BCM2835_GPIO_FSEL_OUTP);
	leds.push_back(RED1);
	bcm2835_gpio_fsel(GREEN2, BCM2835_GPIO_FSEL_OUTP);
	leds.push_back(GREEN2);
	bcm2835_gpio_fsel(YELLOW2, BCM2835_GPIO_FSEL_OUTP);
	leds.push_back(YELLOW2);
	bcm2835_gpio_fsel(BLUE2, BCM2835_GPIO_FSEL_OUTP);
	leds.push_back(BLUE2);
	bcm2835_gpio_fsel(WHITE2, BCM2835_GPIO_FSEL_OUTP);
	leds.push_back(WHITE2);
	bcm2835_gpio_fsel(RED2, BCM2835_GPIO_FSEL_OUTP);
	leds.push_back(RED2);
	bcm2835_gpio_fsel(GREEN3, BCM2835_GPIO_FSEL_OUTP);
	leds.push_back(GREEN3);
	bcm2835_gpio_fsel(YELLOW3, BCM2835_GPIO_FSEL_OUTP);
	leds.push_back(YELLOW3);
	bcm2835_gpio_fsel(BLUE3, BCM2835_GPIO_FSEL_OUTP);
	leds.push_back(BLUE3);
	bcm2835_gpio_fsel(WHITE3, BCM2835_GPIO_FSEL_OUTP);
	leds.push_back(WHITE3);
	bcm2835_gpio_fsel(RED3, BCM2835_GPIO_FSEL_OUTP);
	leds.push_back(RED3);
	bcm2835_gpio_fsel(GREEN4, BCM2835_GPIO_FSEL_OUTP);
	leds.push_back(GREEN4);
	bcm2835_gpio_fsel(BUTTON, BCM2835_GPIO_FSEL_INPT);
	bcm2835_gpio_set_pud(BUTTON, BCM2835_GPIO_PUD_UP);
	uint8_t old_button_stat = HIGH;
	std::tr1::shared_ptr<mode_1> mode1(new mode_1(leds));
	std::tr1::shared_ptr<mode_2> mode2(new mode_2(leds));
	std::tr1::shared_ptr<mode_3> mode3(new mode_3(leds));
	std::tr1::shared_ptr<mode> blink = mode1;
	blink->init();
	Mode mode = MODE_1;
	while(work)
	{
		uint8_t new_button_stat = bcm2835_gpio_lev(BUTTON);
		if (old_button_stat == HIGH && new_button_stat == LOW)
		{
			switch(mode)
			{
			case MODE_1:
				mode = MODE_2;
				blink = mode2;
				break;
			case MODE_2:
				mode = MODE_3;
				blink = mode3;
				break;
			case MODE_3:
				mode = MODE_1;
				blink = mode1;
				break;
			}
			blink->init();
		}
		blink->work();
		old_button_stat = new_button_stat;
		bcm2835_delay(100);
	}
	blink->set_all(HIGH);
	bcm2835_close();
	return EXIT_SUCCESS;
}
catch(const std::exception& er)
{
	std::cerr << argv[0] << ": " << er.what() << std::endl;
	return EXIT_FAILURE;
}
catch(...)
{
	std::cerr << argv[0] << ": " << "Unknown error" << std::endl;
	return EXIT_FAILURE;
}

