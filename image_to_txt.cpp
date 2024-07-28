#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
//no google translate here idc
//Ñaitlin <3<3<3<3<3<3<3<3<3<3<3<3<3 (she helped me w brightness func)
double fbrightness (unsigned char ir, unsigned char ig, unsigned char ib)  // ir - input red
{
    //1st step
    double r = (double)ir / 255;
    double g = (double)ig / 255;
    double b = (double)ib / 255;
    //2nd step
    if(r > 0.04045)
    {
        r =  pow(((r + 0.055) / 1.055), 2.4);
    }
    else
    {
        r = r / 12.92;
    }
    if(g > 0.04045)
    {
        g =  pow(((g + 0.055) / 1.055), 2.4);
    }
    else
    {
        g = g / 12.92;
    }
    if(b > 0.04045)
    {
        b =  pow(((b + 0.055) / 1.055), 2.4);
    }
    else
    {
        b = b / 12.92;
    }
    //3rd step
    double x = ((r * 0.4124564) + (g * 0.3575761) + (b * 0.1804375));
    double y = ((r * 0.2126729) + (g * 0.7151522) + (b * 0.0721750));
    double z = ((r * 0.0193339) + (g * 0.1191920) + (b * 0.9503041));
    return sqrt((x*x)+(y*y)+(z*z));
}

double r_fbrightness (unsigned char ir, unsigned char ig, unsigned char ib)
{
    return fbrightness((255 - ir), (255 - ig), (255 - ib));
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

    int x, y, n;

    bool format_check = stbi_info(file_path, &x, &y, &n);

    if(format_check == 0)
    {
        printf("Invalid path or format of file.\n");
        printf("Enter anything to close console.\n");
        char w;
        fflush(stdin);
        scanf("%c",&w);
        return 0;
    }

    unsigned char* raw_image = stbi_load(file_path, &x, &y, &n, 0);

    if (raw_image == NULL)
    {
        printf("Image loading failed.\n");
        printf("Enter anything to close console.\n");
        char w;
        fflush(stdin);
        scanf("%c",&w);
        return 0;
    }

    if(n != 3 && n != 4){printf("Unsupported file in this version of program. Please, check the program is up to date.\n");printf("Enter anything to close console.\n");char w;fflush(stdin);scanf("%c",&w);return 0;}

    printf("Do you want reverse colors of the image? y/n\n\n");
    char reverse_;
    double (*pointer) (unsigned char , unsigned char , unsigned char );
    fflush(stdin);
    scanf("%c", &reverse_);
    switch(reverse_)
    {
        case 'y':
        {
            pointer = &r_fbrightness;
            break;
        }
        case 'n':
        {
            pointer = &fbrightness;
            break;
        }
        default: printf("Wrong input.\n");printf("Enter anything to close console.\n");char w;fflush(stdin);scanf("%c",&w);return 0;
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
                //pb means pixel brightness
                double pb = pointer(image[((finx*i) + j)*n], image[(((finx*i) + j)*n)+1], image[(((finx*i) + j)*n)+2]); //pointer is chosen function, all creepy shit because of ++ instead of +=n
                if(pb < 0.219692)  //(max value of fbrightness / 8) (cuz we have 8 chars)
                {
                    putc(' ', output); putc(' ', output);
                }
                else if (pb < 0.439384)
                {
                    putc('-', output); putc('-', output);
                }
                else if (pb < 0.659077)
                {
                    putc('/', output); putc('/', output);
                }
                else if (pb < 0.878769)
                {
                    fputs(c1, output); fputs(c1, output);
                }
                else if (pb < 1.098461)
                {
                    putc('#', output); putc('#', output);
                }
                else if (pb < 1.318154)
                {
                    fputs(c2, output); fputs(c2, output);
                }
                else if (pb < 1.537846)
                {
                    fputs(c3, output); fputs(c3, output);
                }
                else
                {
                    fputs(c4, output); fputs(c4, output);
                }
            }
            putc('\n', output);
        }

    free(image);
    fclose(output);
    printf("Success. Enter anything to close console.\n");
    char w;
    fflush(stdin);
    scanf("%c",&w);

    return 0;
}
