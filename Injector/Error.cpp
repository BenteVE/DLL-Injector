// display error messages
#include "Error.h"
void DisplayError(const char* errorTitle, const char* errorMessage)
{
    MessageBox(NULL, errorMessage, errorTitle, NULL);
    exit(-1);
}