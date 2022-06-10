#include "EdKeys.h"
#include "Client.h"

#include <exception>
#include <iostream>



int main(int argc, char* argv[])
{
  
    try
    {
        Client c("testnet");
    }
    catch (const std::exception& e)
    {
        std::cout << e.what();
    }
   
    return 0;
}