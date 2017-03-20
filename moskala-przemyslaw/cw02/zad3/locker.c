#include <memory.h>
#include <fcntl.h>
#include <zconf.h>
#include "stdlib.h"
#include "stdio.h"

const int BFFR_L = 256;
enum Options {
    lock_r, unlock_r, lock_w, unlock_w, reado, writeo, list, help, nothing, quit
};
enum Version {
    repeat, normal, nothingv
};
enum Permission {
    writep, readp
};

void display_man() {
    printf("\nUse:");
    printf("\n\t-lr\tfor locking read mode on byte,");
    printf("\n\t-ur\tfor unlocking read mode on byte,");
    printf("\n\t-lw\tfor locking write mode on byte,");
    printf("\n\t-uw\tfor locking write mode on byte,");
    printf("\n\t-r\tfor read a byte,");
    printf("\n\t-w\tfor write a byte,");
    printf("\n\t-l\tfor list locked bytes.\n");
}
/**
 * Scans number of byte to read/write/lock/unlock
 * @return number of byte
 */
int scan_byte_no() {
    char inp[BFFR_L];
    printf("Which byte?\n");
    fgets(inp, BFFR_L, stdin);
    return atoi(inp);
}
/**
 * Asking user for version of locking
 * @return
 */
enum Version scan_version() {
    char inp;
    printf("Chose version (r for repating, s for single attempt)\n");
    inp = getchar();
    switch (inp) {
        case 'r':
            return repeat;
        case 's':
            return normal;
        default:
            printf("\nerr:Unknown version\n");
            return scan_version();

    }


}
/**
 * Decodes string to enum
 * @param input - argument to decode
 * @return
 */
enum Options decode(char input[BFFR_L]) {
    if (strcmp(input, "-lr\n") == 0) return lock_r;
    if (strcmp(input, "-ur\n") == 0) return unlock_r;
    if (strcmp(input, "-lw\n") == 0) return lock_w;
    if (strcmp(input, "-uw\n") == 0) return unlock_w;
    if (strcmp(input, "-r\n") == 0) return reado;
    if (strcmp(input, "-w\n") == 0) return writeo;
    if (strcmp(input, "-l\n") == 0) return list;
    if (strcmp(input, "-help\n") == 0) return help;
    if (strcmp(input, "-quit\n") == 0) return quit;
    return nothing;
}
/**
 * Locks a byte in file
 * @param file  - file descriptpr
 * @param byte - byte to unlock
 * @param permission - lock type (read, write)
 * @param v - version (repating or single attempt
 */
void lock(int file, int byte, enum Permission permission, enum Version v) {
    struct flock *opt = malloc(sizeof(struct flock));
    switch (v) {
        case repeat:
            switch (permission) {
                case writep:
                    opt->l_type = F_WRLCK;
                    opt->l_whence = SEEK_SET;
                    opt->l_len = 1;
                    opt->l_start = byte;
                    fcntl(file, F_SETLKW, opt);
                    break;
                case readp:
                    opt->l_type = F_RDLCK;
                    opt->l_whence = SEEK_SET;
                    opt->l_len = 1;
                    opt->l_start = byte;
                    fcntl(file, F_SETLKW, opt);
                    break;
            }
            break;
        case normal:
            switch (permission) {
                case writep:
                    opt->l_type = F_WRLCK;
                    opt->l_whence = SEEK_SET;
                    opt->l_len = 1;
                    opt->l_start = byte;
                    fcntl(file, F_SETLK, opt);
                    break;
                case readp:
                    opt->l_type = F_RDLCK;
                    opt->l_whence = SEEK_SET;
                    opt->l_len = 1;
                    opt->l_start = byte;
                    fcntl(file, F_SETLK, opt);
                    break;
            }
        case nothingv:
            break;

    }
}
/**
 * Relase lock on a byte of file
 * @param file  - file descriptpr
 * @param byte - byte to unlock
 * @param permission - lock type (read, write)
 * @param v - version (repating or single attempt
 */
