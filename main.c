#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


long long decode_base(const char *value, int base) {
    long long result = 0;
    while (*value) {
        char digit = *value;
        int numeric_value = (digit >= '0' && digit <= '9') ? (digit - '0') : (digit - 'a' + 10);
        result = result * base + numeric_value;
        value++;
    }
    return result;
}


double calculate_constant_term(int k, int x[], long long y[]) {
    double constant = 0.0;
    for (int i = 0; i < k; i++) {
        double term = y[i];
        for (int j = 0; j < k; j++) {
            if (i != j) {
                term *= (double)(0 - x[j]) / (x[i] - x[j]);
            }
        }
        constant += term;
    }
    return constant;
}


void parse_json(const char *filename, int *n, int *k, int x[], long long y[]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Unable to open JSON file.\n");
        exit(1);
    }

    char buffer[8192];
    fread(buffer, 1, sizeof(buffer), file);
    fclose(file);

    char *pos;
    char key[10], value[50], base_str[10];

   
    pos = strstr(buffer, "\"n\":");
    sscanf(pos, "\"n\": %d", n);

    pos = strstr(buffer, "\"k\":");
    sscanf(pos, "\"k\": %d", k);

    int count = 0;
    for (int i = 1; i <= *n + 2; i++) { 
        snprintf(key, sizeof(key), "\"%d\":", i);
        pos = strstr(buffer, key);
        if (pos) {
            sscanf(pos, "\"%*d\": { \"base\": \"%[^\"]\", \"value\": \"%[^\"]\" }", base_str, value);

            x[count] = i; // Use i as the x-coordinate
            y[count] = decode_base(value, atoi(base_str));
            count++;
        }
    }

    
    if (count < *k) {
        printf("Error: Not enough roots to compute polynomial (k=%d, available=%d).\n", *k, count);
        exit(1);
    }
}

int main() {
    int n, k;
    int x[10];           
    long long y[10];

    parse_json("input.json", &n, &k, x, y);

    double constant = calculate_constant_term(k, x, y);

    printf("The constant term (c) of the polynomial is: %.10lf\n", constant);

    return 0;
}
