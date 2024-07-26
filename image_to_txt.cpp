#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
//no google translate here idc
//coefficients from random site, i'll add (r+g+b)/3  later

unsigned char fbrightness (unsigned char r, unsigned char g, unsigned char b)
{
    float bg = 0.59f * g;
    float br = 0.3f * r;
    float bb = 0.11f * b;
    unsigned char answer = (unsigned char)(bg + br + bb);
    answer = answer / 32;
    return answer;
}

unsigned char r_fbrightness (unsigned char r, unsigned char g, unsigned char b)
{
    float bg = 0.59f * g;
    float br = 0.3f * r;
    float bb = 0.11f * b;
    unsigned char answer = 255 - (unsigned char)(bg + br + bb);
    answer = answer / 32;
    return answer;
}

int main()
{
    printf("Enter path to file or its name, if file in the same folder with program.\n\nIf u will enter more then 127 symbols, you will lost all data on your hard drive.\n\nJust kidding! :-)))\n\n");
    int buffer_size = 128;
    char file_path[buffer_size];
    char enter;
    scanf("%c", &enter);
    int enter_i = 0;
    for(;enter_i < (buffer_size - 2) && enter != '\n'; enter_i++)
    {
        file_path[enter_i] = enter;
        scanf("%c", &enter);
    }
    file_path[enter_i] = '\0';  //idk is this needed lol

    printf("%s\n", file_path);

    int x, y, n;

    bool format_check = stbi_info(file_path, &x, &y, &n);

    if(format_check == 0)
    {
        printf("Invalid path or format of file.\n");
        printf("Enter anything to close console.\n");
        char w;
        scanf(" %c",&w);
        return 0;
    }

    unsigned char* raw_image = stbi_load(file_path, &x, &y, &n, 0);

    if (raw_image == NULL)
    {
        printf("Image loading failed.\n");
        printf("Enter anything to close console.\n");
        char w;
        scanf(" %c",&w);
        return 0;
    }

    if(n != 3 && n != 4){printf("Unsupported file in this version of program. Please, check the program is up to date.\n");printf("Enter anything to close console.\n");char w;scanf(" %c",&w);return 0;}

    printf("Do you want reverse colors of the image? y/n\n\n"); //here u r actually choose brightness function
    char reverse_;
    unsigned char (*pointer) (unsigned char , unsigned char , unsigned char );
    fflush(stdin);
    scanf("%c", &reverse_);
    switch(reverse_)
    {
        case 'y': pointer = &r_fbrightness;break;
        case 'n': pointer = &fbrightness;break;
        default: printf("Wrong input.\n");printf("Enter anything to close console.\n");char w;scanf(" %c",&w);return 0;
    }

    printf("Enter integer coefficient of resolution reduction (or 1 to save original size of image).\n\n");
    int coeff;
    scanf("%d", &coeff);
    unsigned char* image = NULL;
    int finx = x;
    int finy = y;
    if(coeff == 1)
    {
        image = raw_image;
    }
    else
    {
        finx = x / coeff;
        finy = y / coeff;
        int memory_int = finx * finy;
        image = (unsigned char*)malloc(sizeof(unsigned char)*n*memory_int);
         //pixel++ is kind of square made from pixels
         //c1 for pixels in strings oh pixels++
         //c2 for strings in pixels++
         //c3 for pixels++ in string++
         //c4 for strings++
        int r;
        int g;
        int b;
        for(int c4 = 0; c4 < finy; c4++)
        {
            for(int c3 = 0; c3 < finx ; c3++)
            {
                for(int c2 = 0; c2 < coeff; c2++)
                {
                    for(int c1 = 0; c1 < coeff; c1++)
                    {
                        r += raw_image[(c4*n*x*coeff) + (c3*n*coeff) + (c2*x*n) + (c1*n)];
                        g += raw_image[(c4*n*x*coeff) + (c3*n*coeff) + (c2*x*n) + (c1*n) + 1];
                        b += raw_image[(c4*n*x*coeff) + (c3*n*coeff) + (c2*x*n) + (c1*n) + 2];
                    }
                }

                r = r / coeff; g = g / coeff; b = b / coeff;
                r = r / coeff; g = g / coeff; b = b / coeff;
                image[(c4*n*finx) + (c3*n)] = r;
                image[(c4*n*finx) + (c3*n) + 1] = g;
                image[(c4*n*finx) + (c3*n) + 2] = b;
                r = 0; g = 0; b = 0;
            }
        }
        free(raw_image);
     }

    FILE* output = fopen("output.txt", "wb");
    //chars alphabet
    const char* c1 = "\xE2\x96\x88";
    const char* c2 = "\xE2\x96\x91";
    const char* c3 = "\xE2\x96\x92";
    const char* c4 = "\xE2\x96\x93";

    fputs("\xEF\xBB\xBF", output); //.txt: ASCI -> UTF-8
    //pixel -> char
    for(int i = 0; i < finy; i++)
    {
        for(int j = 0; j <= finx; j ++)
        {
            switch(pointer(image[((finx*i) + j)*n], image[(((finx*i) + j)*n)+1], image[(((finx*i) + j)*n)+2]))   //pointer is chosen function, all creepy shit because of ++ instead of +=n
            {
                case 0:
                    putc(' ', output); putc(' ', output);
                    break;
                case 1:
                    putc('-', output); putc('-', output);
                    break;
                case 2:
                    putc('/', output); putc('/', output);
                    break;
                case 3:
                    putc('#', output); putc('#', output);
                    break;
                case 4:
                    fputs(c1, output); fputs(c1, output);
                    break;
                case 5:
                    fputs(c2, output); fputs(c2, output);
                    break;
                case 6:
                    fputs(c3, output); fputs(c3, output);
                    break;
                case 7:
                    fputs(c4, output); fputs(c4, output);
                    break;
            }
        }
        putc('\n', output);
    }

    free(image);
    fclose(output);
    printf("Success. Enter anything to close console.\n");
    char w;
    scanf(" %c",&w);
    return 0;
}
