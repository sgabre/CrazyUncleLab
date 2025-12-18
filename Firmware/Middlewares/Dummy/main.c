#include "Dummy.h"

int main(int argc, char const *argv[])
{
    uint8_t Major = 23;
    uint8_t Minor = 23;
    uint8_t Patch = 23;
    /* code */
    Dummy_Init();
    
    Dummy_PublicFunction(DUMMY_VERSION_MAJOR, 1, &Major);
    Dummy_PublicFunction(DUMMY_VERSION_MINOR, 1, &Minor);
    Dummy_PublicFunction(DUMMY_VERSION_PATCH, 1, &Patch);
    
    
    Dummy_Handler();
    return 0;
}
