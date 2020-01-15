#include <stdio.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <time.h>


//Create space to hold the file path
#define BUFFER_SIZE (256)

//char newPath[1024];

void listFiles(char *path) {
	//Initialise variable for dir
	DIR *dir;
	//Initialise buffer for snprintf
	char buffer[BUFFER_SIZE];
	//Create new dirent structure 
	struct dirent *entry;
	//If path exists
	if ((dir = opendir(path)) != NULL) {
		//While files exists in path
		while ((entry = readdir(dir)) != NULL) {
			//Skip '.' and '..' entries to avoid recursion issues
			if((strcmp(entry->d_name,"..") != 0) && (strcmp(entry->d_name,".") != 0)) {
				////Creating variables to store attributes
				//Create stat structure 'sb'
				struct stat sb;
				//Direct output of printf (file path) to buffer
				snprintf(buffer, BUFFER_SIZE, "%s/%s", path, entry->d_name);
				//System call to return attributes of an inode (returns 0 or -1 if error, errno is set)
				stat(buffer, &sb);
				//Type mode_t stores st_mode, mode of the file
				mode_t mode;
				//Type uid_t stores st_uid, ID of owner
				uid_t owner;
				//Type gid_t stores st_gid, group ID of owner
				gid_t group;
				//Type off_t stores st_size, filesize of the file in bytes
				off_t size;
				//Type time_t stores st_mtime, time of last modification
				time_t time;
				
				////Storing attributes to created variables
				//Mode stored
				mode = sb.st_mode;
				//Owner stored
				owner = sb.st_uid;
				//Group stored
				group = sb.st_gid;
				//File size stored
				size = sb.st_size;
				//Time of last modification stored
				time = sb.st_mtime;

				////Print attributes
				//Print file mode (permissions). Code snippet here that parses the stat macro outputs from https://bit.ly/2StYoxZ
				printf( (S_ISDIR(mode)) ? "d" : "-");
				printf( (mode & S_IRUSR) ? "r" : "-");
				printf( (mode & S_IWUSR) ? "w" : "-");
				printf( (mode & S_IXUSR) ? "x" : "-");
				printf( (mode & S_IRGRP) ? "r" : "-");
				printf( (mode & S_IWGRP) ? "w" : "-");
				printf( (mode & S_IXGRP) ? "x" : "-");
				printf( (mode & S_IROTH) ? "r" : "-");
				printf( (mode & S_IWOTH) ? "w" : "-");
				printf( (mode & S_IXOTH) ? "x" : "-");
				printf(" ");
		 
				//Create new struct for user info
				struct passwd *pwd;
				//Return user account info for id
				pwd = getpwuid(owner);
				//Print username formatted as string
				printf("%s ", pwd->pw_name);	 

				//Create new struct for group info
				struct group *grp;
				//Return group info for id
				grp = getgrgid(group);
				//Print group name formatted as string
				printf("%s ", grp->gr_name);

				//Print file size
				printf("%d ", size);
		 
				//Create new time struct
				struct tm lt;
				//Convert st_mtime to localtime
				localtime_r(&time, &lt);
				//Initialise variable to store formatted time
				char formatted_time[30];
				//Format the date and time
				strftime(formatted_time, sizeof formatted_time, "%b %d %H:%M", &lt);
				//Print date and time
				printf("%s ", formatted_time);

				//Print file name
				printf("%s\n", entry->d_name); 
				
				//S_ISREG() macro from sys/stat.h returns 0 if dir, 1 if regular file
				if (S_ISREG(sb.st_mode) == 0) {
					//New variable to store 'new path' to call listFiles recursively
					char newPath[1024];
					//Direct printf output to generate correct 'new path', using previous path and current entry name
					snprintf(newPath, sizeof(newPath), "%s/%s", path, entry->d_name);
					//Recursively call function
					listFiles(newPath);
				}
			}
		}
	}
	//Close open directory
	closedir (dir);
}

int main() {
	listFiles("/nfs/pihome/eddiewastaken/test");
	return 0;
}
