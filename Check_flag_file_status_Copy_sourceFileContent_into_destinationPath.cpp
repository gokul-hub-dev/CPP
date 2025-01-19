#include <iostream>
#include <fstream>
#include <string>

// For Secure Partition
#define SECURE_MOUNT_FILE_PATH "/tmp/.SecurePartition"
#define SECURE_WPA_CONF "/opt/SecurePartition/wifi/wpa_supplicant.conf"
#define NON_SECURE_WPA_CONF "/opt/secure/wifi/wpa_supplicant.conf"

int main() {
    // Read the secure partition status
    std::ifstream secureFile(SECURE_MOUNT_FILE_PATH);
    if (!secureFile) {
        std::cerr << "Error: Unable to open secure mount file: " << SECURE_MOUNT_FILE_PATH << std::endl;
        return 1;
    }

    std::string secureContent;
    if (!std::getline(secureFile, secureContent)) {
        std::cerr << "Error: Secure file is empty or unreadable." << std::endl;
        return 1;
    }
    secureFile.close();

    std::string sourceFilePath;
    std::string destinationFilePath;
    // Determine source and destination paths
    if (secureContent == "status=1") {
        sourceFilePath = SECURE_WPA_CONF;
        destinationFilePath = NON_SECURE_WPA_CONF;
    } else {
        sourceFilePath = NON_SECURE_WPA_CONF;
        destinationFilePath = SECURE_WPA_CONF;
    }

    // Open the source file for reading
    std::ifstream sourceFile(sourceFilePath, std::ios::binary); // Use binary mode for potential non-text files
    if (!sourceFile) {
        std::cerr << "Error: Unable to open source file " << sourceFilePath << std::endl;
        return 1;
    }

    // Open the destination file for writing
    std::ofstream destinationFile(destinationFilePath, std::ios::binary); // Use binary mode for consistency
    if (!destinationFile) {
        std::cerr << "Error: Unable to open destination file " << destinationFilePath << std::endl;
        return 1;
    }

    // Perform file copy using stream buffer
    destinationFile << sourceFile.rdbuf();

    // Close files
    sourceFile.close();
    destinationFile.close();

    std::cout << "File copied successfully from " << sourceFilePath
              << " to " << destinationFilePath << std::endl;

    return 0;
}
