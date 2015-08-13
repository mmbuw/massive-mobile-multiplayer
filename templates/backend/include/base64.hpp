#ifndef BASE_64_HPP
#define BASE_64_HPP

#include <string>

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int len);
std::string base64_decode(std::string const& s);

#endif //BASE_64_HPP