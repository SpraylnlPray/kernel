#include "danos.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "file.h"

void print_usage()
{
    printf("Usage:\n");
    printf("\t ls DIRECTORY \t to show the contents of a directory and each element's size\n");
    printf("\t ls FILE \t to show the filename and its size\n");

    return;
}

bool list_file(char *path)
{
    struct stat file_stat;
    int res = stat(path, &file_stat);
    if (res != 0)
    {
        printf("Error reading %s\n", path);
        res = -1;
        goto out;
    }

    printf("%40s %i\n", path, file_stat.size);

out:
    return res;
}

bool list_directory(char *path)
{
    int res = 0;
    struct dirent *dirent = NULL;
    int fd = opendir(path);
    int dir_fd = 0;
    struct dirent *el_dirent = NULL;

    if (!fd)
    {
        printf("Error opening directory: %s\n", path);
        res = -1;
        goto out;
    }

    dirent = readdir(fd);
    if (!dirent)
    {
        printf("Error reading directory %s\n", path);
        res = -1;
        goto out;
    }

    while (dirent != NULL)
    {
        int total_size = 0;
        if (dirent->d_type == DT_DIR)
        {
            if (strncmp(dirent->d_name, ".", 1) == 0)
            {
                printf("%s\n", dirent->d_name);
                dirent = readdir(fd);
                continue;
            }

            if (strncmp(dirent->d_name, "..", 2) == 0)
            {
                printf("%s\n", dirent->d_name);
                dirent = readdir(fd);
                continue;
            }

            char dir_path[1024] = {0};
            path_concat(dir_path, path, dirent->d_name);

            int dir_fd = opendir(dir_path);
            if (!dir_fd)
            {
                printf("Error reading %s\n", dir_path);
                res = -1;
                goto out;
            }

            el_dirent = readdir(dir_fd);
            if (!el_dirent)
            {
                printf("Error reading %s\n", dir_path);
                res = -1;
                goto out;
            }

            while (el_dirent != NULL)
            {
                if (strncmp(el_dirent->d_name, ".", 1) == 0)
                {
                    el_dirent = readdir(dir_fd);
                    continue;
                }

                if (strncmp(el_dirent->d_name, "..", 2) == 0)
                {
                    el_dirent = readdir(dir_fd);
                    continue;
                }

                char el_path[1024] = {0};
                path_concat(el_path, dir_path, el_dirent->d_name);
                struct stat el_stat;

                int res = stat(el_path, &el_stat);
                if (res != 0)
                {
                    printf("Error reading %s\n", el_path);
                    res = -1;
                    goto out;
                }

                total_size = total_size + el_stat.size;
                el_dirent = readdir(dir_fd);
            }
        }
        else if (dirent->d_type == DT_REG)
        {
            char file_path[1024] = {0};
            path_concat(file_path, path, dirent->d_name);
            struct stat file_stat;
            int res = stat(file_path, &file_stat);
            if (res != 0)
            {
                printf("Error reading %s\n", dirent->d_name);
                res = -1;
                goto out;
            }

            total_size = file_stat.size;
        }

        printf("%20s %i\n", dirent->d_name, total_size);
        dirent = readdir(fd);
    }

out:
    if (fd)
    {
        closedir(fd);
    }

    if (dirent)
    {
        free(dirent);
    }

    if (dir_fd)
    {
        closedir(dir_fd);
    }

    if (el_dirent)
    {
        free(el_dirent);
    }

    return res;
}

int main(int argc, char **argv)
{
    if (argc <= 1 || argc > 2)
    {
        print_usage();
        return 0;
    }

    struct stat buf;
    int res = stat(argv[1], &buf);
    if (res != 0)
    {
        printf("No such file or directory: %s", argv[1]);
        return -1;
    }

    if (buf.type == FILE_TYPE_DIRECTORY)
    {
        return list_directory(argv[1]);
    }
    if (buf.type == FILE_TYPE_FILE)
    {
        return list_file(argv[1]);
    }

    printf("Unknown file type\n");
    return -1;
}