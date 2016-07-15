#include <stdio.h>
#include <string.h>
/* simplified data base and query function */
int is_array(const char *s)
{
    int len = strlen(s);
    return s[len-1] == ']';
}
int array_len(const char *s)
{
    int l = strlen(s)-3;
    if (s[l] == 'b') return 3;
    if (s[l] == 'c') return 4;
    if (s[l] == 'd') return 2;
    return 0;
}
#define whatever printf
#define fmt ("%s - %d,%d,%d\n")
void function (const char **exp, int *para, int deepth)
{
    if (is_array(*exp)) {
        int num = array_len(exp[0]);
        for (int i = 0; i < num; ++i) {
            if (*(exp+1) == NULL) {
                para[deepth] = i+1; // just put it here, so it's easy to be catched
                whatever(fmt, *exp, para[0],para[1],para[2]);
            } else {
                para[deepth] = i+1; // just put it here, so it's easy to be catched
                function(exp+1, para, deepth+1);
            }  
        }      
    } else {   
        if (*(exp+1) == NULL) {
            whatever(fmt, *exp, para[0],para[1],para[2]);
        } else {
            function(exp+1, para, deepth);
        }  
    }      
}          
/**        
 * the origin string is: "a.b[].c[].d[].e", which can be split as follow using strtok
 * which then will be easy for recursion processing.  **/
const char *expr[] = {"a", 
                      "a.b[]",
                      "a.b[].c[]",
                      "a.b[].c[].d[]",
                      "a.b[].c[].d[].e",
                      NULL};

int main()
{
    int param[20] = {0};
    function(expr, param, 0);
    printf("done\n");
    return 0;
}
