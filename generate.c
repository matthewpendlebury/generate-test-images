//Matthew Pendlebury 2023
//This code is free to use/modify commercially or personally however you want, I'd appreciate 
//a mention but I am not going to loose legal attack dogs to hunt you down. Hope it helps!


//Open an existing valid HEIC file.  
//Then overwrite a part of it with a unique sequence for this run, followed by a counter. 
//(Intention being this run of the programe will generate a unique signature AS will each file.)
//Write out the current version, then remodify and repeat.  
//Generates a large volume of similar but distinct test images in a short
//time.  Around 10,000 x 2MB images in 24 seconds on an m1 mac. 
//Output images are not guaranteed to be visually meaningful but are guaranteed to be 
//distinct at a binary level. 
//Each run of the program should generate a different set of output.  

//Build with associated makefile. Only tested on m1 mac using gcc, but there are
//no exotic dependencies or advanced features so should build in many places on 
//an ANSI C compiler.  


//usage: generate  source_filename numimages 


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void PrintUsage();

#define NUM_RANDOM_BYTES 32

int main (int argc, char **argv)
{
    unsigned int num_images = 1;
    FILE *fp_src;
    FILE *fp_out;
    char session_random[NUM_RANDOM_BYTES];  //A set of bytes that make this run unique
    char *src_buffer = NULL;
    int offset = -1;  //Where we will write the modifying data (session before, counter after)
                      //is later set to source filesize/2
                             
    char* output_prefix = "outputfile";
    char* output_suffix = ".HEIC";  //Doesn't check just names things like this


    if (argc != 3)
    {
        PrintUsage();
        exit (-1);
    }

    num_images = atoi(argv[2]); 

    //generate some session random bytes - start with unix time and then fille the rest up with
    //some randomness.  

    
    unsigned long unix_time = (unsigned long)time(NULL);
    memcpy(session_random, &unix_time, sizeof(unsigned long)); // should be good enough for one run a second. 

    //fill up with some random bytes
    srand (time(NULL));
    for (int n = 0; n < NUM_RANDOM_BYTES-sizeof(unsigned long); n++)
    {
        session_random[sizeof(unsigned long)+n] = random() % 256;
    }
    

    //load in src file
    if ((fp_src = fopen(argv[1], "rb")) == NULL)
    {
        fprintf(stderr, "Can't read file %s exiting\n", argv[1]);
        exit(-2);
    }
    fseek(fp_src, 0L, SEEK_END);
    int file_sz = ftell(fp_src);
    fseek(fp_src, 0L, SEEK_SET);

    src_buffer = malloc (file_sz ); 

    printf("Source file size is %d\n", file_sz);


    if(fread(src_buffer, 1, file_sz, fp_src) != file_sz)
    {
        fprintf(stderr, "Didn't successfully read the source file\n");
        exit(-3);
    }

    fclose(fp_src);

    if (file_sz < 128)
    {
        fprintf(stderr, "Source file is too small to meaningfully modify/n");
        exit(-5);
    }
    offset = file_sz/2;  //modify the middle of the file. 

// Write the session munge 

    memcpy(src_buffer+offset-NUM_RANDOM_BYTES, session_random, NUM_RANDOM_BYTES); //TODO magic num


//Main image munging loop.  

    char *filename_out = (char *) malloc(1024);
    strcpy(filename_out, output_prefix);

    for (unsigned long n = 0; n < num_images; n++)
    {
        //overwrite the sequential bytes
        memcpy(src_buffer+offset, &n, sizeof(unsigned long));
        
        //create new filename
        sprintf(filename_out+strlen(output_prefix), "%lu%s", n, output_suffix);

        //create file for filename and write out current buffer
        if ((fp_out = fopen(filename_out, "wb")) == NULL)
        {
            fprintf(stderr, "Can't create file %s exiting\n", filename_out);
            exit(-4);
        }
        printf("writing file %s, (%lu/%s)\n", filename_out, n+1, argv[2]);
        
        fwrite(src_buffer, file_sz, 1, fp_out);
        fclose(fp_out);
    }//for n

    return 0;

} //main 


void PrintUsage()
{
   fprintf(stderr, "Wrong number parameters - Usage: //usage: program.exe source_filename numimages\n");
        fprintf(stderr, "e.g.   program source.heic 10000 \n");

}

