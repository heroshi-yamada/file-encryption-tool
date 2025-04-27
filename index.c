#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to encrypt text using ASCII + key
void encryptAscii(char *text, int key, char *encryptedText) {
    int i = 0;
    while (text[i] != '\0') {
        encryptedText[i] = text[i] + key;
        i++;
    }
    encryptedText[i] = '\0';
}

void decryptAscii(char *encryptedText, int key, char *decryptedText) {
    int i = 0;
    while (encryptedText[i] != '\0') {
        decryptedText[i] = encryptedText[i] - key;
        i++;
    }
    decryptedText[i] = '\0';
}

void saveToFile(const char *title, const char *username, const char *description, const char *encryptedText, const char *filename) {
    FILE *file = fopen(filename, "a");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }

    fprintf(file, "Title: %s\n", title);
    fprintf(file, "Username: %s\n", username);
    fprintf(file, "Description: %s\n", description);
    fprintf(file, "Encrypted Text: %s\n", encryptedText);
    fprintf(file, "---------------------------------\n");

    fclose(file);
    printf("Encrypted text saved to file successfully.\n");
}

void saveKeyToFile(const char *title, const char *username, int key, const char *keyFile) {
    FILE *file = fopen(keyFile, "a");
    if (!file) {
        printf("Error opening key file!\n");
        return;
    }

    fprintf(file, "Title: %s\n", title);
    fprintf(file, "Username: %s\n", username);
    fprintf(file, "Key: %d\n", key);
    fprintf(file, "---------------------------------\n");

    fclose(file);
    printf("Encryption key saved to key file successfully.\n");
}

int getKeyFromFile(const char *filename, const char *title, const char *username) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening key file!\n");
        return -1;
    }

    char line[256];
    int key = -1;

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "Title:") && strstr(line, title)) {
            fgets(line, sizeof(line), file);
            char fileUsername[50];
            sscanf(line, "Username: %[^\n]", fileUsername);

            if (strcmp(fileUsername, username) == 0) {
                fgets(line, sizeof(line), file);
                sscanf(line, "Key: %d", &key);
                break;
            }
        }
    }

    fclose(file);
    return key;
}

void decryptFromFile(const char *filename, const char *title, const char *username, int key) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }

    char line[256];
    int found = 0;

    printf("\n--- Searching for Title: %s ---\n", title);
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "Title:") && strstr(line, title)) {
            found = 1;
            printf("Title Found: %s\n", title);

            char fileUsername[50], encryptedText[256];
            fgets(line, sizeof(line), file);
            sscanf(line, "Username: %[^\n]", fileUsername);

            if (strcmp(fileUsername, username) != 0) {
                printf("Username does not match. Decryption aborted.\n");
                fclose(file);
                return;
            }

            fgets(line, sizeof(line), file);

            fgets(line, sizeof(line), file);
            sscanf(line, "Encrypted Text: %[^\n]", encryptedText);

            char decryptedText[256];
            decryptAscii(encryptedText, key, decryptedText);
            printf("Decrypted Text: %s\n", decryptedText);

            break;
        }
    }

    if (!found) {
        printf("Key or title is incorrect.\n");
    }

    fclose(file);
}

int main() {
    int choice, key;
    char text[256], encryptedText[256], decryptedText[256];
    char username[50], description[100], title[50];
    char filename[50] = "encrypted_data.txt";
    char keyFile[50] = "keys.txt";

    while (1) {
        printf("\n--- Encryption/Decryption Tool ---\n");
        printf("1. Encrypt Text\n");
        printf("2. Decrypt Text\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("Enter text to encrypt: ");
                fgets(text, sizeof(text), stdin);
                text[strcspn(text, "\n")] = '\0';

                printf("Enter encryption key (integer): ");
                scanf("%d", &key);
                getchar();

                printf("Enter title for this text: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = '\0';

                printf("Enter username: ");
                fgets(username, sizeof(username), stdin);
                username[strcspn(username, "\n")] = '\0';

                printf("Enter description: ");
                fgets(description, sizeof(description), stdin);
                description[strcspn(description, "\n")] = '\0';

                encryptAscii(text, key, encryptedText);
                printf("Encrypted Text: %s\n", encryptedText);

                saveToFile(title, username, description, encryptedText, filename);
                saveKeyToFile(title, username, key, keyFile);
                break;

            case 2:
                printf("Enter the title of the text to decrypt: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = '\0';

                printf("Enter the username associated with the text: ");
                fgets(username, sizeof(username), stdin);
                username[strcspn(username, "\n")] = '\0';

                key = getKeyFromFile(keyFile, title, username);
                if (key == -1) {
                    printf("Key not found for the given title and username.\n");
                    break;
                }

                decryptFromFile(filename, title, username, key);
                break;

            case 3:
                printf("Exiting program...\n");
                exit(0);

            default:
                printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}
