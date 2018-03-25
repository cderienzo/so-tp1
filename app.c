#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
/* "readdir" etc. are defined here. */
#include <dirent.h>
#include <errno.h>
#include <limits.h>

static void list_dir (const char * dir_name, int tabs);

int main(int argc, char const *argv[])
{
	if(argc<2)
	{
		printf("A path must be specified\n");
		return 1;
	}
	list_dir(argv[1],0);
	return 0;
}


static void list_dir (const char * dir_name, int tabs)
{
    DIR * d;
    /* Open the directory specified by "dir_name". */
    d = opendir(dir_name);
    /* Check it was opened. */
    if (d==NULL) {
        //printf("Cannot open directory: %s\n", dir_name);
        fprintf (stderr, "Cannot open directory '%s': %s\n",
                 dir_name, strerror (errno));
        exit (EXIT_FAILURE);
        return;
    }
    struct dirent * entry;
    const char * d_name;
    while ((entry=readdir(d))!=NULL) {	        
	    /* "Readdir" gets subsequent entries from "d". */         
	    d_name = entry->d_name;
	    /* Print the name of the file and directory. */
		if (strcmp (d_name, "..") != 0 && strcmp (d_name, ".") != 0){
			for(int i = 0 ; i < tabs ; i ++){
				printf("\t");
			}
			printf ("%s\n",d_name);			
		}		
	    if (entry->d_type & DT_DIR) {
	        /* Check that the directory is not "d" or d's parent. */	      
	        if (strcmp (d_name, "..") != 0 && strcmp (d_name, ".") != 0) {
	        	int path_length;
                char path[PATH_MAX];
 
                path_length = snprintf (path, PATH_MAX,
                                        "%s/%s", dir_name, d_name);               
                if (path_length >= PATH_MAX) {
                    fprintf (stderr, "Path length has got too long.\n");
                    exit (EXIT_FAILURE);
                }
	                /* Recursively call "list_dir" with the new path. */
	                list_dir (path,tabs+1);
	         }
		}
	}
	    
	/* After going through all the entries, close the directory. */
	if (closedir (d)) {
	    printf("Error ocurred closing directory\n");	       
	}
	
}