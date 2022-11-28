#include <bits/stdc++.h>
#include <unistd.h>
#include <dirent.h>
char cmd[1005], filename[105];

int main() {
    DIR *test_cases_dir = opendir("test_cases");
    while(true) {
        dirent *d = readdir(test_cases_dir);
        if(!d) {
            puts("Test done!");
            return 0;
        }
        if(d->d_type == DT_DIR) continue;
        int len = strlen(d->d_name);
        if(strcmp(".sy", d->d_name + len - 3)) continue;
        strcpy(filename, d->d_name);
        filename[len - 3] = '\0';
        sprintf(cmd, "./main < ./test_cases/%s > ./test_cases_output/%s.out", d->d_name, filename);
        printf("testing %s\n", d->d_name);
        if(system(cmd)) {
            printf("Faild in %s\n", d->d_name);
            return 0;
        } else {
            puts("OK");
        }
    }
    return 0;
}