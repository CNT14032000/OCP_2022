
#include <ctype.h>
#include <stdbool.h>

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
#include <time.h>

#include "plugin_api.h"


static char *g_lib_name = "lib_add.so";

static char *g_plugin_purpose = "Compare the length of the file and the input argument";

static char *g_plugin_author = "Cao Ngoc Tuan - N3250";

#define OPT_LENG_STR "leng"

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
            OPT_LENG_STR,
            required_argument,
            0, 0,
        },
        "Requiered file size"
    }
    
    
};

static int g_po_arr_len = sizeof(g_po_arr)/sizeof(g_po_arr[0]);


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
    int ret_lib2 = -1;		// Это должно быть, но в данном случае без этого можно обойтись. В ваших лабораторных эта строка быть должна!
    
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
    
    
    int leng = 0;
    int got_leng = 0;
    int saved_errno = 0;
    
    #define OPT_CHECK(opt_var, is_double) \
    if (got_##opt_var) { \
        if (DEBUG) { \
            fprintf(stderr, "DEBUG: %s: Option '%s' was already supplied\n", \
                g_lib_name, in_opts[i].name); \
        } \
        errno = EINVAL; \
        return -1; \
    } \
    else { \
        char *endptr = NULL; \
        opt_var = is_double ? \
                    strtod((char*)in_opts[i].flag, &endptr): \
                    strtol((char*)in_opts[i].flag, &endptr, 0); \
        if (*endptr != '\0') { \
            if (DEBUG) { \
                fprintf(stderr, "DEBUG: %s: Failed to convert '%s'\n", \
                    g_lib_name, (char*)in_opts[i].flag); \
            } \
            errno = EINVAL; \
            return -1; \
        } \
        got_##opt_var = 1; \
    }
    for (size_t i = 0; i < in_opts_len; i++) {
        if (!strcmp(in_opts[i].name, OPT_LENG_STR)) {
            OPT_CHECK(leng, 0)
        }
	else {
            errno = EINVAL;
            return -1;
        }
    }
    
    if (!got_leng) {
        if (DEBUG) {
            fprintf(stderr, "DEBUG: %s: The length value was not supplied.\n",
                g_lib_name);
        }
        errno = EINVAL;
        return -1;
    }
    
    if (leng <= 0) {
        if (DEBUG) {
            fprintf(stderr, "DEBUG: %s: Leng should be >= 1!\n",
                g_lib_name);
        }
        errno = ERANGE;
        return -1;
    }
    /*
    if (DEBUG) {
       fprintf(stderr, "DEBUG: %s: Inputs: leng = %d\n", g_lib_name, leng);
    }
    */


    int fd = open(fname, O_RDONLY);
    if (fd < 0) {
        // errno is set by open()
        return -1;
    }

    
    
    struct stat st = {0};
    int res_lib2 = fstat(fd, &st);
    if (res_lib2 < 0) {
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

    if(st.st_size == leng) ret_lib2 = 0;
    else ret_lib2 = 1;

    END:
    close(fd);
    // Restore errno value
    errno = saved_errno;
    
    return ret_lib2; 
}      



































