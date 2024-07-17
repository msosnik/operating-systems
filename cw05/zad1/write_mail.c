#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#define MAX_LINE_LENGTH 1024
#define MAILS_DIR "mails"

typedef struct {
    char sender[MAX_LINE_LENGTH];
    char date[MAX_LINE_LENGTH];
    char filename[MAX_LINE_LENGTH];
} Mail;

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <sender> <title> <contents>\n", argv[0]);
        return 1;
    }

    // Get the current date and time
    time_t now = time(NULL);
    char date[MAX_LINE_LENGTH];
    strftime(date, MAX_LINE_LENGTH, "%a, %d %b %Y %T %z", localtime(&now));

    // Create the mail structure
    Mail mail;
    strcpy(mail.sender, argv[1]);
    strcpy(mail.date, date);

    // Generate a unique filename for the mail
    sprintf(mail.filename, "%s/%ld_%d_%d_%d_%d_%d_%d", MAILS_DIR, now,
            rand(), rand(), rand(), rand(), rand(), rand());

    // Create the directory for the mails if it doesn't exist
    struct stat st = {0};
    if (stat(MAILS_DIR, &st) == -1) {
        mkdir(MAILS_DIR, 0700);
    }

    // Open the mail file for writing
    FILE *fp = fopen(mail.filename, "w");
    if (fp == NULL) {
        perror("Failed to create mail file");
        return 1;
    }

    // Write the mail headers and contents to the file
    fprintf(fp, "Sender: %s\n", mail.sender);
    fprintf(fp, "Date: %s\n", mail.date);
    fprintf(fp, "\nTitle: %s\t", argv[2]);
    fprintf(fp, "Contents: %s", argv[3]);

    // Close the mail file
    fclose(fp);

    printf("Mail saved to %s\n", mail.filename);

    return 0;
}
