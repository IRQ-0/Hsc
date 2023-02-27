#ifndef CLOCK_H
#define CLOCK_H

/*
 Class that contains functions to get time and date in std::string
*/

#include <iostream>
#include <ctime>

std::string getDate(void);
/* Returns date */

std::string getTime(void);
/* Returns time */

std::string getTimeStamp(void);
/*Returns time + date */

#endif