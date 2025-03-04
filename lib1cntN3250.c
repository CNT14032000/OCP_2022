#include <dirent.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "plugin_api.h"


static char *g_lib_name = "lib1cntN3250.so";

static char *g_plugin_purpose = "Option2 : Check mac-addr";

static char *g_plugin_author = "Cao Ngoc Tuan - N3250";

#define OPT_MAC "mac-addr"

static struct plugin_option g_po_arr[] = {
/*
    struct plugin_option {
        struct option {
           const char *name;
           int         has_arg;
           int        *flag;
           int         val;
        } opt,
        char *opt_descr
    }
*/
    {
        {
            OPT_MAC,
            required_argument,
            0, 0,
        },
        "Target id of mac and check mac-addr"
    },
    
    
};



static int g_po_arr_len = sizeof(g_po_arr)/sizeof(g_po_arr[0]);

//
//  Private functions
//

bool isMacAddress (const char* mac, int index_from);


//
//  API functions
//
int plugin_get_info(struct plugin_info* ppi) {
    if (!ppi) {
        fprintf(stderr, "ERROR: invalid argument\n");
        return -1;
    }
    
    ppi->plugin_purpose = g_plugin_purpose;
    ppi->plugin_author = g_plugin_author;
    ppi->sup_opts_len = g_po_arr_len;
    ppi->sup_opts = g_po_arr;
    
    return 0;
}

int plugin_process_file(const char *fname,
        struct option in_opts[],
        size_t in_opts_len) {

    // Return error by default
    int ret_lib1 = 1;
    
    // Pointer to file mapping
    unsigned char *ptr = NULL;
    char *f = NULL;
    
    char *DEBUG = getenv("LAB1DEBUG");
    
    if (!fname || !in_opts || !in_opts_len) {
        errno = EINVAL;
        return -1;
    }
    
     
   if (DEBUG) {
        for (size_t i = 0; i < in_opts_len; i++) {
            fprintf(stderr, "DEBUG: %s: Got option '%s' with arg '%s'\n",
                g_lib_name, in_opts[i].name, (char*)in_opts[i].flag);
        }
    }
    

    const char* mac_id;
    int size_mac_id = 0;

    for (size_t i = 0; i < in_opts_len; i++) {
        if (!strcmp(in_opts[i].name, OPT_MAC)) {

            mac_id = (char*) in_opts[i].flag;
            size_mac_id = strlen(mac_id);

        }
        
        else {
            errno = EINVAL;
            return -1;
        }
    }

    //check input id_mac;


    if(!isMacAddress(mac_id,0)){
        if (DEBUG) {
            fprintf(stderr, "DEBUG: %s: Mac-addr has the form [02X(:or-)02X(:or-)02X(:or-)02X(:or-)02X(:or-)02X],[02X02X02X02X02X02X]\n",
                g_lib_name);
        }
        errno = ERANGE;
        return -1;
    }  
    
        
    if (DEBUG) {
        fprintf(stderr, "DEBUG: %s: Inputs: mac-addr = %s \n",
            g_lib_name, mac_id);
    }
    


    int saved_errno = 0;
    int size_f ;
    
    
    int fd = open(fname, O_RDONLY);
    if (fd < 0) {
        // errno is set by open()
        return -1;
    }

    
    
    struct stat st = {0};
    int res_lib1 = fstat(fd, &st);
    if (res_lib1 < 0) {
        saved_errno = errno;
        goto END;
    }
    
    // Check that size of file is > 0
    if (st.st_size == 0) {
        if (DEBUG) {
            fprintf(stderr, "DEBUG: %s: File size should be > 0\n",
                g_lib_name);
        }
        saved_errno = ERANGE;
        goto END;
    }

    ptr = mmap(0, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (ptr == MAP_FAILED) {
        saved_errno = errno;
        goto END;
    }
     
    
    size_f = st.st_size;

   

    f = (char *) mmap(0, st.st_size, PROT_READ, MAP_PRIVATE,fd, 0);

    
    char* buffer; //save mac address in file
    buffer = (char*) malloc (sizeof(char)*size_mac_id+1);

    ret_lib1 = 1;
    for(int i = 0; i < size_f; i++){
        if(isMacAddress(f,i)){
            for(int j = 0; j < size_mac_id; j++){
                buffer[j] = f[i+j];
            }
            buffer[size_mac_id] = 0;

            if(!strcmp(buffer,mac_id)){

                ret_lib1 = 0;
            
                if (DEBUG) {
                    fprintf(stderr, "DEBUG: %s: file have mac-addr '%s'\n", 
                        g_lib_name, mac_id);
                }
                
            }
            else {

                if(ret_lib1) ret_lib1 = 1;
                if (DEBUG) {
                    fprintf(stderr, "DEBUG: %s: file not have mac-addr '%s'\n", 
                        g_lib_name, mac_id);
                }
                
            }

        }
    }
    free(buffer);
   

    END:
    close(fd);
    if (ptr != MAP_FAILED && ptr != NULL) munmap(ptr, st.st_size); 
    if (f != MAP_FAILED && f != NULL) munmap(f,st.st_size);
    // Restore errno value
    errno = saved_errno;
    return ret_lib1;
}
bool isMacAddress(const char* mac, int index_from) {
    int mac_idx = 0;
    int mac_s = 0;
    const int maxsize_mac = 17;

    for (int i = 0;i < maxsize_mac; i++) {
        if((mac[index_from+12] == '\n' || mac[index_from+12] == 0 || mac[index_from+12] ==' ') && mac_idx == 12 && mac_s == 0 ) return true;
        if (!mac[i + index_from]) return false;
        if (isxdigit(mac[i+index_from])) {
            mac_idx++;
        }
        else if ((mac[i+index_from] == ':' || mac[i+index_from] == '-' || mac[i+index_from] ==' ' ) && i % 3 == 2) {

            if (mac_idx == 0 || mac_idx / 2 - 1 != mac_s)
                break;

            ++mac_s;
        }
        else {
            mac_s = -1;
        }

    }

    return (mac_idx == 12 && (mac_s == 5 || mac_s == 0));

  
}
