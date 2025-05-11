#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>

#define PNG_SIG_CAP 8
const uint8_t png_sig[PNG_SIG_CAP] = {137, 80, 78, 71, 13, 10, 26, 10};
#define CHUNK_BUF_CAP (32 * 1024)

#define read_bytes_or_panic(file, buf, buf_cap) read_bytes_or_panic_(file, buf, buf_cap, __FILE__, __LINE__)
static void read_bytes_or_panic_(FILE *file, void *buf, size_t buf_cap, const char *source_file, int source_line) {
    size_t n = fread(buf, buf_cap, 1, file);
    if (n != 1) {
        if (ferror(file)) {
            fprintf(stderr, "%s:%d: ERROR: could not read %zu bytes from file: %s\n",
                    source_file, source_line,
                    buf_cap, strerror(errno));
            exit(1);
        } else if (feof(file)) {
            fprintf(stderr, "%s:%d: ERROR: could not read %zu bytes from file: reached the end of file\n",
                    source_file, source_line,
                    buf_cap);
            exit(1);
        } else {
            assert(0 && "Unreachable");
        }
    }
}

static void print_bytes(const uint8_t *buf, size_t buf_cap) {
    for (size_t i = 0; i < buf_cap; ++i) {
        printf("%u ", buf[i]);
    }
    printf("\n");
}

void reverse_bytes(void *buf0, size_t buf_cap) {
    uint8_t *bytes = buf0;
    for (size_t i = 0; i < buf_cap/2; ++i) {
        uint8_t t = bytes[i];
        bytes[i] = bytes[buf_cap - i - 1];
        bytes[buf_cap - i - 1] = t;
    }
}

static void usage(FILE *file, const char *program) {
    fprintf(file, "Usage: %s <input.png>\n", program);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        usage(stderr, argv[0]);
        fprintf(stderr, "ERROR: exactly one input file is required\n");
        exit(1);
    }
    const char *input_filepath = argv[1];
    FILE *input_file = fopen(input_filepath, "rb");
    if (input_file == NULL) {
        fprintf(stderr, "ERROR: could not open file %s: %s\n",
                input_filepath, strerror(errno));
        exit(1);
    }

    uint8_t sig[PNG_SIG_CAP];
    read_bytes_or_panic(input_file, sig, PNG_SIG_CAP);
    printf("Signature: ");
    print_bytes(sig, PNG_SIG_CAP);
    if (memcmp(sig, png_sig, PNG_SIG_CAP) != 0) {
        fprintf(stderr, "ERROR: %s does not appear to be a valid PNG image\n",
                input_filepath);
                fclose(input_file);
        exit(1);
    }

    while (true) {
        uint32_t chunk_sz;
        read_bytes_or_panic(input_file, &chunk_sz, sizeof(chunk_sz));
        reverse_bytes(&chunk_sz, sizeof(chunk_sz));

        uint8_t chunk_type[5] = {0};
        read_bytes_or_panic(input_file, chunk_type, 4);

        if (strncmp((char*)chunk_type, "faKE", 4) == 0) {
            char *data = malloc(chunk_sz + 1);
            if (!data) {
                fprintf(stderr, "ERROR: memory allocation failed for %u bytes\n", chunk_sz);
                fclose(input_file);
                exit(1);
            }

            read_bytes_or_panic(input_file, data, chunk_sz);
            data[chunk_sz] = '\0';
            printf("Decoded message from 'faKE' chunk: %s\n", data);
            free(data);

            // Read and discard CRC
            uint32_t crc;
            read_bytes_or_panic(input_file, &crc, sizeof(crc));
            break;
        } else {
            // Skip chunk data and CRC
            if (fseek(input_file, chunk_sz + 4, SEEK_CUR) != 0) {
                fprintf(stderr, "ERROR: failed to seek past chunk: %s\n", strerror(errno));
                fclose(input_file);
                exit(1);
            }
        }
    }

    fclose(input_file);
    return 0;
}
