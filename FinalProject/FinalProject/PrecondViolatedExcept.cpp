/***************************************************
*	Final Project 2016
*	Team Water
*	Precondition Violated Exception implementation
*		file from the textbook.
***************************************************/
/** Listing 7-6.
    @file PrecondViolatedExcept.cpp */
#include "PrecondViolatedExcept.h"  

PrecondViolatedExcept::PrecondViolatedExcept(const std::string& message): std::logic_error("Precondition Violated Exception: " + message)
{
}  // end constructor

// End of implementation file.

