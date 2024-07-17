#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>


#define MAX_LINE_LENGTH 1024
#define MAILS_DIR "mails"

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


    if (argc == 4) {



        // Get the current date and time
        time_t now = time(NULL);
        char date[MAX_LINE_LENGTH];
        strftime(date, MAX_LINE_LENGTH, "%a, %d %b %Y %T %z", localtime(&now));

        // Create the mail structure
        Mail mail;
        strcpy(mail.sender, argv[1]);
        strcpy(mail.date, date);

        // Generate a unique filename for the mail
        sprintf(mail.contents, "%s/%ld_%d_%d_%d_%d_%d_%d", MAILS_DIR, now,
                rand(), rand(), rand(), rand(), rand(), rand());

        // Create the directory for the mails if it doesn't exist
        struct stat st = {0};
        if (stat(MAILS_DIR, &st) == -1) {
            mkdir(MAILS_DIR, 0700);
        }

        // Open the mail file for writing
        FILE *fp = fopen(mail.contents, "w");
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

        printf("Mail saved to %s\n", mail.contents);
        return 0;
    } else if (argc == 2) {

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
    } else {

        printf("Usage: %s <mode> or\n", argv[0]);
        printf("Usage: %s <sender> <title> <contents>\n", argv[0]);
        return EXIT_FAILURE;
    }
}