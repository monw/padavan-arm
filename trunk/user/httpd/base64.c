#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* Safe Base64 decoding lookup table */
static const uint8_t b64_decode_table[256] = {
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255, 62,255,255,255, 63,
     52, 53, 54, 55, 56, 57, 58, 59, 60, 61,255,255,255,254,255,255, /* '=' is mapped to 254 */
    255,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
     15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,255,255,255,255,255,
    255, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
     41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255
};

int b64_decode(const char* str, unsigned char* space, int size)
{
    const char* cp;
    uint32_t d, prev_d = 0;
    int space_idx = 0;
    int phase = 0;

    for (cp = str; *cp != '\0'; ++cp) {
        /* Fix 1: Cast to unsigned char to prevent negative index out-of-bounds */
        uint8_t val = b64_decode_table[(unsigned char)*cp];
        
        /* Fix 2: Break safely when encountering the padding character '=' */
        if (val == 254) { 
            break; 
        }
        
        /* Skip invalid or whitespace characters (value 255) */
        if (val != 255) {
            d = (uint32_t)val;
            
            /* Fix 3: Prevent buffer overflow by checking remaining capacity */
            if (phase > 0 && space_idx >= size) {
                return -1; 
            }

            switch (phase) {
                case 0:
                    phase = 1;
                    break;
                case 1:
                    space[space_idx++] = (unsigned char)((prev_d << 2) | ((d & 0x30) >> 4));
                    phase = 2;
                    break;
                case 2:
                    space[space_idx++] = (unsigned char)(((prev_d & 0x0f) << 4) | ((d & 0x3c) >> 2));
                    phase = 3;
                    break;
                case 3:
                    space[space_idx++] = (unsigned char)(((prev_d & 0x03) << 6) | d);
                    phase = 0;
                    break;
            }
            prev_d = d;
        }
    }
    return space_idx;
}

#if 0

/* Helper function to execute and log test scenarios */
void run_test(int test_num, const char* description, const char* input, int buf_size) {
    unsigned char output[100] = {0}; 
    
    printf("--- Test Case %d: %s ---\n", test_num, description);
    printf("Input string: \"%s\"\n", input);
    printf("Provided buffer size: %d\n", buf_size);
    
    int result = b64_decode(input, output, buf_size);
    
    if (result == -1) {
        printf("Result: [FAILED] Buffer overflow prevented successfully!\n\n");
    } else {
        printf("Result: [SUCCESS] Decoded bytes = %d\n", result);
        printf("Decoded text: \"");
        for(int i = 0; i < result; i++) {
            /* Print printable ASCII chars normally, print others as hex escape */
            if (output[i] >= 32 && output[i] <= 126) {
                putchar(output[i]);
            } else {
                printf("\\x%02X", output[i]);
            }
        }
        printf("\"\n\n");
    }
}

int main() {
    /* Case 1: Standard aligned base64 string without padding */
    run_test(1, "Standard no padding (4-byte aligned)", "YW55", 50);

    /* Case 2: Base64 string with single '=' padding */
    run_test(2, "Single '=' padding character", "YW55Pz0=", 50);

    /* Case 3: Base64 string with double '=' padding */
    run_test(3, "Double '=' padding characters", "YW55Pw==", 50);

    /* Case 4: Security test for destination buffer truncation */
    run_test(4, "Insufficient buffer size (Expected overflow mitigation)", "YW55", 1);

    /* Case 5: Robustness test against malicious non-ASCII inputs */
    run_test(5, "Malicious non-ASCII input handling", "YW55\x80\xffYW55", 50);

    /* Case 6: Input containing whitespaces and newlines to be ignored */
    run_test(6, "Ignoring whitespaces and newline characters", "YW55\r\n YW55", 50);

    return 0;
}

#endif
