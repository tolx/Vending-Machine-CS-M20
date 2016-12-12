/***************************************************
*	Final Project 2016
*	Team Water
*	Precondition Violated Exception header file
*		from the textbook.
***************************************************/
/** Listing 7-5.
    @file PrecondViolatedExcept.h */

#ifndef _PRECOND_VIOLATED_EXCEPT
#define _PRECOND_VIOLATED_EXCEPT

#include <stdexcept>
#include <string>

class PrecondViolatedExcept : public std::logic_error
{
public:
   PrecondViolatedExcept(const std::string& message = "");
}; // end PrecondViolatedExcep 
#endif
