
#include "optionalfunctions.h"
#include <string.h>
#include <stdarg.h>

//Basic function to swap two characters.
void swap(char *x, char *y){
    char temp = *x;
    *x = *y;
    *y = temp;
}

//Function to reverse the character buffer that will be given to itoa
char *reverse(char *buffer, int i, int j){
    //This loop iteratively steps and swaps the first and last character in the buffer
    while(i<j){
        swap(&buffer[i++], &buffer[j--]);
    }
    return buffer;
}

int abs(int x){
    x = (x < 0)? x*-1 : x;
    return x;
}

/*********************************************************
 * itoa converts an integer value to a string using the given base and stores the result.
 * If the base is 10 and the value is negative then the string will have a leading minus
 * sign. With all other bases the value is considered unsigned. The number base is 
 * limited between 2 and 36. The returned string is null terminated.
*********************************************************/
char *itoa(int value, char *str, int base){
    //Reject bases outside of the range
    if(base < 2 || base > 36){
        return str;
    }

    //This keeps track of the index in the return string
    int i = 0;

    //Handle the zero case separately and take the absolute value of the number
    int n = abs(value);
    if(n == 0){
        str[i++] = 0;
        str[i] = '\0';
        return str;
    }

    /*******************************************************
     * Since itoa is defined to allow users to convert to arbitrary number bases we
     * have to do that before converting to a string. The way to do this is to continually
     * take the modulo of the original number and append the remainder to the string while
     * dividing the original number by the base. You do this until the original number is not
     * greater than 0.
    *******************************************************/
    while(n > 0){
        int remainder = n % base;
        /***************************************************
         * If the remainder is greater than 9 then it cannot be represented by a single number.
         * Therefore, characters A-Z are used. In ASCII, '0' is 48 and 'A' is 65. These numbers
         * are used as offsets to get the correct ASCII characters. This behavior is easily
         * represented with a ternary operator.
        ***************************************************/
        str[i++] = (remainder > 9)? 65 + (remainder - 10) : 48 + remainder;
        n = n/base;
    }

    //If the number base is 10 and the number is negative a '-' is appended
    if(base == 10 && value < 0){
        str[i++] = '-';
    }
    str[i] = '\0';

    //When using the remainder method to convert number bases, the result has to be reversed to
    //get the correct answer
    
    return reverse(str, 0, i-1);
}

int sprintf(char *str, const char *format, ...){
    va_list ap;
    va_start(ap, format);
    char *index;
    int i = 0;
    int length = 0;
    char s[33];
    char *s2;

    for(index = format; *index != '\0'; index++){
        while(*index != '%'){
            *str++ = *index++;
        }
        index++;

        switch(*index){

            //Decimal/Integer argument
            case 'd':
                i = va_arg(ap, int);
                strcpy(s, itoa(i, s, 10));
                length = strlen(s);
                strcat(str, s);
                str += length;
                memset(s, 0, 33);
                break;

            //Alternate decimal/integer argument
            case 'i':
                i = va_arg(ap, int);
                strcpy(s, itoa(i, s, 10));
                length = strlen(s);
                strcat(str, s);
                str += length;
                memset(s, 0, 33);
                break;

            //Hex argument
            case 'x':
                i = va_arg(ap, int);
                strcpy(s, itoa(i, s, 16));
                length = strlen(s);
                strcat(str, s);
                str += length;
                memset(s, 0, 33);
                break;

            //Octal argument
            case 'o':
                i = va_arg(ap, int);
                strcpy(s, itoa(i, s, 8));
                length = strlen(s);
                strcat(str, s);
                str += length;
                memset(s, 0, 33);
                break;

            //Character argument
            case 'c':
                i = va_arg(ap, int);
                *str++ = i;
                break;

            //String argument
            case 's':
                s2 = va_arg(ap, char*);
                length = strlen(s2);
                strcat(str, s2);
                str += length;
                break;

        }
    }
    va_end(ap);
    return strlen(str)+1;
}
