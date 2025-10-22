#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to safely convert a 2-character hex string to a decimal integer
int hex_to_int(const char *hex_str) {
    int val = 0;
    sscanf(hex_str, "%2x", &val);
    return val;
}

long get_intel_hex_data_size(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    char line[256];
    long total_data_bytes = 0;

    while (fgets(line, sizeof(line), file)) {
        // 1. Check for the mandatory start code ':'
        if (line[0] != ':') {
            // Ignore lines that don't start with ':', like comments
            continue;
        }

        // 2. Extract the Byte Count (2 hex characters at position 1)
        // Format: :LLAAAATT[DD...]CC
        // Index:  0123456789
        char byte_count_str[3] = {line[1], line[2], '\0'};
        int byte_count = hex_to_int(byte_count_str);

        // 3. Extract the Record Type (2 hex characters at position 7)
        char record_type_str[3] = {line[7], line[8], '\0'};
        int record_type = hex_to_int(record_type_str);

        // 4. Sum the byte count *only* for Data Records (Type '00')
        if (record_type == 0x00) {
            total_data_bytes += byte_count;
        }
        // Note: We ignore other record types (01: EOF, 02: ESA, 04: ELA, etc.)
    }

    fclose(file);
    return total_data_bytes;
}

int main() {
    const char *hex_file = "HVAC_CONTROL_WITH_STM32.hex";
    
    // Create a dummy hex file for demonstration
    // :10000000300100000000000000000000000000005F (16 bytes of data)
    // :04001000ABCD012344                           (4 bytes of data)
    // :00000001FF                                   (End Of File - 0 data bytes)
    FILE *demo_file = fopen(hex_file, "r");
    // if (demo_file) {
    //     fprintf(demo_file, ":10000000300100000000000000000000000000005F\n");
    //     fprintf(demo_file, ":04001000ABCD012344\n");
    //     fprintf(demo_file, ":02000004FFFFFC\n"); // Extended Linear Address Record (ignored)
    //     fprintf(demo_file, ":00000001FF\n");
    //     fclose(demo_file);
    // }
    
    long data_size = get_intel_hex_data_size(hex_file);

    if (data_size >= 0) {
        printf("The actual binary data size in '%s' is: %ld bytes\n", hex_file, data_size);
        // Expected output for the demo file: 16 + 4 = 20 bytes
    }

    return 0;
}