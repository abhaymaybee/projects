#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zip.h>

// Print usage help
void print_usage(const char *prog_name) {
    fprintf(stderr, "Usage: %s <zip-file> <destination-folder>\n", prog_name);
}

// Create directories in the path (simple version)
void ensure_path_exists(const char *path) {
    char tmp[1024];
    strncpy(tmp, path, sizeof(tmp) - 1);
    tmp[sizeof(tmp) - 1] = '\0';
    
    for (char *p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            mkdir(tmp, 0755);
            *p = '/';
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    const char *zip_file = argv[1];
    const char *out_dir  = argv[2];
    int err_code;

    // Open the ZIP archive read-only
    zip_t *zip = zip_open(zip_file, ZIP_RDONLY, &err_code);
    if (!zip) {
        fprintf(stderr, "Could not open \"%s\" (error code: %d)\n", zip_file, err_code);
        return EXIT_FAILURE;
    }

    zip_int64_t total_files = zip_get_num_entries(zip, 0);
    printf("Found %lld entries in \"%s\".\n", total_files, zip_file);

    for (zip_uint64_t i = 0; i < total_files; i++) {
        const char *name = zip_get_name(zip, i, 0);
        if (!name) {
            fprintf(stderr, "Error fetching entry name at index %llu\n", i);
            continue;
        }

        zip_file_t *zf = zip_fopen_index(zip, i, 0);
        if (!zf) {
            fprintf(stderr, "Could not read \"%s\" from zip\n", name);
            continue;
        }

        // Build the full output file path
        char dest_path[1024];
        snprintf(dest_path, sizeof(dest_path), "%s/%s", out_dir, name);

        // If it's a folder entry, skip file creation
        if (name[strlen(name) - 1] == '/') {
            ensure_path_exists(dest_path);
            zip_fclose(zf);
            continue;
        }

        // Ensure directories exist
        ensure_path_exists(dest_path);

        // Open output file for writing
        FILE *fout = fopen(dest_path, "wb");
        if (!fout) {
            fprintf(stderr, "Failed to create \"%s\"\n", dest_path);
            zip_fclose(zf);
            continue;
        }

        // Read from zip and write to disk
        char buffer[4096];
        zip_int64_t bytes;
        while ((bytes = zip_fread(zf, buffer, sizeof(buffer))) > 0) {
            fwrite(buffer, 1, bytes, fout);
        }

        fclose(fout);
        zip_fclose(zf);
        printf("Extracted: %s\n", dest_path);
    }

    zip_close(zip);
    printf("Extraction complete ?\n");
    return EXIT_SUCCESS;
}

