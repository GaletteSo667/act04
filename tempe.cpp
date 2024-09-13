#include <wiringPiI2C.h>
#include <iostream>
#include <unistd.h>

class TemperatureSensor {
private:
    int deviceHandle;
    const int MCP9808_ADDRESS = 0x18;  // Adresse I2C par défaut du MCP9808
    const int AMBIENT_TEMP_REGISTER = 0x05;  // Registre de température ambiante

public:
    TemperatureSensor() {
        // Initialiser le capteur avec wiringPi pour l'I2C
        deviceHandle = wiringPiI2CSetup(MCP9808_ADDRESS);
        if (deviceHandle == -1) {
            std::cerr << "Erreur de connexion au capteur MCP9808" << std::endl;
        }
    }

    float readTemperature() {
        // Lire la température depuis le registre approprié
        int rawTemp = wiringPiI2CReadReg16(deviceHandle, AMBIENT_TEMP_REGISTER);
        if (rawTemp == -1) {
            std::cerr << "Erreur de lecture du capteur" << std::endl;
            return -273.15;  // Retourner une valeur impossible en cas d'erreur
        }

        // Conversion des données brutes en température Celsius
        rawTemp = ((rawTemp << 8) & 0xFF00) + (rawTemp >> 8); // Inverser les octets
        float temp = rawTemp & 0x0FFF;
        temp /= 16.0;
        if (rawTemp & 0x1000) {
            temp -= 256;
        }

        return temp;
    }
};

int main() {
    // Créer une instance du capteur et lire la température
    TemperatureSensor sensor;
    float temperature = sensor.readTemperature();

    std::cout << "Température actuelle : " << temperature << "°C" << std::endl;
    return 0;
}
