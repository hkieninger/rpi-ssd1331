//c++
#include <stdexcept> //std::runtime_error
#include <string> //std::string

/*
 * represents an exception connected to the gpio pins
 */
class GPIOException: public std::runtime_error {
    public:
        explicit GPIOException(const std::string& what_arg) : std::runtime_error(what_arg) {};
        explicit GPIOException(const char* what_arg) : std::runtime_error(what_arg) {};
};