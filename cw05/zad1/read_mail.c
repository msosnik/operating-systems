//
// Created by michal on 11.04.23.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAX_MAIL_FILES 100

typedef struct {
    char sender[MAX_LINE_LENGTH];
    char date[MAX_LINE_LENGTH];
    char contents[MAX_LINE_LENGTH];
} Mail;

void print_mail(const Mail* mail) {
    printf("Sender: %s\nDate: %s\nContents: %s\n\n", mail->sender, mail->date, mail->contents);
}

int compare_by_sender(const void* a, const void* b) {
    const Mail* mail_a = (const Mail*)a;
    const Mail* mail_b = (const Mail*)b;
    return strcmp(mail_a->sender, mail_b->sender);
}

//this is very hard so i left that version. it may not function correctly.
int compare_by_date(const void* a, const void* b) {
    const Mail* mail_a = (const Mail*)a;
    const Mail* mail_b = (const Mail*)b;
    return strcmp(mail_a->date, mail_b->date);
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <mode>\n", argv[0]);
        return EXIT_FAILURE;
    }
    // Sort the mails by sender or date, depending on run parameter
    int sort_by_sender;
    if (strcmp(argv[1], "data")==0){
        sort_by_sender = 0;
    } else if(strcmp(argv[1], "nadawca") == 0){
        sort_by_sender = 1;
    } else{
        printf("available modes for %s: \"data\" or \"nadawca\"\n", argv[0]);
        return EXIT_FAILURE;
    }

    int mail_count = 0;
    Mail mails[MAX_MAIL_FILES];

//    // Open a pipe to the find command to retrieve all mail files
//    FILE* pipe = popen("find ./mails -type f", "r");
//    if (pipe == NULL) {
//        perror("popen");
//        return 1;
//    }
    // Use popen to list the mail files in the mail directory
    FILE *ls_fp = popen("ls mails", "r");
    if (ls_fp == NULL) {
        perror("Failed to run ls command");
        return EXIT_FAILURE;
    }

    char filename[MAX_LINE_LENGTH];
    // Read each filename from the ls output
    while (fgets(filename, MAX_LINE_LENGTH, ls_fp) != NULL) {
        // Remove the newline character from the filename
        filename[strcspn(filename, "\n")] = 0;

        // Construct the path to the mail file
        char path[MAX_LINE_LENGTH];
        snprintf(path, MAX_LINE_LENGTH, "mails/%.*s", MAX_LINE_LENGTH-7, filename);

        // Open the mail file for reading
        FILE *mail_fp = fopen(path, "r");
        if (mail_fp == NULL) {
            perror("Failed to open mail file");
            return EXIT_FAILURE;
        }

        // Read the mail headers and contents from the file
        fgets(mails[mail_count].sender, MAX_LINE_LENGTH, mail_fp);
        fgets(mails[mail_count].date, MAX_LINE_LENGTH, mail_fp);

        fgets(mails[mail_count].contents, MAX_LINE_LENGTH, mail_fp);
        fgets(mails[mail_count].contents, MAX_LINE_LENGTH, mail_fp);

        // Close the mail file
        fclose(mail_fp);

        // Increment the mail count
        mail_count++;
    }
//    // Read the filenames of all mail files
//    char buffer[MAX_LINE_LENGTH];
//    while (fgets(buffer, sizeof(buffer), ls_fp) != NULL && num_mails < MAX_MAIL_FILES) {
//        buffer[strcspn(buffer, "\n")] = '\0'; // Remove trailing newline
//        strncpy(mails[num_mails].filename, buffer, sizeof(mails[num_mails].filename));
//        num_mails++;
//    }
//
//    // Close the pipe
//    pclose(ls_fp);
//
//    // Read the headers of each mail file to retrieve the sender and date
//    for (int i = 0; i < num_mails; i++) {
//        FILE* pipe = popen("cat %s | grep -E '^From:|^Date:' | sed 's/^[^:]*: *//' | tr -d '\\n'", "r");
//        if (pipe == NULL) {
//            perror("popen");
//            return 1;
//        }
//
//        fgets(buffer, sizeof(buffer), pipe); // Read the sender
//        strncpy(mails[i].sender, buffer, sizeof(mails[i].sender));
//
//        fgets(buffer, sizeof(buffer), pipe); // Read the date
//        strncpy(mails[i].date, buffer, sizeof(mails[i].date));
//
//        pclose(pipe);
//    }

    if (sort_by_sender) {
        qsort(mails, mail_count, sizeof(Mail), compare_by_date);
    } else {
        qsort(mails, mail_count, sizeof(Mail), compare_by_sender);
    }

    // Print the sorted mails
    for (int i = 0; i < mail_count; i++) {
        print_mail(&mails[i]);
    }

    return 0;
}