void unlock(int file, int byte, enum Permission permission, enum Version v) {
    struct flock *opt = malloc(sizeof(struct flock));
    switch (v) {
        case repeat:
            switch (permission) {
                case writep:
                    opt->l_type = F_UNLCK;
                    opt->l_whence = SEEK_SET;
                    opt->l_len = 1;
                    opt->l_start = byte;
                    fcntl(file, F_SETLKW, opt);

                    break;
                case readp:
                    opt->l_type = F_UNLCK;
                    opt->l_whence = SEEK_SET;
                    opt->l_len = 1;
                    opt->l_start = byte;
                    fcntl(file, F_SETLKW, opt);
                    break;
                default:
                    printf("perm err");

            }
            break;
        case normal:
            switch (permission) {
                case writep:
                    opt->l_type = F_UNLCK;
                    opt->l_whence = SEEK_SET;
                    opt->l_len = 1;
                    opt->l_start = byte;
                    fcntl(file, F_SETLK, opt);
                    break;
                case readp:
                    opt->l_type = F_UNLCK;
                    opt->l_whence = SEEK_SET;
                    opt->l_len = 1;
                    opt->l_start = byte;
                    fcntl(file, F_SETLK, opt);
                    break;
                default:
                    printf("perm err");

            }
            break;
        default:
            printf("err v");
            break;

    }
}
/**
 * Displays all locks given to a file
 * @param file - file descriptor
 */
void display_locks(int file) {
    struct flock r;
    off_t end = lseek(file, 0, SEEK_END);
    for (off_t byte = 0; byte < end; byte++) {
        lseek(file, byte, SEEK_SET);
        r.l_type = F_WRLCK;
        r.l_whence = SEEK_SET;
        r.l_start = byte;
        r.l_len = 1;
        fcntl(file, F_GETLK, &r);

        if (r.l_type != F_UNLCK) {
            if (r.l_type == F_RDLCK) {
                printf("\nLocked on read byte %ld PID %d", byte, r.l_pid);
            }
            if (r.l_type == F_WRLCK) {
                printf("\nLocked on write byte %ld PID %d", byte, r.l_pid);

            }

        }

    }


}
/**
 * Reads byte from file
 * @param file - file descriptor
 * @param byte - number of byte to read
 */
void read_from(int file, int byte) {
    char res[BFFR_L];

    lseek(file, byte, SEEK_SET);
    read(file, res, 1);
    printf("%c\n", res[0]);
}
/**
 * Wites a byte to a file
 * @param file  - file descriptor
 * @param byte - number of byte to replace
 * @param new - value of byte to write
 */
void write_to(int file, int byte, char *new) {
    lseek(file, byte, SEEK_SET);
    write(file, new, strlen(new) * sizeof(char));
}

/**
 * Eneters in interaction with file mode, asking questions to user.
 * @param filename - path to file
 */
void interact(char filename[BFFR_L]) {
    char input[BFFR_L];
    int byte;
    enum Version v;
    int file;
    int run = 1;

    while (run) {
        fgets(input, BFFR_L, stdin);
        switch (decode(input)) {
            case lock_r:
                printf("\nlocking read:\n");
                file = open(filename, O_RDONLY);
                byte = scan_byte_no();
                v = scan_version();
                lock(file, byte, readp, v);
                break;
            case unlock_r:
                printf("\nunlocking read:\n");
                file = open(filename, O_RDONLY);
                byte = scan_byte_no();
                unlock(file, byte, readp, repeat);
                break;
            case lock_w:
                printf("\nlocking write:\n");
                file = open(filename, O_RDWR);
                byte = scan_byte_no();
                v = scan_version();
                lock(file, byte, writep, v);
                break;
            case unlock_w:
                printf("\nunlocking write:\n");
                file = open(filename, O_RDONLY);
                byte = scan_byte_no();
                unlock(file, byte, writep, repeat);
                break;
            case reado:
                printf("\nread:\n");
                file = open(filename, O_RDONLY);
                byte = scan_byte_no();
                read_from(file, byte);
                break;
            case writeo:
                printf("\nwrite:\n");
                file = open(filename, O_RDWR);
                byte = scan_byte_no();
                char inp[2];
                printf("\ntype a char\n");
                fgets(inp, 2, stdin);
                write_to(file, byte, inp);
                break;
            case list:
                printf("\nlisting locks:\n");
                file = open(filename, O_RDONLY);
                display_locks(file);
                break;
            case help:
                display_man();
                break;
            case quit:
                printf("\nexiting...\n");
                close(file);
                run = 0;
            case nothing:
                break;


        }

    }
}

int main(int argc, char *argv[]) {
    display_man();
    if(argc>1){
        printf("Working with:\n");
        printf("%s",argv[1]);
        interact(argv[1]);
    }else{
        printf("\nGive a file path as an argument!\n");
    }




}