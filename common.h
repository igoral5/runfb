/*
 * common.h
 *
 *  Created on: 28.04.2013
 *      Author: igor
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <vector>
#include <stdint.h>

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


class mode
{
public:
	explicit mode(std::vector<uint8_t>& vec) : vec_(vec), it_(vec.begin()) {}
	virtual ~mode() throw () {}
	virtual void init() = 0;
	virtual void work() = 0;
	void set_all(uint8_t value);
protected:
	virtual void next();
	std::vector<uint8_t>& vec_;
	std::vector<uint8_t>::iterator it_;
};

class mode_1 : public mode
{
public:
	explicit mode_1(std::vector<uint8_t>& vec) : mode(vec) {}
	virtual ~mode_1() throw() {}
	virtual void init();
	virtual void work();
};

class mode_2 : public mode
{
public:
	explicit mode_2(std::vector<uint8_t>& vec) : mode(vec) {}
	virtual ~mode_2() throw() {}
	virtual void init();
	virtual void work();
};

enum Cnt {ODD, EVEN};

class mode_3 : public mode
{
public:
	explicit mode_3(std::vector<uint8_t>& vec) : mode(vec), cnt_(ODD) {}
	virtual ~mode_3() throw() {}
	virtual void init();
	virtual void work();
protected:
	virtual void next();
	Cnt cnt_;
};






#endif /* COMMON_H_ */
