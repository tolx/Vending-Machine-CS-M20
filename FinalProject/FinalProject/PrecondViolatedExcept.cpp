/*
CS M20 - Final Project
Team Water
Members who worked on this:
The textbook :P
*/
/** Listing 7-6.
    @file PrecondViolatedExcept.cpp */
#include "PrecondViolatedExcept.h"  

PrecondViolatedExcept::PrecondViolatedExcept(const string& message): logic_error("Precondition Violated Exception: " + message)
{
}  // end constructor


// End of implementation file.

