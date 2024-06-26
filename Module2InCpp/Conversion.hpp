#ifndef CONVERSION_HPP
#define CONVERSION_HPP

#include <openssl/evp.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>

class Conversion {
public:
    // Converts a byte array to a hex string
    static std::string byteToHex(const std::vector<unsigned char>& hash) {
        std::stringstream ss;
        for (unsigned char b : hash) {
            ss << std::hex << std::setw(2) << std::setfill('0') << (int)b;
        }
        return ss.str();
    }

    // Converts a hex string to a byte array
    static std::vector<unsigned char> hexToByte(const std::string& s) {
        std::vector<unsigned char> data(s.length() / 2);
        for (size_t i = 0; i < s.length(); i += 2) {
            data[i / 2] = (unsigned char)((std::stoi(s.substr(i, 1), nullptr, 16) << 4) + std::stoi(s.substr(i + 1, 1), nullptr, 16));
        }
        return data;
    }

    // Converts a byte array to a public key
    static EVP_PKEY* getPublicKey(const std::vector<unsigned char>& encodedKey) {
        const unsigned char* p = encodedKey.data();
        EVP_PKEY* pubKey = d2i_PUBKEY(nullptr, &p, encodedKey.size());
        if (!pubKey) {
            throw std::runtime_error("Failed to decode public key");
        }
        return pubKey;
    }

    // Converts a byte array to a private key
    static EVP_PKEY* getPrivateKey(const std::vector<unsigned char>& encodedKey) {
        const unsigned char* p = encodedKey.data();
        EVP_PKEY* privKey = d2i_PrivateKey(EVP_PKEY_DSA, nullptr, &p, encodedKey.size());
        if (!privKey) {
            throw std::runtime_error("Failed to decode private key");
        }
        return privKey;
    }
};

#endif // CONVERSION_HPP
