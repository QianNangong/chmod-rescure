#include <stdio.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <unistd.h>

#define CHMOD_PATH_OLD "/bin/chmod"
#define CHMOD_PATH_NEW "/usr/bin/chmod"

int main(int argc, const char* argv[])
{
    const char* chmod_path;
    
    if (geteuid() == 0)
    {
        chmod_path = NULL;
        if (access(CHMOD_PATH_NEW, F_OK) == 0)
        {
            chmod_path = CHMOD_PATH_NEW;
        }
        else if (access(CHMOD_PATH_OLD, F_OK) == 0)
        {
            chmod_path = CHMOD_PATH_OLD;
        }
        else if (argc >= 2)
        {
            if (access(argv[1], F_OK) == 0)
            {
                chmod_path = argv[1];
            }
            else
            {
                printf("%s not exist.\n", argv[1]);
                return -1;
            }
        }
        else
        {
            printf("Failed to locate chmod in /usr/bin and /bin\n"
                   "Please provide a valid path as arguments\n"
                   "    %s path/to/chmod\n", argv[0]);
                   return -1;
        }
        if (chmod(chmod_path, 0755) == 0)
        {
            puts("Jobs done!\n");
        }
        else
        {
            printf("Failed to reset %s to 0755 mode, e = %d\n", chmod_path, errno);
            return -1;
        }
        return 0;
    }
    else
    {
        puts("Only root can do.\n");
        return -1;
    }
}
